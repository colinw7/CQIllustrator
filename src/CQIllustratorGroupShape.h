#ifndef CQIllustratorGroupShape_H
#define CQIllustratorGroupShape_H

#include <CQIllustratorShape.h>

class CQIllustratorGroupShape : public CQIllustratorShape {
  Q_OBJECT

 public:
  CQIllustratorGroupShape();
  CQIllustratorGroupShape(const CQIllustratorGroupShape &group);

  CQIllustratorGroupShape *dup() const;

  const char *getClassName() const { return "group"; }

  void childrenChanged();

  void getControlPoints(ControlPointList &points,
                        ControlType type=ControlType::GEOMETRY) const;

  void setControlPoint(const CQIllustratorShapeControlPoint *point);

  void scale(const CPoint2D &c, double sx, double sy);

  void rotate(double da);

  void transform(const CPoint2D &c, const CMatrix2D &m);

  bool getPolygon(CPolygon2D &polygon) const;

  bool getPath(CPathShapePartList &path) const;

  void getPolygons(std::vector<CPolygon2D> &polygons) const;

  void moveBy(const CPoint2D &d);

  bool flip(bool x_axis);

  void drawShape(CQIllustratorShapeDrawer *drawer) const;

  CQIllustratorShapeGeometry *getGeometry();

  void doSetGeometry(const CQIllustratorShapeGeometry *geom);

  void saveSVG(const CQIllustratorSaveData &saveData);
  void saveCmd(const CQIllustratorSaveData &saveData);

 protected:
  void updateBBox() const;
};

//------

class CQIllustratorGroupShapeControlPoint : public CQIllustratorShapeControlPoint {
 public:
  CQIllustratorGroupShapeControlPoint(uint ind, const CPoint2D &p);

  CQIllustratorGroupShapeControlPoint *dup() const;

  CPoint2D getPoint(const CQIllustratorShape *shape) const;

  void setPoint(CQIllustratorShape *shape, const CPoint2D &point);

  int getInd() const { return ind_; }

 protected:
  int ind_ { 0 };
};

#endif
