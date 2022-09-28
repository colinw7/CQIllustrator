#include <CQIllustratorRectShape.h>
#include <CQIllustratorRectGeometry.h>
#include <CQIllustratorShapeDrawer.h>

CQIllustratorRectShape::
CQIllustratorRectShape(const CPoint2D &p1, const CPoint2D &p2, double rx, double ry) :
 CQIllustratorShape(), p1_(std::min(p1.x, p2.x), std::min(p1.y, p2.y)),
 p2_(std::max(p1.x, p2.x), std::max(p1.y, p2.y)), rx_(rx), ry_(ry)
{
}

CQIllustratorRectShape::
CQIllustratorRectShape(const CQIllustratorRectShape &rect) :
 CQIllustratorShape(rect), p1_(rect.p1_), p2_(rect.p2_), rx_(rect.rx_), ry_(rect.ry_)
{
}

CQIllustratorRectShape *
CQIllustratorRectShape::
dup() const
{
  return new CQIllustratorRectShape(*this);
}

void
CQIllustratorRectShape::
getControlPoints(ControlPointList &points, ControlType type) const
{
  if (type == ControlType::GEOMETRY) {
    const CMatrix2D &m = getFlatMatrix();

    CPoint2D p1 = m*p1_;
    CPoint2D p2 = m*p2_;

    CPoint2D rx1(rx_, 0);
    CPoint2D ry1(0, ry_);

    CQIllustratorRectShapeControlPoint *ll =
      new CQIllustratorRectShapeControlPoint(ControlPointType::LL, p1);
    CQIllustratorRectShapeControlPoint *ur =
      new CQIllustratorRectShapeControlPoint(ControlPointType::UR, p2);
    CQIllustratorRectShapeControlPoint *rx =
      new CQIllustratorRectShapeControlPoint(ControlPointType::RX, rx1);
    CQIllustratorRectShapeControlPoint *ry =
      new CQIllustratorRectShapeControlPoint(ControlPointType::RY, ry1);

    points.push_back(ll);
    points.push_back(ur);
    points.push_back(rx);
    points.push_back(ry);
  }
  else
    CQIllustratorShape::getControlPoints(points, type);
}

void
CQIllustratorRectShape::
setControlPoint(const CQIllustratorShapeControlPoint *point)
{
  const CQIllustratorRectShapeControlPoint *rpoint =
    dynamic_cast<const CQIllustratorRectShapeControlPoint *>(point);

  assert(rpoint);

  const CMatrix2D &m = getFlatMatrix();

  CMatrix2D im = m.inverse();

  CPoint2D p = im*point->getPoint(this);

  CQIllustratorRectShape::ControlPointType type = rpoint->getType();

  if      (type == CQIllustratorRectShape::ControlPointType::LL)
    p1_ = p;
  else if (type == CQIllustratorRectShape::ControlPointType::UR)
    p2_ = p;
  else if (type == CQIllustratorRectShape::ControlPointType::RX)
    rx_ = std::min(std::max(p.x - p1_.x, 0.0), fabs(p2_.x - p1_.x)/2);
  else if (type == CQIllustratorRectShape::ControlPointType::RY)
    ry_ = std::min(std::max(p2_.y - p.y, 0.0), fabs(p2_.y - p1_.y)/2);

  invalidateBBox();
}

void
CQIllustratorRectShape::
setRadiusX(double rx)
{
  rx_ = rx;
}

void
CQIllustratorRectShape::
setRadiusY(double ry)
{
  ry_ = ry;
}

CPoint2D
CQIllustratorRectShape::
getLL() const
{
  const CMatrix2D &m = getFlatMatrix();

  return m*p1_;
}

CPoint2D
CQIllustratorRectShape::
getUR() const
{
  const CMatrix2D &m = getFlatMatrix();

  return m*p2_;
}

CPoint2D
CQIllustratorRectShape::
getRXPoint() const
{
  const CMatrix2D &m = getFlatMatrix();

  return m*CPoint2D(p1_.x + rx_, p2_.y);
}

CPoint2D
CQIllustratorRectShape::
getRYPoint() const
{
  const CMatrix2D &m = getFlatMatrix();

  return m*CPoint2D(p1_.x, p2_.y - ry_);
}

void
CQIllustratorRectShape::
setLL(const CPoint2D &p)
{
  const CMatrix2D &m = getFlatMatrix();

  CMatrix2D im = m.inverse();

  CPoint2D p1 = im*p;

  p1_ = p1;

  invalidateBBox();
}

void
CQIllustratorRectShape::
setUR(const CPoint2D &p)
{
  const CMatrix2D &m = getFlatMatrix();

  CMatrix2D im = m.inverse();

  CPoint2D p1 = im*p;

  p2_ = p1;

  invalidateBBox();
}

