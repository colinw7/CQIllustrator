#include <CQIllustratorSandbox.h>
#include <CQIllustrator.h>

CQIllustratorSandbox::
CQIllustratorSandbox(CQIllustrator *illustrator) :
 illustrator_(illustrator)
{
}

void
CQIllustratorSandbox::
addSelection()
{
  clear();

  CQIllustratorSelectedShapes *selection = illustrator_->getSelection();

  CQIllustratorSelectedShapes::iterator ps1, ps2;

  for (ps1 = selection->begin(), ps2 = selection->end(); ps1 != ps2; ++ps1) {
    CQIllustratorShape *shape = (*ps1).getShape();

    if (shape->getFixed()) continue;

    shape->setVisible(false);

    uint id = shape->getId();

    CQIllustratorShape *shape1 = shape->dup();

    shape1->setId(id);

    edit_shapes_.push_back(shape1);
  }
}

void
CQIllustratorSandbox::
addSelectionPath()
{
  clear();

  CQIllustratorSelectedShapes *selection = illustrator_->getSelection();

  CQIllustratorSelectedShapes::iterator ps1, ps2;

  for (ps1 = selection->begin(), ps2 = selection->end(); ps1 != ps2; ++ps1) {
    CQIllustratorShape *shape = (*ps1).getShape();

    if (shape->getFixed()) continue;

    CPathShapePartList parts;

    if (! shape->getPath(parts)) continue;

    CPathShape *pshape = illustrator_->createPathShape();

    pshape->setParts(parts);

    pshape->setStroke(shape->getStroke());
    pshape->setFill  (shape->getFill ());

    edit_shapes_.push_back(pshape);
  }
}

void
CQIllustratorSandbox::
clear()
{
  EditShapes::iterator ps1, ps2;

  for (ps1 = edit_shapes_.begin(), ps2 = edit_shapes_.end(); ps1 != ps2; ++ps1) {
    CQIllustratorShape *shape = *ps1;

    CQIllustratorShape *shape1 = illustrator_->getShape(shape->getId());

    if (shape1)
      shape1->setVisible(true);

    delete shape;
  }

  edit_shapes_.clear();
}

CBBox2D
CQIllustratorSandbox::
getBBox() const
{
  CBBox2D bbox;

  EditShapes::const_iterator ps1, ps2;

  for (ps1 = edit_shapes_.begin(), ps2 = edit_shapes_.end(); ps1 != ps2; ++ps1) {
    CQIllustratorShape *shape = *ps1;

    bbox += shape->getBBox();
  }

  return bbox;
}

void
CQIllustratorSandbox::
moveBy(const CPoint2D &d)
{
  EditShapes::iterator ps1, ps2;

  for (ps1 = edit_shapes_.begin(), ps2 = edit_shapes_.end(); ps1 != ps2; ++ps1) {
    CQIllustratorShape *shape = *ps1;

    shape->moveBy(d);
  }
}

void
CQIllustratorSandbox::
commit(CQIllustratorData::ChangeType changeType)
{
  illustrator_->startUndoGroup("Commit");

  EditShapes::iterator ps1, ps2;

  for (ps1 = edit_shapes_.begin(), ps2 = edit_shapes_.end(); ps1 != ps2; ++ps1) {
    CQIllustratorShape *shape = *ps1;

    CQIllustratorShape *shape1 = illustrator_->getShape(shape->getId());
    assert(shape1);

    if      (changeType == CQIllustratorData::CHANGE_GEOMETRY) {
      CQIllustratorShapeGeometry *geom = shape->getGeometry();

      shape1->setGeometry(geom);

      delete geom;
    }
    else if (changeType == CQIllustratorData::CHANGE_STROKE) {
      const CQIllustratorShapeStroke &stroke = shape->getStroke();

      shape1->setStroke(stroke);
    }
    else if (changeType == CQIllustratorData::CHANGE_FILL) {
      const CQIllustratorShapeFill &fill = shape->getFill();

      shape1->setFill(fill);
    }
    else
      assert(false);
  }

  illustrator_->endUndoGroup();

  clear();

  illustrator_->redraw();
}

void
CQIllustratorSandbox::
draw(CQIllustratorShapeDrawer *drawer)
{
  EditShapes::iterator ps1, ps2;

  for (ps1 = edit_shapes_.begin(), ps2 = edit_shapes_.end(); ps1 != ps2; ++ps1) {
    CQIllustratorShape *shape = *ps1;

    shape->draw(drawer);
  }
}
