#include <CQIllustratorOffsetPathMode.h>
#include <CQIllustrator.h>
#include <CQIllustratorHandle.h>
#include <CQIllustratorShapeDrawer.h>

#include <QPainter>
#include <QHBoxLayout>

#include <CQMenu.h>
#include <CQUtil.h>
#include <CQPointEdit.h>
#include <CQRealEdit.h>
#include <CQSwatch.h>

#include <svg/offset_path_svg.h>

#include <cursors/rect.xbm>
#include <cursors/rectmask.xbm>

CQIllustratorOffsetPathMode::
CQIllustratorOffsetPathMode(CQIllustrator *illustrator) :
 CQIllustratorMode(illustrator, (uint) CQIllustrator::Mode::OFFSET_PATH), w_(0.0)
{
  setCursor(rect_bits, rectmask_bits, 2, 2);
}

CQIllustratorOffsetPathToolbar *
CQIllustratorOffsetPathMode::
createToolbar()
{
  toolbar_ = new CQIllustratorOffsetPathToolbar(this);

  return toolbar_;
}

CQIllustratorOffsetPathSizer *
CQIllustratorOffsetPathMode::
createSizer()
{
  sizer_ = new CQIllustratorOffsetPathSizer(this);

  return sizer_;
}

CQMenuItem *
CQIllustratorOffsetPathMode::
createMenuItem(CQMenu *menu)
{
  QIcon icon = CQPixmapCacheInst->getIcon("OFFSET_PATH");

  menuItem_ = new CQMenuItem(menu, icon, "&Offset Path", CQMenuItem::CHECKABLE);

  menuItem_->setStatusTip("Offset Path");

  connect(menuItem_->getAction(), SIGNAL(toggled(bool)), this, SLOT(menuItemSlot()));

  return menuItem_;
}

void
CQIllustratorOffsetPathMode::
handleMouseRelease(const MouseEvent &e)
{
  //CPoint2D p2 = CQUtil::fromQPoint(release_wpos_);

  if (dragging_) {
    if (! moving_) {
      selectControlPoint(e);
    }
    // dragging finished so commit
    else {
      illustrator_->getSandbox()->clear();

      CQIllustratorSelectedShapes *selection = illustrator_->getSelection();

      if (selection->size() == 1) {
        CQIllustratorShape *shape = selection->front().getShape();

        CPathShapePartList parts;

        if (! shape->getPath(parts)) return;

        CPathShapePartList parts1;

        parts.offsetPath(w_, shape->getCenter(), parts1);

        CPathShape *pshape = illustrator_->createPathShape();

        pshape->setParts(parts1);

        pshape->setStroke(shape->getStroke());
        pshape->setFill  (shape->getFill ());

        illustrator_->addShape(pshape);

        illustrator_->setSelectShape(pshape);
      }
    }
  }
  // not dragging so do a select
  else {
    CPoint2D p1 = CQUtil::fromQPoint(press_wpos_);

    // point click - select at point
    if (! moving_) {
      (void) illustrator_->selectAt(p1, e.event->isControlKey(), e.event->isShiftKey());
    }
    // drag
    else {
    }
  }

  dragging_ = false;

  illustrator_->redraw();
}

void
CQIllustratorOffsetPathMode::
handleMouseDrag(const MouseEvent &e)
{
  if (dragging_) {
    CQIllustratorShape *shape = illustrator_->getSandbox()->frontShape();

    sizer_->updatePoint(shape, e.window);
  }
  else {
  }

  illustrator_->redrawOverlay();
}

void
CQIllustratorOffsetPathMode::
addSelectionToSandbox()
{
  illustrator_->getSandbox()->addSelectionPath();
}

void
CQIllustratorOffsetPathMode::
drawOverlay(CQIllustratorShapeDrawer *drawer)
{
  QPainter *painter = drawer->getPainter();

  if (! dragging_) {
    // draw rubber band
    if (pressed_) {
      CPoint2D p1 = CQUtil::fromQPoint(press_wpos_);
      CPoint2D p2 = CQUtil::fromQPoint(curr_wpos_);

      CBBox2D bbox(p1, p2);

      QPen pen;

      pen.setColor(QColor(0,0,0));
      pen.setStyle(Qt::DashLine);
      pen.setWidth(0);

      painter->setPen(pen);
      painter->setBrush(Qt::NoBrush);

      painter->drawRect(CQUtil::toQRect(bbox));
    }

    //------

    // draw handles for single selected shape
    CQIllustratorSelectedShapes *selection = illustrator_->getSelection();

    if (selection->size() == 1) {
      const CQIllustratorSelectedShape &sshape = selection->front();

      const CQIllustratorShape *shape = sshape.getShape();

      sizer_->draw(painter, shape);

      sizer_->drawSelectionHandles(painter, sshape);
    }
  }
  else {
    CQIllustratorShape *shape = illustrator_->getSandbox()->frontShape();

    sizer_->draw(painter, shape);
  }
}

