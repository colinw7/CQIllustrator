#ifndef CQIllustratorRectShape_H
#define CQIllustratorRectShape_H

#include <CQIllustratorShape.h>

class CQIllustratorRectShape : public CQIllustratorShape {
  Q_OBJECT

  Q_PROPERTY(double radiusX READ getRadiusX WRITE setRadiusX)
  Q_PROPERTY(double radiusY READ getRadiusY WRITE setRadiusY)

 public:
  enum class ControlPointType {
    LL, UR, RX, RY
  };

 public:
  CQIllustratorRectShape(const CPoint2D &p1, const CPoint2D &p2, double rx=0.0, double ry=0.0);
  CQIllustratorRectShape(const CQIllustratorRectShape &rect);

  CQIllustratorRectShape *dup() const;

  const char *getClassName() const { return "rect"; }

  void getControlPoints(ControlPointList &points, ControlType type=ControlType::GEOMETRY) const;

  void setControlPoint(const CQIllustratorShapeControlPoint *point);

  double getRadiusX() const { return rx_; }
  void setRadiusX(double rx);

  double getRadiusY() const { return ry_; }
  void setRadiusY(double ry);

  CPoint2D getLL() const;
  void setLL(const CPoint2D &p);

  CPoint2D getUR() const;
  void setUR(const CPoint2D &p);

  CPoint2D getRXPoint() const;
  void setRXPoint(const CPoint2D &p);

  CPoint2D getRYPoint() const;
  void setRYPoint(const CPoint2D &p);

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
  CPoint2D p1_, p2_;
  double   rx_ { 0 }, ry_ { 0 };
};

//------

class CQIllustratorRectShapeControlPoint : public CQIllustratorShapeControlPoint {
 public:
  CQIllustratorRectShapeControlPoint(CQIllustratorRectShape::ControlPointType type,
                                     const CPoint2D &p);

  CQIllustratorRectShapeControlPoint *dup() const;

  CQIllustratorRectShape::ControlPointType getType() const { return type_; }

  CPoint2D getPoint(const CQIllustratorShape *shape) const;

  void setPoint(CQIllustratorShape *shape, const CPoint2D &point);

 protected:
  CQIllustratorRectShape::ControlPointType type_;
};

#endif
