#ifndef CPathShape_H
#define CPathShape_H

#include <CPoint2D.h>
#include <CPolygon2D.h>
#include <CMatrix2D.h>
#include <CPathPartType.h>
#include <CBooleanOp.h>
#include <C3Bezier2D.h>

class CPathShapePart {
 public:
  CPathShapePart(CPathPartType type) : type_(type) { }

  virtual ~CPathShapePart() { }

  virtual CPathShapePart *dup() const = 0;

  CPathPartType getType() const { return type_; }

 protected:
  CPathPartType type_;
};

struct CPathShapeMoveTo : public CPathShapePart {
 public:
  CPathShapeMoveTo(const CPoint2D &p) :
   CPathShapePart(CPATH_PART_TYPE_MOVE_TO), p_(p) {
  }

  CPathShapeMoveTo *dup() const { return new CPathShapeMoveTo(p_); }

  const CPoint2D &getPoint() const { return p_; }

  CPoint2D &getPoint() { return p_; }

  void setPoint(const CPoint2D &p) { p_ = p; }

 protected:
  CPoint2D p_;
};

struct CPathShapeLineTo : public CPathShapePart {
 public:
  CPathShapeLineTo(const CPoint2D &p) :
   CPathShapePart(CPATH_PART_TYPE_LINE_TO), p_(p) {
  }

  CPathShapeLineTo *dup() const { return new CPathShapeLineTo(p_); }

  const CPoint2D &getPoint() const { return p_; }

  CPoint2D &getPoint() { return p_; }

  void setPoint(const CPoint2D &p) { p_ = p; }

 protected:
  CPoint2D p_;
};

struct CPathShapeCurve2To : public CPathShapePart {
 public:
  CPathShapeCurve2To(const CPoint2D &p1, const CPoint2D &p2) :
   CPathShapePart(CPATH_PART_TYPE_CURVE2_TO), p1_(p1), p2_(p2) {
  }

  CPathShapeCurve2To *dup() const {
    return new CPathShapeCurve2To(p1_, p2_);
  }

  const CPoint2D &getPoint1() const { return p1_; }
  const CPoint2D &getPoint2() const { return p2_; }

  CPoint2D &getPoint1() { return p1_; }
  CPoint2D &getPoint2() { return p2_; }

  void setPoint1(const CPoint2D &p) { p1_ = p; }
  void setPoint2(const CPoint2D &p) { p2_ = p; }

 protected:
  CPoint2D p1_, p2_;
};

struct CPathShapeCurve3To : public CPathShapePart {
 public:
  CPathShapeCurve3To(const CPoint2D &p1, const CPoint2D &p2, const CPoint2D &p3) :
   CPathShapePart(CPATH_PART_TYPE_CURVE3_TO), p1_(p1), p2_(p2), p3_(p3) {
  }

  CPathShapeCurve3To *dup() const {
    return new CPathShapeCurve3To(p1_, p2_, p3_);
  }

  const CPoint2D &getPoint1() const { return p1_; }
  const CPoint2D &getPoint2() const { return p2_; }
  const CPoint2D &getPoint3() const { return p3_; }

  CPoint2D &getPoint1() { return p1_; }
  CPoint2D &getPoint2() { return p2_; }
  CPoint2D &getPoint3() { return p3_; }

  void setPoint1(const CPoint2D &p) { p1_ = p; }
  void setPoint2(const CPoint2D &p) { p2_ = p; }
  void setPoint3(const CPoint2D &p) { p3_ = p; }

 protected:
  CPoint2D p1_, p2_, p3_;
};

class CPathShapeArc : public CPathShapePart {
 public:
  CPathShapeArc(const CPoint2D &c, double xr, double yr, double angle1, double angle2) :
   CPathShapePart(CPATH_PART_TYPE_ARC), c_(c), xr_(xr), yr_(yr),
   angle1_(angle1), angle2_(angle2) {
  }

  CPathShapeArc *dup() const {
    return new CPathShapeArc(c_, xr_, yr_, angle1_, angle2_);
  }

  const CPoint2D &getCenter() const { return c_; }

  CPoint2D &getCenter() { return c_; }

  double getRadiusX() const { return xr_; }
  double getRadiusY() const { return yr_; }

