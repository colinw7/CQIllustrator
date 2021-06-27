#ifndef CQIllustratorShapeNearestPoint2D_H
#define CQIllustratorShapeNearestPoint2D_H

#include <CQIllustratorShapeControlPoint.h>

class CQIllustratorShapeNearestPoint2D {
 public:
  CQIllustratorShapeNearestPoint2D() :
   set_(false), dist_(0.0), p_(0) {
  }

  CQIllustratorShapeNearestPoint2D(const CQIllustratorShapeNearestPoint2D &p) :
   set_(p.set_), dist_(p.dist_), p_(p.p_ ? p.p_->dup() : 0) {
  }

  CQIllustratorShapeNearestPoint2D &operator=(const CQIllustratorShapeNearestPoint2D &p) {
    set_  = p.set_;
    dist_ = p.dist_;
    p_    = (p.p_ ? p.p_->dup() : 0);

    return *this;
  }

 ~CQIllustratorShapeNearestPoint2D() {
    delete p_;
  }

  bool isSet() const { return set_; }

  void updatePoint(CQIllustratorShapeControlPoint *p, double dist) {
    if (! set_ || dist < dist_) {
      set_  = true;
      dist_ = dist;

      delete p_;

      p_ = p->dup();
    }
  }

  double getDist() const { return dist_; }

  CQIllustratorShapeControlPoint *getPoint() const { return p_; }

 private:
  bool                            set_ { false };
  double                          dist_ { 0 };
  CQIllustratorShapeControlPoint *p_;
};

#endif
