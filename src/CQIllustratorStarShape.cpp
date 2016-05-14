#include <CQIllustratorStarShape.h>
#include <CQIllustratorStarGeometry.h>
#include <CQIllustratorShapeDrawer.h>

CQIllustratorStarShape::
CQIllustratorStarShape(const CPoint2D &c, uint n, double r1, double r2, double a1, double a2) :
 CQIllustratorShape(), c_(c), n_(n), r1_(r1), r2_(r2), a1_(a1), a2_(a2)
{
}

CQIllustratorStarShape::
CQIllustratorStarShape(const CQIllustratorStarShape &star) :
 CQIllustratorShape(star), c_(star.c_), n_(star.n_), r1_(star.r1_), r2_(star.r2_),
 a1_(star.a1_), a2_(star.a2_)
{
}

CQIllustratorStarShape *
CQIllustratorStarShape::
dup() const
{
  return new CQIllustratorStarShape(*this);
}

void
CQIllustratorStarShape::
getControlPoints(ControlPointList &points, ControlType type) const
{
  if (type == ControlType::GEOMETRY) {
    CPoint2D pc = getCenterPoint();
    CPoint2D p1 = getInnerPoint();
    CPoint2D p2 = getOuterPoint();

    CQIllustratorStarShapeControlPoint *c  =
      new CQIllustratorStarShapeControlPoint(ControlPointType::CENTER, pc);
    CQIllustratorStarShapeControlPoint *ri =
      new CQIllustratorStarShapeControlPoint(ControlPointType::IR    , p1);
    CQIllustratorStarShapeControlPoint *ro =
      new CQIllustratorStarShapeControlPoint(ControlPointType::OR    , p2);

    points.push_back(c);
    points.push_back(ri);
    points.push_back(ro);
  }
  else
    CQIllustratorShape::getControlPoints(points, type);
}

void
CQIllustratorStarShape::
setControlPoint(const CQIllustratorShapeControlPoint *point)
{
  const CQIllustratorStarShapeControlPoint *spoint =
    dynamic_cast<const CQIllustratorStarShapeControlPoint *>(point);
  if (! spoint) assert(false);

  CPoint2D p = point->getPoint(this);

  CQIllustratorStarShape::ControlPointType type = spoint->getType();

  if      (type == CQIllustratorStarShape::ControlPointType::CENTER)
    setCenterPoint(p);
  else if (type == CQIllustratorStarShape::ControlPointType::IR)
    setInnerPoint(p);
  else if (type == CQIllustratorStarShape::ControlPointType::OR)
    setOuterPoint(p);
}

CPoint2D
CQIllustratorStarShape::
getRotateCenter() const
{
  return c_ + rcenter_;
}

void
CQIllustratorStarShape::
setNum(uint n)
{
  n_ = n;
}

void
CQIllustratorStarShape::
setInnerRadius(double r)
{
  r1_ = r;
}

void
CQIllustratorStarShape::
setOuterRadius(double r)
{
  r2_ = r;
}

void
CQIllustratorStarShape::
setInnerAngle(double a)
{
  a1_ = a;
}

void
CQIllustratorStarShape::
setOuterAngle(double a)
{
  a2_ = a;
}

CPoint2D
CQIllustratorStarShape::
getCenterPoint() const
{
  const CMatrix2D &m = getFlatMatrix();

  return m*c_;
}

void
CQIllustratorStarShape::
setCenterPoint(const CPoint2D &c)
{
  const CMatrix2D &m = getFlatMatrix();

  CMatrix2D im = m.inverse();

  c_ = im*c;

  invalidateBBox();
}

CPoint2D
CQIllustratorStarShape::
getInnerPoint() const
{
  const CMatrix2D &m = getFlatMatrix();

  double c1 = cos(CMathGen::DegToRad(a1_));
  double s1 = sin(CMathGen::DegToRad(a1_));

  return m*CPoint2D(c_.x + r1_*c1, c_.y + r1_*s1);
}

void
CQIllustratorStarShape::
setInnerPoint(const CPoint2D &p)
{
  const CMatrix2D &m = getFlatMatrix();

  CMatrix2D im = m.inverse();

  CVector2D v(c_, im*p);

  r1_ = v.length();
  a1_ = CMathGen::RadToDeg(v.angle());

  invalidateBBox();
}

CPoint2D
CQIllustratorStarShape::
getOuterPoint() const
{
  const CMatrix2D &m = getFlatMatrix();

  double c2 = cos(CMathGen::DegToRad(a2_));
  double s2 = sin(CMathGen::DegToRad(a2_));

  return m*CPoint2D(c_.x + r2_*c2, c_.y + r2_*s2);
}

