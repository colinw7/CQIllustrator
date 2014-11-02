#include <CQIllustratorSizer.h>
#include <CQIllustrator.h>
#include <CQIllustratorHandle.h>
#include <CQIllustratorShapeDrawer.h>

#include <QPainter>

#include <xpm/corner_bl.xpm>
#include <xpm/corner_bl_active.xpm>
#include <xpm/corner_tl.xpm>
#include <xpm/corner_tl_active.xpm>

#include <xpm/rotate_bl.xpm>
#include <xpm/rotate_bl_active.xpm>
#include <xpm/rotate_br.xpm>
#include <xpm/rotate_br_active.xpm>
#include <xpm/rotate_tl.xpm>
#include <xpm/rotate_tl_active.xpm>
#include <xpm/rotate_tr.xpm>
#include <xpm/rotate_tr_active.xpm>

#include <xpm/side_lr.xpm>
#include <xpm/side_lr_active.xpm>
#include <xpm/side_tb.xpm>
#include <xpm/side_tb_active.xpm>

#include <xpm/rcenter.xpm>
#include <xpm/rcenter_active.xpm>

#define IMAGE_DATA(I) I##_data, sizeof(I##_data)/sizeof(char *)

#define ACTIVE_IMAGE_DATA(I) IMAGE_DATA(I), IMAGE_DATA(I##_active)

CQIllustratorSizer::
CQIllustratorSizer(CQIllustrator *illustrator) :
 illustrator_(illustrator), handle_(HANDLE_NONE), press_handle_(HANDLE_NONE), op_(OP_RESIZE)
{
  bl_corner_handle_ = new CQIllustratorHandle(illustrator);
  tr_corner_handle_ = new CQIllustratorHandle(illustrator);
  tl_corner_handle_ = new CQIllustratorHandle(illustrator);
  br_corner_handle_ = new CQIllustratorHandle(illustrator);

  bl_corner_handle_->setImage(ACTIVE_IMAGE_DATA(corner_bl));
  tr_corner_handle_->setImage(ACTIVE_IMAGE_DATA(corner_bl));
  tl_corner_handle_->setImage(ACTIVE_IMAGE_DATA(corner_tl));
  br_corner_handle_->setImage(ACTIVE_IMAGE_DATA(corner_tl));

  //----

  bl_rotate_handle_ = new CQIllustratorHandle(illustrator);
  br_rotate_handle_ = new CQIllustratorHandle(illustrator);
  tl_rotate_handle_ = new CQIllustratorHandle(illustrator);
  tr_rotate_handle_ = new CQIllustratorHandle(illustrator);

  bl_rotate_handle_->setImage(ACTIVE_IMAGE_DATA(rotate_bl));
  br_rotate_handle_->setImage(ACTIVE_IMAGE_DATA(rotate_br));
  tl_rotate_handle_->setImage(ACTIVE_IMAGE_DATA(rotate_tl));
  tr_rotate_handle_->setImage(ACTIVE_IMAGE_DATA(rotate_tr));

  //----

  l_side_handle_ = new CQIllustratorHandle(illustrator);
  b_side_handle_ = new CQIllustratorHandle(illustrator);
  r_side_handle_ = new CQIllustratorHandle(illustrator);
  t_side_handle_ = new CQIllustratorHandle(illustrator);

  l_side_handle_->setImage(ACTIVE_IMAGE_DATA(side_lr));
  b_side_handle_->setImage(ACTIVE_IMAGE_DATA(side_tb));
  r_side_handle_->setImage(ACTIVE_IMAGE_DATA(side_lr));
  t_side_handle_->setImage(ACTIVE_IMAGE_DATA(side_tb));

  //----

  rcenter_handle_ = new CQIllustratorHandle(illustrator);

  rcenter_handle_->setImage(ACTIVE_IMAGE_DATA(rcenter));
}

bool
CQIllustratorSizer::
mousePress(const QPointF &p)
{
  updateActive(p);

  setPressHandleType();

  return (getPressHandleType() != CQIllustratorSizer::HANDLE_NONE);
}

