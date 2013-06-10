#include <CQIllustratorHandle.h>
#include <CQIllustrator.h>

#include <CQImage.h>
#include <CQUtil.h>

#include <QPainter>

//#include "xpm/control_point.xpm"
//#include "xpm/control_point_active.xpm"

CQIllustratorHandle::
CQIllustratorHandle(CQIllustrator *illustrator) :
 illustrator_(illustrator), style_(RECT_STYLE), active_(false), visible_(true), pos_(0,0)
{
}

void
CQIllustratorHandle::
setImage(const char **strings, uint num_strings,
         const char **active_strings, uint num_active_strings)
{
  image_ = CImageMgrInst->createImage(CImageXPMSrc(strings, num_strings));

  if (active_strings)
    active_image_ = CImageMgrInst->createImage(CImageXPMSrc(active_strings, num_active_strings));
  else
    active_image_ = image_;

  style_ = IMAGE_STYLE;
}

void
CQIllustratorHandle::
setStyle(Style style)
{
  style_ = style;
}

CISize2D
CQIllustratorHandle::
getSize()
{
  if (image_.isValid())
    return CISize2D(image_->getWidth(), image_->getHeight());
  else
    return CISize2D(7, 7);
}

bool
CQIllustratorHandle::
updateActive(const QPointF &p)
{
  bool old_active = active_;

  QPointF p1 = illustrator_->getTransform().map(CQUtil::toQPoint(pos_));

  CISize2D size = getSize();

  QPointF ll = p1 - CQUtil::toQPoint(offset_);
  QPointF ur = QPointF(p1.x() + size.getWidth(), p1.y() + size.getHeight());

  active_ = (p.x() >= ll.x() && p.x() <= ur.x() && p.y() >= ll.y() && p.y() <= ur.y());

  return (old_active != active_);
}

void
CQIllustratorHandle::
setPosition(const CPoint2D &pos)
{
  pos_ = pos;
}

void
CQIllustratorHandle::
draw(QPainter *painter, const CPoint2D &pos, const CIPoint2D &offset)
{
  pos_    = pos;
  offset_ = offset;

  drawI(painter);
}

void
CQIllustratorHandle::
draw(QPainter *painter, const CPoint2D &pos)
{
  CISize2D size = getSize();

  pos_    = pos;
  offset_ = CIPoint2D(size.getWidth()/2, size.getHeight()/2);

  drawI(painter);
}

void
CQIllustratorHandle::
draw(QPainter *painter)
{
  CISize2D size = getSize();

  offset_ = CIPoint2D(size.getWidth()/2, size.getHeight()/2);

  drawI(painter);
}

void
CQIllustratorHandle::
drawI(QPainter *painter)
{
  if (style_ == IMAGE_STYLE && image_.isValid()) {
    QPointF p1 = illustrator_->getTransform().map(CQUtil::toQPoint(pos_));

    QImage image1;

    if (! active_)
      image1 = image_.cast<CQImage>()->getQImage();
    else
      image1 = active_image_.cast<CQImage>()->getQImage();

    QPointF ll = p1 - CQUtil::toQPoint(offset_);

    painter->setWorldMatrixEnabled(false);

    painter->drawImage(ll, image1);

    painter->setWorldMatrixEnabled(true);
  }
  else {
    QPointF p1 = CQUtil::toQPoint(pos_);

    CISize2D size = getSize();

    QPointF po    = illustrator_->getITransform().map(QPointF(0, 0));
    QPointF psize = illustrator_->getITransform().map(QPointF(size.getWidth(), size.getHeight()));

    double pw = fabs(psize.x() - po.x());
    double ph = fabs(psize.y() - po.y());

    QRectF rect(p1.x() - pw/2.0, p1.y() - ph/2.0, pw, ph);

    QBrush brush;

    if (active_) {
      brush = QBrush(QColor(0,255,0));

      painter->setPen(QColor(0,0,0));
    }
    else {
      brush = QBrush(Qt::NoBrush);

      painter->setPen(QColor(80,80,80));
    }

    painter->setBrush(brush);

    if (style_ == RECT_STYLE)
      painter->drawRect(rect);
    else {
      painter->drawEllipse(p1, pw/2.0, ph/2.0);
    }

    painter->drawLine(p1, QPointF(p1.x(), p1.y() + ph/2));
  }
}

//----------

#define IMAGE_DATA(I) I, sizeof(I)/sizeof(char *)

CQIllustratorControlPointHandle::
CQIllustratorControlPointHandle(CQIllustrator *illustrator) :
 CQIllustratorHandle(illustrator), point_(NULL)
{
  //setImage(IMAGE_DATA(control_point_data), IMAGE_DATA(control_point_active_data));
}

CQIllustratorControlPointHandle::
~CQIllustratorControlPointHandle()
{
  delete point_;
}

void
CQIllustratorControlPointHandle::
updatePoint(CQIllustratorShape *shape, const CPoint2D &point)
{
  assert(point_);

  point_->setPoint(shape, point);
}

void
CQIllustratorControlPointHandle::
draw(const CQIllustratorShape *shape, CQIllustratorShapeControlPoint *point, QPainter *painter)
{
  assert(point);

  delete point_;

  point_ = point->dup();

  pos_ = point_->getPoint(shape);

  CQIllustratorHandle::draw(painter);
}
