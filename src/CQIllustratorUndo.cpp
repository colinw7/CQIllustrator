#include <CQIllustratorUndo.h>
#include <CQIllustrator.h>
#include <CQIllustratorShapeGeometry.h>

CQIllustratorUndo::
CQIllustratorUndo(CQIllustrator *illustrator) :
 illustrator_(illustrator)
{
  connect(illustrator_->getData(), SIGNAL(objectCreated(CQIllustratorShape *)),
          this, SLOT(objectCreated(CQIllustratorShape *)));
  connect(illustrator_->getData(), SIGNAL(objectDeleted(CQIllustratorShape *)),
          this, SLOT(objectDeleted(CQIllustratorShape *)));
  connect(illustrator_->getData(), SIGNAL(objectPreModify (CQIllustratorShape *, ChangeType)),
          this, SLOT(objectPreModify (CQIllustratorShape *, ChangeType)));
  connect(illustrator_->getData(), SIGNAL(objectPostModify(CQIllustratorShape *, ChangeType)),
          this, SLOT(objectPostModify(CQIllustratorShape *, ChangeType)));
}

bool
CQIllustratorUndo::
addUndo(CUndoData *data)
{
  bool rc = CUndo::addUndo(data);

  emitUndoChanged();

  return rc;
}

bool
CQIllustratorUndo::
undo(uint n)
{
  bool rc = CUndo::undo(n);

  emitUndoChanged();

  return rc;
}

bool
CQIllustratorUndo::
redo(uint n)
{
  bool rc = CUndo::redo(n);

  emitUndoChanged();

  return rc;
}

void
CQIllustratorUndo::
objectCreated(CQIllustratorShape *shape)
{
  if (locked()) return;

  addUndo(new CQIllustratorUndoCreateShape(this, shape, true));
}

void
CQIllustratorUndo::
objectDeleted(CQIllustratorShape *shape)
{
  if (locked()) return;

  addUndo(new CQIllustratorUndoCreateShape(this, shape, false));
}

void
CQIllustratorUndo::
objectPreModify(CQIllustratorShape *shape, CQIllustratorData::ChangeType type)
{
  if (locked()) return;

  if      (type == CQIllustratorData::ChangeType::GEOMETRY)
    addUndo(new CQIllustratorUndoSetShapeGeometry(this, shape));
  else if (type == CQIllustratorData::ChangeType::STROKE)
    addUndo(new CQIllustratorUndoSetShapeStroke(this, shape));
  else if (type == CQIllustratorData::ChangeType::FILL)
    addUndo(new CQIllustratorUndoSetShapeFill(this, shape));
  else
    std::cerr << "object pre-modify (unknown)" << std::endl;
}

void
CQIllustratorUndo::
objectPostModify(CQIllustratorShape *, CQIllustratorData::ChangeType type)
{
  if      (type == CQIllustratorData::ChangeType::GEOMETRY) {
  }
  else if (type == CQIllustratorData::ChangeType::STROKE) {
  }
  else if (type == CQIllustratorData::ChangeType::FILL) {
  }
  else
    std::cerr << "object post-modify (unknown)" << std::endl;
}

void
CQIllustratorUndo::
emitUndoChanged()
{
  emit undoChanged();
}

//----------------

CQIllustratorUndoSetShapeGeometry::
CQIllustratorUndoSetShapeGeometry(CQIllustratorUndo *undo, CQIllustratorShape *shape) :
 undo_(undo), id_(shape->getId())
{
  geom_ = shape->getGeometry()->dup();
}

bool
CQIllustratorUndoSetShapeGeometry::
exec()
{
  CQIllustrator *illustrator = undo_->getIllustrator();

  CQIllustratorShape *shape = illustrator->getShape(id_);
  assert(shape);

  CQIllustratorShapeGeometry *geom = shape->getGeometry()->dup();

  shape->setGeometry(geom_);

  delete geom_;

  geom_ = geom;

  return true;
}

std::string
CQIllustratorUndoSetShapeGeometry::
getDesc() const
{
  CQIllustrator *illustrator = undo_->getIllustrator();

  CQIllustratorShape *shape = illustrator->getShape(id_);

  if (shape)
    return std::string("Set ") + shape->getClassName() + " geometry";
  else
    return "Set ?? geometry";
}

//----------------

CQIllustratorUndoSetShapeStroke::
CQIllustratorUndoSetShapeStroke(CQIllustratorUndo *undo, CQIllustratorShape *shape) :
 undo_(undo), id_(shape->getId())
{
  stroke_ = shape->getStroke();
}

bool
CQIllustratorUndoSetShapeStroke::
exec()
{
  CQIllustrator *illustrator = undo_->getIllustrator();

  CQIllustratorShape *shape = illustrator->getShape(id_);
  assert(shape);

  CQIllustratorShapeStroke stroke = shape->getStroke();

  shape->setStroke(stroke_);

  stroke_ = stroke;

  return true;
}

std::string
CQIllustratorUndoSetShapeStroke::
getDesc() const
{
  CQIllustrator *illustrator = undo_->getIllustrator();

  CQIllustratorShape *shape = illustrator->getShape(id_);

  if (shape)
    return std::string("Set ") + shape->getClassName() + " stroke";
  else
    return "Set ?? stroke";
}

//----------------

CQIllustratorUndoSetShapeFill::
CQIllustratorUndoSetShapeFill(CQIllustratorUndo *undo, CQIllustratorShape *shape) :
 undo_(undo), id_(shape->getId())
{
  fill_ = shape->getFill();
}

bool
CQIllustratorUndoSetShapeFill::
exec()
{
  CQIllustrator *illustrator = undo_->getIllustrator();

  CQIllustratorShape *shape = illustrator->getShape(id_);
  assert(shape);

  CQIllustratorShapeFill fill = shape->getFill();

  shape->setFill(fill_);

  fill_ = fill;

  return true;
}

std::string
CQIllustratorUndoSetShapeFill::
getDesc() const
{
  CQIllustrator *illustrator = undo_->getIllustrator();

  CQIllustratorShape *shape = illustrator->getShape(id_);

  if (shape)
    return std::string("Set ") + shape->getClassName() + " fill";
  else
    return "Set ?? fill";
}

//----------------

CQIllustratorUndoCreateShape::
CQIllustratorUndoCreateShape(CQIllustratorUndo *undo, CQIllustratorShape *shape, bool create) :
 undo_(undo), create_(create), shape_(nullptr), id_(0)
{
  if (create)
    id_ = shape->getId();
  else {
    shape_ = shape->dup();

    shape_->setId(shape->getId());
  }
}

bool
CQIllustratorUndoCreateShape::
exec()
{
  CQIllustrator *illustrator = undo_->getIllustrator();

  // if was deleted then add back
  if (! create_) {
    illustrator->addShape(shape_);

    illustrator->setSelectShape(shape_);

    id_    = shape_->getId();
    shape_ = nullptr;
  }
  // if was created then delete
  else {
    CQIllustratorShape *shape = illustrator->getShape(id_);

    shape_ = const_cast<CQIllustratorShape *>(shape);
    id_    = 0;

    illustrator->removeShapeFromSelection(shape);

    illustrator->removeShape(shape);
  }

  create_ = ! create_;

  return true;
}

std::string
CQIllustratorUndoCreateShape::
getDesc() const
{
  if (! create_)
    return std::string("Create ") + shape_->getClassName();
  else {
    CQIllustrator *illustrator = undo_->getIllustrator();

    CQIllustratorShape *shape = illustrator->getShape(id_);

    if (shape)
      return std::string("Delete ") + shape->getClassName();
    else
      return "Delete ??";
  }
}
