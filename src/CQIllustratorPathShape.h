#ifndef CQIllustratorPathShape_H
#define CQIllustratorPathShape_H

#include <CQIllustratorShape.h>

class CQIllustratorPathShape : public CQIllustratorShape {
  Q_OBJECT

 public:
  CQIllustratorPathShape(const CPathShapePartList &parts=CPathShapePartList());
  CQIllustratorPathShape(const CQIllustratorPathShape &path);

  CQIllustratorPathShape *dup() const override;

  const char *getClassName() const override { return "path"; }

  const CPathShapePartList &getParts() const { return parts_; }

  bool getPolygon(CPolygon2D &polygon) const override;

  bool getPath(CPathShapePartList &path) const override;

  void moveBy(const CPoint2D &d) override;

  void startGroup();
  void endGroup();

  void setParts(const CPathShapePartList &parts);

  void addMoveTo(const CPoint2D &p);
  void addLineTo(const CPoint2D &p);
  void addCurveTo(const CPoint2D &p1, const CPoint2D &p2);
  void addCurveTo(const CPoint2D &p1, const CPoint2D &p2, const CPoint2D &p3);
  void addArcTo(const CPoint2D &c, double rx, double ry, double angle1, double angle2);
  void addClose();

  void addLineTo(uint ind, const CPoint2D &p);

  bool empty() const;

  double distance(const CPoint2D &p) const override;

  bool inside(const CPoint2D &p) const override;

  void getControlLines(ControlLineList &lines) const override;

  void getLines(std::vector<CLine2D> &lines) const;

  void getControlPoints(ControlPointList &points,
                        ControlType type=ControlType::GEOMETRY) const override;

  void setControlPoint(const CQIllustratorShapeControlPoint *point) override;

  bool doRemovePoint(const CQIllustratorShapeControlPoint *point) override;

  CLine2D getLine(uint ind) const;

  void setLine(uint ind, const CLine2D &l);

  void setLineType(uint ind, CPathPartType type);

  CPoint2D getPoint(uint ind, uint ind1) const;

  void setPoint(uint ind, uint ind1, const CPoint2D &p);

  void setCornerPoint(uint ind);
  void setCurvePoint(uint ind);

  bool flip(bool x_axis) override;

  void drawShape(CQIllustratorShapeDrawer *drawer) const override;

  CQIllustratorShapeGeometry *getGeometry() override;

  void doSetGeometry(const CQIllustratorShapeGeometry *geom) override;

  void saveSVG(const CQIllustratorSaveData &saveData) override;
  void saveCmd(const CQIllustratorSaveData &saveData) override;

  void processPath(CPathShapeProcess *process);

 protected:
  void updateBBox() const override;

 protected:
  CPathShapePartList parts_;
  mutable int        group_ { 0 };
};

#endif
