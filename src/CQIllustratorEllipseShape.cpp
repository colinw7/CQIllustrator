#include <CQIllustratorEllipseShape.h>
#include <CQIllustratorEllipseGeometry.h>
#include <CQIllustratorShapeDrawer.h>

CQIllustratorEllipseShape::
CQIllustratorEllipseShape(const CPoint2D &p1, const CPoint2D &p2,
                          double angle1, double angle2, ConnectType connectType) :
 CQIllustratorShape(), p1_(std::min(p1.x, p2.x), std::min(p1.y, p2.y)),
 p2_(std::max(p1.x, p2.x), std::max(p1.y, p2.y)), angle1_(angle1), angle2_(angle2),
 connectType_(connectType)
{
}

CQIllustratorEllipseShape::
CQIllustratorEllipseShape(const CQIllustratorEllipseShape &ellipse) :
 CQIllustratorShape(ellipse), p1_(ellipse.p1_), p2_(ellipse.p2_), angle1_(ellipse.angle1_),
 angle2_(ellipse.angle2_), connectType_(ellipse.connectType_)
{
}

CQIllustratorEllipseShape *
CQIllustratorEllipseShape::
dup() const
{
  return new CQIllustratorEllipseShape(*this);
}

void
CQIllustratorEllipseShape::
getControlPoints(ControlPointList &points, ControlType type) const
{
  if (type == ControlType::GEOMETRY) {
    double xr = fabs(p2_.x - p1_.x)/2;
    double yr = fabs(p2_.y - p1_.y)/2;

    CPoint2D pxr(xr, 0.0);
    CPoint2D pyr(0.0, yr);

    double a1 = CMathGen::DegToRad(angle1_);
    double a2 = CMathGen::DegToRad(angle2_);

    double c1 = cos(a1), s1 = sin(a1);
    double c2 = cos(a2), s2 = sin(a2);

    CPoint2D pa1(c1, s1);
    CPoint2D pa2(c2, s2);

    CQIllustratorEllipseShapeControlPoint *cxr =
      new CQIllustratorEllipseShapeControlPoint(ControlPointType::RX, pxr);
    CQIllustratorEllipseShapeControlPoint *cyr =
      new CQIllustratorEllipseShapeControlPoint(ControlPointType::RY, pyr);
    CQIllustratorEllipseShapeControlPoint *ca1 =
      new CQIllustratorEllipseShapeControlPoint(ControlPointType::A1, pa1);
    CQIllustratorEllipseShapeControlPoint *ca2 =
      new CQIllustratorEllipseShapeControlPoint(ControlPointType::A2, pa2);

    points.push_back(cxr);
    points.push_back(cyr);
    points.push_back(ca1);
    points.push_back(ca2);
  }
  else
    CQIllustratorShape::getControlPoints(points, type);
}

void
CQIllustratorEllipseShape::
setControlPoint(const CQIllustratorShapeControlPoint *point)
{
  const CQIllustratorEllipseShapeControlPoint *epoint =
    dynamic_cast<const CQIllustratorEllipseShapeControlPoint *>(point);

  assert(epoint);

  const CMatrix2D &m = getFlatMatrix();

  CMatrix2D im = m.inverse();

  CPoint2D p = im*point->getPoint(this);

  CQIllustratorEllipseShape::ControlPointType type = epoint->getType();

  CPoint2D c = (p1_ + p2_)/2;

  if      (type == CQIllustratorEllipseShape::ControlPointType::RX) {
    double rx = std::max(0.0, p.x);

    p1_.x = c.x - rx/2.0;
    p2_.x = c.x + rx/2.0;
  }
  else if (type == CQIllustratorEllipseShape::ControlPointType::RY) {
    double ry = std::max(0.0, p.y);

    p1_.y = c.y - ry/2.0;
    p2_.y = c.y + ry/2.0;
  }
  else if (type == CQIllustratorEllipseShape::ControlPointType::A1) {
    setAngle1(CMathGen::RadToDeg(atan2(p.y, p.x)));
  }
  else if (type == CQIllustratorEllipseShape::ControlPointType::A2) {
    setAngle2(CMathGen::RadToDeg(atan2(p.y, p.x)));
  }

  invalidateBBox();
}

double
CQIllustratorEllipseShape::
getAngle1() const
{
  return angle1_;
}

double
CQIllustratorEllipseShape::
getAngle2() const
{
  return angle2_;
}

void
CQIllustratorEllipseShape::
setAngle1(double a1)
{
  angle1_ = a1;

  while (angle1_ <    0.0) angle1_ += 360.0;
  while (angle1_ >= 360.0) angle1_ -= 360.0;
}

