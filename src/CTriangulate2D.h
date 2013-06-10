#ifndef CTRIANGULATE_2D_H
#define CTRIANGULATE_2D_H

#include <CPoint2D.h>
#include <CTriangle2D.h>
#include <CListLink.h>

namespace CTriangulate2D {
  struct EarPoint : public CListLink<EarPoint> {
    const CPoint2D &point;
    bool            is_ear;

    EarPoint(const CPoint2D &p) :
     point(p), is_ear(false) {
    }

    bool operator==(const EarPoint &ep) {
      return (&point == &ep.point);
    }
  };

  bool triangulate(const std::vector<CPoint2D> &points, std::vector<CTriangle2D> &triangle_list);

  void addTriangle(std::vector<CTriangle2D> &triangle_list,
                   const EarPoint *ep1, const EarPoint *ep2, const EarPoint *ep3);

  bool isDiagonal(EarPoint *ear_points, const EarPoint *epa, const EarPoint *epb);

  bool isDiagonalInOut(EarPoint *ear_points, const EarPoint *epa, const EarPoint *epb);

  bool inCone(const EarPoint *ep1, const EarPoint *epb);

  bool Intersects(const EarPoint *ep1, const EarPoint *ep2,
                  const EarPoint *ep3, const EarPoint *ep4);

  bool PointLineLeftOn(const EarPoint *ep1, const EarPoint *ep2, const EarPoint *ep3);

  bool PointLineLeft(const EarPoint *ep1, const EarPoint *ep2, const EarPoint *ep3);
};

#endif
