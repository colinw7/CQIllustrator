#include <CTriangulate2D.h>

struct EarPointMgr {
  EarPointMgr() { }

 ~EarPointMgr() {
    uint n = earPoints.size();

    for (uint i = 0; i < n; ++i)
      delete earPoints[i];
  }

  CTriangulate2D::EarPoint *createEarPoint(const CPoint2D &p) {
    CTriangulate2D::EarPoint *earPoint = new CTriangulate2D::EarPoint(p);

    earPoints.push_back(earPoint);

    return earPoint;
  }

  std::vector<CTriangulate2D::EarPoint *> earPoints;
};

// triangulate. Points must be ccw
bool
CTriangulate2D::
triangulate(const std::vector<CPoint2D> &points, std::vector<CTriangle2D> &triangle_list)
{
  EarPointMgr mgr;

  //----

  uint numPoints = points.size();

  if (numPoints < 3) return false;

  double dx1 = points[1].x - points[0].x, dy1 = points[1].y - points[0].y;
  double dx2 = points[2].x - points[1].x, dy2 = points[2].y - points[1].y;

  double orient = dx1*dy2 - dy1*dx2;

  // Initialize ear points
  EarPoint *earPoints = NULL;

  if (orient > 0) {
    for (uint i = 0; i < numPoints; ++i) {
      EarPoint *earPoint = mgr.createEarPoint(points[i]);

      earPoint->addTo(&earPoints);
    }
  }
  else {
    for (uint i = 0; i < numPoints; ++i) {
      EarPoint *earPoint = mgr.createEarPoint(points[numPoints - i - 1]);

      earPoint->addTo(&earPoints);
    }
  }

  // Set is_ear for all points
  EarPoint *ep1 = earPoints;

  do {
    EarPoint *ep2 = ep1->next;
    EarPoint *ep0 = ep1->prev;

    ep1->is_ear = isDiagonal(earPoints, ep0, ep2);

    ep1 = ep1->next;
  } while (ep1 != earPoints);

  //------

  while (numPoints > 3) {
    EarPoint *ep2 = earPoints;

    bool found = false;

    do {
      if (ep2->is_ear) {
        found = true;

        EarPoint *ep3 = ep2->next;
        EarPoint *ep4 = ep3->next;
        EarPoint *ep1 = ep2->prev;
        EarPoint *ep0 = ep1->prev;

        // add output triangle
        addTriangle(triangle_list, ep1, ep2, ep3);

        // update is ear
        ep1->is_ear = isDiagonal(earPoints, ep0, ep3);
        ep3->is_ear = isDiagonal(earPoints, ep1, ep4);

        // remove point
        ep2->removeFrom(&earPoints);

        if (ep2 == earPoints)
          earPoints = ep3;

        --numPoints;

        break;
      }

      ep2 = ep2->next;
    } while (ep2 != earPoints);

    if (! found)
      return false;
  }

  ep1 = earPoints;

  EarPoint *ep2 = ep1->next;
  EarPoint *ep3 = ep2->next;

  addTriangle(triangle_list, ep1, ep2, ep3);

  return true;
}

void
CTriangulate2D::
addTriangle(std::vector<CTriangle2D> &triangle_list, const EarPoint *ep1,
            const EarPoint *ep2, const EarPoint *ep3)
{
  triangle_list.push_back(CTriangle2D(ep1->point, ep2->point, ep3->point));
}

bool
CTriangulate2D::
isDiagonal(EarPoint *earPoints, const EarPoint *epa, const EarPoint *epb)
{
  return inCone(epa, epb) && inCone(epb, epa) && isDiagonalInOut(earPoints, epa, epb);
}

bool
CTriangulate2D::
isDiagonalInOut(EarPoint *earPoints, const EarPoint *a, const EarPoint *b)
{
  EarPoint *c = earPoints;

  do {
    EarPoint *c1 = c->next;

    // skip edges incident to a or b
    if (c != a && c1 != a && c != b && c1 != b) {
      if (Intersects(a, b, c, c1))
        return false;
    }

    c = c->next;
  } while (c != earPoints);

  return true;
}

bool
CTriangulate2D::
inCone(const EarPoint *a, const EarPoint *b)
{
  // a0,a,a1 consecutive
  EarPoint *a1 = a->next;
  EarPoint *a0 = a->prev;

  if (PointLineLeftOn(a, a1, a0))
    return    PointLineLeft(a, b, a0) && PointLineLeft(b, a, a1);
  else
    return ! (PointLineLeft(a, b, a1) && PointLineLeft(b, a, a0));
}

bool
CTriangulate2D::
Intersects(const EarPoint *ep1, const EarPoint *ep2,
           const EarPoint *ep3, const EarPoint *ep4)
{
  CLine2D line1((*ep1).point, (*ep2).point);
  CLine2D line2((*ep3).point, (*ep4).point);

  return line1.intersects(line2);
}

bool
CTriangulate2D::
PointLineLeftOn(const EarPoint *ep1, const EarPoint *ep2, const EarPoint *ep3)
{
  CLine2D line((*ep1).point, (*ep2).point);

  return line.leftOrOn((*ep3).point);
}

bool
CTriangulate2D::
PointLineLeft(const EarPoint *ep1, const EarPoint *ep2, const EarPoint *ep3)
{
  CLine2D line((*ep1).point, (*ep2).point);

  return line.left((*ep3).point);
}
