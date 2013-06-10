#ifndef CRANGE_2D_H
#define CRANGE_2D_H

template<typename T>
class CRange2DT {
 public:
  T x1, y1, x2, y2;

  explicit CRange2DT(T x1=0.0, T y1=0.0, T x2=0.0, T y2=0.0) {
    set(x1, y1, x2, y2);
  }

  void set(T x11, T y11, T x21, T y21) {
    x1 = x11, y1 = y11, x2 = x21, y2 = y21;
  }

  void get(T *x11, T *y11, T *x21, T *y21) const {
    *x11 = x1, *y11 = y1, *x21 = x2, *y21 = y2;
  }

  T dx() const { return x2 - x1; }
  T dy() const { return y2 - y1; }

  T xmid() const { return 0.5*(x2 + x1); }
  T ymid() const { return 0.5*(y2 + y1); }

  T xmin() const { return min(x1, x2); }
  T ymin() const { return min(y1, y2); }
  T xmax() const { return max(x1, x2); }
  T ymax() const { return max(y1, y2); }

  T xsize() const { return fabs(x2 - x1); }
  T ysize() const { return fabs(y2 - y1); }

  void inc(T dx, T dy) { x1 += dx; y1 += dy; x2 += dx; y2 += dy; }

  void incX(T dx) { x1 += dx; x2 += dx; }
  void incY(T dy) { y1 += dy; y2 += dy; }

  CRange2DT &operator=(const CRange2DT &range) {
    x1 = range.x1; y1 = range.y1;
    x2 = range.x2; y2 = range.y2;

    return *this;
  }
};

typedef CRange2DT<double> CRange2D;

#endif
