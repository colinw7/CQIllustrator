#ifndef CQIllustratorPathShape_H
#define CQIllustratorPathShape_H

#include <CQIllustratorShape.h>

class CQIllustratorPathShape : public CQIllustratorShape {
  Q_OBJECT

 public:
  CQIllustratorPathShape(const CPathShapePartList &parts=CPathShapePartList());
  CQIllustratorPathShape(const CQIllustratorPathShape &path);

  CQIllustratorPathShape *dup() const;

  const char *getClassName() const { return "path"; }

  const CPathShapePartList &getParts() const { return parts_; }

  bool getPolygon(CPolygon2D &polygon) const;

  bool getPath(CPathShapePartList &path) const;

  void moveBy(const CPoint2D &d);

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

  double distance(const CPoint2D &p) const;

  bool inside(const CPoint2D &p) const;

  void getControlLines(ControlLineList &lines) const;

  void getLines(std::vector<CLine2D> &lines) const;

  void getControlPoints(ControlPointList &points,
                        ControlType type=ControlType::GEOMETRY) const;

  void setControlPoint(const CQIllustratorShapeControlPoint *point);

  bool doRemovePoint(const CQIllustratorShapeControlPoint *point);

  CLine2D getLine(uint ind) const;

  void setLine(uint ind, const CLine2D &l);

  void setLineType(uint ind, CPathPartType type);

  CPoint2D getPoint(uint ind, uint ind1) const;

  void setPoint(uint ind, uint ind1, const CPoint2D &p);

  void setCornerPoint(uint ind);
  void setCurvePoint(uint ind);

  bool flip(bool x_axis);

  void drawShape(CQIllustratorShapeDrawer *drawer) const;

  CQIllustratorShapeGeometry *getGeometry();

  void doSetGeometry(const CQIllustratorShapeGeometry *geom);

  void saveSVG(const CQIllustratorSaveData &saveData);
  void saveCmd(const CQIllustratorSaveData &saveData);

  void processPath(CPathShapeProcess *process);

 protected:
  void updateBBox() const;

 protected:
  CPathShapePartList parts_;
  mutable int        group_ { 0 };
};

#endif
