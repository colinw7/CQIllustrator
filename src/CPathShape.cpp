#include <CPathShape.h>
#include <CMathGeom2D.h>
#include <CStrUtil.h>

CPathShapePartList::
CPathShapePartList()
{
}

CPathShapePartList::
CPathShapePartList(const CPathShapePartList &parts)
{
  uint num_parts = parts.parts_.size();

  parts_.resize(num_parts);

  for (uint i = 0; i < num_parts; ++i)
    parts_[i] = parts.parts_[i]->dup();
}

CPathShapePartList::
~CPathShapePartList()
{
  clear();
}

CPathShapePartList &
CPathShapePartList::
operator=(const CPathShapePartList &parts)
{
  clear();

  uint num_parts = parts.parts_.size();

  parts_.resize(num_parts);

  for (uint i = 0; i < num_parts; ++i)
    parts_[i] = parts.parts_[i]->dup();

  return *this;
}

void
CPathShapePartList::
clear()
{
  uint num_parts = parts_.size();

  for (uint i = 0; i < num_parts; ++i)
    delete parts_[i];

  parts_.clear();
}

void
CPathShapePartList::
addPart(CPathShapePart *part)
{
  parts_.push_back(part);
}

void
CPathShapePartList::
addPart(uint ind, CPathShapePart *part)
{
  uint num_parts = parts_.size();

  assert(ind <= num_parts);

  if (ind <= num_parts) {
    parts_.push_back(NULL);

    ++num_parts;

    for (int i = int(num_parts) - 1; i > int(ind); --i)
      parts_[i] = parts_[i - 1];

    parts_[ind] = part;
  }
  else
    parts_.push_back(part);
}

void
CPathShapePartList::
setPart(uint i, CPathShapePart *part)
{
  parts_[i] = part;
}

void
CPathShapePartList::
removePart(uint ind)
{
  uint num_parts = parts_.size();

  assert(ind < num_parts);

  delete parts_[ind];

  for (uint i = ind; i < num_parts; ++i)
    parts_[i - 1] = parts_[i];

  parts_.pop_back();
}

void
CPathShapePartList::
addLine(const CLine2D &line)
{
  if (parts_.empty())
    addPart(new CPathShapeMoveTo(line.start()));

  addPart(new CPathShapeLineTo(line.end()));
}

void
CPathShapePartList::
processPath(CPathShapeProcess *process)
{
  uint num_parts = parts_.size();

  for (uint i = 0; i < num_parts; ++i) {
    CPathShapePart *part = parts_[i];

    CPathPartType type = part->getType();

    if      (type == CPATH_PART_TYPE_MOVE_TO) {
      CPathShapeMoveTo *moveToPart = dynamic_cast<CPathShapeMoveTo *>(part);

      const CPoint2D &p = moveToPart->getPoint();

      process->moveTo(p);
    }
    else if (type == CPATH_PART_TYPE_LINE_TO) {
      CPathShapeLineTo *lineToPart = dynamic_cast<CPathShapeLineTo *>(part);

      const CPoint2D &p = lineToPart->getPoint();

      process->lineTo(p);
    }
    else if (type == CPATH_PART_TYPE_CURVE2_TO) {
      CPathShapeCurve2To *curveToPart = dynamic_cast<CPathShapeCurve2To *>(part);

      const CPoint2D &p1 = curveToPart->getPoint1();
      const CPoint2D &p2 = curveToPart->getPoint2();

      process->curve2To(p1, p2);
    }
    else if (type == CPATH_PART_TYPE_CURVE3_TO) {
      CPathShapeCurve3To *curveToPart = dynamic_cast<CPathShapeCurve3To *>(part);

      const CPoint2D &p1 = curveToPart->getPoint1();
      const CPoint2D &p2 = curveToPart->getPoint2();
      const CPoint2D &p3 = curveToPart->getPoint3();

      process->curve3To(p1, p2, p3);
    }
    else if (type == CPATH_PART_TYPE_ARC) {
      CPathShapeArc *arcToPart = dynamic_cast<CPathShapeArc *>(part);

      const CPoint2D &c      = arcToPart->getCenter();
      double          xr     = arcToPart->getRadiusX();
      double          yr     = arcToPart->getRadiusY();
      double          angle1 = arcToPart->getAngle1();
      double          angle2 = arcToPart->getAngle2();

      process->arcTo(c, xr, yr, angle1, angle2);
    }
    else if (type == CPATH_PART_TYPE_CLOSE) {
      process->close();
    }
  }
}

class CPathShapeStroke : public CPathShapeProcess {
 public:
  CPathShapeStroke(double w, CPathShapePartList &parts) :
   w_      (w),
   start_  (0,0),
   current_(0,0),
   closed_ (false),
   inner_  (),
   outer_  (),
   parts_  (parts) {
  }

  void init() {
  }

  void term() {
    buildPath();
  }

  void moveTo(const CPoint2D &p) {
    start_   = p;
    current_ = p;
    closed_  = false;
  }

  void lineTo(const CPoint2D &p) {
    strokeLine(current_, p);

    current_ = p;
    closed_  = false;
  }

  void curve2To(const CPoint2D &p1, const CPoint2D &p2) {
    strokeBezier2(current_, p1, p2);

    current_ = p2;
    closed_  = false;
  }

  void curve3To(const CPoint2D &p1, const CPoint2D &p2, const CPoint2D &p3) {
    strokeBezier3(current_, p1, p2, p3);

    current_ = p3;
    closed_  = false;
  }

  void arcTo(const CPoint2D &c, double xr, double yr, double angle1, double angle2) {
    strokeArc(c, xr, yr, angle1, angle2);

    int    fa, fs;
    double x0, y0, x1, y1;

    CMathGeom2D::ConvertToSVGArc(c.x, c.y, xr, yr, angle1, angle2 - angle1, 0.0,
                                 &x0, &y0, &x1, &y1, &fa, &fs);

    current_ = CPoint2D(x1, y1);
    closed_  = false;
  }

  void close() {
    if (! closed_) {
      strokeLine(current_, start_);

      closed_ = true;
    }

    buildPath();
  }

 private:
  void strokeBezier2(const CPoint2D &p1, const CPoint2D &p2, const CPoint2D &p3) {
    C2Bezier2D bezier(p1, p2, p3);

    std::vector<CPoint2D> points;

    CMathGeom2D::BezierToLines(bezier, points);

    uint numPoints = points.size();

    for (uint i = 1; i < numPoints; ++i)
      strokeLine(points[i - 1], points[i]);
  }

  void strokeBezier3(const CPoint2D &p1, const CPoint2D &p2,
                     const CPoint2D &p3, const CPoint2D &p4) {
    C3Bezier2D bezier(p1, p2, p3, p4);

    std::vector<CPoint2D> points;

    CMathGeom2D::BezierToLines(bezier, points);

    uint numPoints = points.size();

    for (uint i = 1; i < numPoints; ++i)
      strokeLine(points[i - 1], points[i]);
  }

  void strokeArc(const CPoint2D &c, double xr, double yr, double angle1, double angle2) {
    std::vector<C3Bezier2D> beziers;

    CMathGeom2D::ArcToBeziers(c.x, c.y, xr, yr, angle1, angle2, beziers);

    uint num_beziers = beziers.size();

    for (uint i = 0; i < num_beziers; ++i) {
      const C3Bezier2D &bezier = beziers[i];

      const CPoint2D &p1 = bezier.getFirstPoint   ();
      const CPoint2D &p2 = bezier.getControlPoint1();
      const CPoint2D &p3 = bezier.getControlPoint2();
      const CPoint2D &p4 = bezier.getLastPoint    ();

      strokeBezier3(p1, p2, p3, p4);
    }
  }

  void strokeLine(const CPoint2D &p1, const CPoint2D &p2) {
    double w = w_/2.0;

    double x21 = p2.x - p1.x;
    double y21 = p2.y - p1.y;

    if (x21 == 0.0 && y21 == 0.0) return;

    double g = CMathGen::atan2(y21, x21);

    double dx = w*cos(g);
    double dy = w*sin(g);

    inner_.push_back(CLine2D(CPoint2D(p1.x + dx, p1.y - dy), CPoint2D(p2.x + dx, p2.y - dy)));
    outer_.push_back(CLine2D(CPoint2D(p2.x - dx, p2.y + dy), CPoint2D(p1.x - dx, p1.y + dy)));
  }

  void buildPath() {
    uint num_inner = inner_.size();
    uint num_outer = outer_.size();

    if (num_inner == 0 || num_outer == 0)
      return;

    if (! closed_) { // not closed so inner + outer form a single shape
      const CPoint2D *p1 = &inner_[0].start();
      const CPoint2D *p2 = &inner_[0].end  ();

      parts_.addPart(new CPathShapeMoveTo(*p1));

      for (uint i = 1; i < num_inner; ++i) {
        const CPoint2D *p3 = &inner_[i].start();
        const CPoint2D *p4 = &inner_[i].end  ();

        CPoint2D pi;
        double   mu1, mu2;

        CMathGeom2D::IntersectLine(*p1, *p2, *p3, *p4, &pi, &mu1, &mu2);

        if (mu1 >= 0.0 && mu1 <= 1.0) {
          parts_.addPart(new CPathShapeLineTo(pi));
        }
        else {
          parts_.addPart(new CPathShapeLineTo(pi));
        //parts_.addPart(new CPathShapeLineTo(*p2));
        //parts_.addPart(new CPathShapeLineTo(*p3));
        }

        p1 = p3;
        p2 = p4;
      }

      for (uint i = 0; i < num_outer; ++i) {
        const CPoint2D *p3 = &outer_[num_outer - i - 1].start();
        const CPoint2D *p4 = &outer_[num_outer - i - 1].end  ();

        CPoint2D pi;
        double   mu1, mu2;

        CMathGeom2D::IntersectLine(*p1, *p2, *p3, *p4, &pi, &mu1, &mu2);

        if (mu1 >= 0.0 && mu1 <= 1.0) {
          parts_.addPart(new CPathShapeLineTo(pi));
        }
        else {
          parts_.addPart(new CPathShapeLineTo(pi));
          //parts_.addPart(new CPathShapeLineTo(*p2));
          //parts_.addPart(new CPathShapeLineTo(*p3));
        }

        p1 = p3;
        p2 = p4;
      }

      parts_.addPart(new CPathShapeLineTo(*p2));

      parts_.addPart(new CPathShapeClose());
    }
    else { // closed inner and outer form separate shapes
      // inner
      {
      const CPoint2D *p1 = &inner_[num_inner - 1].start();
      const CPoint2D *p2 = &inner_[num_inner - 1].end  ();

      const CPoint2D *p3 = &inner_[0].start();
      const CPoint2D *p4 = &inner_[0].end  ();

      CPoint2D pi;
      double   mu1, mu2;

      CMathGeom2D::IntersectLine(*p1, *p2, *p3, *p4, &pi, &mu1, &mu2);

      parts_.addPart(new CPathShapeMoveTo(pi));

      p1 = p3;
      p2 = p4;

      for (uint i = 1; i < num_inner; ++i) {
        const CPoint2D *ip3 = &inner_[i].start();
        const CPoint2D *ip4 = &inner_[i].end  ();

        CPoint2D ipi;
        double   imu1, imu2;

        CMathGeom2D::IntersectLine(*p1, *p2, *ip3, *ip4, &ipi, &imu1, &imu2);

        parts_.addPart(new CPathShapeLineTo(ipi));

        p1 = ip3;
        p2 = ip4;
      }

      parts_.addPart(new CPathShapeClose());
      }

      // outer
      {
      const CPoint2D *op1 = &outer_[0].start();
      const CPoint2D *op2 = &outer_[0].end  ();

      const CPoint2D *op3 = &outer_[num_outer - 1].start();
      const CPoint2D *op4 = &outer_[num_outer - 1].end  ();

      CPoint2D opi;
      double   omu1, omu2;

      CMathGeom2D::IntersectLine(*op1, *op2, *op3, *op4, &opi, &omu1, &omu2);

      parts_.addPart(new CPathShapeMoveTo(opi));

      op1 = op3;
      op2 = op4;

      for (uint i = 1; i < num_outer; ++i) {
        const CPoint2D *op31 = &outer_[num_outer - i - 1].start();
        const CPoint2D *op41 = &outer_[num_outer - i - 1].end  ();

        CPoint2D opi1;
        double   omu11, omu21;

        CMathGeom2D::IntersectLine(*op1, *op2, *op31, *op41, &opi1, &omu11, &omu21);

        parts_.addPart(new CPathShapeLineTo(opi1));

        op1 = op31;
        op2 = op41;
      }

      parts_.addPart(new CPathShapeClose());
      }
    }

    inner_.clear();
    outer_.clear();
  }

 private:
  double                w_;
  CPoint2D              start_;
  CPoint2D              current_;
  bool                  closed_;
  std::vector<CLine2D>  inner_;
  std::vector<CLine2D>  outer_;
  CPathShapePartList   &parts_;

 private:
  CPathShapeStroke(const CPathShapeStroke &rhs);
  CPathShapeStroke &operator=(const CPathShapeStroke &rhs);
};

void
CPathShapePartList::
strokePath(double w, CPathShapePartList &parts)
{
  CPathShapeStroke stroke(w, parts);

  processPath(&stroke);
}

class CPathShapeOffset : public CPathShapeProcess {
 public:
  CPathShapeOffset(double w, const CPoint2D &c, CPathShapePartList &parts) :
   w_    (w),
   c_    (c),
   parts_(parts) {
  }

  void init() {
  }

  void term() {
  }

  void moveTo(const CPoint2D &p) {
    CPoint2D op = offsetPoint(p);

    parts_.addPart(new CPathShapeMoveTo(op));
  }

  void lineTo(const CPoint2D &p) {
    CPoint2D op = offsetPoint(p);

    parts_.addPart(new CPathShapeLineTo(op));
  }