void
CQIllustratorEllipseShape::
setAngle2(double a2)
{
  angle2_ = a2;

  while (angle2_ <=   0.0) angle2_ += 360.0;
  while (angle2_ >  360.0) angle2_ -= 360.0;
}

CPoint2D
CQIllustratorEllipseShape::
getRXPoint() const
{
  CPoint2D c = (p1_ + p2_)/2;

  const CMatrix2D &m = getFlatMatrix();

  return m*CPoint2D(p1_.x, c.y);
}

CPoint2D
CQIllustratorEllipseShape::
getRYPoint() const
{
  CPoint2D c = (p1_ + p2_)/2;

  const CMatrix2D &m = getFlatMatrix();

  return m*CPoint2D(c.x, p1_.y);
}

void
CQIllustratorEllipseShape::
setRXPoint(const CPoint2D &p)
{
  const CMatrix2D &m = getFlatMatrix();

  CMatrix2D im = m.inverse();

  CPoint2D p1 = im*p;

  CPoint2D c = (p1_ + p2_)/2;

  double xr = std::max(c.x - p1.x, 0.0);

  p1_.x = c.x - xr;
  p2_.x = c.x + xr;
}

void
CQIllustratorEllipseShape::
setRYPoint(const CPoint2D &p)
{
  const CMatrix2D &m = getFlatMatrix();

  CMatrix2D im = m.inverse();

  CPoint2D p1 = im*p;

  CPoint2D c = (p1_ + p2_)/2;

  double yr = std::max(c.y - p1.y, 0.0);

  p1_.y = c.y - yr;
  p2_.y = c.y + yr;
}

CPoint2D
CQIllustratorEllipseShape::
getAngle1Point() const
{
  CPoint2D c = (p1_ + p2_)/2;

  double xr = fabs(p2_.x - p1_.x)/2;
  double yr = fabs(p2_.y - p1_.y)/2;

  double a1 = CMathGen::DegToRad(angle1_);

  double c1 = cos(a1), s1 = sin(a1);

  const CMatrix2D &m = getFlatMatrix();

  return m*CPoint2D(c.x + c1*xr, c.y + s1*yr);
}

CPoint2D
CQIllustratorEllipseShape::
getAngle2Point() const
{
  CPoint2D c = (p1_ + p2_)/2;

  double xr = fabs(p2_.x - p1_.x)/2;
  double yr = fabs(p2_.y - p1_.y)/2;

  double a2 = CMathGen::DegToRad(angle2_);

  double c2 = cos(a2), s2 = sin(a2);

  const CMatrix2D &m = getFlatMatrix();

  return m*CPoint2D(c.x + c2*xr, c.y + s2*yr);
}

void
CQIllustratorEllipseShape::
setAngle1Point(const CPoint2D &p)
{
  CPoint2D c = (p1_ + p2_)/2;

  //double xr = fabs(p2_.x - p1_.x)/2;
  //double yr = fabs(p2_.y - p1_.y)/2;

  double dx = p.x - c.x;
  double dy = p.y - c.y;

  setAngle1(CMathGen::RadToDeg(atan2(dy, dx)));
}

void
CQIllustratorEllipseShape::
setAngle2Point(const CPoint2D &p)
{
  CPoint2D c = (p1_ + p2_)/2;

  //double xr = fabs(p2_.x - p1_.x)/2;
  //double yr = fabs(p2_.y - p1_.y)/2;

  double dx = p.x - c.x;
  double dy = p.y - c.y;

  setAngle2(CMathGen::RadToDeg(atan2(dy, dx)));
}

void
CQIllustratorEllipseShape::
updateBBox() const
{
  if (! bbox_valid_) {
    bbox_.reset();

    const CMatrix2D &m = getMatrix();

    CPoint2D p1 = m*CPoint2D(p1_.x, p1_.y);
    CPoint2D p2 = m*CPoint2D(p2_.x, p1_.y);
    CPoint2D p3 = m*CPoint2D(p2_.x, p2_.y);
    CPoint2D p4 = m*CPoint2D(p1_.x, p2_.y);

    bbox_ = CBBox2D(p1, p2);

    bbox_ += p3;
    bbox_ += p4;

    bbox_valid_ = true;
  }
}

bool
CQIllustratorEllipseShape::
getPolygon(CPolygon2D &) const
{
  return false;
}

