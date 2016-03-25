#include <CQIllustratorData.h>
#include <CQIllustratorShape.h>
#include <set>

CQIllustratorData::
CQIllustratorData(CQIllustrator *illustrator) :
 illustrator_(illustrator)
{
  quadTree_ = new QuadTree(CBBox2D(0, 0, 1000, 1000));
}

CQIllustratorData::
~CQIllustratorData()
{
  delete quadTree_;
}

void
CQIllustratorData::
raiseShape(CQIllustratorShape *shape)
{
  shapes_.raise(shape);
}

void
CQIllustratorData::
lowerShape(CQIllustratorShape *shape)
{
  shapes_.lower(shape);
}

void
CQIllustratorData::
addShape(CQIllustratorShape *shape)
{
  shape->setData(this);

  shapes_.push(shape);

  if (shape->getBBox().isSet())
    quadTree_->add(shape);

  emit objectCreated(shape);
}

void
CQIllustratorData::
removeShape(CQIllustratorShape *shape)
{
  shapes_.remove(shape);

  quadTree_->remove(shape);

  shape->setData(0);

  emit objectDeleted(shape);
}

void
CQIllustratorData::
checkoutShape(CQIllustratorShape *shape, ChangeType changeType)
{
  if (! shape->getLocked()) {
    emit objectPreModify(shape, changeType);

    quadTree_->remove(shape);

    //const CQIllustratorShape::ShapeList &children = shape->getChildren();

    //CQIllustratorShape::ShapeList::const_iterator pc1, pc2;

    //for (pc1 = children.begin(), pc2 = children.end(); pc1 != pc2; ++pc1) {
    //  const CQIllustratorShape *child = *pc1;

    //  checkoutShape(child, changeType);
    //}
  }

  shape->lockShape();
}

void
CQIllustratorData::
checkinShape(CQIllustratorShape *shape, ChangeType changeType)
{
  shape->unlockShape();

  if (! shape->getLocked()) {
    //const CQIllustratorShape::ShapeList &children = shape->getChildren();

    //CQIllustratorShape::ShapeList::const_iterator pc1, pc2;

    //for (pc1 = children.begin(), pc2 = children.end(); pc1 != pc2; ++pc1) {
    //  const CQIllustratorShape *child = *pc1;

    //  checkinShape(child, changeType);
    //}

    quadTree_->add(shape);

    emit objectPostModify(shape, changeType);
  }
}

void
CQIllustratorData::
getObjectsAt(const CPoint2D &p, ShapeList &shapes) const
{
  QuadTree::DataList dataList;

  quadTree_->getDataAtPoint(p.x, p.y, dataList);

  QuadTree::DataList::const_iterator ps1, ps2;

  for (ps1 = dataList.begin(), ps2 = dataList.end(); ps1 != ps2; ++ps1) {
    CQIllustratorShape *shape = *ps1;

    if (shape->getParent()) continue;

    shapes.push_back(shape);
  }
}

CQIllustratorShape *
CQIllustratorData::
getObjectAt(const CPoint2D &p) const
{
  ShapeList shapes;

  getObjectsAt(p, shapes);

  CQIllustratorShape *select_shape = 0;
  double              select_area  = 0.0;

  ShapeList::const_iterator ps1, ps2;

  for (ps1 = shapes.begin(), ps2 = shapes.end(); ps1 != ps2; ++ps1) {
    CQIllustratorShape *shape = *ps1;

    double area = shape->getBBox().area();

    if (select_shape == 0 || area < select_area) {
      select_shape = shape;
      select_area  = area;
    }
  }

  return select_shape;
}

void
CQIllustratorData::
getObjectsInside(const CBBox2D &bbox, ShapeList &shapes)
{
  QuadTree::DataList dataList;

  quadTree_->getDataInsideBBox(bbox, dataList);

  QuadTree::DataList::const_iterator ps1, ps2;

  for (ps1 = dataList.begin(), ps2 = dataList.end(); ps1 != ps2; ++ps1) {
    CQIllustratorShape *shape = *ps1;

    if (shape->getParent()) continue;

    shapes.push_back(shape);
  }
}

void
CQIllustratorData::
getObjectsTouching(const CBBox2D &bbox, ShapeList &shapes)
{
  QuadTree::DataList dataList;

  quadTree_->getDataTouchingBBox(bbox, dataList);

  QuadTree::DataList::const_iterator ps1, ps2;

  for (ps1 = dataList.begin(), ps2 = dataList.end(); ps1 != ps2; ++ps1) {
    CQIllustratorShape *shape = *ps1;

    if (shape->getParent()) continue;

    shapes.push_back(shape);
  }
}

void
CQIllustratorData::
getOrderedObjectsTouching(const CBBox2D &bbox, ShapeList &shapes)
{
  typedef std::set<CQIllustratorShape *> ShapeSet;

  ShapeSet shapeSet;

  QuadTree::DataList dataList;

  quadTree_->getDataTouchingBBox(bbox, dataList);

  QuadTree::DataList::const_iterator ps1, ps2;

  for (ps1 = dataList.begin(), ps2 = dataList.end(); ps1 != ps2; ++ps1) {
    CQIllustratorShape *shape = *ps1;

    if (shape->getParent()) continue;

    shapeSet.insert(shape);
  }

  ShapeStack::const_iterator p1, p2;

  for (p1 = shapes_.begin(), p2 = shapes_.end(); p1 != p2; ++p1) {
    CQIllustratorShape *shape = *p1;

    if (shapeSet.find(shape) != shapeSet.end())
      shapes.push_back(shape);
  }
}
