#ifndef CQIllustratorEllipseGeometry_H
#define CQIllustratorEllipseGeometry_H

#include <CQIllustratorShapeGeometry.h>

class CQIllustratorEllipseGeometry : public CQIllustratorShapeGeometry {
 public:
  typedef CQIllustratorEllipseShape::ConnectType ConnectType;

 public:
  CQIllustratorEllipseGeometry(bool fixed=false, const CPoint2D &rcenter=CPoint2D(0,0),
                               const CMatrix2D m=CMatrix2D(), const CPoint2D &p1=CPoint2D(),
                               const CPoint2D &p2=CPoint2D(), double angle1=0.0,
                               double angle2=360.0,
                               ConnectType connectType= ConnectType::LINE) :
   CQIllustratorShapeGeometry(fixed, rcenter, m), p1_(p1), p2_(p2),
                              angle1_(angle1), angle2_(angle2), connectType_(connectType) {
  }

  CQIllustratorEllipseGeometry(const CQIllustratorEllipseGeometry &geometry) :
   CQIllustratorShapeGeometry(geometry), p1_(geometry.p1_), p2_(geometry.p2_),
   angle1_(geometry.angle1_), angle2_(geometry.angle2_) {
  }

  CQIllustratorEllipseGeometry *dup() override {
    return new CQIllustratorEllipseGeometry(*this);
  }

  friend class CQIllustratorEllipseShape;

 private:
  CQIllustratorEllipseGeometry &operator=(const CQIllustratorEllipseGeometry &);

 protected:
  CPoint2D    p1_, p2_;
  double      angle1_ { 0.0 }, angle2_ { 360.0 };
  ConnectType connectType_ { ConnectType::LINE };
};

#endif