  void curve2To(const CPoint2D &p1, const CPoint2D &p2) {
    CPoint2D op1 = offsetPoint(p1);
    CPoint2D op2 = offsetPoint(p2);

    parts_.addPart(new CPathShapeCurve2To(op1, op2));
  }

  void curve3To(const CPoint2D &p1, const CPoint2D &p2, const CPoint2D &p3) {
    CPoint2D op1 = offsetPoint(p1);
    CPoint2D op2 = offsetPoint(p2);
    CPoint2D op3 = offsetPoint(p3);

    parts_.addPart(new CPathShapeCurve3To(op1, op2, op3));
  }

  void arcTo(const CPoint2D &c, double xr, double yr, double angle1, double angle2) {
    std::vector<C3Bezier2D> beziers;

    CMathGeom2D::ArcToBeziers(c.x, c.y, xr, yr, angle1, angle2, beziers);

    uint num_beziers = beziers.size();

    for (uint i = 0; i < num_beziers; ++i) {
      const C3Bezier2D &bezier = beziers[i];

      const CPoint2D &p2 = bezier.getControlPoint1();
      const CPoint2D &p3 = bezier.getControlPoint2();
      const CPoint2D &p4 = bezier.getLastPoint    ();

      curve3To(p2, p3, p4);
    }
  }

  void close() {
    parts_.addPart(new CPathShapeClose());
  }

 private:
  CPoint2D offsetPoint(const CPoint2D &p) {
    CVector2D v(p, c_);

    return (p - v*(w_/v.length()));
  }

 private:
  double                          w_;
  CPoint2D                        c_;
  CPathShapePartList &parts_;

 private:
  CPathShapeOffset(const CPathShapeOffset &rhs);
  CPathShapeOffset &operator=(const CPathShapeOffset &rhs);
};

void
CPathShapePartList::
offsetPath(double w, const CPoint2D &c, CPathShapePartList &parts)
{
  CPathShapeOffset offset(w, c, parts);

  processPath(&offset);
}

typedef std::map<uint,CMathGen::IntersectType> InsideType;
typedef std::map<uint,InsideType>              InsideTypeList;

bool
CPathShapePartList::
intersect(CBooleanOp op, const CPathShapePartList &parts1, const CMatrix2D &m1,
          const CPathShapePartList &parts2, const CMatrix2D &m2,
          CPathShapePartListArray &ipartsList, bool split)
{
  // convert to line/beizer parts
  CPathShapePartListArray cpartsList1;

  parts1.toCurves(m1, cpartsList1);

  uint numCPartsLists1 = cpartsList1.size();

  if (numCPartsLists1 == 0) return false;

  CPathShapePartListArray cpartsList2;

  parts2.toCurves(m2, cpartsList2);

  uint numCPartsLists2 = cpartsList2.size();

  if (numCPartsLists2 == 0) return false;

  //---

  // flatten to polygon for inside test
  std::vector<CPolygon2D> polygons1;

  for (uint i = 0; i < numCPartsLists1; ++i) {
    CPathShapePartList &cparts = cpartsList1[i];

    std::vector<CPoint2D> points;

    cparts.flattenCurve(points);

    polygons1.push_back(CPolygon2D(points));
  }

  std::vector<CPolygon2D> polygons2;

  for (uint i = 0; i < numCPartsLists2; ++i) {
    CPathShapePartList &cparts = cpartsList2[i];

    std::vector<CPoint2D> points;

    cparts.flattenCurve(points);

    polygons2.push_back(CPolygon2D(points));
  }

  //---

  // for multiple paths in a shape find their topology (inside/outside)
  InsideTypeList insideTypeList1;

  for (uint i = 0; i < numCPartsLists1; ++i) {
    CPolygon2D &polygon1 = polygons1[i];

    for (uint j = i + 1; j < numCPartsLists1; ++j) {
      CPolygon2D &polygon2 = polygons1[j];

      CMathGen::IntersectType type = getInsideType(polygon1, polygon2);

      std::cerr << i << ":" << j << ") ";

      if      (type == CMathGen::INTERSECT_NONE   ) std::cerr << "None"    << std::endl;
      else if (type == CMathGen::INTERSECT_INSIDE ) std::cerr << "Inside"  << std::endl;
      else if (type == CMathGen::INTERSECT_OUTSIDE) std::cerr << "Outside" << std::endl;

      insideTypeList1[i][j] = type;
    }
  }

  InsideTypeList insideTypeList2;

  for (uint i = 0; i < numCPartsLists2; ++i) {
    CPolygon2D &polygon1 = polygons2[i];

    for (uint j = i + 1; j < numCPartsLists2; ++j) {
      CPolygon2D &polygon2 = polygons2[j];

      CMathGen::IntersectType type = getInsideType(polygon1, polygon2);

      std::cerr << i << ":" << j << ") ";

      if      (type == CMathGen::INTERSECT_NONE   ) std::cerr << "None"    << std::endl;
      else if (type == CMathGen::INTERSECT_INSIDE ) std::cerr << "Inside"  << std::endl;
      else if (type == CMathGen::INTERSECT_OUTSIDE) std::cerr << "Outside" << std::endl;

      insideTypeList2[i][j] = type;
    }
  }

  //---

  // if no overlaps then just do simple combine
  uint num_intersects = 0;

  for (uint i = 0; i < numCPartsLists1; ++i) {
    CPathShapePartList &cparts1 = cpartsList1[i];

    for (uint j = 0; j < numCPartsLists2; ++j) {
      CPathShapePartList &cparts2 = cpartsList2[j];

      num_intersects += intersectCurves(cparts1, cparts2);
    }
  }

  if (num_intersects == 0) {
    for (uint i = 0; i < numCPartsLists1; ++i) {
      CPathShapePartList &cparts1 = cpartsList1[i];

      for (uint j = 0; j < numCPartsLists2; ++j) {
        CPathShapePartList &cparts2 = cpartsList2[j];

        CPathShapePartList iparts;

        if      (op == BOOLEAN_OR)
          combineOr (cparts1, cparts2, iparts);
        else if (op == BOOLEAN_AND)
          combineAnd(cparts1, cparts2, iparts);
        else if (op == BOOLEAN_XOR)
          combineXor(cparts1, cparts2, iparts);
        else
          return false;

        ipartsList.push_back(iparts);
      }
    }

    return true;
  }

  //---

  // special handling for halo -> normal shape
  // TODO: extend ti multiple insides
  if      (numCPartsLists1 == 2 && numCPartsLists2 == 1 &&
           insideTypeList1[0][1] != CMathGen::INTERSECT_NONE) {
    CPathShapePartList &cparts11 = cpartsList1[0];
    CPathShapePartList &cparts12 = cpartsList1[1];
    CPathShapePartList &cparts2  = cpartsList2[0];

    const CPolygon2D &polygon11 = polygons1[0];
    const CPolygon2D &polygon12 = polygons1[1];
    const CPolygon2D &polygon2  = polygons2[0];

    // first part is outside
    if (insideTypeList1[0][1] == CMathGen::INTERSECT_OUTSIDE) {
      if (! intersectParts(op, cparts11, cparts2, polygon11, polygon2, ipartsList,
                           split, CMathGen::INTERSECT_OUTSIDE))
        return false;

      CPathShapePartList &iparts = ipartsList[0];

      CPathShapePartListArray ipartsList1;

      if (! intersectParts(BOOLEAN_XOR, cparts12, cparts2, polygon12, polygon2, ipartsList1,
                           true, CMathGen::INTERSECT_INSIDE))
        return false;

      uint numPartsList1 = ipartsList1.size();

      for (uint j = 0; j < numPartsList1; ++j) {
        CPathShapePartList &iparts1 = ipartsList1[j];

        uint id = ipartsList1.getId(j);

        if (id != 0) continue;

        CPathShapePartList iparts2;

        combine(iparts, iparts1, iparts2);

        iparts = iparts2;
      }
    }
    // second part is outside
    else {
      if (! intersectParts(op, cparts12, cparts2, polygon12, polygon2, ipartsList,
                           split, CMathGen::INTERSECT_OUTSIDE))
        return false;

      CPathShapePartList &iparts = ipartsList[0];

      CPathShapePartListArray ipartsList1;

      if (! intersectParts(BOOLEAN_XOR, cparts11, cparts2, polygon11, polygon2, ipartsList1,
                           true, CMathGen::INTERSECT_INSIDE))
        return false;

      uint numPartsList1 = ipartsList1.size();

      for (uint j = 0; j < numPartsList1; ++j) {
        CPathShapePartList &iparts1 = ipartsList1[j];

        uint id = ipartsList1.getId(j);

        if (id != 0) continue;

        CPathShapePartList iparts2;

        combine(iparts, iparts1, iparts2);

        iparts = iparts2;
      }
    }

    return true;
  }
  else if (numCPartsLists2 == 2 && numCPartsLists1 == 1 &&
           insideTypeList2[0][1] != CMathGen::INTERSECT_NONE) {
    CPathShapePartList &cparts21 = cpartsList2[0];
    CPathShapePartList &cparts22 = cpartsList2[1];
    CPathShapePartList &cparts1  = cpartsList1[0];

    const CPolygon2D &polygon21 = polygons2[0];
    const CPolygon2D &polygon22 = polygons2[1];
    const CPolygon2D &polygon1  = polygons1[0];

    // first part is outside
    if (insideTypeList2[0][1] == CMathGen::INTERSECT_OUTSIDE) {
      if (! intersectParts(op, cparts21, cparts1, polygon21, polygon1, ipartsList,
                           split, CMathGen::INTERSECT_OUTSIDE))
        return false;

      if (! intersectParts(BOOLEAN_XOR, cparts22, cparts1, polygon22, polygon1, ipartsList,
                           true, CMathGen::INTERSECT_INSIDE))
        return false;
    }
    else {
      if (! intersectParts(op, cparts22, cparts1, polygon22, polygon1, ipartsList,
                           split, CMathGen::INTERSECT_OUTSIDE))
        return false;

      if (! intersectParts(BOOLEAN_XOR, cparts21, cparts1, polygon21, polygon1, ipartsList,
                           true, CMathGen::INTERSECT_INSIDE))
        return false;
    }

    return true;
  }

  //---


  for (uint i = 0; i < numCPartsLists1; ++i) {
    CPathShapePartList &cparts1 = cpartsList1[i];

    for (uint j = 0; j < numCPartsLists2; ++j) {
      CPathShapePartList &cparts2 = cpartsList2[j];

      if (! intersectParts(op, cparts1, cparts2, polygons1[i], polygons2[j], ipartsList,
                           split, CMathGen::INTERSECT_NONE))
        return false;
    }
  }

  return true;
}

bool
CPathShapePartList::
intersectParts(CBooleanOp op, CPathShapePartList &cparts1, CPathShapePartList &cparts2,
               const CPolygon2D &polygon1, const CPolygon2D &polygon2,
               CPathShapePartListArray &ipartsList, bool split,
               CMathGen::IntersectType /*intersectType*/)
{
  setCurveInside(cparts1, polygon2);
  setCurveInside(cparts2, polygon1);

  //---

  uint num_intersects = intersectCurves(cparts1, cparts2);

  //-------

  if (num_intersects > 0) {
    CPathShapePartList cparts11, cparts21;

    bool expand1 = expandParts(cparts1, cparts11);
    bool expand2 = expandParts(cparts2, cparts21);

    if (expand1) {
      cparts1 = cparts11;

      setCurveInside(cparts1, polygon2);
    }

    if (expand2) {
      cparts2 = cparts21;

      setCurveInside(cparts2, polygon1);
    }

    if (expand1 || expand2)
      num_intersects = intersectCurves(cparts1, cparts2);
  }

  //-------

  CPathShapePartListArray ipartsList1;

  CPathShapePartList iparts;

  if (num_intersects > 0) {
    if      (op == BOOLEAN_OR)
      intersectCombineOr (cparts1, cparts2, iparts);
    else if (op == BOOLEAN_AND)
      intersectCombineAnd(cparts1, cparts2, iparts);
    else if (op == BOOLEAN_XOR)
      intersectCombineXor(cparts1, cparts2, ipartsList1, split);
    else
      return false;

    if (op != BOOLEAN_XOR)
      ipartsList1.push_back(iparts);
  }
  else {
    if      (op == BOOLEAN_OR)
      combineOr (cparts1, cparts2, iparts);
    else if (op == BOOLEAN_AND)
      combineAnd(cparts1, cparts2, iparts);
    else if (op == BOOLEAN_XOR)
      combineXor(cparts1, cparts2, iparts);
    else
      return false;

    ipartsList1.push_back(iparts);
  }

  //-------

  uint numIPartsList1 = ipartsList1.size();

  for (uint k = 0; k < numIPartsList1; ++k)
    ipartsList.push_back(ipartsList1.getId(k), ipartsList1[k]);

  return true;
}

void
CPathShapePartList::
setCurveInside(CPathShapePartList &cparts, const CPolygon2D &polygon)
{
  uint num_cparts = cparts.size();

  // set curve1 inside
  for (uint i1 = 0; i1 < num_cparts; ++i1) {
    const CCurveShapePart *cpart = dynamic_cast<const CCurveShapePart *>(cparts[i1]);

    CPathPartType type1 = cpart->getType();

    if      (type1 == CPATH_PART_TYPE_LINE_TO) {
      const CCurveShapeLine *curveLine = dynamic_cast<const CCurveShapeLine *>(cpart);

      const CLine2D &cline = curveLine->getLine();

      const CPoint2D &p1 = cline.start();
      const CPoint2D &p2 = cline.end  ();

      curveLine->inside_[0] = polygon.inside(p1);
      curveLine->inside_[1] = polygon.inside(p2);
    }
    else if (type1 == CPATH_PART_TYPE_CURVE3_TO) {
      const CCurveShapeBezier *curveBezier = dynamic_cast<const CCurveShapeBezier *>(cpart);

      const C3Bezier2D &cbezier = curveBezier->getBezier();

      const CPoint2D &p1 = cbezier.getFirstPoint   ();
      const CPoint2D &p2 = cbezier.getControlPoint1();
      const CPoint2D &p3 = cbezier.getControlPoint2();
      const CPoint2D &p4 = cbezier.getLastPoint    ();

      curveBezier->inside_[0]  = polygon.inside(p1);
      curveBezier->inside_[1]  = polygon.inside(p2);
      curveBezier->inside_[2]  = polygon.inside(p3);
      curveBezier->inside_[3]  = polygon.inside(p4);
    }
  }
}

