#include <CQIllustratorNPolyShape.h>
#include <CQIllustratorNPolyGeometry.h>
#include <CQIllustratorShapeDrawer.h>

CQIllustratorNPolyShape::
CQIllustratorNPolyShape(const CPoint2D &c, int n, double r, double a) :
 CQIllustratorShape(), c_(c), n_(n), r_(r), a_(a)
{
}

CQIllustratorNPolyShape::
CQIllustratorNPolyShape(const CQIllustratorNPolyShape &poly) :
 CQIllustratorShape(poly), c_(poly.c_), n_(poly.n_), r_(poly.r_), a_(poly.a_)
{
}

CQIllustratorNPolyShape *
CQIllustratorNPolyShape::
dup() const
{
  return new CQIllustratorNPolyShape(*this);
}

void
CQIllustratorNPolyShape::
getControlPoints(ControlPointList &points, ControlType type) const
{
  if (type == ControlType::GEOMETRY) {
    CPoint2D pc = getCenterPoint();
    CPoint2D p  = getPoint();

    CQIllustratorNPolyShapeControlPoint *c =
      new CQIllustratorNPolyShapeControlPoint(TYPE_CENTER, pc);
    CQIllustratorNPolyShapeControlPoint *r =
      new CQIllustratorNPolyShapeControlPoint(TYPE_RADIUS, p);

    points.push_back(c);
    points.push_back(r);
  }
  else
    CQIllustratorShape::getControlPoints(points, type);
}

void
CQIllustratorNPolyShape::
setControlPoint(const CQIllustratorShapeControlPoint *point)
{
  const CQIllustratorNPolyShapeControlPoint *spoint =
    dynamic_cast<const CQIllustratorNPolyShapeControlPoint *>(point);
  if (! spoint) assert(false);

  CPoint2D p = point->getPoint(this);

  CQIllustratorNPolyShape::ControlPointType type = spoint->getType();

  if      (type == CQIllustratorNPolyShape::TYPE_CENTER)
    setCenterPoint(p);
  else if (type == CQIllustratorNPolyShape::TYPE_RADIUS)
    setPoint(p);
}

CPoint2D
CQIllustratorNPolyShape::
getRotateCenter() const
{
  return c_ + rcenter_;
}

void
CQIllustratorNPolyShape::
setNum(int n)
{
  n_ = n;
}

void
CQIllustratorNPolyShape::
setRadius(double r)
{
  r_ = r;
}

void
CQIllustratorNPolyShape::
setAngle(double a)
{
  a_ = a;
}

CPoint2D
CQIllustratorNPolyShape::
getCenterPoint() const
{
  const CMatrix2D &m = getFlatMatrix();

  return m*c_;
}

void
CQIllustratorNPolyShape::
setCenterPoint(const CPoint2D &c)
{
  const CMatrix2D &m = getFlatMatrix();

  CMatrix2D im = m.inverse();

  c_ = im*c;

  invalidateBBox();
}

CPoint2D
CQIllustratorNPolyShape::
getPoint() const
{
  const CMatrix2D &m = getFlatMatrix();

  double c = cos(a_), s = sin(a_);

  return m*CPoint2D(c_.x + r_*c, c_.y + r_*s);
}

void
CQIllustratorNPolyShape::
setPoint(const CPoint2D &p)
{
  const CMatrix2D &m = getFlatMatrix();

  CMatrix2D im = m.inverse();

  CVector2D v(c_, im*p);

  r_ = v.length();
  a_ = v.angle();

  invalidateBBox();
}

void
CQIllustratorNPolyShape::
updateBBox() const
{
  if (! bbox_valid_) {
    bbox_.reset();

    const CMatrix2D &m = getMatrix();

    double da = 2*M_PI/n_;

    for (int i = 0; i < n_; ++i) {
      double a = a_ + i*da;

      double c = cos(a), s = sin(a);

      bbox_ += m*CPoint2D(c_.x + r_*c, c_.y + r_*s);
    }

    bbox_valid_ = true;
  }
}

bool
CQIllustratorNPolyShape::
getPolygon(CPolygon2D &polygon) const
{
  const CMatrix2D &m = getFlatMatrix();

  double da = 2*M_PI/n_;

  for (int i = 0; i < n_; ++i) {
    double a = a_ + i*da;

    double c = cos(a), s = sin(a);

    CPoint2D p = m*CPoint2D(c_.x + r_*c, c_.y + r_*s);

    polygon.addPoint(p);
  }

  return true;
}