bool
CQIllustratorEllipseShape::
getPath(CPathShapePartList &parts) const
{
  double xr = fabs(p2_.x - p1_.x)/2;
  double yr = fabs(p2_.y - p1_.y)/2;

  CPoint2D c = (p1_ + p2_)/2;

  double a1 = CMathGen::DegToRad(angle1_);
  double a2 = CMathGen::DegToRad(angle2_);
  double a3 = (a1 + a2)/2.0;

  double c1 = cos(a1), s1 = sin(a1);
  double c2 = cos(a2), s2 = sin(a2);
//double c3 = cos(a3), s3 = sin(a3);

  CPoint2D ps(c.x + xr*c1, c.y + yr*s1);
  CPoint2D pe(c.x + xr*c2, c.y + yr*s2);

//const CMatrix2D &m = getFlatMatrix();

  if (connectType_ == ConnectType::LINE) {
    parts.addPart(new CPathShapeMoveTo((ps + pe)/2));

    parts.addPart(new CPathShapeArc(c, xr, yr, a1, a3));
    parts.addPart(new CPathShapeArc(c, xr, yr, a3, a2));
  }
  else {
    parts.addPart(new CPathShapeMoveTo(c));

    parts.addPart(new CPathShapeArc(c, xr, yr, a1, a3));
    parts.addPart(new CPathShapeArc(c, xr, yr, a3, a2));
  }

  parts.addPart(new CPathShapeClose());

  return true;
}

void
CQIllustratorEllipseShape::
moveBy(const CPoint2D &d)
{
  assert(! getFixed());

  checkoutShape(CQIllustratorData::ChangeType::GEOMETRY);

  const CMatrix2D &m = getFlatMatrix();

  CMatrix2D im = m.inverse();

  p1_ = im*(m*p1_ + d);
  p2_ = im*(m*p2_ + d);

  invalidateBBox();

  checkinShape(CQIllustratorData::ChangeType::GEOMETRY);
}

void
CQIllustratorEllipseShape::
resizeBy(double dw, double dh)
{
  double x = bbox_.getXMin();
  double y = bbox_.getYMin();

  double w = bbox_.getWidth ();
  double h = bbox_.getHeight();

  double xs = (w + dw)/w;
  double ys = (h + dh)/h;

  const CMatrix2D &m = getFlatMatrix();

  CMatrix2D im = m.inverse();

  CPoint2D p1 = m*p1_;
  CPoint2D p2 = m*p2_;

  p1.x = x + (p1.x - x)*xs;
  p1.y = y + (p1.y - y)*ys;
  p2.x = x + (p2.x - x)*xs;
  p2.y = y + (p2.y - y)*ys;

  p1_ = im*p1;
  p2_ = im*p2;

  invalidateBBox();
}

bool
CQIllustratorEllipseShape::
flip(bool x_axis)
{
  // angles ?
  const CMatrix2D &m = getFlatMatrix();

  CMatrix2D im = m.inverse();

  CPoint2D rc = getRotateCenter();

  CPoint2D p1 = m*p1_;
  CPoint2D p2 = m*p2_;

  p1_ = im*p1.flip(rc, x_axis);
  p2_ = im*p2.flip(rc, x_axis);

  return true;
}

