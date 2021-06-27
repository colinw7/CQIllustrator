#ifndef CQIllustratorShapeGaussianFilter_H
#define CQIllustratorShapeGaussianFilter_H

#include <CQIllustratorShapeFilter.h>

class CQIllustratorShapeGaussianFilter : public CQIllustratorShapeFilter {
 public:
  CQIllustratorShapeGaussianFilter(double std_dev=1.0) :
   std_dev_(std_dev) {
  }

  double std_dev() const { return std_dev_; }

 private:
  double std_dev_ { 1.0 };
};

#endif