void
CQIllustratorSizer::
mouseRelease(const QPointF &)
{
  resetPressHandleType();
}

void
CQIllustratorSizer::
updateShape(CQIllustratorShape *shape, const QPointF &oldPoint,
            const QPointF &newPoint, bool equal_scale)
{
  double dx = newPoint.x() - oldPoint.x();
  double dy = newPoint.y() - oldPoint.y();

  const CBBox2D &oldBBox = shape->getBBox();

  if (getOpType() == CQIllustratorSizer::OP_RESIZE) {
    CBBox2D newBBox = oldBBox;

    CQIllustratorSizer::HandleType press_handle = getPressHandleType();

    bool is_l = (press_handle & CQIllustratorSizer::HANDLE_L);
    bool is_r = (press_handle & CQIllustratorSizer::HANDLE_R);
    bool is_b = (press_handle & CQIllustratorSizer::HANDLE_B);
    bool is_t = (press_handle & CQIllustratorSizer::HANDLE_T);

    if (illustrator_->getFlipY())
      std::swap(is_b, is_t);

    if (equal_scale) {
      if ((is_l || is_r) && (! is_b && ! is_t)) is_t = true;
      if ((is_b || is_t) && (! is_l && ! is_r)) is_r = true;

      if (fabs(dx) > fabs(dy)) {
        if ((is_r && is_t) || (is_l && is_b))
          dy = dx;
        else
          dy = -dx;
      }
      else {
        if ((is_r && is_t) || (is_l && is_b))
          dx = dy;
        else
          dx = -dy;
      }
    }

    if      (is_l) newBBox.moveBy(CPoint2D(dx,  0), CPoint2D( 0,  0));
    else if (is_r) newBBox.moveBy(CPoint2D( 0,  0), CPoint2D(dx,  0));

    if      (is_b) newBBox.moveBy(CPoint2D( 0, dy), CPoint2D( 0,  0));
    else if (is_t) newBBox.moveBy(CPoint2D( 0,  0), CPoint2D( 0, dy));

    shape->setBBox(newBBox);
  }
  else {
    if (press_handle_ != HANDLE_RC) {
      CPoint2D rc = shape->getRotateCenter();

      double a1 = atan2(oldPoint.y() - rc.y, oldPoint.x() - rc.x);
      double a2 = atan2(newPoint.y() - rc.y, newPoint.x() - rc.x);

      double da = a1 - a2;

      shape->rotate(da);
    }
    else {
      CPoint2D rc = shape->getRotateCenter();

      shape->setRotateCenter(rc + CPoint2D(dx, dy));
    }
  }
}

bool
CQIllustratorSizer::
updateActive(const QPointF &p)
{
  QPointF p1 = transform_.map(p);

  HandleType old_handle = handle_;

  handle_ = HANDLE_NONE;

  if (op_ == OP_RESIZE) {
    bl_corner_handle_->updateActive(p1);
    br_corner_handle_->updateActive(p1);
    tl_corner_handle_->updateActive(p1);
    tr_corner_handle_->updateActive(p1);

    if (bl_corner_handle_->getActive()) handle_ = HANDLE_BL;
    if (br_corner_handle_->getActive()) handle_ = HANDLE_BR;
    if (tl_corner_handle_->getActive()) handle_ = HANDLE_TL;
    if (tr_corner_handle_->getActive()) handle_ = HANDLE_TR;

    l_side_handle_->updateActive(p1);
    b_side_handle_->updateActive(p1);
    r_side_handle_->updateActive(p1);
    t_side_handle_->updateActive(p1);

    if (l_side_handle_->getActive()) handle_ = HANDLE_L;
    if (b_side_handle_->getActive()) handle_ = HANDLE_B;
    if (r_side_handle_->getActive()) handle_ = HANDLE_R;
    if (t_side_handle_->getActive()) handle_ = HANDLE_T;
  }
  else {
    bl_rotate_handle_->updateActive(p1);
    br_rotate_handle_->updateActive(p1);
    tl_rotate_handle_->updateActive(p1);
    tr_rotate_handle_->updateActive(p1);

    if (bl_rotate_handle_->getActive()) handle_ = HANDLE_BL;
    if (br_rotate_handle_->getActive()) handle_ = HANDLE_BR;
    if (tl_rotate_handle_->getActive()) handle_ = HANDLE_TL;
    if (tr_rotate_handle_->getActive()) handle_ = HANDLE_TR;

    l_side_handle_->updateActive(p1);
    b_side_handle_->updateActive(p1);
    r_side_handle_->updateActive(p1);
    t_side_handle_->updateActive(p1);

    if (l_side_handle_->getActive()) handle_ = HANDLE_L;
    if (b_side_handle_->getActive()) handle_ = HANDLE_B;
    if (r_side_handle_->getActive()) handle_ = HANDLE_R;
    if (t_side_handle_->getActive()) handle_ = HANDLE_T;

    rcenter_handle_->updateActive(p1);

    if (rcenter_handle_->getActive()) handle_ = HANDLE_RC;
  }

  return (handle_ != old_handle);
}