uint
CPathShapePartList::
intersectCurves(CPathShapePartList &cparts1, CPathShapePartList &cparts2)
{
  uint num_itersects = 0;

  uint num_cparts1 = cparts1.size();
  uint num_cparts2 = cparts2.size();

  for (uint i1 = 0; i1 < num_cparts1; ++i1) {
    const CCurveShapePart *cpart1 = dynamic_cast<const CCurveShapePart *>(cparts1[i1]);

    cpart1->ppoints_.clear();
    cpart1->visited_ = false;
  }

  for (uint i2 = 0; i2 < num_cparts2; ++i2) {
    const CCurveShapePart *cpart2 = dynamic_cast<const CCurveShapePart *>(cparts2[i2]);

    cpart2->ppoints_.clear();
    cpart2->visited_ = false;
  }

  // intersect curve1 with curve2
  for (uint i1 = 0; i1 < num_cparts1; ++i1) {
    const CCurveShapePart *cpart1 = dynamic_cast<const CCurveShapePart *>(cparts1[i1]);

    CPathPartType type1 = cpart1->getType();

    if      (type1 == CPATH_PART_TYPE_LINE_TO) {
      const CCurveShapeLine *curveLine = dynamic_cast<const CCurveShapeLine *>(cpart1);

      const CLine2D &cline = curveLine->getLine();

      num_itersects += intersect(cpart1, i1, cparts2, cline);
    }
    else if (type1 == CPATH_PART_TYPE_CURVE3_TO) {
      const CCurveShapeBezier *curveBezier = dynamic_cast<const CCurveShapeBezier *>(cpart1);

      const C3Bezier2D &cbezier = curveBezier->getBezier();

      num_itersects += intersect(cpart1, i1, cparts2, cbezier);
    }
  }

  return num_itersects;
}

bool
CPathShapePartList::
expandParts(CPathShapePartList &cparts, CPathShapePartList &cparts1)
{
  uint num_cparts = cparts.size();

  if (num_cparts == 0) return false;

  bool expand = false;

  for (uint i = 0; i < num_cparts; ++i) {
    const CCurveShapePart *cpart = dynamic_cast<const CCurveShapePart *>(cparts[i]);

    uint num_ppoints = cpart->ppoints_.size();

    if (num_ppoints > 1) {
      expand = true;
      break;
    }
  }

  if (! expand) return false;

  for (uint i = 0; i < num_cparts; ++i) {
    const CCurveShapePart *cpart = dynamic_cast<const CCurveShapePart *>(cparts[i]);

    uint num_ppoints = cpart->ppoints_.size();

    CPathPartType type = cpart->getType();

    if      (type == CPATH_PART_TYPE_LINE_TO) {
      const CCurveShapeLine *curveLine = dynamic_cast<const CCurveShapeLine *>(cpart);

      CLine2D cline = curveLine->getLine();

      if (num_ppoints > 1) {
        double t1 = 0.0;
        double t2 = 1.0;

        CLine2D sline1, sline2;

        for (uint j = 1; j < num_ppoints; ++j) {
          const CCurveShapePart::PartIPoint &ppoint1 = cpart->ppoints_[j - 1];
          const CCurveShapePart::PartIPoint &ppoint2 = cpart->ppoints_[j    ];

          double dt1 = (ppoint1.ibpoint.t1 + ppoint2.ibpoint.t1)/2;
          double dt2 = (dt1 - t1)/(t2 - t1);

          std::cerr << "Split Line @ " << dt2 << " " << cline.interp(dt2) << std::endl;

          cline.split(dt2, sline1, sline2);

          std::cerr << "Expand Line " << sline1 << std::endl;

          cparts1.addPart(new CCurveShapeLine(sline1));

          cline  = sline2;
          t1    += dt2;
        }

        std::cerr << "Expand Line " << sline2 << std::endl;

        cparts1.addPart(new CCurveShapeLine(sline2));
      }
      else {
        std::cerr << "Normal Line " << cline << std::endl;

        cparts1.addPart(new CCurveShapeLine(cline));
      }
    }
    else if (type == CPATH_PART_TYPE_CURVE3_TO) {
      const CCurveShapeBezier *curveBezier = dynamic_cast<const CCurveShapeBezier *>(cpart);

      C3Bezier2D cbezier = curveBezier->getBezier();

      if (num_ppoints > 1) {
        double t1 = 0.0;
        double t2 = 1.0;

        C3Bezier2D sbezier1, sbezier2;

        for (uint j = 1; j < num_ppoints; ++j) {
          const CCurveShapePart::PartIPoint &ppoint1 = cpart->ppoints_[j - 1];
          const CCurveShapePart::PartIPoint &ppoint2 = cpart->ppoints_[j    ];

          double dt1 = (ppoint1.ibpoint.t1 + ppoint2.ibpoint.t1)/2;
          double dt2 = (dt1 - t1)/(t2 - t1);

          std::cerr << "Split Curve @ " << dt2 << " " << cbezier.calc(dt2) << std::endl;

          cbezier.split(dt2, sbezier1, sbezier2);

          std::cerr << "Expand Curve " << sbezier1 << std::endl;

          cparts1.addPart(new CCurveShapeBezier(sbezier1));

          cbezier  = sbezier2;
          t1      += dt2;
        }

        std::cerr << "Expand Curve " << sbezier2 << std::endl;

        cparts1.addPart(new CCurveShapeBezier(sbezier2));
      }
      else {
        std::cerr << "Normal Curve " << cbezier << std::endl;

        cparts1.addPart(new CCurveShapeBezier(cbezier));
      }
    }
  }

  return true;
}

bool
CPathShapePartList::
intersectCombineOr(CPathShapePartList &cparts1, CPathShapePartList &cparts2,
                   CPathShapePartList &iparts)
{
  uint num_cparts1 = cparts1.size();
  uint num_cparts2 = cparts2.size();

  // process pair or path curves, flipping between them at intersection points
  uint i = 0;

  int                             ii[2];
  int                             dir[2];
  int                             num_cparts[2];
  CPathShapePartList *cparts[2];

  ii[0] = 0;
  ii[1] = 0;

  dir[0] = 1;
  dir[1] = 1;

  num_cparts[0] = num_cparts1;
  num_cparts[1] = num_cparts2;

  cparts[0] = &cparts1;
  cparts[1] = &cparts2;

  // for 'boolean or' find first part with start on outside
  while (ii[i] < num_cparts[i]) {
    const CCurveShapePart *cpart = dynamic_cast<const CCurveShapePart *>((*cparts[i])[ii[i]]);

    if (cpart->isStartOutside()) break;

    ++ii[i];
  }

  if (ii[i] >= num_cparts[i])
    return false;

  //----

  // add start point (move to)

  const CCurveShapePart *cpart = dynamic_cast<const CCurveShapePart *>((*cparts[i])[ii[i]]);

  CPathPartType type = cpart->getType();

  if      (type == CPATH_PART_TYPE_LINE_TO) {
    const CCurveShapeLine *curveLine = dynamic_cast<const CCurveShapeLine *>(cpart);

    const CLine2D &cline = curveLine->getLine();

    const CPoint2D &p1 = cline.start();

    std::cerr << i << ") Add Move Start To " << p1 << std::endl;

    iparts.addPart(new CPathShapeMoveTo(p1));
  }
  else if (type == CPATH_PART_TYPE_CURVE3_TO) {
    const CCurveShapeBezier *curveBezier = dynamic_cast<const CCurveShapeBezier *>(cpart);

    const C3Bezier2D &cbezier = curveBezier->getBezier();

    const CPoint2D &p1 = cbezier.getFirstPoint();

    std::cerr << i << ") Add Move Start To " << p1 << std::endl;

    iparts.addPart(new CPathShapeMoveTo(p1));
  }

  //----

  while (true) {
    intersectStatus(cparts1, cparts2, i, ii, dir);

    //------

    const CCurveShapePart *cpart1 = dynamic_cast<const CCurveShapePart *>((*cparts[i])[ii[i]]);

    if (cpart1->visited_) break;

    cpart1->visited_ = true;

    uint num_ppoints = cpart1->ppoints_.size();

    CPathPartType type1 = cpart1->getType();

    if      (type1 == CPATH_PART_TYPE_LINE_TO) {
      const CCurveShapeLine *curveLine = dynamic_cast<const CCurveShapeLine *>(cpart1);

      if (num_ppoints != 0) {
        const CCurveShapePart::PartIPoint &ppoint = cpart1->ppoints_[0];

        // add intersection point and switch curve (if not totally inside)
        CCurveShapePart *cpart2 = dynamic_cast<CCurveShapePart *>((*cparts[1 - i])[ppoint.part]);

        if (cpart2->isStartOutside() || cpart2->isEndOutside()) {
          const CPoint2D &ipoint = ppoint.ibpoint.p;

          std::cerr << i << ") Add Line To Intersect " << ipoint << std::endl;

          iparts.addPart(new CPathShapeLineTo(ipoint));

          if (cpart2->visited_) break;

          cpart2->visited_ = true;

          i     = 1 - i;
          ii[i] = ppoint.part;

          intersectStatus(cparts1, cparts2, i, ii, dir);

          if (dir[i] > 0) { if (cpart2->isOutsideInside()) dir[i] = -dir[i]; }
          else            { if (cpart2->isInsideOutside()) dir[i] = -dir[i]; }

          CPathPartType type1 = cpart2->getType();

          if      (type1 == CPATH_PART_TYPE_LINE_TO) {
            const CCurveShapeLine *curveLine1 = dynamic_cast<const CCurveShapeLine *>(cpart2);

            const CLine2D &cline1 = curveLine1->getLine();

            if      (curveLine1->isEndOutside()) {
              const CPoint2D &p2 = cline1.end();

              std::cerr << i << ") Add Line End To " << p2 << std::endl;

              iparts.addPart(new CPathShapeLineTo(p2));
            }
            else if (curveLine1->isStartOutside()) {
              const CPoint2D &p1 = cline1.start();

              std::cerr << i << ") Add Line Start To " << p1 << std::endl;

              iparts.addPart(new CPathShapeLineTo(p1));
            }
          }
          else if (type1 == CPATH_PART_TYPE_CURVE3_TO) {
            const CCurveShapeBezier *curveBezier1 = dynamic_cast<const CCurveShapeBezier *>(cpart2);

            const C3Bezier2D &cbezier1 = curveBezier1->getBezier();

            double it = ppoint.ibpoint.t2;

            C3Bezier2D sbezier1, sbezier2;

            std::cerr << "Split Curve @ " << it << " " << cbezier1.calc(it) << std::endl;

            cbezier1.split(it, sbezier1, sbezier2);

            if      (curveBezier1->isEndOutside()) {
              const CPoint2D &p2 = sbezier2.getControlPoint1();
              const CPoint2D &p3 = sbezier2.getControlPoint2();
              const CPoint2D &p4 = sbezier2.getLastPoint    ();

              std::cerr << i << ") Add Curve2 To Intersect " <<
                           p2 << " " << p3 << " " << p4 << std::endl;

              iparts.addPart(new CPathShapeCurve3To(p2, p3, p4));
            }
            else if (curveBezier1->isStartOutside()) {
              const CPoint2D &p1 = sbezier1.getFirstPoint   ();
              const CPoint2D &p2 = sbezier1.getControlPoint1();
              const CPoint2D &p3 = sbezier1.getControlPoint2();

              std::cerr << i << ") Add Curve1 To Intersect " <<
                           p3 << " " << p2 << " " << p1 << std::endl;

              iparts.addPart(new CPathShapeCurve3To(p3, p2, p1));
            }
          }
        }
        else {
          // add line
          const CLine2D &cline = curveLine->getLine();

          if (dir[i] > 0) {
            const CPoint2D &p2 = cline.end();

            std::cerr << i << ") Add Line End To " << p2 << std::endl;

            iparts.addPart(new CPathShapeLineTo(p2));
          }
          else {
            const CPoint2D &p1 = cline.start();

            std::cerr << i << ") Add Line Start To " << p1 << std::endl;

            iparts.addPart(new CPathShapeLineTo(p1));
          }
        }
      }
      else {
        // add line
        const CLine2D &cline = curveLine->getLine();

        if (dir[i] > 0) {
          if (cpart1->isEndOutside()) {
            const CPoint2D &p2 = cline.end();

            std::cerr << i << ") Add Line End To " << p2 << std::endl;

            iparts.addPart(new CPathShapeLineTo(p2));
          }
        }
        else {
          if (cpart1->isStartOutside()) {
            const CPoint2D &p1 = cline.start();

            std::cerr << i << ") Add Line Start To " << p1 << std::endl;

            iparts.addPart(new CPathShapeLineTo(p1));
          }
        }
      }

      ii[i] += dir[i];

      if      (ii[i] <  0            ) ii[i] = num_cparts[i] - 1;
      else if (ii[i] >= num_cparts[i]) ii[i] = 0;
    }
    else if (type1 == CPATH_PART_TYPE_CURVE3_TO) {
      const CCurveShapeBezier *curveBezier = dynamic_cast<const CCurveShapeBezier *>(cpart1);

      const C3Bezier2D &cbezier = curveBezier->getBezier();

      if (num_ppoints != 0) {
        // add intersection point
        const CCurveShapePart::PartIPoint &ppoint = cpart1->ppoints_[0];

        double it = ppoint.ibpoint.t1;

        C3Bezier2D sbezier1, sbezier2;

        std::cerr << "Split Curve @ " << it << " " << cbezier.calc(it) << std::endl;

        cbezier.split(it, sbezier1, sbezier2);

        if (dir[i] > 0) {
          const CPoint2D &p2 = sbezier1.getControlPoint1();
          const CPoint2D &p3 = sbezier1.getControlPoint2();
          const CPoint2D &p4 = sbezier1.getLastPoint    ();

          std::cerr << i << ") Add Curve1 To Intersect " <<
                       p2 << " " << p3 << " " << p4 << std::endl;

          iparts.addPart(new CPathShapeCurve3To(p2, p3, p4));
        }
        else {
          const CPoint2D &p1 = sbezier2.getFirstPoint   ();
          const CPoint2D &p2 = sbezier2.getControlPoint1();
          const CPoint2D &p3 = sbezier2.getControlPoint2();

          std::cerr << i << ") Add Curve2 To Intersect " <<
                       p3 << " " << p2 << " " << p1 << std::endl;

          iparts.addPart(new CPathShapeCurve3To(p3, p2, p1));
        }

        // switch curve
        i     = 1 - i;
        ii[i] = ppoint.part;

        intersectStatus(cparts1, cparts2, i, ii, dir);

        CCurveShapePart *cpart2 = dynamic_cast<CCurveShapePart *>((*cparts[i])[ii[i]]);

        if (cpart2->visited_) break;

        cpart2->visited_ = true;

        if (dir[i] > 0) { if (cpart2->isOutsideInside()) dir[i] = -dir[i]; }
        else            { if (cpart2->isInsideOutside()) dir[i] = -dir[i]; }

        CPathPartType type1 = cpart2->getType();

        if      (type1 == CPATH_PART_TYPE_LINE_TO) {
          const CCurveShapeLine *curveLine1 = dynamic_cast<const CCurveShapeLine *>(cpart2);

          const CLine2D &cline1 = curveLine1->getLine();

          if (curveLine1->isEndOutside()) {
            const CPoint2D &p2 = cline1.end();

            std::cerr << i << ") Add Line End To " << p2 << std::endl;

            iparts.addPart(new CPathShapeLineTo(p2));
          }
          else {
            const CPoint2D &p1 = cline1.start();

            std::cerr << i << ") Add Line Start To " << p1 << std::endl;

            iparts.addPart(new CPathShapeLineTo(p1));
          }
        }
        else if (type1 == CPATH_PART_TYPE_CURVE3_TO) {
          const CCurveShapeBezier *curveBezier1 = dynamic_cast<const CCurveShapeBezier *>(cpart2);

          const C3Bezier2D &cbezier1 = curveBezier1->getBezier();

          double it = ppoint.ibpoint.t2;

          C3Bezier2D sbezier1, sbezier2;

          std::cerr << "Split Curve @ " << it << " " << cbezier1.calc(it) << std::endl;

          cbezier1.split(it, sbezier1, sbezier2);

          if (curveBezier1->isEndOutside()) {
            const CPoint2D &p2 = sbezier2.getControlPoint1();
            const CPoint2D &p3 = sbezier2.getControlPoint2();
            const CPoint2D &p4 = sbezier2.getLastPoint    ();

            std::cerr << i << ") Add Curve2 To Intersect " <<
                         p2 << " " << p3 << " " << p4 << std::endl;

            iparts.addPart(new CPathShapeCurve3To(p2, p3, p4));
          }
          else {
            const CPoint2D &p1 = sbezier1.getFirstPoint   ();
            const CPoint2D &p2 = sbezier1.getControlPoint1();
            const CPoint2D &p3 = sbezier1.getControlPoint2();

            std::cerr << i << ") Add Curve1 To Intersect " <<
                         p3 << " " << p2 << " " << p1 << std::endl;

            iparts.addPart(new CPathShapeCurve3To(p3, p2, p1));
          }
        }
      }
      else {
        // add curve
        const CPoint2D &p2 = cbezier.getControlPoint1();
        const CPoint2D &p3 = cbezier.getControlPoint2();
        const CPoint2D &p4 = cbezier.getLastPoint    ();

        if (cpart1->isEndOutside()) {
          std::cerr << i << ") Add Curve To " << p2 << " " << p3 << " " << p4 << std::endl;

          iparts.addPart(new CPathShapeCurve3To(p2, p3, p4));
        }
      }

      ii[i] += dir[i];

      if      (ii[i] <  0            ) ii[i] = num_cparts[i] - 1;
      else if (ii[i] >= num_cparts[i]) ii[i] = 0;
    }
  }

  std::cerr << "Add Close" << std::endl;

  iparts.addPart(new CPathShapeClose());

  return true;
}

