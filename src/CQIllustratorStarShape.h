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

  CQIllustratorStarShape *dup() const override;

  const char *getClassName() const override { return "star"; }

  void getControlPoints(ControlPointList &points,
                        ControlType type=ControlType::GEOMETRY) const override;

  void setControlPoint(const CQIllustratorShapeControlPoint *point) override;

  CPoint2D getCenter() const override { return c_; }

  CPoint2D getRotateCenter() const override;

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

  CQIllustratorStarShapeControlPoint *dup() const override;

  CQIllustratorStarShape::ControlPointType getType() const { return type_; }

  CPoint2D getPoint(const CQIllustratorShape *shape) const override;

  void setPoint(CQIllustratorShape *shape, const CPoint2D &point) override;

 protected:
  CQIllustratorStarShape::ControlPointType type_;
};

#endif
