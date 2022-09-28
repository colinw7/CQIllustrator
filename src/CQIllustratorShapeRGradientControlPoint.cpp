#include <CQIllustratorShapeRGradientControlPoint.h>
#include <CQIllustratorShape.h>
#include <CRadialGradient.h>
#include <CBBox2D.h>

CQIllustratorShapeRGradientControlPoint::
CQIllustratorShapeRGradientControlPoint(Position pos, const CPoint2D &p) :
 CQIllustratorShapeControlPoint(uint(pos), p), pos_(pos)
{
}

CQIllustratorShapeRGradientControlPoint *
CQIllustratorShapeRGradientControlPoint::
dup() const
{
  return new CQIllustratorShapeRGradientControlPoint(pos_, point_);
}

CPoint2D
CQIllustratorShapeRGradientControlPoint::
getPoint(const CQIllustratorShape *shape) const
{
  CBBox2D bbox = shape->getFlatBBox();

  const CQIllustratorShapeFill &fill = shape->getFill();

  const CGenGradient *g = fill.getGradient();

  const CRadialGradient *rg = dynamic_cast<const CRadialGradient *>(g);

  assert(rg);

  CPoint2D p;

  if (rg) {
    if      (pos_ == Position::CENTER)
      p = CPoint2D(rg->getCenterX(), rg->getCenterY());
    else if (pos_ == Position::FOCUS)
      p = CPoint2D(rg->getFocusX(), rg->getFocusY());
    else {
      double r1 = rg->getRadius()/sqrt(2);

      p = CPoint2D(rg->getCenterX() + r1, rg->getCenterY() + r1);
    }
  }
  else {
    if      (pos_ == Position::CENTER)
      p = CPoint2D(0.5, 0.5);
    else if (pos_ == Position::FOCUS)
      p = CPoint2D(0.5, 0.5);
    else
      p = CPoint2D(1.0, 0.0);
  }

  double x1 = p.x*(bbox.getXMax() - bbox.getXMin()) + bbox.getXMin();
  double y1 = p.y*(bbox.getYMax() - bbox.getYMin()) + bbox.getYMin();

  return CPoint2D(x1, y1);
}

void
CQIllustratorShapeRGradientControlPoint::
setPoint(CQIllustratorShape *shape, const CPoint2D &point)
{
  CQIllustratorShapeFill &fill = shape->getFill();

  CGenGradient *g = fill.getGradient();

  CRadialGradient *rg = dynamic_cast<CRadialGradient *>(g);

  if (! rg) return;

  CBBox2D bbox = shape->getFlatBBox();

  CPoint2D p;

  p.x = (point.x - bbox.getXMin())/(bbox.getXMax() - bbox.getXMin());
  p.y = (point.y - bbox.getYMin())/(bbox.getYMax() - bbox.getYMin());

  if      (pos_ == Position::CENTER)
    rg->setCenter(p);
  else if (pos_ == Position::FOCUS)
    rg->setFocus(p);
  else {
    double d = p.distanceTo(CPoint2D(rg->getCenterX(), rg->getCenterY()));

    rg->setRadius(d);
  }

  rg->init(1, 1);

  point_ = p;
}