void
CQIllustratorOffsetPathMode::
setSelectedShape(CQIllustratorShape *shape)
{
  sizer_->setSelectedShape(shape);

  CQIllustratorMode::setSelectedShape(shape);
}

void
CQIllustratorOffsetPathMode::
setWidth(double w)
{
  w_ = w;

  toolbar_->setWidth(w_);
}

//------------

CQIllustratorOffsetPathToolbar::
CQIllustratorOffsetPathToolbar(CQIllustratorOffsetPathMode *mode) :
 CQIllustratorToolbar(mode), mode_(mode)
{
}

QIcon
CQIllustratorOffsetPathToolbar::
getIcon()
{
  return CQPixmapCacheInst->getIcon("OFFSET_PATH");
}

void
CQIllustratorOffsetPathToolbar::
addWidgets()
{
  QBoxLayout *layout = qobject_cast<QBoxLayout *>(CQIllustratorToolbar::layout());

  //-----

  widthEdit_ = new CQRealEdit(0.0);

  widthEdit_->setFocusPolicy(Qt::ClickFocus);

  connect(widthEdit_, SIGNAL(valueChanged(double)), this, SLOT(updateShape()));

  CQSwatch *widthSwatch = new CQSwatch("Width", widthEdit_ );

  layout->addWidget(widthSwatch);

  widthEdit_->setEnabled(false);
}

void
CQIllustratorOffsetPathToolbar::
setSelectedShape(const CQIllustratorShape *shape)
{
  widthEdit_->setEnabled(shape != 0);

  widthEdit_->setValue(0.0);
}

void
CQIllustratorOffsetPathToolbar::
updateShape()
{
  CQIllustrator *illustrator = mode_->getIllustrator();

  CQIllustratorShape *shape =
    CQIllustratorUtil::getCurrentShape<CQIllustratorShape>(illustrator);

  if (shape) {
    double width = widthEdit_->getValue();

    CPathShapePartList parts;

    if (! shape->getPath(parts)) return;

    CPathShapePartList parts1;

    parts.offsetPath(width, shape->getCenter(), parts1);

    CPathShape *pshape = illustrator->createPathShape();

    pshape->setParts(parts1);

    pshape->setStroke(shape->getStroke());
    pshape->setFill  (shape->getFill ());

    illustrator->addShape(pshape);

    illustrator->setSelectShape(pshape);
  }

  illustrator->redraw();
}

void
CQIllustratorOffsetPathToolbar::
setWidth(double w)
{
  widthEdit_->setValue(w);
}

//------

CQIllustratorOffsetPathSizer::
CQIllustratorOffsetPathSizer(CQIllustratorOffsetPathMode *mode) :
 CQIllustratorModeSizer(mode), shape_(0)
{
  CQIllustrator *illustrator = mode_->getIllustrator();

  width_handle_ = new CQIllustratorHandle(illustrator);

  addHandle(width_handle_);
}

void
CQIllustratorOffsetPathSizer::
setSelectedShape(CQIllustratorShape *shape)
{
  delete shape_;

  if (shape)
    shape_ = shape->dup();
  else
    shape_ = 0;

  if (shape_) {
    const CBBox2D &bbox = shape_->getBBox();

    width_handle_->setPosition(bbox.getUL());
  }
  else
    width_handle_->setPosition(CPoint2D(0, 0));
}

void
CQIllustratorOffsetPathSizer::
updatePoint(CQIllustratorShape *shape, const QPointF &p)
{
  if (! shape_) return;

  CPathShape *path = dynamic_cast<CPathShape *>(shape);

  if (! path) return;

  width_handle_->setPosition(CQUtil::fromQPoint(p));

  CPathShapePartList parts;

  if (! shape_->getPath(parts)) return;

  const CBBox2D &bbox = shape_->getBBox();

  double w = bbox.getXMin() - p.x();

  dynamic_cast<CQIllustratorOffsetPathMode *>(mode_)->setWidth(w);

  CPathShapePartList parts1;

  parts.offsetPath(w, shape_->getCenter(), parts1);

  path->setParts(parts1);
}

void
CQIllustratorOffsetPathSizer::
drawHandles(QPainter *painter, const CQIllustratorShape *)
{
  width_handle_->draw(painter);
}
