#ifndef CGIFT_WRAP_H
#define CGIFT_WRAP_H

#include <CPoint2D.h>
#include <vector>
#include <sys/types.h>

class CGiftWrap {
 public:
  CGiftWrap();
 ~CGiftWrap();

  bool calc(const std::vector<CPoint2D> &points, std::vector<uint> &poly_inds);
  bool calc(const double *x, const double *y, uint num_xy, uint **poly_ind, uint *poly_num_ind);

 private:
  void addPoint(uint ind);
  bool isPoint(uint ind);

 private:
  uint  num_;
  uint  max_;
  uint *ind_;
};

#endif