void
CQIllustratorStarShape::
setOuterPoint(const CPoint2D &p)
{
  const CMatrix2D &m = getFlatMatrix();

  CMatrix2D im = m.inverse();

  CVector2D v(c_, im*p);

  r2_ = v.length();
  a2_ = CMathGen::RadToDeg(v.angle());

  invalidateBBox();
}

void
CQIllustratorStarShape::
updateBBox() const
{
  if (! bbox_valid_) {
    bbox_.reset();

    const CMatrix2D &m = getMatrix();

    double da = 2*M_PI/n_;

    for (uint i = 0; i < n_; ++i) {
      double a1 = CMathGen::DegToRad(a1_) + i*da;
      double a2 = CMathGen::DegToRad(a2_) + i*da;

      double c1 = cos(a1), s1 = sin(a1);
      double c2 = cos(a2), s2 = sin(a2);

      bbox_ += m*CPoint2D(c_.x + r1_*c1, c_.y + r1_*s1);
      bbox_ += m*CPoint2D(c_.x + r2_*c2, c_.y + r2_*s2);
    }

    bbox_valid_ = true;
  }
}

bool
CQIllustratorStarShape::
getPolygon(CPolygon2D &polygon) const
{
  const CMatrix2D &m = getFlatMatrix();

  double da = 2*M_PI/n_;

  for (uint i = 0; i < n_; ++i) {
    double a1 = CMathGen::DegToRad(a1_) + i*da;
    double a2 = CMathGen::DegToRad(a2_) + i*da;

    double c1 = cos(a1), s1 = sin(a1);
    double c2 = cos(a2), s2 = sin(a2);

    CPoint2D p1 = m*CPoint2D(c_.x + r1_*c1, c_.y + r1_*s1);
    CPoint2D p2 = m*CPoint2D(c_.x + r2_*c2, c_.y + r2_*s2);

    polygon.addPoint(p1);
    polygon.addPoint(p2);
  }

  return true;
}

bool
CQIllustratorStarShape::
getPath(CPathShapePartList &parts) const
{
  const CMatrix2D &m = getFlatMatrix();

  double da = 2*M_PI/n_;

  for (uint i = 0; i < n_; ++i) {
    double a1 = CMathGen::DegToRad(a1_) + i*da;
    double a2 = CMathGen::DegToRad(a2_) + i*da;

    double c1 = cos(a1), s1 = sin(a1);
    double c2 = cos(a2), s2 = sin(a2);

    CPoint2D p1 = m*CPoint2D(c_.x + r1_*c1, c_.y + r1_*s1);
    CPoint2D p2 = m*CPoint2D(c_.x + r2_*c2, c_.y + r2_*s2);

    if (i == 0)
      parts.addPart(new CPathShapeMoveTo(p1));
    else
      parts.addPart(new CPathShapeLineTo(p1));

    parts.addPart(new CPathShapeLineTo(p2));
  }

  parts.addPart(new CPathShapeClose());

  return true;
}

void
CQIllustratorStarShape::
moveBy(const CPoint2D &d)
{
  assert(! getFixed());

  checkoutShape(CQIllustratorData::ChangeType::GEOMETRY);

  const CMatrix2D &m = getFlatMatrix();

  CMatrix2D im = m.inverse();

  c_ = im*(m*c_ + d);

  invalidateBBox();

  checkinShape(CQIllustratorData::ChangeType::GEOMETRY);
}

bool
CQIllustratorStarShape::
flip(bool)
{
  return false;
}

void
CQIllustratorStarShape::
drawShape(CQIllustratorShapeDrawer *drawer) const
{
  const CMatrix2D &m = getFlatMatrix();

  drawer->pushMatrix(m, true);

  double da = 2*M_PI/n_;

  std::vector<CPoint2D> pi, po;

  pi.resize(n_);
  po.resize(n_);

  for (uint i = 0; i < n_; ++i) {
    double a1 = CMathGen::DegToRad(a1_) + i*da;
    double a2 = CMathGen::DegToRad(a2_) + i*da;

    double c1 = cos(a1), s1 = sin(a1);
    double c2 = cos(a2), s2 = sin(a2);

    pi[i] = CPoint2D(c_.x + r1_*c1, c_.y + r1_*s1);
    po[i] = CPoint2D(c_.x + r2_*c2, c_.y + r2_*s2);
  }

  drawer->pathInit();

  drawer->pathMoveTo(pi[0]);
  drawer->pathLineTo(po[0]);

  for (uint i = 1; i < n_; ++i) {
    drawer->pathLineTo(pi[i]);
    drawer->pathLineTo(po[i]);
  }

  drawer->pathClose();

  if (! clip_) {
    fill_  .draw(this, drawer);
    stroke_.draw(this, drawer);
  }
  else
    drawer->pathClip();

  drawer->popMatrix();
}