bool
CQIllustratorNPolyShape::
getPath(CPathShapePartList &parts) const
{
  const CMatrix2D &m = getFlatMatrix();

  double da = 2*M_PI/n_;

  for (int i = 0; i < n_; ++i) {
    double a = a_ + i*da;

    double c = cos(a), s = sin(a);

    CPoint2D p = m*CPoint2D(c_.x + r_*c, c_.y + r_*s);

    if (i == 0)
      parts.addPart(new CPathShapeMoveTo(p));
    else
      parts.addPart(new CPathShapeLineTo(p));
  }

  parts.addPart(new CPathShapeClose());

  return true;
}

void
CQIllustratorNPolyShape::
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
CQIllustratorNPolyShape::
flip(bool)
{
  return false;
}

void
CQIllustratorNPolyShape::
drawShape(CQIllustratorShapeDrawer *drawer) const
{
  const CMatrix2D &m = getFlatMatrix();

  drawer->pushMatrix(m, true);

  double da = 2*M_PI/n_;

  std::vector<CPoint2D> p;

  p.resize(n_);

  for (int i = 0; i < n_; ++i) {
    double a = a_ + i*da;

    double c = cos(a), s = sin(a);

    p[i] = CPoint2D(c_.x + r_*c, c_.y + r_*s);
  }

  drawer->pathInit();

  drawer->pathMoveTo(p[0]);

  for (int i = 1; i < n_; ++i)
    drawer->pathLineTo(p[i]);

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
CQIllustratorNPolyShape::
getGeometry()
{
  return new CQIllustratorNPolyGeometry(fixed_, rcenter_, m_, c_, n_, r_, a_);
}

void
CQIllustratorNPolyShape::
doSetGeometry(const CQIllustratorShapeGeometry *geom)
{
  const CQIllustratorNPolyGeometry *geom1 =
    dynamic_cast<const CQIllustratorNPolyGeometry *>(geom);

  if (! geom1) return;

  fixed_   = geom1->fixed_;
  rcenter_ = geom1->rcenter_;
  m_       = geom1->m_;
  c_       = geom1->c_;
  n_       = geom1->n_;
  r_       = geom1->r_;
  a_       = geom1->a_;

  invalidateBBox();
}

void
CQIllustratorNPolyShape::
saveSVG(const CQIllustratorSaveData &saveData)
{
  CFile *file = saveData.file;

  std::vector<CPoint2D> p;

  getPoints(p);

  uint n = p.size();

  file->printf("<path d=\"");

  file->printf("M %g,%g ", p[0].x, p[0].y);

  for (uint i = 1; i < n; ++i)
    file->printf("L %g,%g ", p[i].x, p[i].y);

  file->printf("z\"%s %s/>\n", getSVGMatrix().c_str(), getSVGStroke().c_str());
}

void
CQIllustratorNPolyShape::
saveCmd(const CQIllustratorSaveData & /*saveData*/)
{
}

void
CQIllustratorNPolyShape::
getPoints(std::vector<CPoint2D> &p) const
{
  const CMatrix2D &m = getFlatMatrix();

  double da = 2*M_PI/n_;

  p.resize(n_);

  for (int i = 0; i < n_; ++i) {
    double a = a_ + i*da;

    double c = cos(a), s = sin(a);

    p[i] = m*CPoint2D(c_.x + r_*c, c_.y + r_*s);
  }
}

//-----

CQIllustratorNPolyShapeControlPoint::
CQIllustratorNPolyShapeControlPoint(CQIllustratorNPolyShape::ControlPointType type,
                                    const CPoint2D &p) :
 CQIllustratorShapeControlPoint(uint(type), p), type_(type)
{
}

CQIllustratorNPolyShapeControlPoint *
CQIllustratorNPolyShapeControlPoint::
dup() const
{
  return new CQIllustratorNPolyShapeControlPoint(type_, point_);
}

CPoint2D
CQIllustratorNPolyShapeControlPoint::
getPoint(const CQIllustratorShape *shape) const
{
  const CQIllustratorNPolyShape *star = dynamic_cast<const CQIllustratorNPolyShape *>(shape);
  assert(star);

  if      (type_ == CQIllustratorNPolyShape::TYPE_CENTER)
    return star->getCenterPoint();
  else if (type_ == CQIllustratorNPolyShape::TYPE_RADIUS)
    return star->getPoint();
  else
    assert(false);
}

void
CQIllustratorNPolyShapeControlPoint::
setPoint(CQIllustratorShape *shape, const CPoint2D &point)
{
  CQIllustratorNPolyShape *star = dynamic_cast<CQIllustratorNPolyShape *>(shape);
  assert(star);

  if      (type_ == CQIllustratorNPolyShape::TYPE_CENTER)
    star->setCenterPoint(point);
  else if (type_ == CQIllustratorNPolyShape::TYPE_RADIUS)
    star->setPoint(point);
  else
    assert(false);
}
