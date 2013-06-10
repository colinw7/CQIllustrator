#include <CGiftWrap.h>
#include <CMathGeom2D.h>
#include <cstring>

CGiftWrap::
CGiftWrap()
{
  num_ = 0;
  max_ = 0;
  ind_ = NULL;
}

CGiftWrap::
~CGiftWrap()
{
  delete [] ind_;
}

//! create a convex polygon from an array of points
bool
CGiftWrap::
calc(const std::vector<CPoint2D> &points, std::vector<uint> &poly_inds)
{
  uint num_points = points.size();

  if (num_points <= 0)
    return false;

  uint   ind1 = 0;
  double xmin = points[ind1].x;
  double ymin = points[ind1].y;

  for (uint i = 1; i < num_points; ++i) {
    if      (points[i].y < ymin) {
      ind1 = i;
      xmin = points[ind1].x;
      ymin = points[ind1].y;
    }
    else if (points[i].y == ymin && points[i].x > xmin) {
      ind1 = i;
      xmin = points[ind1].x;
      ymin = points[ind1].y;
    }
  }

  num_ = 0;

  addPoint(ind1);

  uint ind2, ind3;

  while (num_ != num_points) {
    for (ind2 = 0; ind2 < num_points; ++ind2) {
      if (isPoint(ind2)) continue;

      for (ind3 = 0; ind3 < num_points; ++ind3) {
        if (ind3 == ind2 || ind3 == ind1) continue;

        CPolygonOrientation orient =
          CMathGeom2D::PolygonOrientation(points[ind1].x, points[ind1].y,
                                          points[ind2].x, points[ind2].y,
                                          points[ind3].x, points[ind3].y);

        if (orient < 0)
          break;
      }

      if (ind3 >= num_points)
        break;
    }

    if (ind2 >= num_points)
      break;

    addPoint(ind2);

    ind1 = ind2;
  }

  poly_inds = std::vector<uint>(ind_, ind_ + num_);

  return true;
}

//! create a convex polygon from an array of points
//! (return value poly_ind is a pointer to static memory and should not be freed or retained)
bool
CGiftWrap::
calc(const double *x, const double *y, uint num_xy, uint **poly_ind, uint *poly_num_ind)
{
  std::vector<CPoint2D> points;

  points.resize(num_xy);

  for (uint i = 0; i < num_xy; ++i) {
    points[i].x = x[i];
    points[i].y = y[i];
  }

  std::vector<uint> poly_inds;

  if (! calc(points, poly_inds))
    return false;

  *poly_ind     = ind_;
  *poly_num_ind = num_;

  return true;
}

void
CGiftWrap::
addPoint(uint ind)
{
  if (num_ >= max_) {
    uint  max1;
    uint *ind1;

    max1 = 2*max_ + 32;
    ind1 = new uint [max1];

    memcpy(ind1, ind_, num_*sizeof(int));

    delete [] ind_;

    max_ = max1;
    ind_ = ind1;
  }

  ind_[num_++] = ind;
}

bool
CGiftWrap::
isPoint(uint ind)
{
  for (uint i = 0; i < num_; ++i)
    if (ind_[i] == ind)
      return true;

  return false;
}