CQIllustratorShapeGeometry *
CQIllustratorStarShape::
getGeometry()
{
  return new CQIllustratorStarGeometry(fixed_, rcenter_, m_, c_, n_, r1_, r2_, a1_, a2_);
}

void
CQIllustratorStarShape::
doSetGeometry(const CQIllustratorShapeGeometry *geom)
{
  const CQIllustratorStarGeometry *geom1 = dynamic_cast<const CQIllustratorStarGeometry *>(geom);

  if (! geom1) return;

  fixed_   = geom1->fixed_;
  rcenter_ = geom1->rcenter_;
  m_       = geom1->m_;
  c_       = geom1->c_;
  n_       = geom1->n_;
  r1_      = geom1->r1_;
  r2_      = geom1->r2_;
  a1_      = geom1->a1_;
  a2_      = geom1->a2_;

  invalidateBBox();
}

void
CQIllustratorStarShape::
saveSVG(const CQIllustratorSaveData &saveData)
{
  CFile *file = saveData.file;

  std::vector<CPoint2D> p1, p2;

  getPoints(p1, p2);

  uint n = p1.size();

  file->printf("<path d=\"");

  file->printf("M %g,%g ", p1[0].x, p1[0].y);
  file->printf("L %g,%g ", p2[0].x, p2[0].y);

  for (uint i = 1; i < n; ++i) {
    file->printf("L %g,%g ", p1[i].x, p1[i].y);
    file->printf("L %g,%g ", p2[i].x, p2[i].y);
  }

  file->printf("z\"%s %s/>\n", getSVGMatrix().c_str(), getSVGStroke().c_str());
}

void
CQIllustratorStarShape::
saveCmd(const CQIllustratorSaveData & /*saveData*/)
{
}

void
CQIllustratorStarShape::
getPoints(std::vector<CPoint2D> &pi, std::vector<CPoint2D> &po) const
{
  const CMatrix2D &m = getFlatMatrix();

  double da = 2*M_PI/n_;

  pi.resize(n_);
  po.resize(n_);

  for (uint i = 0; i < n_; ++i) {
    double a1 = CMathGen::DegToRad(a1_) + i*da;
    double a2 = CMathGen::DegToRad(a2_) + i*da;

    double c1 = cos(a1), s1 = sin(a1);
    double c2 = cos(a2), s2 = sin(a2);

    pi[i] = m*CPoint2D(c_.x + r1_*c1, c_.y + r1_*s1);
    po[i] = m*CPoint2D(c_.x + r2_*c2, c_.y + r2_*s2);
  }
}

//-----

CQIllustratorStarShapeControlPoint::
CQIllustratorStarShapeControlPoint(CQIllustratorStarShape::ControlPointType type,
                                   const CPoint2D &p) :
 CQIllustratorShapeControlPoint(uint(type), p), type_(type)
{
}

CQIllustratorStarShapeControlPoint *
CQIllustratorStarShapeControlPoint::
dup() const
{
  return new CQIllustratorStarShapeControlPoint(type_, point_);
}

CPoint2D
CQIllustratorStarShapeControlPoint::
getPoint(const CQIllustratorShape *shape) const
{
  const CQIllustratorStarShape *star = dynamic_cast<const CQIllustratorStarShape *>(shape);
  assert(star);

  if      (type_ == CQIllustratorStarShape::ControlPointType::CENTER)
    return star->getCenterPoint();
  else if (type_ == CQIllustratorStarShape::ControlPointType::IR)
    return star->getInnerPoint();
  else if (type_ == CQIllustratorStarShape::ControlPointType::OR)
    return star->getOuterPoint();
  else
    assert(false);
}

void
CQIllustratorStarShapeControlPoint::
setPoint(CQIllustratorShape *shape, const CPoint2D &point)
{
  CQIllustratorStarShape *star = dynamic_cast<CQIllustratorStarShape *>(shape);
  assert(star);

  if      (type_ == CQIllustratorStarShape::ControlPointType::CENTER)
    star->setCenterPoint(point);
  else if (type_ == CQIllustratorStarShape::ControlPointType::IR)
    star->setInnerPoint(point);
  else if (type_ == CQIllustratorStarShape::ControlPointType::OR)
    star->setOuterPoint(point);
  else
    assert(false);
}