bool
CPathShapePartList::
intersectCombineAnd(CPathShapePartList &cparts1, CPathShapePartList &cparts2,
                    CPathShapePartList &iparts)
{
  uint num_cparts1 = cparts1.size();
  uint num_cparts2 = cparts2.size();

  // process pair or path curves, flipping between them at intersection points
  uint i = 0;

  int                             ii[2];
  int                             dir[2];
  int                             num_cparts[2];
  CPathShapePartList *cparts[2];

  ii[0] = 0;
  ii[1] = 0;

  dir[0] = 1;
  dir[1] = 1;

  num_cparts[0] = num_cparts1;
  num_cparts[1] = num_cparts2;

  cparts[0] = &cparts1;
  cparts[1] = &cparts2;

  // for 'boolean and' find first part with start on inside
  while (ii[i] < num_cparts[i]) {
    const CCurveShapePart *cpart = dynamic_cast<const CCurveShapePart *>((*cparts[i])[ii[i]]);

    if (cpart->isStartInside()) break;

    ++ii[i];
  }

  if (ii[i] >= num_cparts[i]) {
    i = 1 - i;

    while (ii[i] < num_cparts[i]) {
      const CCurveShapePart *cpart = dynamic_cast<const CCurveShapePart *>((*cparts[i])[ii[i]]);

      if (cpart->isStartInside()) break;

      ++ii[i];
    }
  }

  if (ii[i] >= num_cparts[i])
    return false;

  //----

  // add start point (move to)

  const CCurveShapePart *cpart = dynamic_cast<const CCurveShapePart *>((*cparts[i])[ii[i]]);

  CPathPartType type = cpart->getType();

  if      (type == CPATH_PART_TYPE_LINE_TO) {
    const CCurveShapeLine *curveLine = dynamic_cast<const CCurveShapeLine *>(cpart);

    const CLine2D &cline = curveLine->getLine();

    const CPoint2D &p1 = cline.start();

    std::cerr << i << ") Add Move Start To " << p1 << std::endl;

    iparts.addPart(new CPathShapeMoveTo(p1));
  }
  else if (type == CPATH_PART_TYPE_CURVE3_TO) {
    const CCurveShapeBezier *curveBezier = dynamic_cast<const CCurveShapeBezier *>(cpart);

    const C3Bezier2D &cbezier = curveBezier->getBezier();

    const CPoint2D &p1 = cbezier.getFirstPoint();

    std::cerr << i << ") Add Move Start To " << p1 << std::endl;

    iparts.addPart(new CPathShapeMoveTo(p1));
  }

  //----

  while (true) {
    intersectStatus(cparts1, cparts2, i, ii, dir);

    //------

    const CCurveShapePart *cpart = dynamic_cast<const CCurveShapePart *>((*cparts[i])[ii[i]]);

    if (cpart->visited_) break;

    cpart->visited_ = true;

    uint num_ppoints = cpart->ppoints_.size();

    CPathPartType type = cpart->getType();

    if      (type == CPATH_PART_TYPE_LINE_TO) {
      const CCurveShapeLine *curveLine = dynamic_cast<const CCurveShapeLine *>(cpart);

      if (num_ppoints != 0) {
        // add intersection point
        const CCurveShapePart::PartIPoint &ppoint = cpart->ppoints_[0];

        const CPoint2D &ipoint = ppoint.ibpoint.p;

        std::cerr << i << ") Add Line To Intersect " << ipoint << std::endl;

        iparts.addPart(new CPathShapeLineTo(ipoint));

        // switch curve
        i     = 1 - i;
        ii[i] = ppoint.part;

        intersectStatus(cparts1, cparts2, i, ii, dir);

        CCurveShapePart *cpart2 = dynamic_cast<CCurveShapePart *>((*cparts[i])[ii[i]]);

        if (cpart2->visited_) break;

        cpart2->visited_ = true;

        if (dir[i] > 0) { if (cpart2->isEndOutside()) dir[i] = -dir[i]; }
        else            { if (cpart2->isEndInside ()) dir[i] = -dir[i]; }

        CPathPartType type1 = cpart2->getType();

        if      (type1 == CPATH_PART_TYPE_LINE_TO) {
          const CCurveShapeLine *curveLine1 = dynamic_cast<const CCurveShapeLine *>(cpart2);

          const CLine2D &cline1 = curveLine1->getLine();

          if (curveLine1->isEndInside()) {
            const CPoint2D &p2 = cline1.end();

            std::cerr << i << ") Add Line End To " << p2 << std::endl;

            iparts.addPart(new CPathShapeLineTo(p2));
          }
          else {
            const CPoint2D &p1 = cline1.start();

            std::cerr << i << ") Add Line Start To " << p1 << std::endl;

            iparts.addPart(new CPathShapeLineTo(p1));
          }
        }
        else if (type1 == CPATH_PART_TYPE_CURVE3_TO) {
          const CCurveShapeBezier *curveBezier1 = dynamic_cast<const CCurveShapeBezier *>(cpart2);

          const C3Bezier2D &cbezier1 = curveBezier1->getBezier();

          double it = ppoint.ibpoint.t2;

          C3Bezier2D sbezier1, sbezier2;

          std::cerr << "Split Curve @ " << it << " " << cbezier1.calc(it) << std::endl;

          cbezier1.split(it, sbezier1, sbezier2);

          if (curveBezier1->isEndInside()) {
            const CPoint2D &p2 = sbezier2.getControlPoint1();
            const CPoint2D &p3 = sbezier2.getControlPoint2();
            const CPoint2D &p4 = sbezier2.getLastPoint    ();

            std::cerr << i << ") Add Curve2 To Intersect " <<
                         p2 << " " << p3 << " " << p4 << std::endl;

            iparts.addPart(new CPathShapeCurve3To(p2, p3, p4));
          }
          else {
            const CPoint2D &p1 = sbezier1.getFirstPoint   ();
            const CPoint2D &p2 = sbezier1.getControlPoint1();
            const CPoint2D &p3 = sbezier1.getControlPoint2();

            std::cerr << i << ") Add Curve1 To Intersect " <<
                         p3 << " " << p2 << " " << p1 << std::endl;

            iparts.addPart(new CPathShapeCurve3To(p3, p2, p1));
          }
        }
      }
      else {
        // add line
        const CLine2D &cline = curveLine->getLine();

        if (dir[i] > 0) {
          if (cpart->isEndOutside()) {
            const CPoint2D &p2 = cline.end();

            std::cerr << i << ") Add Line End To " << p2 << std::endl;

            iparts.addPart(new CPathShapeLineTo(p2));
          }
        }
        else {
          if (cpart->isStartOutside()) {
            const CPoint2D &p1 = cline.start();

            std::cerr << i << ") Add Line Start To " << p1 << std::endl;

            iparts.addPart(new CPathShapeLineTo(p1));
          }
        }
      }

      ii[i] += dir[i];

      if      (ii[i] <  0            ) ii[i] = num_cparts[i] - 1;
      else if (ii[i] >= num_cparts[i]) ii[i] = 0;
    }
    else if (type == CPATH_PART_TYPE_CURVE3_TO) {
      const CCurveShapeBezier *curveBezier = dynamic_cast<const CCurveShapeBezier *>(cpart);

      const C3Bezier2D &cbezier = curveBezier->getBezier();

      if (num_ppoints != 0) {
        // add intersection point
        const CCurveShapePart::PartIPoint &ppoint = cpart->ppoints_[0];

        double it = ppoint.ibpoint.t1;

        C3Bezier2D isbezier1, isbezier2;

        std::cerr << "Split Curve @ " << it << " " << cbezier.calc(it) << std::endl;

        cbezier.split(it, isbezier1, isbezier2);

        if (dir[i] > 0) {
          const CPoint2D &p2 = isbezier1.getControlPoint1();
          const CPoint2D &p3 = isbezier1.getControlPoint2();
          const CPoint2D &p4 = isbezier1.getLastPoint    ();

          std::cerr << i << ") Add Curve1 To Intersect " <<
                       p2 << " " << p3 << " " << p4 << std::endl;

          iparts.addPart(new CPathShapeCurve3To(p2, p3, p4));
        }
        else {
          const CPoint2D &p1 = isbezier2.getFirstPoint   ();
          const CPoint2D &p2 = isbezier2.getControlPoint1();
          const CPoint2D &p3 = isbezier2.getControlPoint2();

          std::cerr << i << ") Add Curve2 To Intersect " <<
                       p3 << " " << p2 << " " << p1 << std::endl;

          iparts.addPart(new CPathShapeCurve3To(p3, p2, p1));
        }

        // switch curve
        i     = 1 - i;
        ii[i] = ppoint.part;

        intersectStatus(cparts1, cparts2, i, ii, dir);

        CCurveShapePart *cpart2 = dynamic_cast<CCurveShapePart *>((*cparts[i])[ii[i]]);

        if (cpart2->visited_) break;

        cpart2->visited_ = true;

        if (dir[i] > 0) { if (cpart2->isEndOutside  ()) dir[i] = -dir[i]; }
        else            { if (cpart2->isStartOutside()) dir[i] = -dir[i]; }

        CPathPartType type1 = cpart2->getType();

        if      (type1 == CPATH_PART_TYPE_LINE_TO) {
          const CCurveShapeLine *curveLine1 = dynamic_cast<const CCurveShapeLine *>(cpart2);

          const CLine2D &cline1 = curveLine1->getLine();

          if (cpart2->isEndInside()) {
            const CPoint2D &p2 = cline1.end();

            std::cerr << i << ") Add Line End To " << p2 << std::endl;

            iparts.addPart(new CPathShapeLineTo(p2));
          }
          else {
            const CPoint2D &p2 = cline1.end();

            std::cerr << i << ") Add Line Start To " << p2 << std::endl;

            iparts.addPart(new CPathShapeLineTo(p2));
          }
        }
        else if (type1 == CPATH_PART_TYPE_CURVE3_TO) {
          const CCurveShapeBezier *curveBezier1 = dynamic_cast<const CCurveShapeBezier *>(cpart2);

          const C3Bezier2D &cbezier1 = curveBezier1->getBezier();

          double it = ppoint.ibpoint.t2;

          C3Bezier2D isbezier1, isbezier2;

          std::cerr << "Split Curve @ " << it << " " << cbezier1.calc(it) << std::endl;

          cbezier1.split(it, isbezier1, isbezier2);

          if (cpart2->isEndInside()) {
            const CPoint2D &p2 = isbezier2.getControlPoint1();
            const CPoint2D &p3 = isbezier2.getControlPoint2();
            const CPoint2D &p4 = isbezier2.getLastPoint    ();

            std::cerr << i << ") Add Curve2 To Intersect " <<
                         p2 << " " << p3 << " " << p4 << std::endl;

            iparts.addPart(new CPathShapeCurve3To(p2, p3, p4));
          }
          else {
            const CPoint2D &p1 = isbezier1.getFirstPoint   ();
            const CPoint2D &p2 = isbezier1.getControlPoint1();
            const CPoint2D &p3 = isbezier1.getControlPoint2();

            std::cerr << i << ") Add Curve1 To Intersect " <<
                         p3 << " " << p2 << " " << p1 << std::endl;

            iparts.addPart(new CPathShapeCurve3To(p3, p2, p1));
          }
        }
      }
      else {
        // add curve
        const CPoint2D &p2 = cbezier.getControlPoint1();
        const CPoint2D &p3 = cbezier.getControlPoint2();
        const CPoint2D &p4 = cbezier.getLastPoint    ();

        std::cerr << i << ") Add Curve To " << p2 << " " << p3 << " " << p4 << std::endl;

        iparts.addPart(new CPathShapeCurve3To(p2, p3, p4));
      }

      ii[i] += dir[i];

      if      (ii[i] <  0            ) ii[i] = num_cparts[i] - 1;
      else if (ii[i] >= num_cparts[i]) ii[i] = 0;
    }
  }

  std::cerr << "Add Close" << std::endl;

  iparts.addPart(new CPathShapeClose());

  return true;
}