void
CQIllustratorRectShape::
setRXPoint(const CPoint2D &p)
{
  const CMatrix2D &m = getFlatMatrix();

  CMatrix2D im = m.inverse();

  CPoint2D p1 = im*p;

  rx_ = std::min(std::max(p1.x - p1_.x, 0.0), fabs(p2_.x - p1_.x)/2);
}

void
CQIllustratorRectShape::
setRYPoint(const CPoint2D &p)
{
  const CMatrix2D &m = getFlatMatrix();

  CMatrix2D im = m.inverse();

  CPoint2D p1 = im*p;

  ry_ = std::min(std::max(p2_.y - p1.y, 0.0), fabs(p2_.y - p1_.y)/2);
}

void
CQIllustratorRectShape::
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

//  double sopacity = getStroke().getOpacity();
//
//  if (sopacity > 0.0) {
//    double lw2 = getStrokeWidth()/2;

//    bbox_.expand(-lw2, -lw2, lw2, lw2);
//  }

    bbox_valid_ = true;
  }
}

bool
CQIllustratorRectShape::
getPolygon(CPolygon2D &polygon) const
{
  const CMatrix2D &m = getFlatMatrix();

  CPoint2D p1 = m*CPoint2D(p1_.x, p1_.y);
  CPoint2D p2 = m*CPoint2D(p2_.x, p1_.y);
  CPoint2D p3 = m*CPoint2D(p2_.x, p2_.y);
  CPoint2D p4 = m*CPoint2D(p1_.x, p2_.y);

  polygon.addPoint(p1);
  polygon.addPoint(p2);
  polygon.addPoint(p3);
  polygon.addPoint(p4);

  return true;
}

bool
CQIllustratorRectShape::
getPath(CPathShapePartList &parts) const
{
  const CMatrix2D &m = getFlatMatrix();

  CPoint2D p1 = m*CPoint2D(p1_.x, p1_.y);
  CPoint2D p2 = m*CPoint2D(p2_.x, p1_.y);
  CPoint2D p3 = m*CPoint2D(p2_.x, p2_.y);
  CPoint2D p4 = m*CPoint2D(p1_.x, p2_.y);

  parts.addPart(new CPathShapeMoveTo(p1));
  parts.addPart(new CPathShapeLineTo(p2));
  parts.addPart(new CPathShapeLineTo(p3));
  parts.addPart(new CPathShapeLineTo(p4));
  parts.addPart(new CPathShapeClose());

  return true;
}

