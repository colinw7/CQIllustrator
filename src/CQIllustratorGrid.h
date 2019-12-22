#ifndef CQIllustratorGrid_H
#define CQIllustratorGrid_H

class CQIllustratorGrid {
 public:
  CQIllustratorGrid() { }

  bool getEnabled() const { return enabled_; }
  void setEnabled(bool enabled) { enabled_ = enabled; }

  void draw(CQIllustratorShapeDrawer *drawer, const CBBox2D &bbox);

 private:
  bool     enabled_ { false };
  CPoint2D origin_ { 0, 0 };
//double   dx_ { 10 }, dy_ { 10 };
};

#endif
