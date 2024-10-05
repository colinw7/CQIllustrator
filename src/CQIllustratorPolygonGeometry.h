#ifndef CQIllustratorPolygonGeometry_H
#define CQIllustratorPolygonGeometry_H

#include <CQIllustratorShapeGeometry.h>

class CQIllustratorPolygonGeometry : public CQIllustratorShapeGeometry {
 public:
  typedef std::vector<CPoint2D> PointList;

 public:
  CQIllustratorPolygonGeometry(bool fixed=false, const CPoint2D &rcenter=CPoint2D(0,0),
                               const CMatrix2D m=CMatrix2D(), const PointList points=PointList()) :
   CQIllustratorShapeGeometry(fixed, rcenter, m), points_(points) {
  }

  CQIllustratorPolygonGeometry(const CQIllustratorPolygonGeometry &geometry) :
   CQIllustratorShapeGeometry(geometry), points_(geometry.points_) {
  }

  CQIllustratorPolygonGeometry *dup() override {
    return new CQIllustratorPolygonGeometry(*this);
  }

  friend class CQIllustratorPolygonShape;

 private:
  CQIllustratorPolygonGeometry &operator=(const CQIllustratorPolygonGeometry &);

 protected:
  PointList points_;
};

#endif
