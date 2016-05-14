#ifndef CQIllustratorTextGeometry_H
#define CQIllustratorTextGeometry_H

#include <CQIllustratorShapeGeometry.h>

class CQIllustratorTextGeometry : public CQIllustratorShapeGeometry {
 public:
  CQIllustratorTextGeometry(bool fixed=false, const CPoint2D &rcenter=CPoint2D(0,0),
                            const CMatrix2D m=CMatrix2D(), const CPoint2D &p1=CPoint2D(),
                            const CPoint2D &p2=CPoint2D(), const std::string &str="",
                            CFontPtr font=CFontPtr(), CHAlignType halign=CHALIGN_TYPE_CENTER,
                            CVAlignType valign=CVALIGN_TYPE_CENTER) :
   CQIllustratorShapeGeometry(fixed, rcenter, m), p1_(p1), p2_(p2), str_(str), font_(font),
   halign_(halign), valign_(valign) {
  }

  CQIllustratorTextGeometry(const CQIllustratorTextGeometry &geometry) :
   CQIllustratorShapeGeometry(geometry), p1_(geometry.p1_), p2_(geometry.p2_), str_(geometry.str_),
   font_(geometry.font_), halign_(geometry.halign_), valign_(geometry.valign_) {
  }

  virtual CQIllustratorTextGeometry *dup() {
    return new CQIllustratorTextGeometry(*this);
  }

  friend class CQIllustratorTextShape;

 private:
  CQIllustratorTextGeometry &operator=(const CQIllustratorTextGeometry &);

 protected:
  CPoint2D    p1_;
  CPoint2D    p2_;
  std::string str_;
  CFontPtr    font_;
  CHAlignType halign_ { CHALIGN_TYPE_CENTER };
  CVAlignType valign_ { CVALIGN_TYPE_CENTER };
};

#endif
