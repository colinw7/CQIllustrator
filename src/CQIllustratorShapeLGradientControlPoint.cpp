#include <CQIllustratorShapeLGradientControlPoint.h>
#include <CQIllustratorShapeFill.h>
#include <CQIllustratorShape.h>
#include <CBBox2D.h>

CQIllustratorShapeLGradientControlPoint::
CQIllustratorShapeLGradientControlPoint(Position pos, const CPoint2D &p) :
 CQIllustratorShapeControlPoint(uint(pos), p), pos_(pos)
{
}

CQIllustratorShapeLGradientControlPoint *
CQIllustratorShapeLGradientControlPoint::
dup() const
{
  return new CQIllustratorShapeLGradientControlPoint(pos_, point_);
}

CPoint2D
CQIllustratorShapeLGradientControlPoint::
getPoint(const CQIllustratorShape *shape) const
{
  auto bbox = shape->getFlatBBox();

  const auto &fill = shape->getFill();

  const auto *g  = fill.getGradient();
  const auto *lg = dynamic_cast<const CLinearGradient *>(g);
  assert(lg);

  CPoint2D p;

  if (lg) {
    if (pos_ == Position::START)
      p = CPoint2D(lg->getX1(), lg->getY1());
    else
      p = CPoint2D(lg->getX2(), lg->getY2());
  }
  else {
    if (pos_ == Position::START)
      p = CPoint2D(0, 0);
    else
      p = CPoint2D(1, 1);
  }

  double x1 = p.x*(bbox.getXMax() - bbox.getXMin()) + bbox.getXMin();
  double y1 = p.y*(bbox.getYMax() - bbox.getYMin()) + bbox.getYMin();

  return CPoint2D(x1, y1);
}

void
CQIllustratorShapeLGradientControlPoint::
setPoint(CQIllustratorShape *shape, const CPoint2D &point)
{
  auto &fill = shape->getFill();

  auto *g  = fill.getGradient();
  auto *lg = dynamic_cast<CLinearGradient *>(g);
  if (! lg) return;

  CBBox2D bbox = shape->getFlatBBox();

  CPoint2D p;

  p.x = (point.x - bbox.getXMin())/(bbox.getXMax() - bbox.getXMin());
  p.y = (point.y - bbox.getYMin())/(bbox.getYMax() - bbox.getYMin());

  if (pos_ == Position::START) {
    lg->setX1(p.x);
    lg->setY1(p.y);
  }
  else {
    lg->setX2(p.x);
    lg->setY2(p.y);
  }

  point_ = p;
}
