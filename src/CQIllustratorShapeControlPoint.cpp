#include <CQIllustratorShapeControlPoint.h>

CQIllustratorShapeControlPoint::
CQIllustratorShapeControlPoint(int id, const CPoint2D &point, ControlPointType type) :
 id_(id), point_(point), type_(type)
{
}

CPoint2D
CQIllustratorShapeControlPoint::
getPoint(const CQIllustratorShape *) const
{
  return point_;
}

void
CQIllustratorShapeControlPoint::
setPoint(CQIllustratorShape *, const CPoint2D &point)
{
  point_ = point;
}

void
CQIllustratorShapeControlPoint::
setPoint(const CPoint2D &point)
{
  point_ = point;
}

void
CQIllustratorShapeControlPoint::
updatePoint(CQIllustratorShape *shape, CQIllustratorShapeControlPoint *point)
{
  setPoint(shape, point->point_);
}

bool
operator==(const CQIllustratorShapeControlPoint &p1, const CQIllustratorShapeControlPoint &p2)
{
  return (p1.id_ == p2.id_);
}

bool
operator!=(const CQIllustratorShapeControlPoint &p1, const CQIllustratorShapeControlPoint &p2)
{
  return ! (p1 == p2);
}

double
CQIllustratorShapeControlPoint::
distance(const CQIllustratorShape *shape, const CPoint2D &point) const
{
  return pointDist(getPoint(shape), point);
}

void
CQIllustratorShapeControlPoint::
movePointBy(CQIllustratorShape *shape, const CPoint2D &d)
{
  setPoint(shape, getPoint(shape) + d);
}

void
CQIllustratorShapeControlPoint::
movePointTo(CQIllustratorShape *shape, const CPoint2D &pos)
{
  setPoint(shape, pos);
}

double
CQIllustratorShapeControlPoint::
pointDist(const CPoint2D &p1, const CPoint2D &p2)
{
  double dx = p1.x - p2.x;
  double dy = p1.y - p2.y;

  return sqrt(dx*dx + dy*dy);
}
