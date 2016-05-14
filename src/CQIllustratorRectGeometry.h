#ifndef CQIllustratorRectGeometry_H
#define CQIllustratorRectGeometry_H

#include <CQIllustratorShapeGeometry.h>

class CQIllustratorRectGeometry : public CQIllustratorShapeGeometry {
 public:
  CQIllustratorRectGeometry(bool fixed=false, const CPoint2D &rcenter=CPoint2D(0,0),
                            const CMatrix2D m=CMatrix2D(), const CPoint2D &p1=CPoint2D(),
                            const CPoint2D &p2=CPoint2D(), double rx=0.0, double ry=0.0) :
   CQIllustratorShapeGeometry(fixed, rcenter, m), p1_(p1), p2_(p2), rx_(rx), ry_(ry) {
  }

  CQIllustratorRectGeometry(const CQIllustratorRectGeometry &geometry) :
   CQIllustratorShapeGeometry(geometry), p1_(geometry.p1_), p2_(geometry.p2_),
   rx_(geometry.rx_), ry_(geometry.ry_) {
  }

  virtual CQIllustratorRectGeometry *dup() {
    return new CQIllustratorRectGeometry(*this);
  }

  friend class CQIllustratorRectShape;

 private:
  CQIllustratorRectGeometry &operator=(const CQIllustratorRectGeometry &);

 private:
  CPoint2D p1_, p2_;
  double   rx_ { 0 }, ry_ { 0 };
};

#endif