  double getAngle1() const { return angle1_; }
  double getAngle2() const { return angle2_; }

  void getEndPoints(CPoint2D &point1, CPoint2D &point2) const;

  void setCenter(const CPoint2D &p) { c_ = p; }

  void setAngle1(double angle1) { angle1_ = angle1; }
  void setAngle2(double angle2) { angle2_ = angle2; }

 protected:
  CPoint2D c_;
  double   xr_, yr_;
  double   angle1_, angle2_;
};

struct CPathShapeClose : public CPathShapePart {
 public:
  CPathShapeClose() :
   CPathShapePart(CPATH_PART_TYPE_CLOSE) {
  }

  CPathShapeClose *dup() const { return new CPathShapeClose; }
};

class CPathShapeProcess {
 public:
  CPathShapeProcess() { }

  virtual ~CPathShapeProcess() { }

  virtual void moveTo  (const CPoint2D &p) = 0;
  virtual void lineTo  (const CPoint2D &p) = 0;
  virtual void curve2To(const CPoint2D &p1, const CPoint2D &p2) = 0;
  virtual void curve3To(const CPoint2D &p1, const CPoint2D &p2, const CPoint2D &p3) = 0;
  virtual void arcTo   (const CPoint2D &c, double xr, double yr, double angle1, double angle2) = 0;
  virtual void close   () = 0;
};

//-----------

class CCurveShapePart : public CPathShapePart {
 public:
  CCurveShapePart(CPathPartType type) :
   CPathShapePart(type), ppoints_(), visited_(false) {
  }

  virtual bool isStartInside() const = 0;
  virtual bool isEndInside  () const = 0;

  bool isStartOutside() const { return ! isStartInside(); }
  bool isEndOutside  () const { return ! isEndInside  (); }

  bool isOutsideInside() const { return isStartOutside() && isEndInside (); }
  bool isInsideOutside() const { return isStartInside () && isEndOutside(); }

 public:
  // temp values for intersect code
  struct PartIPoint {
    uint           part;
    CPoint2DParam2 ibpoint;

    PartIPoint(uint tpart, const CPoint2DParam2 &tibpoint) :
     part(tpart), ibpoint(tibpoint) {
    }
  };

  mutable std::vector<PartIPoint> ppoints_;
  mutable bool                    visited_;
};

class CCurveShapeLine : public CCurveShapePart {
 public:
  CCurveShapeLine(const CLine2D &line) :
   CCurveShapePart(CPATH_PART_TYPE_LINE_TO), line_(line) {
  }

  CCurveShapeLine *dup() const { return new CCurveShapeLine(line_); }

  const CLine2D &getLine() const { return line_; }

  void setLine(const CLine2D &line) { line_ = line; }

  bool isStartInside() const { return inside_[0]; }
  bool isEndInside  () const { return inside_[1]; }

 private:
  CLine2D line_;

 public:
  mutable bool inside_[2];
};

class CCurveShapeBezier : public CCurveShapePart {
 public:
  CCurveShapeBezier(const C3Bezier2D &bezier) :
   CCurveShapePart(CPATH_PART_TYPE_CURVE3_TO), bezier_(bezier) {
  }

  CCurveShapeBezier *dup() const { return new CCurveShapeBezier(bezier_); }

  const C3Bezier2D &getBezier() const { return bezier_; }

  void setBezier(const C3Bezier2D &bezier) { bezier_ = bezier; }

  bool isStartInside() const { return inside_[0]; }
  bool isEndInside  () const { return inside_[3]; }

 private:
  C3Bezier2D bezier_;

 public:
  mutable bool inside_[4];
};

//-----------

class CPathShapePartList;

class CPathShapePartListArray {
 public:
  CPathShapePartListArray() { }

  void push_back(const CPathShapePartList &iparts) {
    push_back(0, iparts);
  }

  void push_back(uint id, const CPathShapePartList &iparts) {
    ids_      .push_back(id);
    partsList_.push_back(iparts);
  }

  const CPathShapePartList &operator[](int i) const {
    return partsList_[i];
  }

  CPathShapePartList &operator[](int i) {
    return partsList_[i];
  }

