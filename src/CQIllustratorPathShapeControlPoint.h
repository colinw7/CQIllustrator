#ifndef CQIllustratorPathShapeControlPoint_H
#define CQIllustratorPathShapeControlPoint_H

#include <CQIllustratorShapeControlPoint.h>

class CQIllustratorPathShapeControlPoint : public CQIllustratorShapeControlPoint {
 public:
  CQIllustratorPathShapeControlPoint(uint ind, const CPoint2D &p);

  CQIllustratorPathShapeControlPoint(uint ind, uint ind1, const CPoint2D &p);

  CQIllustratorPathShapeControlPoint *dup() const;

  CPoint2D getPoint(const CQIllustratorShape *shape) const;

  void setPoint(CQIllustratorShape *shape, const CPoint2D &point);

  uint getInd () const { return ind_ ; }
  uint getInd1() const { return ind1_; }

  bool isMaster() const { return (ind1_ == 0); }

  void setCornerNode(CQIllustratorShape *shape) const;
  void setCurveNode (CQIllustratorShape *shape) const;

 protected:
  uint ind_  { 0 };
  uint ind1_ { 0 };
};

#endif
