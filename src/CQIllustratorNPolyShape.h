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

  CQIllustratorNPolyShape *dup() const;

  const char *getClassName() const { return "npoly"; }

  void getControlPoints(ControlPointList &points, ControlType type=ControlType::GEOMETRY) const;

  void setControlPoint(const CQIllustratorShapeControlPoint *point);

  CPoint2D getCenter() const { return c_; }

  CPoint2D getRotateCenter() const;

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

  bool getPolygon(CPolygon2D &polygon) const;

  bool getPath(CPathShapePartList &path) const;

  void moveBy(const CPoint2D &d);

  bool flip(bool x_axis);

  void drawShape(CQIllustratorShapeDrawer *drawer) const;

  CQIllustratorShapeGeometry *getGeometry();

  void doSetGeometry(const CQIllustratorShapeGeometry *geom);

  void saveSVG(const CQIllustratorSaveData &saveData);
  void saveCmd(const CQIllustratorSaveData &saveData);

 protected:
  void updateBBox() const;

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

  CQIllustratorNPolyShapeControlPoint *dup() const;

  CQIllustratorNPolyShape::ControlPointType getType() const { return type_; }

  CPoint2D getPoint(const CQIllustratorShape *shape) const;

  void setPoint(CQIllustratorShape *shape, const CPoint2D &point);

 protected:
  CQIllustratorNPolyShape::ControlPointType type_;
};

#endif
