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

  CQIllustratorRectShape *dup() const override;

  const char *getClassName() const override { return "rect"; }

  void getControlPoints(ControlPointList &points,
                        ControlType type=ControlType::GEOMETRY) const override;

  void setControlPoint(const CQIllustratorShapeControlPoint *point) override;

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

  double getAngle() const;

 protected:
  void updateBBox() const override;

 protected:
  CPoint2D p1_, p2_;
  double   rx_ { 0 }, ry_ { 0 };
};

//------

class CQIllustratorRectShapeControlPoint : public CQIllustratorShapeControlPoint {
 public:
  CQIllustratorRectShapeControlPoint(CQIllustratorRectShape::ControlPointType type,
                                     const CPoint2D &p);

  CQIllustratorRectShapeControlPoint *dup() const override;

  CQIllustratorRectShape::ControlPointType getType() const { return type_; }

  CPoint2D getPoint(const CQIllustratorShape *shape) const override;

  void setPoint(CQIllustratorShape *shape, const CPoint2D &point) override;

 protected:
  CQIllustratorRectShape::ControlPointType type_;
};

#endif
