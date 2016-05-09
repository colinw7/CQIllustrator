#ifndef CQIllustratorData_H
#define CQIllustratorData_H

#include <QObject>

#include <CStack.h>
#include <CBBox2D.h>
#include <CQIllustratorQuadTree.h>

class CQIllustrator;
class CQIllustratorShape;

class CQIllustratorData : public QObject {
  Q_OBJECT

 public:
  enum ChangeType {
    CHANGE_NAME,
    CHANGE_GEOMETRY,
    CHANGE_STROKE,
    CHANGE_FILL,
    CHANGE_FONT,
    CHANGE_GENERIC
  };

  typedef CStack<CQIllustratorShape>                        ShapeStack;
  typedef std::vector<CQIllustratorShape *>                 ShapeList;
  typedef CQIllustratorQuadTree<CQIllustratorShape,CBBox2D> QuadTree;

 public:
  CQIllustratorData(CQIllustrator *illustrator);
 ~CQIllustratorData();

  const ShapeStack &getShapes() const { return shapes_; }

  const QuadTree *getQuadTree() const { return quadTree_; }

  void raiseShape(CQIllustratorShape *shape);
  void lowerShape(CQIllustratorShape *shape);

  void addShape(CQIllustratorShape *shape);

  void removeShape(CQIllustratorShape *shape);

  void checkoutShape(CQIllustratorShape *shape, ChangeType changeType);
  void checkinShape (CQIllustratorShape *shape, ChangeType changeType);

  void getObjectsAt(const CPoint2D &p, ShapeList &shapes) const;

  CQIllustratorShape *getObjectAt(const CPoint2D &p) const;

  void getObjectsInside(const CBBox2D &bbox, ShapeList &shapes);

  void getObjectsTouching(const CBBox2D &bbox, ShapeList &shapes);

  void getOrderedObjectsTouching(const CBBox2D &bbox, ShapeList &shapes);

 signals:
  void objectCreated(CQIllustratorShape *shape);
  void objectDeleted(CQIllustratorShape *shape);

  void objectPreModify (CQIllustratorShape *shape, ChangeType changeType);
  void objectPostModify(CQIllustratorShape *shape, ChangeType changeType);

 private:
  CQIllustrator *illustrator_;
  ShapeStack     shapes_;
  QuadTree      *quadTree_;
};

#endif
