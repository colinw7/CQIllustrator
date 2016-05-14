#ifndef CQIllustratorSnap_H
#define CQIllustratorSnap_H

#include <CPoint2D.h>

class CQIllustratorSnap {
 public:
  CQIllustratorSnap() { }

  bool getEnabled() const { return enabled_; }
  void setEnabled(bool enabled) { enabled_ = enabled; }

  double getXPitch() const { return xpitch_; }
  void setXPitch(double xpitch) { xpitch_ = xpitch; }

  double getYPitch() const { return ypitch_; }
  void setYPitch(double ypitch) { ypitch_ = ypitch; }

  CPoint2D snapPoint(const CPoint2D &point) const;

 private:
  bool   enabled_ { false };
  double xpitch_ { 1 }, ypitch_ { 1 };
};

#endif
