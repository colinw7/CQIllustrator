#ifndef CQIllustratorNPolyGeometry_H
#define CQIllustratorNPolyGeometry_H

#include <CQIllustratorShapeGeometry.h>

class CQIllustratorNPolyGeometry : public CQIllustratorShapeGeometry {
 public:
  CQIllustratorNPolyGeometry(bool fixed=false, const CPoint2D &rcenter=CPoint2D(0,0),
                             const CMatrix2D m=CMatrix2D(), const CPoint2D &c=CPoint2D(),
                             uint n=5, double r=0.0, double a=0.0) :
   CQIllustratorShapeGeometry(fixed, rcenter, m), c_(c), n_(n), r_(r), a_(a) {
  }

  CQIllustratorNPolyGeometry(const CQIllustratorNPolyGeometry &geometry) :
   CQIllustratorShapeGeometry(geometry), c_(geometry.c_), n_(geometry.n_),
   r_(geometry.r_), a_(geometry.a_) {
  }

  virtual CQIllustratorNPolyGeometry *dup() {
    return new CQIllustratorNPolyGeometry(*this);
  }

  friend class CQIllustratorNPolyShape;

 private:
  CQIllustratorNPolyGeometry &operator=(const CQIllustratorNPolyGeometry &);

 protected:
  CPoint2D c_;
  uint     n_ { 5 };
  double   r_ { 0.0 };
  double   a_ { 0.0 };
};

#endif
