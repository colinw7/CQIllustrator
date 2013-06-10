#ifndef CBEZIER_TO_LINE_H
#define CBEZIER_TO_LINE_H

#include <C3Bezier2D.h>
#include <C2Bezier2D.h>

class CBezierToLine {
 public:
  typedef std::vector<CPoint2D> PointList;

 public:
  CBezierToLine();

  virtual ~CBezierToLine();

  void setTolerance(double tol) { tol_ = tol; }

  void calc(const C3Bezier2D &bezier);
  void calc(const C2Bezier2D &bezier);

  virtual void init(const C3Bezier2D &bezier);
  virtual void init(const C2Bezier2D &bezier);

  virtual bool checkLength(double s);

  uint getNumPoints() const { return bezier_points_.size(); }

  const CPoint2D &getPoint(uint i) const { return bezier_points_[i]; }

 private:
  void calc1(const C3Bezier2D &bezier, uint *num_points, uint depth);
  void calc1(const C2Bezier2D &bezier, uint *num_points, uint depth);

 protected:
  double    tol_;
  PointList bezier_points_;
};

#endif
