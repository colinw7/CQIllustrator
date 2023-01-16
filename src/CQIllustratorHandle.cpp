#include <CQIllustratorHandle.h>
#include <CQIllustrator.h>

#include <CQImage.h>
#include <CQUtil.h>
#include <CQUtilGeom.h>

#include <QPainter>

CQIllustratorHandle::
CQIllustratorHandle(CQIllustrator *illustrator) :
 illustrator_(illustrator)
{
}

void
CQIllustratorHandle::
setImage(const QIcon &icon, const QIcon &active_icon)
{
  icon_ = icon;

  if (! active_icon.isNull())
    active_icon_ = active_icon;
  else
    active_icon_ = icon;

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
#if 0
  if (icon_.isValid())
    return CISize2D(icon_->getWidth(), icon_->getHeight());
  else
    return CISize2D(7, 7);
#endif
  return CISize2D(13, 13);
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
  if (style_ == IMAGE_STYLE && ! icon_.isNull()) {
    QPointF p1 = illustrator_->getTransform().map(CQUtil::toQPoint(pos_));

    QPixmap pixmap;

    if (! active_)
      pixmap = icon_.pixmap(QSize(16,16));
    else
      pixmap = active_icon_.pixmap(QSize(16,16));

    QPointF ll = p1 - CQUtil::toQPoint(offset_);

    painter->setWorldMatrixEnabled(false);

    painter->drawPixmap(ll, pixmap);

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

    QPen   pen;
    QBrush brush;

    if (active_) {
      pen   = QPen(QColor(0,0,0));
      brush = QBrush(QColor(0,255,0));
    }
    else {
      pen   = QPen(QColor(80,80,80));
      brush = QBrush(Qt::NoBrush);
    }

    pen.setWidth(0);

    painter->setPen  (pen);
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

CQIllustratorControlPointHandle::
CQIllustratorControlPointHandle(CQIllustrator *illustrator) :
 CQIllustratorHandle(illustrator), point_(0)
{
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
