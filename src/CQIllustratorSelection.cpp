#include <CQIllustratorSelection.h>
#include <CQIllustrator.h>
#include <CQIllustratorShape.h>
#include <CQIllustratorShapeControlLine.h>

CQIllustratorSelectedShapes::
CQIllustratorSelectedShapes(CQIllustrator *illustrator) :
 illustrator_(illustrator), locked_(false)
{
}

void
CQIllustratorSelectedShapes::
clear()
{
  shapes_.clear();

  emit selectionChanged();
}

void
CQIllustratorSelectedShapes::
startSelect()
{
  assert(! locked_);

  locked_ = true;
}

void
CQIllustratorSelectedShapes::
endSelect()
{
  assert(locked_);

  locked_ = false;

  emit selectionChanged();
}

void
CQIllustratorSelectedShapes::
add(CQIllustratorShape *shape)
{
  if (exists(shape)) return;

  shapes_.push_back(CQIllustratorSelectedShape(shape));

  if (! locked_)
    emit selectionChanged();
}

void
CQIllustratorSelectedShapes::
remove(iterator i)
{
  shapes_.erase(i);

  if (! locked_)
    emit selectionChanged();
}

bool
CQIllustratorSelectedShapes::
exists(const CQIllustratorShape *shape) const
{
  SelectedShapes::const_iterator p1, p2;

  for (p1 = shapes_.begin(), p2 = shapes_.end(); p1 != p2; ++p1) {
    if ((*p1).getShape()->getId() == shape->getId())
      return true;
  }

  return false;
}

bool
CQIllustratorSelectedShapes::
exists(const CQIllustratorShape *shape, const CQIllustratorShapeControlPoint *point) const
{
  SelectedShapes::const_iterator p1, p2;

  for (p1 = shapes_.begin(), p2 = shapes_.end(); p1 != p2; ++p1) {
    if ((*p1).getShape()->getId() == shape->getId() && (*p1).exists(point))
      return true;
  }

  return false;
}

CQIllustratorSelectedShape &
CQIllustratorSelectedShapes::
get(CQIllustratorShape *shape)
{
  SelectedShapes::iterator p1, p2;

  for (p1 = shapes_.begin(), p2 = shapes_.end(); p1 != p2; ++p1) {
    if ((*p1).getShape()->getId() == shape->getId())
      return *p1;
  }

  assert(false);
}

CQIllustratorSelectedShape &
CQIllustratorSelectedShapes::
checkoutShape(const CQIllustratorSelectedShape &sshape, CQIllustratorData::ChangeType changeType)
{
  CQIllustratorSelectedShape &sshape1 = const_cast<CQIllustratorSelectedShape &>(sshape);

  CQIllustratorShape *shape = sshape1.getShape();

  illustrator_->checkoutShape(shape, changeType);

  return sshape1;
}

void
CQIllustratorSelectedShapes::
checkinShape(const CQIllustratorSelectedShape &sshape, CQIllustratorData::ChangeType changeType)
{
  CQIllustratorShape *shape = const_cast<CQIllustratorShape *>(sshape.getShape());

  illustrator_->checkinShape(shape, changeType);

  if (! locked_)
    emit selectionChanged();
}

//----

void
CQIllustratorSelectedShapes::
clearShapeLines(CQIllustratorSelectedShape &shape)
{
  shape.clearLines();

  if (! locked_)
    emit selectionChanged();
}

void
CQIllustratorSelectedShapes::
addShapeLine(CQIllustratorSelectedShape &shape, CQIllustratorShapeControlLine *line)
{
  shape.addLine(line);

  if (! locked_)
    emit selectionChanged();
}

void
CQIllustratorSelectedShapes::
removeShapeLine(CQIllustratorSelectedShape &shape, CQIllustratorShapeControlLine *line)
{
  shape.removeLine(line);

  if (! locked_)
    emit selectionChanged();
}

void
CQIllustratorSelectedShapes::
setShapeLine(CQIllustratorSelectedShape &shape, uint i, const CLine2D &l)
{
  CQIllustratorShapeControlLine *line = shape.getLine(i);

  line->setLine(shape.getShape(), l);

  if (! locked_)
    emit selectionChanged();
}