bool
CPathShapePartList::
intersectCombineXor(CPathShapePartList &cparts1, CPathShapePartList &cparts2,
                    CPathShapePartListArray &ipartsList, bool split)
{
  uint num_cparts1 = cparts1.size();
  uint num_cparts2 = cparts2.size();

  CPathShapePartList *cparts[2];
  int                             num_cparts[2];

  cparts[0] = &cparts1; num_cparts[0] = num_cparts1;
  cparts[1] = &cparts2; num_cparts[1] = num_cparts2;

  CPathShapePartList iparts;

  // for each shape
  for (uint j = 0; j < 2; ++j) {
    // for each outside start point
    for (;;) {
      // find first unvisited part with start outside and mark visited
      int i = 0;

      const CCurveShapePart *cpart;

      for ( ; i < num_cparts[j]; ++i) {
        cpart = dynamic_cast<const CCurveShapePart *>((*cparts[j])[i]);

        if (cpart->visited_) continue;

        if (cpart->isStartOutside()) break;
      }

      // no part found so we are done
      if (i >= num_cparts[j]) break;

      std::cerr << "Unvisited: " << j << ":" << i << std::endl;

      cpart->visited_ = true;

      // find previous part with start inside
      int ii = i;

      do {
        --i;

        if (i < 0) { i = num_cparts[j] - 1; }

        if (i == ii) break;

        cpart = dynamic_cast<const CCurveShapePart *>((*cparts[j])[i]);
      }
      while (cpart->isStartOutside());

      // TODO: how does this happen
      if (i == ii) { std::cerr << "Failed to find start" << std::endl; break; }

      std::cerr << "Start: " << j << ":" << i << std::endl;

      // add and save intersection point
      //const CCurveShapePart *cparti = cpart;

      if (cpart->ppoints_.empty()) {
        std::cerr << "No intersections on outside->inside" << std::endl;
        break;
      }

      const CCurveShapePart::PartIPoint &ppoint = cpart->ppoints_[0];

      const CPoint2D &ipoint = ppoint.ibpoint.p;

      std::cerr << i << ") Add Move To Intersect " << ipoint << std::endl;

      iparts.addPart(new CPathShapeMoveTo(ipoint));

      // add to next intersection
      bool first = true;

      do {
        cpart->visited_ = true;

        CPathPartType type = cpart->getType();

        if      (type == CPATH_PART_TYPE_LINE_TO) {
          const CCurveShapeLine *curveLine = dynamic_cast<const CCurveShapeLine *>(cpart);

          const CLine2D &cline = curveLine->getLine();

          const CPoint2D &p2 = cline.end();

          std::cerr << i << ") Add Line To " << p2 << std::endl;

          iparts.addPart(new CPathShapeLineTo(p2));
        }
        else if (type == CPATH_PART_TYPE_CURVE3_TO) {
          const CCurveShapeBezier *curveBezier = dynamic_cast<const CCurveShapeBezier *>(cpart);

          const C3Bezier2D &cbezier = curveBezier->getBezier();

          // if first use end of split bezier
          if (first) {
            double it = ppoint.ibpoint.t1;

            C3Bezier2D isbezier1, isbezier2;

            std::cerr << "Split Curve @ " << it << " " << cbezier.calc(it) << std::endl;

            cbezier.split(it, isbezier1, isbezier2);

            const CPoint2D &p2 = isbezier2.getControlPoint1();
            const CPoint2D &p3 = isbezier2.getControlPoint2();
            const CPoint2D &p4 = isbezier2.getLastPoint    ();

            std::cerr << i << ") Add Curve To " << p2 << " " << p3 << " " << p4 << std::endl;

            iparts.addPart(new CPathShapeCurve3To(p2, p3, p4));
          }
          // otherwise use whole curve
          else {
            const CPoint2D &p2 = cbezier.getControlPoint1();
            const CPoint2D &p3 = cbezier.getControlPoint2();
            const CPoint2D &p4 = cbezier.getLastPoint    ();

            std::cerr << i << ") Add Curve To " << p2 << " " << p3 << " " << p4 << std::endl;

            iparts.addPart(new CPathShapeCurve3To(p2, p3, p4));
          }
        }

        first = false;

        ++i;

        if (i >= num_cparts[j]) i = 0;

        cpart = dynamic_cast<const CCurveShapePart *>((*cparts[j])[i]);
      }
      while (cpart->ppoints_.empty());

      // add intersection point

      cpart->visited_ = true;

      const CCurveShapePart::PartIPoint &ppoint1 = cpart->ppoints_[0];

      CPathPartType type = cpart->getType();

      if      (type == CPATH_PART_TYPE_LINE_TO) {
        const CPoint2D &ipoint = ppoint1.ibpoint.p;

        std::cerr << i << ") Add Line To Intersect " << ipoint << std::endl;

        iparts.addPart(new CPathShapeLineTo(ipoint));
      }
      else if (type == CPATH_PART_TYPE_CURVE3_TO) {
        const CCurveShapeBezier *curveBezier = dynamic_cast<const CCurveShapeBezier *>(cpart);

        const C3Bezier2D &cbezier = curveBezier->getBezier();

        double it = ppoint1.ibpoint.t1;

        C3Bezier2D sbezier1, sbezier2;

        std::cerr << "Split Curve @ " << it << " " << cbezier.calc(it) << std::endl;

        cbezier.split(it, sbezier1, sbezier2);

        const CPoint2D &p2 = sbezier1.getControlPoint1();
        const CPoint2D &p3 = sbezier1.getControlPoint2();
        const CPoint2D &p4 = sbezier1.getLastPoint    ();

        std::cerr << i << ") Add Curve To Intersect " << p2 << " " << p3 << " " << p4 << std::endl;

        iparts.addPart(new CPathShapeCurve3To(p2, p3, p4));
      }

      // switch curve

      int j1 = 1 - j;

      i = ppoint1.part;

      cpart = dynamic_cast<const CCurveShapePart *>((*cparts[j1])[i]);

      first = true;

      if (cpart->isEndInside()) {
        do {
          cpart->visited_ = true;

          CPathPartType type = cpart->getType();

          if      (type == CPATH_PART_TYPE_LINE_TO) {
            const CCurveShapeLine *curveLine = dynamic_cast<const CCurveShapeLine *>(cpart);

            const CLine2D &cline = curveLine->getLine();

            const CPoint2D &p2 = cline.end();

            std::cerr << i << ") Add Line To " << p2 << std::endl;

            iparts.addPart(new CPathShapeLineTo(p2));
          }
          else if (type == CPATH_PART_TYPE_CURVE3_TO) {
            const CCurveShapeBezier *curveBezier = dynamic_cast<const CCurveShapeBezier *>(cpart);

            const C3Bezier2D &cbezier = curveBezier->getBezier();

            if (first) {
              // add intersect to end point if first part
              C3Bezier2D sbezier1, sbezier2;

              double it = ppoint1.ibpoint.t2;

              std::cerr << "Split Curve @ " << it << " " << cbezier.calc(it) << std::endl;

              cbezier.split(it, sbezier1, sbezier2);

              const CPoint2D &p2 = sbezier2.getControlPoint1();
              const CPoint2D &p3 = sbezier2.getControlPoint2();
              const CPoint2D &p4 = sbezier2.getLastPoint    ();

              std::cerr << i << ") Add Curve To " << p2 << " " << p3 << " " << p4 << std::endl;

              iparts.addPart(new CPathShapeCurve3To(p2, p3, p4));
            }
            else {
              // add whole curve if non-first part
              const CPoint2D &p2 = cbezier.getControlPoint1();
              const CPoint2D &p3 = cbezier.getControlPoint2();
              const CPoint2D &p4 = cbezier.getLastPoint    ();

              std::cerr << i << ") Add Curve To " << p2 << " " << p3 << " " << p4 << std::endl;

              iparts.addPart(new CPathShapeCurve3To(p2, p3, p4));
            }
          }

          first = false;

          ++i;

          if (i >= num_cparts[j1]) i = 0;

          cpart = dynamic_cast<const CCurveShapePart *>((*cparts[j1])[i]);
        }
        while (cpart->ppoints_.empty());

        // add closing curve if needed

        cpart->visited_ = true;

        if (cpart->getType() == CPATH_PART_TYPE_CURVE3_TO) {
          const CCurveShapeBezier *curveBezier = dynamic_cast<const CCurveShapeBezier *>(cpart);

          const C3Bezier2D &cbezier = curveBezier->getBezier();

          const CCurveShapePart::PartIPoint &ppoint1 = cpart->ppoints_[0];

          double it = ppoint1.ibpoint.t1;

          C3Bezier2D sbezier1, sbezier2;

          std::cerr << "Split Curve @ " << it << " " << cbezier.calc(it) << std::endl;

          cbezier.split(it, sbezier1, sbezier2);

          const CPoint2D &p2 = sbezier1.getControlPoint1();
          const CPoint2D &p3 = sbezier1.getControlPoint2();
          const CPoint2D &p4 = sbezier1.getLastPoint    ();

          std::cerr << i << ") Add Curve To Intersect " <<
                       p2 << " " << p3 << " " << p4 << std::endl;

          iparts.addPart(new CPathShapeCurve3To(p2, p3, p4));
        }
      }
      else {
        do {
          cpart->visited_ = true;

          CPathPartType type = cpart->getType();

          if      (type == CPATH_PART_TYPE_LINE_TO) {
            const CCurveShapeLine *curveLine = dynamic_cast<const CCurveShapeLine *>(cpart);

            const CLine2D &cline = curveLine->getLine();

            const CPoint2D &p1 = cline.start();

            std::cerr << i << ") Add Line To " << p1 << std::endl;

            iparts.addPart(new CPathShapeLineTo(p1));
          }
          else if (type == CPATH_PART_TYPE_CURVE3_TO) {
            const CCurveShapeBezier *curveBezier = dynamic_cast<const CCurveShapeBezier *>(cpart);

            const C3Bezier2D &cbezier = curveBezier->getBezier();

            if (first) {
              // add intersect to start point if first part
              C3Bezier2D sbezier1, sbezier2;

              double it = ppoint1.ibpoint.t2;

              std::cerr << "Split Curve @ " << it << " " << cbezier.calc(it) << std::endl;

              cbezier.split(it, sbezier1, sbezier2);

              const CPoint2D &p1 = sbezier1.getFirstPoint   ();
              const CPoint2D &p2 = sbezier1.getControlPoint1();
              const CPoint2D &p3 = sbezier1.getControlPoint2();

              std::cerr << i << ") Add Curve To " << p3 << " " << p2 << " " << p1 << std::endl;

              iparts.addPart(new CPathShapeCurve3To(p3, p2, p1));
            }
            else {
              // add whole curve if non-first part
              const CPoint2D &p1 = cbezier.getFirstPoint   ();
              const CPoint2D &p2 = cbezier.getControlPoint1();
              const CPoint2D &p3 = cbezier.getControlPoint2();

              std::cerr << i << ") Add Curve To " << p3 << " " << p2 << " " << p1 << std::endl;

              iparts.addPart(new CPathShapeCurve3To(p3, p2, p1));
            }
          }

          first = false;

          --i;

          if (i < 0) i = num_cparts[j1] - 1;

          cpart = dynamic_cast<const CCurveShapePart *>((*cparts[j1])[i]);
        }
        while (cpart->ppoints_.empty());

        // add closing curve if needed

        cpart->visited_ = true;

        if (cpart->getType() == CPATH_PART_TYPE_CURVE3_TO) {
          const CCurveShapeBezier *curveBezier = dynamic_cast<const CCurveShapeBezier *>(cpart);

          const C3Bezier2D &cbezier = curveBezier->getBezier();

          const CCurveShapePart::PartIPoint &ppoint1 = cpart->ppoints_[0];

          double it = ppoint1.ibpoint.t1;

          C3Bezier2D sbezier1, sbezier2;

          std::cerr << "Split Curve @ " << it << " " << cbezier.calc(it) << std::endl;

          cbezier.split(it, sbezier1, sbezier2);

          const CPoint2D &p1 = sbezier2.getFirstPoint   ();
          const CPoint2D &p2 = sbezier2.getControlPoint1();
          const CPoint2D &p3 = sbezier2.getControlPoint2();

          std::cerr << i << ") Add Curve To Intersect " <<
                       p3 << " " << p2 << " " << p1 << std::endl;

          iparts.addPart(new CPathShapeCurve3To(p3, p2, p1));
        }
      }

      std::cerr << "Add Close" << std::endl;

      iparts.addPart(new CPathShapeClose());

      if (split) {
        ipartsList.push_back(j, iparts);

        iparts.clear();
      }
    }
  }

  if (! split)
    ipartsList.push_back(iparts);

  return true;
}

