#ifndef CQIllustratorNPolyShape_H
#define CQIllustratorNPolyShape_H

#include <CQIllustratorShape.h>

class CQIllustratorNPolyShape : public CQIllustratorShape {
  Q_OBJECT

  Q_PROPERTY(int    numPoints READ getNum                   )
  Q_PROPERTY(double radius    READ getRadius WRITE setRadius)
  Q_PROPERTY(double angle     READ getAngle  WRITE setAngle )

 public:
  enum ControlPointType {
    TYPE_CENTER, TYPE_RADIUS
  };

 public:
  CQIllustratorNPolyShape(const CPoint2D &c, int n, double r, double a);
  CQIllustratorNPolyShape(const CQIllustratorNPolyShape &poly);

  CQIllustratorNPolyShape *dup() const override;

  const char *getClassName() const override { return "npoly"; }

  void getControlPoints(ControlPointList &points,
                        ControlType type=ControlType::GEOMETRY) const override;

  void setControlPoint(const CQIllustratorShapeControlPoint *point) override;

  CPoint2D getCenter() const override { return c_; }

  CPoint2D getRotateCenter() const override;

  int getNum() const { return n_; }
  void setNum(int n);

  double getRadius() const { return r_; }
  void setRadius(double r);

  double getAngle() const { return a_; }
  void setAngle(double a);

  CPoint2D getCenterPoint() const;
  void setCenterPoint(const CPoint2D &c);

  CPoint2D getPoint() const;
  void setPoint(const CPoint2D &p);

  bool getPolygon(CPolygon2D &polygon) const override;

  bool getPath(CPathShapePartList &path) const override;

  void moveBy(const CPoint2D &d) override;

  bool flip(bool x_axis) override;

  void drawShape(CQIllustratorShapeDrawer *drawer) const override;

  CQIllustratorShapeGeometry *getGeometry() override;

  void doSetGeometry(const CQIllustratorShapeGeometry *geom) override;

  void saveSVG(const CQIllustratorSaveData &saveData) override;
  void saveCmd(const CQIllustratorSaveData &saveData) override;

 protected:
  void updateBBox() const override;

  void getPoints(std::vector<CPoint2D> &p) const;

 protected:
  CPoint2D c_;
  int      n_ { 0 };
  double   r_ { 0 };
  double   a_ { 0 };
};

//------

class CQIllustratorNPolyShapeControlPoint : public CQIllustratorShapeControlPoint {
 public:
  CQIllustratorNPolyShapeControlPoint(CQIllustratorNPolyShape::ControlPointType type,
                                      const CPoint2D &p);

  CQIllustratorNPolyShapeControlPoint *dup() const override;

  CQIllustratorNPolyShape::ControlPointType getType() const { return type_; }

  CPoint2D getPoint(const CQIllustratorShape *shape) const override;

  void setPoint(CQIllustratorShape *shape, const CPoint2D &point) override;

 protected:
  CQIllustratorNPolyShape::ControlPointType type_;
};

#endif
