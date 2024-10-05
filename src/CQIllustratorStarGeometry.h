#ifndef CQIllustratorStarGeometry_H
#define CQIllustratorStarGeometry_H

#include <CQIllustratorShapeGeometry.h>

class CQIllustratorStarGeometry : public CQIllustratorShapeGeometry {
 public:
  CQIllustratorStarGeometry(bool fixed=false, const CPoint2D &rcenter=CPoint2D(0,0),
                            const CMatrix2D m=CMatrix2D(), const CPoint2D &c=CPoint2D(),
                            uint n=5, double r1=0.0, double r2=0.0, double a1=0.0, double a2=0.0) :
   CQIllustratorShapeGeometry(fixed, rcenter, m), c_(c), n_(n), r1_(r1), r2_(r2), a1_(a1), a2_(a2) {
  }

  CQIllustratorStarGeometry(const CQIllustratorStarGeometry &geometry) :
   CQIllustratorShapeGeometry(geometry), c_(geometry.c_), n_(geometry.n_), r1_(geometry.r1_),
   r2_(geometry.r2_), a1_(geometry.a1_), a2_(geometry.a2_) {
  }

  CQIllustratorStarGeometry *dup() override {
    return new CQIllustratorStarGeometry(*this);
  }

  friend class CQIllustratorStarShape;

 private:
  CQIllustratorStarGeometry &operator=(const CQIllustratorStarGeometry &);

 protected:
  CPoint2D c_;
  uint     n_ { 0 };
  double   r1_ { 0 }, r2_ { 0 };
  double   a1_ { 0 }, a2_ { 0 };
};

#endif
