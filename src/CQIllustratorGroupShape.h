#ifndef CQIllustratorGroupShape_H
#define CQIllustratorGroupShape_H

#include <CQIllustratorShape.h>

class CQIllustratorGroupShape : public CQIllustratorShape {
  Q_OBJECT

 public:
  CQIllustratorGroupShape();
  CQIllustratorGroupShape(const CQIllustratorGroupShape &group);

  CQIllustratorGroupShape *dup() const override;

  const char *getClassName() const override { return "group"; }

  void childrenChanged() override;

  void getControlPoints(ControlPointList &points,
                        ControlType type=ControlType::GEOMETRY) const override;

  void setControlPoint(const CQIllustratorShapeControlPoint *point) override;

  void scale(const CPoint2D &c, double sx, double sy) override;

  void rotate(double da) override;

  void transform(const CPoint2D &c, const CMatrix2D &m) override;

  bool getPolygon(CPolygon2D &polygon) const override;

  bool getPath(CPathShapePartList &path) const override;

  void getPolygons(std::vector<CPolygon2D> &polygons) const;

  void moveBy(const CPoint2D &d) override;

  bool flip(bool x_axis) override;

  void drawShape(CQIllustratorShapeDrawer *drawer) const override;

  CQIllustratorShapeGeometry *getGeometry() override;

  void doSetGeometry(const CQIllustratorShapeGeometry *geom) override;

  void saveSVG(const CQIllustratorSaveData &saveData) override;
  void saveCmd(const CQIllustratorSaveData &saveData) override;

 protected:
  void updateBBox() const override;
};

//------

class CQIllustratorGroupShapeControlPoint : public CQIllustratorShapeControlPoint {
 public:
  CQIllustratorGroupShapeControlPoint(uint ind, const CPoint2D &p);

  CQIllustratorGroupShapeControlPoint *dup() const override;

  CPoint2D getPoint(const CQIllustratorShape *shape) const override;

  void setPoint(CQIllustratorShape *shape, const CPoint2D &point) override;

  int getInd() const { return ind_; }

 protected:
  int ind_ { 0 };
};

#endif
