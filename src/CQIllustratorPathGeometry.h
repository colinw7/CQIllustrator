#ifndef CQIllustratorPathGeometry_H
#define CQIllustratorPathGeometry_H

#include <CQIllustratorShapeGeometry.h>

class CQIllustratorPathGeometry : public CQIllustratorShapeGeometry {
 public:
  CQIllustratorPathGeometry(bool fixed=false, const CPoint2D &rcenter=CPoint2D(0,0),
                            const CMatrix2D m=CMatrix2D(),
                            const CPathShapePartList &parts=CPathShapePartList()) :
   CQIllustratorShapeGeometry(fixed, rcenter, m), parts_(parts) {
  }

  CQIllustratorPathGeometry(const CQIllustratorPathGeometry &geometry) :
   CQIllustratorShapeGeometry(geometry), parts_(geometry.parts_) {
  }

  CQIllustratorPathGeometry *dup() override {
    return new CQIllustratorPathGeometry(*this);
  }

  friend class CQIllustratorPathShape;

 private:
  CQIllustratorPathGeometry &operator=(const CQIllustratorPathGeometry &);

 protected:
  CPathShapePartList parts_;
};

#endif
