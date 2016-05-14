#include <CQIllustratorPathShapeControlPoint.h>
#include <CQIllustratorPathShape.h>

CQIllustratorPathShapeControlPoint::
CQIllustratorPathShapeControlPoint(uint ind, const CPoint2D &p) :
 CQIllustratorShapeControlPoint((ind << 4), p), ind_(ind)
{
}

CQIllustratorPathShapeControlPoint::
CQIllustratorPathShapeControlPoint(uint ind, uint ind1, const CPoint2D &p) :
 CQIllustratorShapeControlPoint((ind << 4) | ind1, p), ind_(ind), ind1_(ind1)
{
}

CQIllustratorPathShapeControlPoint *
CQIllustratorPathShapeControlPoint::
dup() const
{
  return new CQIllustratorPathShapeControlPoint(ind_, ind1_, point_);
}

CPoint2D
CQIllustratorPathShapeControlPoint::
getPoint(const CQIllustratorShape *shape) const
{
  const CQIllustratorPathShape *path = dynamic_cast<const CQIllustratorPathShape *>(shape);
  assert(path);

  return path->getPoint(ind_, ind1_);
}

void
CQIllustratorPathShapeControlPoint::
setPoint(CQIllustratorShape *shape, const CPoint2D &point)
{
  CQIllustratorPathShape *path = dynamic_cast<CQIllustratorPathShape *>(shape);
  assert(path);

  path->setPoint(ind_, ind1_, point);
}

void
CQIllustratorPathShapeControlPoint::
setCornerNode(CQIllustratorShape *shape) const
{
  CQIllustratorPathShape *path = dynamic_cast<CQIllustratorPathShape *>(shape);
  assert(path);

  path->setCornerPoint(ind_);
}

void
CQIllustratorPathShapeControlPoint::
setCurveNode(CQIllustratorShape *shape) const
{
  CQIllustratorPathShape *path = dynamic_cast<CQIllustratorPathShape *>(shape);
  assert(path);

  path->setCurvePoint(ind_);
}
