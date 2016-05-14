#ifndef CQIllustratorEllipseShape_H
#define CQIllustratorEllipseShape_H

#include <CQIllustratorShape.h>

class CQIllustratorEllipseShape : public CQIllustratorShape {
  Q_OBJECT

  Q_PROPERTY(double angle1 READ getAngle1 WRITE setAngle1)
  Q_PROPERTY(double angle2 READ getAngle2 WRITE setAngle2)

 public:
  enum class ConnectType {
    LINE,  // Line
    CENTER // Center
  };

  enum class ControlPointType {
    RX, RY, A1, A2
  };

 public:
  CQIllustratorEllipseShape(const CPoint2D &p1, const CPoint2D &p2,
                            double angle1=0.0, double angle2=360.0,
                            ConnectType connectType=ConnectType::LINE);
  CQIllustratorEllipseShape(const CQIllustratorEllipseShape &ellipse);

  CQIllustratorEllipseShape *dup() const;

  const char *getClassName() const { return "ellipse"; }

  void setConnectType(ConnectType connectType) { connectType_ = connectType; }

  ConnectType getConnectType() const { return connectType_; }

  void getControlPoints(ControlPointList &points,
                        ControlType type=ControlType::GEOMETRY) const;

  void setControlPoint(const CQIllustratorShapeControlPoint *point);

  double getAngle1() const;
  void setAngle1(double a1);

  double getAngle2() const;
  void setAngle2(double a2);

  CPoint2D getRXPoint() const;
  void setRXPoint(const CPoint2D &p);

  CPoint2D getRYPoint() const;
  void setRYPoint(const CPoint2D &p);

  CPoint2D getAngle1Point() const;
  void setAngle1Point(const CPoint2D &p);

  CPoint2D getAngle2Point() const;
  void setAngle2Point(const CPoint2D &p);

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

  double getAngle() const;

 protected:
  void updateBBox() const;

 protected:
  CPoint2D    p1_, p2_;
  double      angle1_ { 0 };
  double      angle2_ { 0 };
  ConnectType connectType_ { ConnectType::LINE };
};

//------

class CQIllustratorEllipseShapeControlPoint : public CQIllustratorShapeControlPoint {
 public:
  typedef CQIllustratorEllipseShape::ControlPointType ControlPointType;

 public:
  CQIllustratorEllipseShapeControlPoint(ControlPointType type, const CPoint2D &p);

  CQIllustratorEllipseShapeControlPoint *dup() const;

  CQIllustratorEllipseShape::ControlPointType getType() const { return type_; }

  CPoint2D getPoint(const CQIllustratorShape *shape) const;

  void setPoint(CQIllustratorShape *shape, const CPoint2D &point);

 protected:
  ControlPointType type_;
};

#endif