bool
CPathShapePartList::
combineOr(CPathShapePartList &cparts1, CPathShapePartList &cparts2, CPathShapePartList &iparts)
{
  // or - result is outside curve

  uint num_cparts1 = cparts1.size();
  uint num_cparts2 = cparts2.size();

  if (num_cparts1 == 0 || num_cparts2 == 0) return false;

  // find which curve is outside
  const CCurveShapePart *cpart = dynamic_cast<const CCurveShapePart *>(cparts1[0]);

  CPathShapePartList *cparts;
  uint                            num_cparts;

  if (cpart->isStartOutside()) {
    cparts = &cparts1; num_cparts = num_cparts1;
  }
  else {
    cparts = &cparts2; num_cparts = num_cparts2;
  }

  for (uint i = 0; i < num_cparts; ++i) {
    const CCurveShapePart *cpart = dynamic_cast<const CCurveShapePart *>((*cparts)[i]);

    CPathPartType type = cpart->getType();

    if      (type == CPATH_PART_TYPE_LINE_TO) {
      const CCurveShapeLine *curveLine = dynamic_cast<const CCurveShapeLine *>(cpart);

      const CLine2D &cline = curveLine->getLine();

      if (i == 0) {
        const CPoint2D &p1 = cline.start();

        iparts.addPart(new CPathShapeMoveTo(p1));
      }

      const CPoint2D &p2 = cline.end();

      iparts.addPart(new CPathShapeLineTo(p2));
    }
    else if (type == CPATH_PART_TYPE_CURVE3_TO) {
      const CCurveShapeBezier *curveBezier = dynamic_cast<const CCurveShapeBezier *>(cpart);

      const C3Bezier2D &cbezier = curveBezier->getBezier();

      if (i == 0) {
        const CPoint2D &p1 = cbezier.getFirstPoint();

        iparts.addPart(new CPathShapeMoveTo(p1));
      }

      const CPoint2D &p2 = cbezier.getControlPoint1();
      const CPoint2D &p3 = cbezier.getControlPoint2();
      const CPoint2D &p4 = cbezier.getLastPoint    ();

      iparts.addPart(new CPathShapeCurve3To(p2, p3, p4));
    }
  }

  iparts.addPart(new CPathShapeClose());

  return true;
}

bool
CPathShapePartList::
combineAnd(CPathShapePartList &cparts1, CPathShapePartList &cparts2, CPathShapePartList &iparts)
{
  // and - result is inside curve

  uint num_cparts1 = cparts1.size();
  uint num_cparts2 = cparts2.size();

  if (num_cparts1 == 0 || num_cparts2 == 0) return false;

  // find which curve is inside
  const CCurveShapePart *cpart = dynamic_cast<const CCurveShapePart *>(cparts1[0]);

  CPathShapePartList *cparts;
  uint                num_cparts;

  if (cpart->isStartInside()) {
    cparts = &cparts1; num_cparts = num_cparts1;
  }
  else {
    cparts = &cparts2; num_cparts = num_cparts2;
  }

  for (uint i = 0; i < num_cparts; ++i) {
    const CCurveShapePart *cpart = dynamic_cast<const CCurveShapePart *>((*cparts)[i]);

    CPathPartType type = cpart->getType();

    if      (type == CPATH_PART_TYPE_LINE_TO) {
      const CCurveShapeLine *curveLine = dynamic_cast<const CCurveShapeLine *>(cpart);

      const CLine2D &cline = curveLine->getLine();

      if (i == 0) {
        const CPoint2D &p1 = cline.start();

        iparts.addPart(new CPathShapeMoveTo(p1));
      }

      const CPoint2D &p2 = cline.end();

      iparts.addPart(new CPathShapeLineTo(p2));
    }
    else if (type == CPATH_PART_TYPE_CURVE3_TO) {
      const CCurveShapeBezier *curveBezier = dynamic_cast<const CCurveShapeBezier *>(cpart);

      const C3Bezier2D &cbezier = curveBezier->getBezier();

      if (i == 0) {
        const CPoint2D &p1 = cbezier.getFirstPoint();

        iparts.addPart(new CPathShapeMoveTo(p1));
      }

      const CPoint2D &p2 = cbezier.getControlPoint1();
      const CPoint2D &p3 = cbezier.getControlPoint2();
      const CPoint2D &p4 = cbezier.getLastPoint    ();

      iparts.addPart(new CPathShapeCurve3To(p2, p3, p4));
    }
  }

  iparts.addPart(new CPathShapeClose());

  return true;
}

bool
CPathShapePartList::
combineXor(CPathShapePartList &cparts1, CPathShapePartList &cparts2, CPathShapePartList &iparts)
{
  uint num_cparts1 = cparts1.size();
  uint num_cparts2 = cparts2.size();

  if (num_cparts1 == 0 || num_cparts2 == 0) return false;

  CPathShapePartList *cparts[2];
  uint                            num_cparts[2];

  cparts[0] = &cparts1; num_cparts[0] = num_cparts1;
  cparts[1] = &cparts2; num_cparts[1] = num_cparts2;

  for (uint j = 0; j < 2; ++j) {
    for (uint i = 0; i < num_cparts[j]; ++i) {
      const CCurveShapePart *cpart = dynamic_cast<const CCurveShapePart *>((*cparts[j])[i]);

      CPathPartType type = cpart->getType();

      if      (type == CPATH_PART_TYPE_LINE_TO) {
        const CCurveShapeLine *curveLine = dynamic_cast<const CCurveShapeLine *>(cpart);

        const CLine2D &cline = curveLine->getLine();

        if (i == 0) {
          const CPoint2D &p1 = cline.start();

          iparts.addPart(new CPathShapeMoveTo(p1));
        }

        const CPoint2D &p2 = cline.end();

        iparts.addPart(new CPathShapeLineTo(p2));
      }
      else if (type == CPATH_PART_TYPE_CURVE3_TO) {
        const CCurveShapeBezier *curveBezier = dynamic_cast<const CCurveShapeBezier *>(cpart);

        const C3Bezier2D &cbezier = curveBezier->getBezier();

        if (i == 0) {
          const CPoint2D &p1 = cbezier.getFirstPoint();

          iparts.addPart(new CPathShapeMoveTo(p1));
        }

        const CPoint2D &p2 = cbezier.getControlPoint1();
        const CPoint2D &p3 = cbezier.getControlPoint2();
        const CPoint2D &p4 = cbezier.getLastPoint    ();

        iparts.addPart(new CPathShapeCurve3To(p2, p3, p4));
      }
    }

    iparts.addPart(new CPathShapeClose());
  }

  return true;
}

CMathGen::IntersectType
CPathShapePartList::
getInsideType(const CPolygon2D &polygon1, const CPolygon2D &polygon2)
{
  CMathGen::IntersectType type = CMathGen::INTERSECT_NONE;

  const std::vector<CPoint2D> &points = polygon1.getPoints();

  uint num_points = points.size();

  for (uint i = 0; i < num_points; ++i) {
    bool inside = polygon2.inside(points[i]);

    if (type == CMathGen::INTERSECT_NONE)
      type = (inside ? CMathGen::INTERSECT_INSIDE : CMathGen::INTERSECT_OUTSIDE);
    else {
      if (  inside && type != CMathGen::INTERSECT_INSIDE ) return CMathGen::INTERSECT_NONE;
      if (! inside && type != CMathGen::INTERSECT_OUTSIDE) return CMathGen::INTERSECT_NONE;
    }
  }

  return type;
}

void
CPathShapePartList::
intersectStatus(CPathShapePartList &cparts1, CPathShapePartList &cparts2,
                int i, int *ii, int *dir)
{
  uint num_cparts1 = cparts1.size();

  std::cerr << "Curve 0: ";

  for (uint i1 = 0; i1 < num_cparts1; ++i1) {
    const CCurveShapePart *cpart1 = dynamic_cast<const CCurveShapePart *>(cparts1[i1]);

    std::cerr << (cpart1->visited_ ? CStrUtil::toString(i1) : "_");
  }

  std::cerr << (i == 0 ? " (*)" : "    ") << " Ind: " << ii[0] << " Dir: " << dir[0] << std::endl;

  uint num_cparts2 = cparts2.size();

  std::cerr << "Curve 1: ";

  for (uint i2 = 0; i2 < num_cparts2; ++i2) {
    const CCurveShapePart *cpart2 = dynamic_cast<const CCurveShapePart *>(cparts2[i2]);

    std::cerr << (cpart2->visited_ ? CStrUtil::toString(i2) : "_");
  }

  std::cerr << (i == 1 ? " (*)" : "    ") << " Ind: " << ii[1] << " Dir: " << dir[1] << std::endl;
}

