#ifndef CQIllustratorPolygonShape_H
#define CQIllustratorPolygonShape_H

#include <CQIllustratorShape.h>

class CQIllustratorPolygonShape : public CQIllustratorShape {
  Q_OBJECT

 public:
  CQIllustratorPolygonShape(const PointList &points=PointList());
  CQIllustratorPolygonShape(const CQIllustratorPolygonShape &polygon);

  CQIllustratorPolygonShape *dup() const;

  const char *getClassName() const { return "polygon"; }

  void setPoints(const CPolygon2D &polygon);

  void addPoint(const CPoint2D &p);

  double distance(const CPoint2D &p) const;

  bool inside(const CPoint2D &p) const;

  CPoint2D getPoint(uint i) const;

  void setPoint(uint i, const CPoint2D &p);

  void getControlPoints(ControlPointList &points,
                        ControlType type=ControlType::GEOMETRY) const;

  void setControlPoint(const CQIllustratorShapeControlPoint *point);

  bool addPoint(const CQIllustratorShapeControlPoint *before_point, const CPoint2D &p);

  bool doRemovePoint(const CQIllustratorShapeControlPoint *point);

  bool getPolygon(CPolygon2D &polygon) const;

  bool getPath(CPathShapePartList &path) const;

  void moveBy(const CPoint2D &d);

  void resizeBy(double dw, double dh);

  bool flip(bool x_axis);

  void drawShape(CQIllustratorShapeDrawer *drawer) const;

  CQIllustratorShapeGeometry *getGeometry();

  void doSetGeometry(const CQIllustratorShapeGeometry *geom);

  void saveSVG(const CQIllustratorSaveData &saveData);
  void saveCmd(const CQIllustratorSaveData &saveData);

 protected:
  void updateBBox() const;

 protected:
  PointList points_;
};

//------

class CQIllustratorPolygonShapeControlPoint : public CQIllustratorShapeControlPoint {
 public:
  CQIllustratorPolygonShapeControlPoint(uint ind, const CPoint2D &p);

  CQIllustratorPolygonShapeControlPoint *dup() const;

  CPoint2D getPoint(const CQIllustratorShape *shape) const;

  void setPoint(CQIllustratorShape *shape, const CPoint2D &point);

  int getInd() const { return ind_; }

 protected:
  int ind_;
};

#endif
