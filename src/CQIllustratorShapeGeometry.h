#ifndef CQIllustratorShapeGeometry_H
#define CQIllustratorShapeGeometry_H

class CQIllustratorShapeGeometry {
 public:
  CQIllustratorShapeGeometry(bool fixed, const CPoint2D &rcenter, const CMatrix2D &m) :
   fixed_(fixed), rcenter_(rcenter), m_(m) {
  }

  virtual ~CQIllustratorShapeGeometry() { }

  virtual CQIllustratorShapeGeometry *dup() = 0;

 protected:
  bool      fixed_ { false };
  CPoint2D  rcenter_;
  CMatrix2D m_;
};

#endif