uint
CPathShapePartList::
intersect(const CCurveShapePart *cpart1, uint i1,
          const CPathShapePartList &cparts2, const CLine2D &line)
{
  uint num_itersects = 0;

  uint num_cparts2 = cparts2.size();

  if (num_cparts2 == 0) return 0;

  for (uint i2 = 0; i2 < num_cparts2; ++i2) {
    const CCurveShapePart *cpart2 = dynamic_cast<const CCurveShapePart *>(cparts2[i2]);

    CPathPartType type2 = cpart2->getType();

    if      (type2 == CPATH_PART_TYPE_LINE_TO) {
      const CCurveShapeLine *curveLine = dynamic_cast<const CCurveShapeLine *>(cpart2);

      const CLine2D &cline = curveLine->getLine();

      const CPoint2D &p1 = cline.start();
      const CPoint2D &p2 = cline.end  ();

      CLine2D line1(p1, p2);

      CPoint2D ipoint;
      double   mu1, mu2;

      if (! CMathGeom2D::LinesAreCoincident(line, line1)) {
        if (CMathGeom2D::IntersectLine(line, line1, &ipoint, &mu1, &mu2) &&
            mu1 >= 0.0 && mu1 <= 1.0 && mu2 >= 0.0 && mu2 <= 1.0) {
          // set intersect for part 1 line
          std::cerr << "0," << i2 << ": Add Intersect " << ipoint << std::endl;

          if (! cpart1->ppoints_.empty())
            std::cerr << "0," << i2 << ": L/L Multiple Intersects" << std::endl;

          // set intersect for part 2 line
          cpart1->ppoints_.push_back(
            CCurveShapePart::PartIPoint(i2, CPoint2DParam2(ipoint, mu1, mu2)));

          std::cerr << "1," << i1 << ": Add Intersect " << ipoint << std::endl;

          if (! cpart2->ppoints_.empty())
            std::cerr << "1," << i1 << ": L/L Multiple Intersects" << std::endl;

          cpart2->ppoints_.push_back(
            CCurveShapePart::PartIPoint(i1, CPoint2DParam2(ipoint, mu2, mu1)));

          ++num_itersects;
        }
      }
      else {
#if 0
        mu1 = line.getParam(line1.start());
        mu2 = 0.0;

        if (mu1 >= 0.0 && mu1 <= 1.0) {
          // set intersect for part 1 line
          std::cerr << "0," << i2 << ": Add Intersect " << line1.start() << std::endl;

          if (! cpart1->ppoints_.empty())
            std::cerr << "0," << i2 << ": L/L Multiple Intersects" << std::endl;

          // set intersect for part 2 line
          cpart1->ppoints_.push_back(
            CCurveShapePart::PartIPoint(i2, CPoint2DParam2(line1.start(), mu1, mu2)));

          std::cerr << "1," << i1 << ": Add Intersect " << line1.start() << std::endl;

          if (! cpart2->ppoints_.empty())
            std::cerr << "1," << i1 << ": L/L Multiple Intersects" << std::endl;

          cpart2->ppoints_.push_back(
            CCurveShapePart::PartIPoint(i1, CPoint2DParam2(line1.start(), mu2, mu1)));

          ++num_itersects;
        }

        mu1 = line.getParam(line1.end());
        mu2 = 1.0;

        if (mu1 >= 0.0 && mu1 <= 1.0) {
          // set intersect for part 1 line
          std::cerr << "0," << i2 << ": Add Intersect " << line1.end() << std::endl;

          if (! cpart1->ppoints_.empty())
            std::cerr << "0," << i2 << ": L/L Multiple Intersects" << std::endl;

          // set intersect for part 2 line
          cpart1->ppoints_.push_back(
            CCurveShapePart::PartIPoint(i2, CPoint2DParam2(line1.end(), mu1, mu2)));

          std::cerr << "1," << i1 << ": Add Intersect " << line1.end() << std::endl;

          if (! cpart2->ppoints_.empty())
            std::cerr << "1," << i1 << ": L/L Multiple Intersects" << std::endl;

          cpart2->ppoints_.push_back(
            CCurveShapePart::PartIPoint(i1, CPoint2DParam2(line1.end(), mu2, mu1)));

          ++num_itersects;
        }

        mu1 = 0.0;
        mu2 = line1.getParam(line.start());

        if (mu2 >= 0.0 && mu2 <= 1.0) {
          // set intersect for part 1 line
          std::cerr << "0," << i2 << ": Add Intersect " << line.start() << std::endl;

          if (! cpart1->ppoints_.empty())
            std::cerr << "0," << i2 << ": L/L Multiple Intersects" << std::endl;

          // set intersect for part 2 line
          cpart1->ppoints_.push_back(
            CCurveShapePart::PartIPoint(i2, CPoint2DParam2(line.start(), mu1, mu2)));

          std::cerr << "1," << i1 << ": Add Intersect " << line.start() << std::endl;

          if (! cpart2->ppoints_.empty())
            std::cerr << "1," << i1 << ": L/L Multiple Intersects" << std::endl;

          cpart2->ppoints_.push_back(
            CCurveShapePart::PartIPoint(i1, CPoint2DParam2(line.start(), mu2, mu1)));

          ++num_itersects;
        }

        mu1 = 1.0;
        mu2 = line1.getParam(line.end());

        if (mu2 >= 0.0 && mu2 <= 1.0) {
          // set intersect for part 1 line
          std::cerr << "0," << i2 << ": Add Intersect " << line.end() << std::endl;

          if (! cpart1->ppoints_.empty())
            std::cerr << "0," << i2 << ": L/L Multiple Intersects" << std::endl;

          // set intersect for part 2 line
          cpart1->ppoints_.push_back(
            CCurveShapePart::PartIPoint(i2, CPoint2DParam2(line.end(), mu1, mu2)));

          std::cerr << "1," << i1 << ": Add Intersect " << line.end() << std::endl;

          if (! cpart2->ppoints_.empty())
            std::cerr << "1," << i1 << ": L/L Multiple Intersects" << std::endl;

          cpart2->ppoints_.push_back(
            CCurveShapePart::PartIPoint(i1, CPoint2DParam2(line.end(), mu2, mu1)));

          ++num_itersects;
        }
#endif
      }
    }
    else if (type2 == CPATH_PART_TYPE_CURVE3_TO) {
      const CCurveShapeBezier *curveBezier = dynamic_cast<const CCurveShapeBezier *>(cpart2);

      const C3Bezier2D &cbezier = curveBezier->getBezier();

      const CPoint2D &p1 = cbezier.getFirstPoint   ();
      const CPoint2D &p2 = cbezier.getControlPoint1();
      const CPoint2D &p3 = cbezier.getControlPoint2();
      const CPoint2D &p4 = cbezier.getLastPoint    ();

      C3Bezier2D bezier1(p1, p2, p3, p4);

      std::vector<CPoint2DParam2> ibpoints;

      CMathGeom2D::BezierLineIntersect(bezier1, line, ibpoints, 1E-4);

      uint num_ibpoints = ibpoints.size();

      if (num_ibpoints > 0) {
        if (! cpart1->ppoints_.empty())
          std::cerr << "0," << i2 << ": C/L Multiple Intersects" << std::endl;
        if (! cpart2->ppoints_.empty())
          std::cerr << "1," << i1 << ": C/L Multiple Intersects" << std::endl;

        for (uint k = 0; k < num_ibpoints; ++k) {
          // set intersect for part 2 bezier
          std::cerr << "1," << i1 << ": Add Intersect " << ibpoints[k].p <<
                       "(" << ibpoints[k].t1 << ")" << std::endl;
          cpart2->ppoints_.push_back(CCurveShapePart::PartIPoint(i1, ibpoints[k]));

          // set intersect for part 1 line
          std::swap(ibpoints[k].t1, ibpoints[k].t2);

          std::cerr << "0," << i2 << ": Add Intersect " << ibpoints[k].p <<
                       "(" << ibpoints[k].t1 << ")" << std::endl;
          cpart1->ppoints_.push_back(CCurveShapePart::PartIPoint(i2, ibpoints[k]));

          std::swap(ibpoints[k].t1, ibpoints[k].t2);
        }

        num_itersects += num_ibpoints;
      }
    }
  }

  return num_itersects;
}

uint
CPathShapePartList::
intersect(const CCurveShapePart *cpart1, uint i1, const CPathShapePartList &cparts2,
          const C3Bezier2D &bezier)
{
  uint num_itersects = 0;

  uint num_cparts2 = cparts2.size();

  if (num_cparts2 == 0) return 0;

  for (uint i2 = 0; i2 < num_cparts2; ++i2) {
    const CCurveShapePart *cpart2 = dynamic_cast<const CCurveShapePart *>(cparts2[i2]);

    CPathPartType type2 = cpart2->getType();

    if      (type2 == CPATH_PART_TYPE_LINE_TO) {
      const CCurveShapeLine *curveLine = dynamic_cast<const CCurveShapeLine *>(cpart2);

      const CLine2D &cline = curveLine->getLine();

      const CPoint2D &p1 = cline.start();
      const CPoint2D &p2 = cline.end  ();

      CLine2D line1(p1, p2);

      std::vector<CPoint2DParam2> ibpoints;

      CMathGeom2D::BezierLineIntersect(bezier, line1, ibpoints, 1E-4);

      uint num_ibpoints = ibpoints.size();

      if (num_ibpoints > 0) {
        if (! cpart1->ppoints_.empty())
          std::cerr << "0," << i2 << ": C/L Multiple Intersects" << std::endl;
        if (! cpart2->ppoints_.empty())
          std::cerr << "1," << i1 << ": C/L Multiple Intersects" << std::endl;

        for (uint k = 0; k < num_ibpoints; ++k) {
          // set intersect for part 1 bezier
          std::cerr << "0," << i2 << ": Add Intersect " << ibpoints[k].p <<
                       "(" << ibpoints[k].t1 << ")" << std::endl;
          cpart1->ppoints_.push_back(CCurveShapePart::PartIPoint(i2, ibpoints[k]));

          // set intersect for part 2 line
          std::swap(ibpoints[k].t1, ibpoints[k].t2);

          std::cerr << "1," << i1 << ": Add Intersect " << ibpoints[k].p <<
                       "(" << ibpoints[k].t1 << ")" << std::endl;
          cpart2->ppoints_.push_back(CCurveShapePart::PartIPoint(i1, ibpoints[k]));

          std::swap(ibpoints[k].t1, ibpoints[k].t2);
        }

        num_itersects += num_ibpoints;
      }
    }
    else if (type2 == CPATH_PART_TYPE_CURVE3_TO) {
      const CCurveShapeBezier *curveBezier = dynamic_cast<const CCurveShapeBezier *>(cpart2);

      const C3Bezier2D &cbezier = curveBezier->getBezier();

      const CPoint2D &p1 = cbezier.getFirstPoint   ();
      const CPoint2D &p2 = cbezier.getControlPoint1();
      const CPoint2D &p3 = cbezier.getControlPoint2();
      const CPoint2D &p4 = cbezier.getLastPoint    ();

      C3Bezier2D bezier1(p1, p2, p3, p4);

      std::vector<CPoint2DParam2> ibpoints;

      CMathGeom2D::BezierBezierIntersect(bezier, bezier1, ibpoints, 1E-4);

      uint num_ibpoints = ibpoints.size();

      if (num_ibpoints > 0) {
        if (! cpart1->ppoints_.empty())
          std::cerr << "0," << i1 << ": C/C Multiple Intersects" << std::endl;
        if (! cpart2->ppoints_.empty())
          std::cerr << "1," << i1 << ": C/C Multiple Intersects" << std::endl;

        for (uint k = 0; k < num_ibpoints; ++k) {
          // set intersect for part 1 bezier
          std::cerr << "0," << i2 << ": Add Intersect " << ibpoints[k].p <<
                       "(" << ibpoints[k].t1 << ")" << std::endl;
          cpart1->ppoints_.push_back(CCurveShapePart::PartIPoint(i2, ibpoints[k]));

          // set intersect for part 2 bezier
          std::swap(ibpoints[k].t1, ibpoints[k].t2);

          std::cerr << "1," << i1 << ": Add Intersect " << ibpoints[k].p <<
                       "(" << ibpoints[k].t1 << ")" << std::endl;
          cpart2->ppoints_.push_back(CCurveShapePart::PartIPoint(i1, ibpoints[k]));

          std::swap(ibpoints[k].t1, ibpoints[k].t2);
        }

        num_itersects += num_ibpoints;
      }
    }
  }

  return num_itersects;
}

bool
CPathShapePartList::
removeArcs(CPathShapePartList &parts) const
{
  uint num_parts = parts_.size();

  uint num_arcs = 0;

  for (uint i = 0; i < num_parts; ++i) {
    const CPathShapePart *part = parts_[i];

    CPathPartType type = part->getType();

    if (type == CPATH_PART_TYPE_ARC)
      ++num_arcs;
  }

  if (num_arcs == 0)
    return false;

  for (uint i = 0; i < num_parts; ++i) {
    const CPathShapePart *part = parts_[i];

    CPathPartType type = part->getType();

    if      (type == CPATH_PART_TYPE_MOVE_TO) {
      parts.addPart(part->dup());
    }
    else if (type == CPATH_PART_TYPE_LINE_TO) {
      parts.addPart(part->dup());
    }
    else if (type == CPATH_PART_TYPE_CURVE2_TO) {
      parts.addPart(part->dup());
    }
    else if (type == CPATH_PART_TYPE_CURVE3_TO) {
      parts.addPart(part->dup());
    }
    else if (type == CPATH_PART_TYPE_ARC) {
      const CPathShapeArc *arcToPart = dynamic_cast<const CPathShapeArc *>(part);

      CPoint2D c      = arcToPart->getCenter();
      double   xr     = arcToPart->getRadiusX();
      double   yr     = arcToPart->getRadiusY();
      double   angle1 = arcToPart->getAngle1();
      double   angle2 = arcToPart->getAngle2();

      std::vector<C3Bezier2D> beziers;

      CMathGeom2D::ArcToBeziers(c.x, c.y, xr, yr, angle1, angle2, beziers);

      uint num_beziers = beziers.size();

      for (uint i = 0; i < num_beziers; ++i) {
        const C3Bezier2D &bezier = beziers[i];

        CPoint2D p2 = bezier.getControlPoint1();
        CPoint2D p3 = bezier.getControlPoint2();
        CPoint2D p4 = bezier.getLastPoint    ();

        parts.addPart(new CPathShapeCurve3To(p2, p3, p4));
      }
    }
    else if (type == CPATH_PART_TYPE_CLOSE) {
      parts.addPart(part->dup());
    }
  }

  return true;
}

void
CPathShapePartList::
flatten(const CMatrix2D &m, std::vector<CPolygon2D> &polygons) const
{
  std::vector<CPoint2D> points;

  CPoint2D p0, p1;

  uint num_parts = parts_.size();

  if (num_parts == 0) return;

  for (uint i = 0; i < num_parts; ++i) {
    const CPathShapePart *part = parts_[i];

    CPathPartType type = part->getType();

    if      (type == CPATH_PART_TYPE_MOVE_TO) {
      const CPathShapeMoveTo *moveToPart = dynamic_cast<const CPathShapeMoveTo *>(part);

      p0 = m*moveToPart->getPoint();

      points.push_back(p0);

      p1 = p0;
    }
    else if (type == CPATH_PART_TYPE_LINE_TO) {
      const CPathShapeLineTo *lineToPart = dynamic_cast<const CPathShapeLineTo *>(part);

      CPoint2D p2 = m*lineToPart->getPoint();

      points.push_back(p2);

      p1 = p2;
    }
    else if (type == CPATH_PART_TYPE_CURVE2_TO) {
      const CPathShapeCurve2To *curveToPart = dynamic_cast<const CPathShapeCurve2To *>(part);

      CPoint2D p2 = m*curveToPart->getPoint1();
      CPoint2D p3 = m*curveToPart->getPoint2();

      C2Bezier2D bezier(p1, p2, p3);

      std::vector<CPoint2D> points1;

      CMathGeom2D::BezierToLines(bezier, points1);

      uint num_points1 = points1.size();

      for (uint i = 1; i < num_points1; ++i)
        points.push_back(points1[i]);

      p1 = p3;
    }
    else if (type == CPATH_PART_TYPE_CURVE3_TO) {
      const CPathShapeCurve3To *curveToPart = dynamic_cast<const CPathShapeCurve3To *>(part);

      CPoint2D p2 = m*curveToPart->getPoint1();
      CPoint2D p3 = m*curveToPart->getPoint2();
      CPoint2D p4 = m*curveToPart->getPoint3();

      C3Bezier2D bezier(p1, p2, p3, p4);

      std::vector<CPoint2D> points1;

      CMathGeom2D::BezierToLines(bezier, points1);

      uint num_points1 = points1.size();

      for (uint i = 1; i < num_points1; ++i)
        points.push_back(points1[i]);

      p1 = p4;
    }
    else if (type == CPATH_PART_TYPE_ARC) {
      const CPathShapeArc *arcToPart = dynamic_cast<const CPathShapeArc *>(part);

      CPoint2D c      = m*arcToPart->getCenter();
      double   xr     = arcToPart->getRadiusX();
      double   yr     = arcToPart->getRadiusY();
      double   angle1 = arcToPart->getAngle1();
      double   angle2 = arcToPart->getAngle2();

      std::vector<C3Bezier2D> beziers;

      CMathGeom2D::ArcToBeziers(c.x, c.y, xr, yr, angle1, angle2, beziers);

      uint num_beziers = beziers.size();

      for (uint i = 0; i < num_beziers; ++i) {
        const C3Bezier2D &bezier = beziers[i];

        std::vector<CPoint2D> points1;

        CMathGeom2D::BezierToLines(bezier, points1);

        uint num_points1 = points1.size();

        for (uint j = 1; j < num_points1; ++j) {
          if (i != 0 || j != 0)
            points.push_back(points1[j]);
        }

        p1 = bezier.getLastPoint();
      }
    }
    else if (type == CPATH_PART_TYPE_CLOSE) {
      points.push_back(p0);

      p1 = p0;

      polygons.push_back(CPolygon2D(points));

      points.clear();
    }
  }

  if (! points.empty())
    polygons.push_back(CPolygon2D(points));
}