void
CQIllustratorEllipseShape::
drawShape(CQIllustratorShapeDrawer *drawer) const
{
  double xr = fabs(p2_.x - p1_.x)/2;
  double yr = fabs(p2_.y - p1_.y)/2;

  CPoint2D c = (p1_ + p2_)/2;

  double a1 = CMathGen::DegToRad(angle1_);
  double a2 = CMathGen::DegToRad(angle2_);
  double a3 = (a1 + a2)/2.0;

  double c1 = cos(a1), s1 = sin(a1);
  double c2 = cos(a2), s2 = sin(a2);
//double c3 = cos(a3), s3 = sin(a3);

  CPoint2D ps(c.x + xr*c1, c.y + yr*s1);
  CPoint2D pe(c.x + xr*c2, c.y + yr*s2);

  const CMatrix2D &m = getFlatMatrix();

  drawer->pushMatrix(m, true);

  drawer->pathInit();

  if (connectType_ == ConnectType::LINE) {
    drawer->pathMoveTo((ps + pe)/2);

    drawer->pathArc(c, xr, yr, a1, a3);
    drawer->pathArc(c, xr, yr, a3, a2);
  }
  else {
    drawer->pathMoveTo(c);

    drawer->pathArc(c, xr, yr, a1, a3);
    drawer->pathArc(c, xr, yr, a3, a2);
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
CQIllustratorEllipseShape::
getGeometry()
{
  return new CQIllustratorEllipseGeometry(fixed_, rcenter_, m_, p1_, p2_,
                                          angle1_, angle2_, connectType_);
}

void
CQIllustratorEllipseShape::
doSetGeometry(const CQIllustratorShapeGeometry *geom)
{
  const CQIllustratorEllipseGeometry *geom1 =
    dynamic_cast<const CQIllustratorEllipseGeometry *>(geom);

  if (! geom1) return;

  fixed_       = geom1->fixed_;
  rcenter_     = geom1->rcenter_;
  m_           = geom1->m_;
  p1_          = geom1->p1_;
  p2_          = geom1->p2_;
  angle1_      = geom1->angle1_;
  angle2_      = geom1->angle2_;
  connectType_ = geom1->connectType_;

  invalidateBBox();
}

void
CQIllustratorEllipseShape::
saveSVG(const CQIllustratorSaveData &saveData)
{
  CFile *file = saveData.file;

  double a = getAngle();

  CMatrix2D m1 = CMatrix2D::rotation(-a);

  const CMatrix2D &m = getFlatMatrix();

  CPoint2D p1 = m1*m*CPoint2D(p1_.x, p1_.y);
  CPoint2D p2 = m1*m*CPoint2D(p2_.x, p2_.y);

  double top = saveData.bbox.getYMax();

  CPoint2D c = (p1 + p2)/2; c.y = top - c.y;

  double xr = fabs(p2.x - p1.x)/2;
  double yr = fabs(p2.y - p1.y)/2;

  CMatrix2D m2 = CMatrix2D::translation(-c.x, -c.y)*
                 CMatrix2D::rotation(a)*
                 CMatrix2D::translation( c.x,  c.y);

  file->printf("<ellipse cx=\"%g\" cy=\"%g\" rx=\"%g\" ry=\"%g\"%s %s/>\n",
               c.x, c.y, xr, yr, getSVGMatrix(m2).c_str(), getSVGStroke().c_str());
}

void
CQIllustratorEllipseShape::
saveCmd(const CQIllustratorSaveData &saveData)
{
  CFile *file = saveData.file;

  double a = getAngle();

  CMatrix2D m1 = CMatrix2D::rotation(-a);

  const CMatrix2D &m = getFlatMatrix();

  CPoint2D c = (m*p1_ + m*p2_)/2;

  CPoint2D p1 = m1*m*p1_;
  CPoint2D p2 = m1*m*p2_;

  double xr = fabs(p2.x - p1.x)/2;
  double yr = fabs(p2.y - p1.y)/2;

  if (name_ != "")
    file->printf("create_ellipse -name %s -x %g -y %g -xr %g -yr %g -a %g\n",
                 name_.c_str(), c.x, c.y, xr, yr, 180.0*a/M_PI);
  else
    file->printf("create_ellipse -x %g -y %g -xr %g -yr %g -a %g\n",
                 c.x, c.y, xr, yr, 180.0*a/M_PI);
}

double
CQIllustratorEllipseShape::
getAngle() const
{
  const CMatrix2D &m = getFlatMatrix();

  CPoint2D p1 = m*CPoint2D(p1_.x, p1_.y);
  CPoint2D p2 = m*CPoint2D(p2_.x, p1_.y);

  return atan2(p2.y - p1.y, p2.x - p1.x);
}

//-----

CQIllustratorEllipseShapeControlPoint::
CQIllustratorEllipseShapeControlPoint(CQIllustratorEllipseShape::ControlPointType type,
                                      const CPoint2D &p) :
 CQIllustratorShapeControlPoint((uint) type, p), type_(type)
{
}

CQIllustratorEllipseShapeControlPoint *
CQIllustratorEllipseShapeControlPoint::
dup() const
{
  return new CQIllustratorEllipseShapeControlPoint(type_, point_);
}

CPoint2D
CQIllustratorEllipseShapeControlPoint::
getPoint(const CQIllustratorShape *shape) const
{
  const CQIllustratorEllipseShape *ellipse =
    dynamic_cast<const CQIllustratorEllipseShape *>(shape);
  assert(ellipse);

  if      (type_ == CQIllustratorEllipseShape::ControlPointType::RX)
    return ellipse->getRXPoint();
  else if (type_ == CQIllustratorEllipseShape::ControlPointType::RY)
    return ellipse->getRYPoint();
  else if (type_ == CQIllustratorEllipseShape::ControlPointType::A1)
    return ellipse->getAngle1Point();
  else if (type_ == CQIllustratorEllipseShape::ControlPointType::A2)
    return ellipse->getAngle2Point();
  else
    assert(false);
}

void
CQIllustratorEllipseShapeControlPoint::
setPoint(CQIllustratorShape *shape, const CPoint2D &point)
{
  CQIllustratorEllipseShape *ellipse = dynamic_cast<CQIllustratorEllipseShape *>(shape);
  assert(ellipse);

  if      (type_ == CQIllustratorEllipseShape::ControlPointType::RX)
    ellipse->setRXPoint(point);
  else if (type_ == CQIllustratorEllipseShape::ControlPointType::RY)
    ellipse->setRYPoint(point);
  else if (type_ == CQIllustratorEllipseShape::ControlPointType::A1)
    ellipse->setAngle1Point(point);
  else if (type_ == CQIllustratorEllipseShape::ControlPointType::A2)
    ellipse->setAngle2Point(point);
  else
    assert(false);
}