  uint getId(uint i) {
    return ids_[i];
  }

  uint size() {
    return partsList_.size();
  }

 private:
  std::vector<CPathShapePartList> partsList_;
  std::vector<uint>                           ids_;
};

class CPathShapePartList {
 public:
  CPathShapePartList();
  CPathShapePartList(const CPathShapePartList &parts);

 ~CPathShapePartList();

  CPathShapePartList &operator=(const CPathShapePartList &parts);

  bool empty() const { return parts_.empty(); }

  uint size() const { return parts_.size(); }

  void clear();

  const CPathShapePart *operator[](int i) const { return parts_[i]; }

  CPathShapePart *operator[](int i) { return parts_[i]; }

  void addPart(CPathShapePart *part);

  void addPart(uint i, CPathShapePart *part);

  void setPart(uint i, CPathShapePart *part);

  void removePart(uint ind);

  void addLine(const CLine2D &line);

  void processPath(CPathShapeProcess *process);

  void strokePath(double w, CPathShapePartList &parts);
  void offsetPath(double w, const CPoint2D &c, CPathShapePartList &parts);

  static bool intersect(CBooleanOp op, const CPathShapePartList &parts1, const CMatrix2D &m1,
                        const CPathShapePartList &parts2, const CMatrix2D &m2,
                        CPathShapePartListArray &partsList, bool split);

  bool removeArcs(CPathShapePartList &parts) const;

  void flatten(const CMatrix2D &m, std::vector<CPolygon2D> &polygons) const;

  void toCurves(const CMatrix2D &m, CPathShapePartListArray &partsList) const;

  void flattenCurve(std::vector<CPoint2D> &points) const;

  void fromCurve(CPathShapePartList &parts) const;

  static bool combine(const CPathShapePartList &parts1, const CPathShapePartList &parts2,
                      CPathShapePartList &parts);
  static bool combine(const CPathShapePartList &parts1, const CMatrix2D &m1,
                      const CPathShapePartList &parts2, const CMatrix2D &m2,
                      CPathShapePartList &parts);

  void flipCurve();

 private:
  static bool intersectParts(CBooleanOp op, CPathShapePartList &cparts1,
                             CPathShapePartList &cparts2, const CPolygon2D &polygon1,
                             const CPolygon2D &polygon2, CPathShapePartListArray &ipartsList,
                             bool split, CMathGen::IntersectType intersectType);

  static void setCurveInside(CPathShapePartList &cparts, const CPolygon2D &polygon);
  static uint intersectCurves(CPathShapePartList &cparts1, CPathShapePartList &cparts2);

  static uint intersect(const CCurveShapePart *cpart1, uint i1,
                        const CPathShapePartList &parts2, const CLine2D &line);
  static uint intersect(const CCurveShapePart *cpart1, uint i1,
                        const CPathShapePartList &parts2, const C3Bezier2D &bezier);

  static bool expandParts(CPathShapePartList &cparts, CPathShapePartList &cparts1);

  static bool intersectCombineOr (CPathShapePartList &cparts1, CPathShapePartList &cparts2,
                                  CPathShapePartList &iparts);
  static bool intersectCombineAnd(CPathShapePartList &cparts1, CPathShapePartList &cparts2,
                                  CPathShapePartList &iparts);
  static bool intersectCombineXor(CPathShapePartList &cparts1, CPathShapePartList &cparts2,
                                  CPathShapePartListArray &ipartsList, bool split);

  static bool combineOr (CPathShapePartList &cparts1, CPathShapePartList &cparts2,
                         CPathShapePartList &iparts);
  static bool combineAnd(CPathShapePartList &cparts1, CPathShapePartList &cparts2,
                         CPathShapePartList &iparts);
  static bool combineXor(CPathShapePartList &cparts1, CPathShapePartList &cparts2,
                         CPathShapePartList &iparts);

  static CMathGen::IntersectType getInsideType(const CPolygon2D &polygon1,
                                               const CPolygon2D &polygon2);

  static void intersectStatus(CPathShapePartList &cparts1, CPathShapePartList &cparts2,
                              int i, int *ii, int *dir);

 private:
  typedef std::vector<CPathShapePart *> PartList;

  PartList parts_;
};

#endif