void
CPathShapePartList::
flattenCurve(std::vector<CPoint2D> &points) const
{
  uint num_parts = parts_.size();

  if (num_parts == 0) return;

  for (uint i = 0; i < num_parts; ++i) {
    const CPathShapePart *part = parts_[i];

    CPathPartType type = part->getType();

    if      (type == CPATH_PART_TYPE_LINE_TO) {
      const CCurveShapeLine *curveLine = dynamic_cast<const CCurveShapeLine *>(part);

      const CLine2D &line = curveLine->getLine();

      if (points.empty())
        points.push_back(line.start());

      points.push_back(line.end());
    }
    else if (type == CPATH_PART_TYPE_CURVE3_TO) {
      const CCurveShapeBezier *curveBezier = dynamic_cast<const CCurveShapeBezier *>(part);

      const C3Bezier2D &bezier = curveBezier->getBezier();

      if (points.empty())
        points.push_back(bezier.getFirstPoint());

      points.push_back(bezier.getControlPoint1());
      points.push_back(bezier.getControlPoint2());
      points.push_back(bezier.getLastPoint    ());
    }
  }
}

void
CPathShapePartList::
toCurves(const CMatrix2D &m, CPathShapePartListArray &partsList) const
{
  CPathShapePartList parts;

  CPoint2D p0, p1;

  uint num_parts = parts_.size();

  if (num_parts == 0) return;

  for (uint i = 0; i < num_parts; ++i) {
    const CPathShapePart *part = parts_[i];

    CPathPartType type = part->getType();

    if      (type == CPATH_PART_TYPE_MOVE_TO) {
      const CPathShapeMoveTo *moveToPart = dynamic_cast<const CPathShapeMoveTo *>(part);

      p0 = m*moveToPart->getPoint();
      p1 = p0;
    }
    else if (type == CPATH_PART_TYPE_LINE_TO) {
      const CPathShapeLineTo *lineToPart = dynamic_cast<const CPathShapeLineTo *>(part);

      CPoint2D p2 = m*lineToPart->getPoint();

      CLine2D line(p1, p2);

      parts.addPart(new CCurveShapeLine(line));

      std::cerr << "Line " << line << std::endl;

      p1 = p2;
    }
    else if (type == CPATH_PART_TYPE_CURVE2_TO) {
      const CPathShapeCurve2To *curveToPart = dynamic_cast<const CPathShapeCurve2To *>(part);

      CPoint2D p2 = m*curveToPart->getPoint1();
      CPoint2D p3 = m*curveToPart->getPoint2();

      C3Bezier2D bezier(C2Bezier2D(p1, p2, p3));

      parts.addPart(new CCurveShapeBezier(bezier));

      std::cerr << "Bezier " << bezier << std::endl;

      p1 = p3;
    }
    else if (type == CPATH_PART_TYPE_CURVE3_TO) {
      const CPathShapeCurve3To *curveToPart = dynamic_cast<const CPathShapeCurve3To *>(part);

      CPoint2D p2 = m*curveToPart->getPoint1();
      CPoint2D p3 = m*curveToPart->getPoint2();
      CPoint2D p4 = m*curveToPart->getPoint3();

      C3Bezier2D bezier(p1, p2, p3, p4);

      parts.addPart(new CCurveShapeBezier(bezier));

      std::cerr << "Bezier " << bezier << std::endl;

      p1 = p4;
    }
    else if (type == CPATH_PART_TYPE_ARC) {
      const CPathShapeArc *arcToPart = dynamic_cast<const CPathShapeArc *>(part);

      const CPoint2D &c      = arcToPart->getCenter();
      double          xr     = arcToPart->getRadiusX();
      double          yr     = arcToPart->getRadiusY();
      double          angle1 = arcToPart->getAngle1();
      double          angle2 = arcToPart->getAngle2();

      std::vector<C3Bezier2D> beziers;

      CMathGeom2D::ArcToBeziers(c.x, c.y, xr, yr, angle1, angle2, beziers);

      uint num_beziers = beziers.size();

      for (uint i = 0; i < num_beziers; ++i) {
        const C3Bezier2D &bezier = beziers[i];

        CPoint2D pb1 = m*bezier.getFirstPoint   ();
        CPoint2D pb2 = m*bezier.getControlPoint1();
        CPoint2D pb3 = m*bezier.getControlPoint2();
        CPoint2D pb4 = m*bezier.getLastPoint    ();

        C3Bezier2D bezier1(pb1, pb2, pb3, pb4);

        parts.addPart(new CCurveShapeBezier(bezier1));

        std::cerr << "Bezier " << bezier1 << std::endl;

        p1 = pb4;
      }
    }
    else if (type == CPATH_PART_TYPE_CLOSE) {
      if (! p1.equal(p0, 1E-4)) {
        CLine2D line(p1, p0);

        parts.addPart(new CCurveShapeLine(line));

        std::cerr << "Line " << line << std::endl;
      }

      p1 = p0;

      if (! parts.empty()) {
        partsList.push_back(parts);

        parts.clear();
      }
    }
  }

  if (! parts.empty())
    partsList.push_back(parts);
}

void
CPathShapePartList::
fromCurve(CPathShapePartList &parts) const
{
  CPoint2D p0, p1;

  uint num_parts = parts_.size();

  if (num_parts == 0) return;

  for (uint i = 0; i < num_parts; ++i) {
    const CPathShapePart *part = parts_[i];

    CPathPartType type = part->getType();

    if      (type == CPATH_PART_TYPE_LINE_TO) {
      const CCurveShapeLine *curveLine = dynamic_cast<const CCurveShapeLine *>(part);

      const CLine2D &line = curveLine->getLine();

      const CPoint2D &p1 = line.start();
      const CPoint2D &p2 = line.end  ();

      if (i == 0)
        parts.addPart(new CPathShapeMoveTo(p1));

      parts.addPart(new CPathShapeLineTo(p2));
    }
    else if (type == CPATH_PART_TYPE_CURVE3_TO) {
      const CCurveShapeBezier *curveBezier = dynamic_cast<const CCurveShapeBezier *>(part);

      const C3Bezier2D &bezier = curveBezier->getBezier();

      const CPoint2D &p1 = bezier.getFirstPoint   ();
      const CPoint2D &p2 = bezier.getControlPoint1();
      const CPoint2D &p3 = bezier.getControlPoint2();
      const CPoint2D &p4 = bezier.getLastPoint    ();

      if (i == 0)
        parts.addPart(new CPathShapeMoveTo(p1));

      parts.addPart(new CPathShapeCurve3To(p2, p3, p4));
    }
  }

  parts.addPart(new CPathShapeClose);
}

void
CPathShapePartList::
flipCurve()
{
  CPoint2D p0, p1;

  uint num_parts = parts_.size();

  if (num_parts == 0) return;

  for (uint i = 0; i < num_parts; ++i) {
    CPathShapePart *part = parts_[i];

    CPathPartType type = part->getType();

    if      (type == CPATH_PART_TYPE_LINE_TO) {
      CCurveShapeLine *curveLine = dynamic_cast<CCurveShapeLine *>(part);

      const CLine2D &line = curveLine->getLine();

      CPoint2D p1 = line.start();
      CPoint2D p2 = line.end  ();

      curveLine->setLine(CLine2D(p2, p1));

      std::swap(curveLine->inside_[0], curveLine->inside_[1]);
    }
    else if (type == CPATH_PART_TYPE_CURVE3_TO) {
      CCurveShapeBezier *curveBezier = dynamic_cast<CCurveShapeBezier *>(part);

      const C3Bezier2D &bezier = curveBezier->getBezier();

      CPoint2D p1 = bezier.getFirstPoint   ();
      CPoint2D p2 = bezier.getControlPoint1();
      CPoint2D p3 = bezier.getControlPoint2();
      CPoint2D p4 = bezier.getLastPoint    ();

      curveBezier->setBezier(C3Bezier2D(p4, p3, p2, p1));

      std::swap(curveBezier->inside_[0], curveBezier->inside_[3]);
      std::swap(curveBezier->inside_[1], curveBezier->inside_[2]);
    }
  }
}

bool
CPathShapePartList::
combine(const CPathShapePartList &parts1, const CPathShapePartList &parts2,
        CPathShapePartList &parts)
{
  CMatrix2D m1, m2;

  m1.setIdentity();
  m2.setIdentity();

  return combine(parts1, m1, parts2, m2, parts);
}

bool
CPathShapePartList::
combine(const CPathShapePartList &parts1, const CMatrix2D &m1,
        const CPathShapePartList &parts2, const CMatrix2D &m2,
        CPathShapePartList &parts)
{
  CPoint2D p0, p1;

  uint num_parts1 = parts1.size();

  if (num_parts1 == 0) return false;

  for (uint i = 0; i < num_parts1; ++i) {
    CPathShapePart *part1 = parts1[i]->dup();

    CPathPartType type = part1->getType();

    parts.addPart(part1);

    if      (type == CPATH_PART_TYPE_MOVE_TO) {
      CPathShapeMoveTo *moveToPart = dynamic_cast<CPathShapeMoveTo *>(part1);

      moveToPart->setPoint(m1*moveToPart->getPoint());
    }
    else if (type == CPATH_PART_TYPE_LINE_TO) {
      CPathShapeLineTo *lineToPart = dynamic_cast<CPathShapeLineTo *>(part1);

      lineToPart->setPoint(m1*lineToPart->getPoint());
    }
    else if (type == CPATH_PART_TYPE_CURVE2_TO) {
      CPathShapeCurve2To *curveToPart = dynamic_cast<CPathShapeCurve2To *>(part1);

      curveToPart->setPoint1(m1*curveToPart->getPoint1());
      curveToPart->setPoint2(m1*curveToPart->getPoint2());
    }
    else if (type == CPATH_PART_TYPE_CURVE3_TO) {
      CPathShapeCurve3To *curveToPart = dynamic_cast<CPathShapeCurve3To *>(part1);

      curveToPart->setPoint1(m1*curveToPart->getPoint1());
      curveToPart->setPoint2(m1*curveToPart->getPoint2());
      curveToPart->setPoint3(m1*curveToPart->getPoint3());
    }
    else if (type == CPATH_PART_TYPE_ARC) {
      CPathShapeArc *arcToPart = dynamic_cast<CPathShapeArc *>(part1);

      arcToPart->setCenter(m1*arcToPart->getCenter());
    }
    else if (type == CPATH_PART_TYPE_CLOSE) {
    }
  }

  //-----

  uint num_parts2 = parts2.size();

  if (num_parts2 == 0) return false;

  for (uint i = 0; i < num_parts2; ++i) {
    CPathShapePart *part2 = parts2[i]->dup();

    CPathPartType type = part2->getType();

    parts.addPart(part2);

    if      (type == CPATH_PART_TYPE_MOVE_TO) {
      CPathShapeMoveTo *moveToPart = dynamic_cast<CPathShapeMoveTo *>(part2);

      moveToPart->setPoint(m2*moveToPart->getPoint());
    }
    else if (type == CPATH_PART_TYPE_LINE_TO) {
      CPathShapeLineTo *lineToPart = dynamic_cast<CPathShapeLineTo *>(part2);

      lineToPart->setPoint(m2*lineToPart->getPoint());
    }
    else if (type == CPATH_PART_TYPE_CURVE2_TO) {
      CPathShapeCurve2To *curveToPart = dynamic_cast<CPathShapeCurve2To *>(part2);

      curveToPart->setPoint1(m2*curveToPart->getPoint1());
      curveToPart->setPoint2(m2*curveToPart->getPoint2());
    }
    else if (type == CPATH_PART_TYPE_CURVE3_TO) {
      CPathShapeCurve3To *curveToPart = dynamic_cast<CPathShapeCurve3To *>(part2);

      curveToPart->setPoint1(m2*curveToPart->getPoint1());
      curveToPart->setPoint2(m2*curveToPart->getPoint2());
      curveToPart->setPoint3(m2*curveToPart->getPoint3());
    }
    else if (type == CPATH_PART_TYPE_ARC) {
      CPathShapeArc *arcToPart = dynamic_cast<CPathShapeArc *>(part2);

      arcToPart->setCenter(m2*arcToPart->getCenter());
    }
    else if (type == CPATH_PART_TYPE_CLOSE) {
    }
  }

  return true;
}

//----------

void
CPathShapeArc::
getEndPoints(CPoint2D &point1, CPoint2D &point2) const
{
  CPoint2D c = getCenter();

  double xr = getRadiusX();
  double yr = getRadiusY();

  double a1 = getAngle1();
  double a2 = getAngle2();

  double x1, y1, x2, y2;

  CMathGeom2D::EllipsePointAtAngle(c.x, c.y, xr, yr, a1, &x1, &y1);
  CMathGeom2D::EllipsePointAtAngle(c.x, c.y, xr, yr, a2, &x2, &y2);

  point1 = CPoint2D(x1, y1);
  point2 = CPoint2D(x2, y2);
}
