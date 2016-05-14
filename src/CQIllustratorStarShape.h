#ifndef CQIllustratorStarShape_H
#define CQIllustratorStarShape_H

#include <CQIllustratorShape.h>

class CQIllustratorStarShape : public CQIllustratorShape {
  Q_OBJECT

  Q_PROPERTY(int    numPoints   READ getNum                             )
  Q_PROPERTY(double innerRadius READ getInnerRadius WRITE setInnerRadius)
  Q_PROPERTY(double outerRadius READ getOuterRadius WRITE setOuterRadius)
  Q_PROPERTY(double innerAngle  READ getInnerAngle  WRITE setInnerAngle )
  Q_PROPERTY(double outerAngle  READ getOuterAngle  WRITE setOuterAngle )

 public:
  enum class ControlPointType {
    CENTER,
    IR,
    OR
  };

 public:
  CQIllustratorStarShape(const CPoint2D &c, uint n, double r1, double r2, double a1, double a2);
  CQIllustratorStarShape(const CQIllustratorStarShape &star);

  CQIllustratorStarShape *dup() const;

  const char *getClassName() const { return "star"; }

  void getControlPoints(ControlPointList &points,
                        ControlType type=ControlType::GEOMETRY) const;

  void setControlPoint(const CQIllustratorShapeControlPoint *point);

  CPoint2D getCenter() const { return c_; }

  CPoint2D getRotateCenter() const;

  uint getNum() const { return n_; }
  void setNum(uint n);

  double getInnerRadius() const { return r1_; }
  void setInnerRadius(double r);

  double getOuterRadius() const { return r2_; }
  void setOuterRadius(double r);

  double getInnerAngle() const { return a1_; }
  void setInnerAngle(double a);

  double getOuterAngle() const { return a2_; }
  void setOuterAngle(double a);

  CPoint2D getCenterPoint() const;
  void setCenterPoint(const CPoint2D &c);

  CPoint2D getInnerPoint() const;
  void setInnerPoint(const CPoint2D &p);

  CPoint2D getOuterPoint() const;
  void setOuterPoint(const CPoint2D &p);

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

  void getPoints(std::vector<CPoint2D> &pi, std::vector<CPoint2D> &po) const;

 protected:
  CPoint2D c_;
  uint     n_ { 0 };
  double   r1_ { 0 }, r2_ { 0 };
  double   a1_ { 0 }, a2_ { 0 };
};

//------

class CQIllustratorStarShapeControlPoint : public CQIllustratorShapeControlPoint {
 public:
  CQIllustratorStarShapeControlPoint(CQIllustratorStarShape::ControlPointType type,
                                     const CPoint2D &p);

  CQIllustratorStarShapeControlPoint *dup() const;

  CQIllustratorStarShape::ControlPointType getType() const { return type_; }

  CPoint2D getPoint(const CQIllustratorShape *shape) const;

  void setPoint(CQIllustratorShape *shape, const CPoint2D &point);

 protected:
  CQIllustratorStarShape::ControlPointType type_;
};

#endif
