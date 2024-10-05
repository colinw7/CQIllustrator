#ifndef CQIllustratorPolygonShape_H
#define CQIllustratorPolygonShape_H

#include <CQIllustratorShape.h>

class CQIllustratorPolygonShape : public CQIllustratorShape {
  Q_OBJECT

 public:
  CQIllustratorPolygonShape(const PointList &points=PointList());
  CQIllustratorPolygonShape(const CQIllustratorPolygonShape &polygon);

  CQIllustratorPolygonShape *dup() const override;

  const char *getClassName() const override { return "polygon"; }

  void setPoints(const CPolygon2D &polygon);

  void addPoint(const CPoint2D &p);

  double distance(const CPoint2D &p) const override;

  bool inside(const CPoint2D &p) const override;

  CPoint2D getPoint(uint i) const;

  void setPoint(uint i, const CPoint2D &p);

  void getControlPoints(ControlPointList &points,
                        ControlType type=ControlType::GEOMETRY) const override;

  void setControlPoint(const CQIllustratorShapeControlPoint *point) override;

  bool addPoint(const CQIllustratorShapeControlPoint *before_point, const CPoint2D &p);

  bool doRemovePoint(const CQIllustratorShapeControlPoint *point) override;

  bool getPolygon(CPolygon2D &polygon) const override;

  bool getPath(CPathShapePartList &path) const override;

  void moveBy(const CPoint2D &d) override;

  void resizeBy(double dw, double dh) override;

  bool flip(bool x_axis) override;

  void drawShape(CQIllustratorShapeDrawer *drawer) const override;

  CQIllustratorShapeGeometry *getGeometry() override;

  void doSetGeometry(const CQIllustratorShapeGeometry *geom) override;

  void saveSVG(const CQIllustratorSaveData &saveData) override;
  void saveCmd(const CQIllustratorSaveData &saveData) override;

 protected:
  void updateBBox() const override;

 protected:
  PointList points_;
};

//------

class CQIllustratorPolygonShapeControlPoint : public CQIllustratorShapeControlPoint {
 public:
  CQIllustratorPolygonShapeControlPoint(uint ind, const CPoint2D &p);

  CQIllustratorPolygonShapeControlPoint *dup() const override;

  CPoint2D getPoint(const CQIllustratorShape *shape) const override;

  void setPoint(CQIllustratorShape *shape, const CPoint2D &point) override;

  int getInd() const { return ind_; }

 protected:
  int ind_;
};

#endif
