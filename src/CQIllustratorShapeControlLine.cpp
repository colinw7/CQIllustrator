#include <CQIllustratorShapeControlLine.h>

CQIllustratorShapeControlLine::
CQIllustratorShapeControlLine(int id, const CLine2D &line) :
 id_(id), line_(line)
{
}

CLine2D
CQIllustratorShapeControlLine::
getLine(const CQIllustratorShape *) const
{
  return line_;
}

void
CQIllustratorShapeControlLine::
setLine(CQIllustratorShape *, const CLine2D &line)
{
  line_ = line;
}

void
CQIllustratorShapeControlLine::
setLine(const CLine2D &line)
{
  line_ = line;
}

void
CQIllustratorShapeControlLine::
updateLine(CQIllustratorShape *shape, CQIllustratorShapeControlLine *line)
{
  setLine(shape, line->line_);
}

void
CQIllustratorShapeControlLine::
setType(CQIllustratorShape *, CPathPartType)
{
}

bool
operator==(const CQIllustratorShapeControlLine &p1, const CQIllustratorShapeControlLine &p2)
{
  return (p1.id_ == p2.id_);
}

bool
operator!=(const CQIllustratorShapeControlLine &p1, const CQIllustratorShapeControlLine &p2)
{
  return ! (p1 == p2);
}

void
CQIllustratorShapeControlLine::
moveLineTo(CQIllustratorShape *shape, const CLine2D &pos)
{
  setLine(shape, pos);
}