void
CQIllustratorSizer::
toggleOp()
{
  op_ = (op_ == OP_RESIZE ? OP_ROTATE : OP_RESIZE);
}

void
CQIllustratorSizer::
draw(CQIllustratorShapeDrawer *drawer, const CQIllustratorShape *shape)
{
  if (! shape) return;

  CBBox2D bbox = shape->getFlatBBox();

  const CPoint2D &rcenter = shape->getRotateCenter();

  QPainter *painter = drawer->getPainter();

  //transform_ = painter->worldTransform();
  transform_ = painter->transform();

  double x1 = bbox.getXMin();
  double y1 = bbox.getYMin();
  double x2 = bbox.getXMax();
  double y2 = bbox.getYMax();

  if (illustrator_->getFlipY())
    std::swap(y1, y2);

  if (op_ == OP_RESIZE) {
    bl_corner_handle_->draw(painter, CPoint2D(x1, y1), CIPoint2D(13, -2));
    br_corner_handle_->draw(painter, CPoint2D(x2, y1), CIPoint2D(-2, -2));
    tl_corner_handle_->draw(painter, CPoint2D(x1, y2), CIPoint2D(13, 13));
    tr_corner_handle_->draw(painter, CPoint2D(x2, y2), CIPoint2D(-2, 13));

    l_side_handle_->draw(painter, CPoint2D(x1, (y1 + y2)/2), CIPoint2D(15,  4));
    r_side_handle_->draw(painter, CPoint2D(x2, (y1 + y2)/2), CIPoint2D(-2,  4));
    b_side_handle_->draw(painter, CPoint2D((x1 + x2)/2, y1), CIPoint2D( 4, -2));
    t_side_handle_->draw(painter, CPoint2D((x1 + x2)/2, y2), CIPoint2D( 4, 15));
  }
  else {
    bl_rotate_handle_->draw(painter, CPoint2D(x1, y1), CIPoint2D(13, -2));
    br_rotate_handle_->draw(painter, CPoint2D(x2, y1), CIPoint2D(-2, -2));
    tl_rotate_handle_->draw(painter, CPoint2D(x1, y2), CIPoint2D(13, 13));
    tr_rotate_handle_->draw(painter, CPoint2D(x2, y2), CIPoint2D(-2, 13));

    l_side_handle_->draw(painter, CPoint2D((x1 + x2)/2, y1), CIPoint2D( 4, -2));
    r_side_handle_->draw(painter, CPoint2D((x1 + x2)/2, y2), CIPoint2D( 4, 15));
    b_side_handle_->draw(painter, CPoint2D(x1, (y1 + y2)/2), CIPoint2D(15,  4));
    t_side_handle_->draw(painter, CPoint2D(x2, (y1 + y2)/2), CIPoint2D(-2,  4));

    rcenter_handle_->draw(painter, rcenter);
  }
}
