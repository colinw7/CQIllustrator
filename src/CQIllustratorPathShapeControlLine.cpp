#include <CQIllustratorPathShapeControlLine.h>
#include <CQIllustratorPathShape.h>

CQIllustratorPathShapeControlLine::
CQIllustratorPathShapeControlLine(uint ind, const CLine2D &p) :
 CQIllustratorShapeControlLine(ind, p), ind_(ind)
{
}

CQIllustratorPathShapeControlLine *
CQIllustratorPathShapeControlLine::
dup() const
{
  return new CQIllustratorPathShapeControlLine(ind_, line_);
}

CLine2D
CQIllustratorPathShapeControlLine::
getLine(const CQIllustratorShape *shape) const
{
  const auto *path = dynamic_cast<const CQIllustratorPathShape *>(shape);
  assert(path);

  return path->getLine(ind_);
}

void
CQIllustratorPathShapeControlLine::
setLine(CQIllustratorShape *shape, const CLine2D &line)
{
  auto *path = dynamic_cast<CQIllustratorPathShape *>(shape);
  assert(path);

  path->setLine(ind_, line);
}

void
CQIllustratorPathShapeControlLine::
setType(CQIllustratorShape *shape, CPathPartType type)
{
  auto *path = dynamic_cast<CQIllustratorPathShape *>(shape);
  assert(path);

  path->setLineType(ind_, type);
}