//----

void
CQIllustratorSelectedShapes::
clearShapePoints(CQIllustratorSelectedShape &shape)
{
  shape.clearPoints();

  if (! locked_)
    emit selectionChanged();
}

void
CQIllustratorSelectedShapes::
addShapePoint(CQIllustratorSelectedShape &shape, CQIllustratorShapeControlPoint *point)
{
  shape.addPoint(point);

  if (! locked_)
    emit selectionChanged();
}

void
CQIllustratorSelectedShapes::
removeShapePoint(CQIllustratorSelectedShape &shape, CQIllustratorShapeControlPoint *point)
{
  shape.removePoint(point);

  if (! locked_)
    emit selectionChanged();
}

//-------------

CQIllustratorSelectedShape::
CQIllustratorSelectedShape(CQIllustratorShape *shape) :
 shape_(shape), points_()
{
}

CQIllustratorSelectedShape::
~CQIllustratorSelectedShape()
{
  clearPoints();
}

CQIllustratorShape *
CQIllustratorSelectedShape::
getShape()
{
  return shape_;
}

const CQIllustratorShape *
CQIllustratorSelectedShape::
getShape() const
{
  return shape_;
}

//----

void
CQIllustratorSelectedShape::
clearLines()
{
  uint num = lines_.size();

  for (uint i = 0; i < num; ++i)
    delete lines_[i];

  lines_.clear();
}

void
CQIllustratorSelectedShape::
addLine(CQIllustratorShapeControlLine *line)
{
  lines_.push_back(line->dup());
}

void
CQIllustratorSelectedShape::
removeLine(CQIllustratorShapeControlLine *line)
{
  uint num = lines_.size();

  uint i = 0;

  for ( ; i < num; ++i)
    if (line->getId() == lines_[i]->getId())
      break;

  if (i < num) {
    ++i;

    for ( ; i < num; ++i)
      lines_[i - 1] = lines_[i];

    lines_.pop_back();
  }
}

bool
CQIllustratorSelectedShape::
exists(const CQIllustratorShapeControlLine *line) const
{
  uint num = lines_.size();

  for (uint i = 0; i < num; ++i)
    if (line->getId() == lines_[i]->getId())
      return true;;

  return false;
}

uint
CQIllustratorSelectedShape::
numLines() const
{
  return lines_.size();
}

const CQIllustratorShapeControlLine *
CQIllustratorSelectedShape::
getLine(uint i) const
{
  return lines_[i];
}

CQIllustratorShapeControlLine *
CQIllustratorSelectedShape::
getLine(uint i)
{
  return lines_[i];
}

//----

void
CQIllustratorSelectedShape::
clearPoints()
{
  uint num = points_.size();

  for (uint i = 0; i < num; ++i)
    delete points_[i];

  points_.clear();
}

void
CQIllustratorSelectedShape::
addPoint(CQIllustratorShapeControlPoint *point)
{
  points_.push_back(point->dup());
}

void
CQIllustratorSelectedShape::
removePoint(CQIllustratorShapeControlPoint *point)
{
  uint num = points_.size();

  uint i = 0;

  for ( ; i < num; ++i)
    if (point->getId() == points_[i]->getId())
      break;

  if (i < num) {
    ++i;

    for ( ; i < num; ++i)
      points_[i - 1] = points_[i];

    points_.pop_back();
  }
}

bool
CQIllustratorSelectedShape::
exists(const CQIllustratorShapeControlPoint *point) const
{
  uint num = points_.size();

  for (uint i = 0; i < num; ++i)
    if (point->getId() == points_[i]->getId())
      return true;;

  return false;
}

uint
CQIllustratorSelectedShape::
numPoints() const
{
  return points_.size();
}

const CQIllustratorShapeControlPoint *
CQIllustratorSelectedShape::
getPoint(uint i) const
{
  return points_[i];
}

CQIllustratorShapeControlPoint *
CQIllustratorSelectedShape::
getPoint(uint i)
{
  return points_[i];
}
