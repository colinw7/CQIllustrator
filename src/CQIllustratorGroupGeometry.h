#ifndef CQIllustratorGroupGeometry_H
#define CQIllustratorGroupGeometry_H

#include <CQIllustratorShapeGeometry.h>

class CQIllustratorGroupGeometry : public CQIllustratorShapeGeometry {
 public:
  CQIllustratorGroupGeometry(bool fixed=false, const CPoint2D &rcenter=CPoint2D(0,0),
                             const CMatrix2D m=CMatrix2D()) :
   CQIllustratorShapeGeometry(fixed, rcenter, m) {
  }

  CQIllustratorGroupGeometry(const CQIllustratorGroupGeometry &geometry) :
   CQIllustratorShapeGeometry(geometry) {
  }

  CQIllustratorGroupGeometry *dup() override {
    return new CQIllustratorGroupGeometry(*this);
  }

  friend class CQIllustratorGroupShape;

 private:
  CQIllustratorGroupGeometry &operator=(const CQIllustratorGroupGeometry &);
};

#endif