void
CQIllustratorRectShape::
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
CQIllustratorRectShape::
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
CQIllustratorRectShape::
flip(bool x_axis)
{
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
CQIllustratorRectShape::
drawShape(CQIllustratorShapeDrawer *drawer) const
{
  const CMatrix2D &m = getFlatMatrix();

  drawer->pushMatrix(m, true);

  CPoint2D p1 = CPoint2D(p1_.x, p1_.y);
  CPoint2D p2 = CPoint2D(p2_.x, p1_.y);
  CPoint2D p3 = CPoint2D(p2_.x, p2_.y);
  CPoint2D p4 = CPoint2D(p1_.x, p2_.y);

  drawer->pathInit();

  if (rx_ > 0 && ry_ > 0) {
    drawer->pathMoveTo(CPoint2D(p1.x + rx_, p1.y      ));
//  drawer->pathMoveTo(CPoint2D(p2.x - rx_, p2.y      ));
    drawer->pathArc   (CPoint2D(p2.x - rx_, p2.y + ry_), rx_, ry_, 3*M_PI/2, 2*M_PI  );
//  drawer->pathLineTo(CPoint2D(p3.x      , p3.y - ry_));
    drawer->pathArc   (CPoint2D(p3.x - rx_, p3.y - ry_), rx_, ry_, 0       , M_PI/2  );
//  drawer->pathLineTo(CPoint2D(p4.x + rx_, p4.y      ));
    drawer->pathArc   (CPoint2D(p4.x + rx_, p4.y - ry_), rx_, ry_, M_PI/2  , M_PI    );
//  drawer->pathLineTo(CPoint2D(p1.x      , p1.y + ry_));
    drawer->pathArc   (CPoint2D(p1.x + rx_, p1.y + ry_), rx_, ry_, M_PI    , 3*M_PI/2);
  }
  else {
    drawer->pathMoveTo(p1);
    drawer->pathLineTo(p2);
    drawer->pathLineTo(p3);
    drawer->pathLineTo(p4);
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
CQIllustratorRectShape::
getGeometry()
{
  return new CQIllustratorRectGeometry(fixed_, rcenter_, m_, p1_, p2_, rx_, ry_);
}

void
CQIllustratorRectShape::
doSetGeometry(const CQIllustratorShapeGeometry *geom)
{
  const CQIllustratorRectGeometry *geom1 =
    dynamic_cast<const CQIllustratorRectGeometry *>(geom);

  if (! geom1) return;

  fixed_   = geom1->fixed_;
  rcenter_ = geom1->rcenter_;
  m_       = geom1->m_;
  p1_      = geom1->p1_;
  p2_      = geom1->p2_;
  rx_      = geom1->rx_;
  ry_      = geom1->ry_;

  invalidateBBox();
}

void
CQIllustratorRectShape::
saveSVG(const CQIllustratorSaveData &saveData)
{
  CFile *file = saveData.file;

  double a = getAngle();

  CMatrix2D m1 = CMatrix2D::rotation(-a);

  const CMatrix2D &m = getFlatMatrix();

  CPoint2D p1 = m1*m*p1_;
  CPoint2D p2 = m1*m*p2_;

  double top = saveData.bbox.getYMax();

  double x =       std::min(p1.x, p2.x);
  double y = top - std::min(p1.y, p2.y);
  double w = fabs(p2.x - p1.x);
  double h = fabs(p2.y - p1.y);

  CPoint2D c = (p1 + p2)/2; c.y = top - c.y;

  CMatrix2D m2 = CMatrix2D::translation(-c.x, -c.y)*
                 CMatrix2D::rotation(a)*
                 CMatrix2D::translation( c.x,  c.y);

  file->printf("<rect x=\"%g\" y=\"%g\" width=\"%g\" height=\"%g\" rx=\"%g\" ry=\"%g\"%s %s/>\n",
               x, y, w, h, rx_, ry_, getSVGMatrix(m2).c_str(), getSVGStroke().c_str());
}

void
CQIllustratorRectShape::
saveCmd(const CQIllustratorSaveData &saveData)
{
  CFile *file = saveData.file;

  double a = getAngle();

  const CMatrix2D &m = getFlatMatrix();

  CPoint2D c = (m*p1_ + m*p2_)/2;

  CMatrix2D m1 = CMatrix2D::translation( c.x,  c.y)*
                 CMatrix2D::rotation(-a)*
                 CMatrix2D::translation(-c.x, -c.y);

  CPoint2D p1 = m1*m*p1_;
  CPoint2D p2 = m1*m*p2_;

  double x = std::min(p1.x, p2.x);
  double y = std::min(p1.y, p2.y);
  double w = fabs(p2.x - p1.x);
  double h = fabs(p2.y - p1.y);

  if (name_ != "")
    file->printf("create_rect -name %s -x %g -y %g -w %g -h %g -rx %g -ry %g -a %g\n",
                 name_.c_str(), x, y, w, h, rx_, ry_, 180.0*a/M_PI);
  else
    file->printf("create_rect -x %g -y %g -w %g -h %g -rx %g -ry %g -a %g\n",
                 x, y, w, h, rx_, ry_, 180.0*a/M_PI);
}

double
CQIllustratorRectShape::
getAngle() const
{
  const CMatrix2D &m = getFlatMatrix();

  CPoint2D p1 = m*CPoint2D(p1_.x, p1_.y);
  CPoint2D p2 = m*CPoint2D(p2_.x, p1_.y);

  return atan2(p2.y - p1.y, p2.x - p1.x);
}

//-----

CQIllustratorRectShapeControlPoint::
CQIllustratorRectShapeControlPoint(CQIllustratorRectShape::ControlPointType type,
                                   const CPoint2D &p) :
 CQIllustratorShapeControlPoint(uint(type), p), type_(type)
{
}

CQIllustratorRectShapeControlPoint *
CQIllustratorRectShapeControlPoint::
dup() const
{
  return new CQIllustratorRectShapeControlPoint(type_, point_);
}

CPoint2D
CQIllustratorRectShapeControlPoint::
getPoint(const CQIllustratorShape *shape) const
{
  const CQIllustratorRectShape *rect = dynamic_cast<const CQIllustratorRectShape *>(shape);
  assert(rect);

  if      (type_ == CQIllustratorRectShape::ControlPointType::LL)
    return rect->getLL();
  else if (type_ == CQIllustratorRectShape::ControlPointType::UR)
    return rect->getUR();
  else if (type_ == CQIllustratorRectShape::ControlPointType::RX)
    return rect->getRXPoint();
  else if (type_ == CQIllustratorRectShape::ControlPointType::RY)
    return rect->getRYPoint();
  else
    assert(false);
}

void
CQIllustratorRectShapeControlPoint::
setPoint(CQIllustratorShape *shape, const CPoint2D &point)
{
  CQIllustratorRectShape *rect = dynamic_cast<CQIllustratorRectShape *>(shape);
  assert(rect);

  if      (type_ == CQIllustratorRectShape::ControlPointType::LL)
    rect->setLL(point);
  else if (type_ == CQIllustratorRectShape::ControlPointType::UR)
    rect->setUR(point);
  else if (type_ == CQIllustratorRectShape::ControlPointType::RX)
    rect->setRXPoint(point);
  else if (type_ == CQIllustratorRectShape::ControlPointType::RY)
    rect->setRYPoint(point);
  else
    assert(false);
}
