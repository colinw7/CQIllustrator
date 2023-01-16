#include <CQIllustratorShape.h>
#include <CQIllustratorPathShape.h>
#include <CQIllustratorData.h>
#include <CQIllustratorShapeDrawer.h>
#include <CQIllustratorShapeFilter.h>
#include <CQIllustratorPathShapeControlLine.h>
#include <CQIllustratorPathShapeControlPoint.h>
#include <CQIllustratorShapeLGradientControlPoint.h>
#include <CQIllustratorShapeRGradientControlPoint.h>

#include <CQUtil.h>
#include <CQUtilGeom.h>
#include <CQImageGaussianBlur.h>
#include <CLinearGradient.h>
#include <CRadialGradient.h>
#include <CMathGeom2D.h>
#include <QPainter>

int CQIllustratorShape:: last_id_ = 1;

CQIllustratorShape::
CQIllustratorShape(CQIllustratorShape *parent) :
 QObject(), id_(last_id_++), parent_(parent)
{
  m_.setIdentity();
}

CQIllustratorShape::
CQIllustratorShape(const CQIllustratorShape &shape) :
 QObject   (),
 id_       (last_id_++),
 parent_   (shape.parent_),
 shapes_   (shape.shapes_),
 name_     (shape.name_),
 stroke_   (shape.stroke_),
 fill_     (shape.fill_),
 filter_id_(shape.filter_id_),
 clip_     (shape.clip_),
 fixed_    (shape.fixed_),
 visible_  (shape.visible_),
 rcenter_  (shape.rcenter_),
 m_        (shape.m_)
{
}

CQIllustratorShape::
~CQIllustratorShape()
{
  if (parent_)
    parent_->removeChild(this);
}

void
CQIllustratorShape::
setMatrix(const CMatrix2D &m)
{
  checkoutShape(CQIllustratorData::ChangeType::GEOMETRY);

  m_ = m;

  invalidateBBox();

  checkinShape(CQIllustratorData::ChangeType::GEOMETRY);
}

CMatrix2D
CQIllustratorShape::
getFlatMatrix() const
{
  CMatrix2D m = m_;

  if (parent_) {
    CQIllustratorShape *parent = parent_;

    while (parent != 0) {
      m = parent->m_*m;

      parent = parent->parent_;
    }
  }

  return m;
}

// get bbox transformed to top
CBBox2D
CQIllustratorShape::
getFlatBBox() const
{
  CBBox2D bbox = getBBox();

  if (! bbox.isSet())
    return bbox;

  if (! parent_)
    return bbox;

  // if parent transform pby parent flat matric
  const CMatrix2D &m = parent_->getFlatMatrix();

  CPoint2D p1 = m*bbox.getLL();
  CPoint2D p2 = m*bbox.getLR();
  CPoint2D p3 = m*bbox.getUL();
  CPoint2D p4 = m*bbox.getUR();

  CBBox2D bbox1(p1, p2);

  bbox1 += p3;
  bbox1 += p4;

  return bbox1;
}

QRectF
CQIllustratorShape::
getFlatQRect() const
{
  CBBox2D bbox = getFlatBBox();

  if (bbox.isSet())
    return CQUtil::toQRect(bbox);
  else
    return QRectF();
}

void
CQIllustratorShape::
getControlLines(ControlLineList &) const
{
}

void
CQIllustratorShape::
setControlLine(const CQIllustratorShapeControlLine *)
{
}

void
CQIllustratorShape::
getControlPoints(ControlPointList &points, ControlType type) const
{
  if      (type == ControlType::GEOMETRY) {
  }
  else if (type == ControlType::LGRADIENT) {
    const CQIllustratorShapeFill &fill = getFill();

    const CGenGradient *g = fill.getGradient();

    const auto *lg = dynamic_cast<const CLinearGradient *>(g);

    if (! lg) return;

    //CBBox2D bbox = getFlatBBox();

    //double x1 = lg->getX1()*(bbox.getXMax() - bbox.getXMin()) + bbox.getXMin();
    //double y1 = lg->getY1()*(bbox.getYMax() - bbox.getYMin()) + bbox.getYMin();
    //double x2 = lg->getX2()*(bbox.getXMax() - bbox.getXMin()) + bbox.getXMin();
    //double y2 = lg->getY2()*(bbox.getYMax() - bbox.getYMin()) + bbox.getYMin();

    CPoint2D p1(lg->getX1(), lg->getY1());
    CPoint2D p2(lg->getX2(), lg->getY2());

    CQIllustratorShapeLGradientControlPoint *start =
      new CQIllustratorShapeLGradientControlPoint(
            CQIllustratorShapeLGradientControlPoint::Position::START, p1);
    CQIllustratorShapeLGradientControlPoint *end   =
      new CQIllustratorShapeLGradientControlPoint(
            CQIllustratorShapeLGradientControlPoint::Position::END  , p2);

    points.push_back(start);
    points.push_back(end);
  }
  else if (type == ControlType::RGRADIENT) {
    const CQIllustratorShapeFill &fill = getFill();

    const CGenGradient *g = fill.getGradient();

    const auto *rg = dynamic_cast<const CRadialGradient *>(g);

    if (! rg) return;

    //CBBox2D bbox = getFlatBBox();

    //double x1 = rg->getX1()*(bbox.getXMax() - bbox.getXMin()) + bbox.getXMin();
    //double y1 = rg->getY1()*(bbox.getYMax() - bbox.getYMin()) + bbox.getYMin();
    //double x2 = rg->getX2()*(bbox.getXMax() - bbox.getXMin()) + bbox.getXMin();
    //double y2 = rg->getY2()*(bbox.getYMax() - bbox.getYMin()) + bbox.getYMin();

    CPoint2D c(rg->getCenterX(), rg->getCenterY());
    CPoint2D f(rg->getFocusX (), rg->getFocusY ());

    double r1 = rg->getRadius()/sqrt(2);

    CPoint2D rp(c.x + r1, c.y + r1);

    CQIllustratorShapeRGradientControlPoint *center =
      new CQIllustratorShapeRGradientControlPoint(
            CQIllustratorShapeRGradientControlPoint::Position::CENTER, c);
    CQIllustratorShapeRGradientControlPoint *focus  =
      new CQIllustratorShapeRGradientControlPoint(
            CQIllustratorShapeRGradientControlPoint::Position::FOCUS , f);
    CQIllustratorShapeRGradientControlPoint *radius =
      new CQIllustratorShapeRGradientControlPoint(
            CQIllustratorShapeRGradientControlPoint::Position::RADIUS, rp);

    points.push_back(center);
    points.push_back(focus);
    points.push_back(radius);
  }
}

void
CQIllustratorShape::
addChild(CQIllustratorShape *child)
{
  child->parent_ = this;

  shapes_.push_back(child);

  childrenChanged();
}

void
CQIllustratorShape::
removeChild(CQIllustratorShape *child)
{
  child->parent_ = 0;

  shapes_.remove(child);

  childrenChanged();
}

void
CQIllustratorShape::
setParent(CQIllustratorShape *newParent)
{
  if (parent_)
    parent_->removeChild(this);

  if (newParent)
    newParent->addChild(this);
  else
    parent_ = 0;
}

void
CQIllustratorShape::
setName(const std::string &name)
{
  checkoutShape(CQIllustratorData::ChangeType::NAME);

  name_ = name;

  checkinShape(CQIllustratorData::ChangeType::NAME);
}

// get bbox of shape
const CBBox2D &
CQIllustratorShape::
getBBox() const
{
  updateBBox();

  return bbox_;
}

QRectF
CQIllustratorShape::
getQRect() const
{
  CBBox2D bbox = getBBox();

  if (bbox.isSet())
    return CQUtil::toQRect(bbox);
  else
    return QRectF();
}

CPoint2D
CQIllustratorShape::
getCenter() const
{
  return getFlatBBox().getCenter();
}

CQIllustratorShapeStroke &
CQIllustratorShape::
getStroke()
{
  return stroke_;
}

const CQIllustratorShapeStroke &
CQIllustratorShape::
getStroke() const
{
  return stroke_;
}

CQIllustratorShapeFill &
CQIllustratorShape::
getFill()
{
  return fill_;
}

const CQIllustratorShapeFill &
CQIllustratorShape::
getFill() const
{
  return fill_;
}

void
CQIllustratorShape::
setStroke(const CQIllustratorShapeStroke &stroke)
{
  checkoutShape(CQIllustratorData::ChangeType::STROKE);

  stroke_ = stroke;

  invalidateBBox();

  checkinShape(CQIllustratorData::ChangeType::STROKE);
}

void
CQIllustratorShape::
setFill(const CQIllustratorShapeFill &fill)
{
  checkoutShape(CQIllustratorData::ChangeType::FILL);

  fill_ = fill;

  checkinShape(CQIllustratorData::ChangeType::FILL);
}

void
CQIllustratorShape::
setStrokeColor(const CRGBA &rgba)
{
  getStroke().setColor(rgba);
}

void
CQIllustratorShape::
setFillColor(const CRGBA &rgba)
{
  fill_.setColor(rgba);
}

void
CQIllustratorShape::
setStrokeOpacity(double opacity)
{
  getStroke().setOpacity(opacity);
}

void
CQIllustratorShape::
setFillOpacity(double opacity)
{
  fill_.setOpacity(opacity);
}

double
CQIllustratorShape::
getStrokeWidth() const
{
  const CMatrix2D &m = getFlatMatrix();

  double w = getStroke().getWidth();

  CVector2D w1 = m*CVector2D(w, w);

  return w1.length();
}

CQIllustratorShapeFilter *
CQIllustratorShape::
getFilter() const
{
  if (filter_id_)
    return CQIllustratorShapeFilterMgrInst->getFilter(filter_id_);
  else
    return 0;
}

void
CQIllustratorShape::
setFilter(uint num)
{
  checkoutShape(CQIllustratorData::ChangeType::GEOMETRY);

  CQIllustratorShapeFilter *filter = CQIllustratorShapeFilterMgrInst->getFilter(num);

  setFilter(filter);

  checkinShape(CQIllustratorData::ChangeType::GEOMETRY);
}

void
CQIllustratorShape::
setFilter(CQIllustratorShapeFilter *filter)
{
  if (filter)
    filter_id_ = filter->getId();
  else
    filter_id_ = 0;
}

void
CQIllustratorShape::
unsetFilter()
{
  checkoutShape(CQIllustratorData::ChangeType::GEOMETRY);

  setFilter(static_cast<CQIllustratorShapeFilter *>(nullptr));

  checkinShape(CQIllustratorData::ChangeType::GEOMETRY);
}

void
CQIllustratorShape::
setClip(bool clip)
{
  checkoutShape(CQIllustratorData::ChangeType::GEOMETRY);

  clip_ = clip;

  checkinShape(CQIllustratorData::ChangeType::GEOMETRY);
}

bool
CQIllustratorShape::
inside(const CPoint2D &p) const
{
  const CBBox2D &bbox = getFlatBBox();

  return bbox.inside(p);
}

CPoint2D
CQIllustratorShape::
getRotateCenter() const
{
  CMatrix2D save_m = m_;

  auto *th = const_cast<CQIllustratorShape *>(this);

  th->m_.setIdentity();

  invalidateBBox();

  const CMatrix2D &m = getFlatMatrix();

  CPoint2D rc = m*(getCenter() + rcenter_);

  th->m_ = save_m;

  invalidateBBox();

  return rc;
}

void
CQIllustratorShape::
setRotateCenter(const CPoint2D &rc)
{
  CMatrix2D save_m = m_;

  auto *th = const_cast<CQIllustratorShape *>(this);

  th->m_.setIdentity();

  invalidateBBox();

  const CMatrix2D &m = getFlatMatrix();

  CMatrix2D im = m.inverse();

  rcenter_ = im*rc - getCenter();

  th->m_ = save_m;

  invalidateBBox();
}

double
CQIllustratorShape::
distance(const CPoint2D &p) const
{
  const CBBox2D &bbox = getFlatBBox();

  const CPoint2D &p1 = bbox.getLL();
  const CPoint2D &p2 = bbox.getUR();

  if      (p.x < p1.x) {
    if      (p.y < p1.y) return pointDist(p, CPoint2D(p1.x, p1.y));
    else if (p.y > p2.y) return pointDist(p, CPoint2D(p1.x, p2.y));
    else                 return (p1.x - p.x);
  }
  else if (p.x > p2.x) {
    if      (p.y < p1.y) return pointDist(p, CPoint2D(p2.x, p1.y));
    else if (p.y > p2.y) return pointDist(p, CPoint2D(p2.x, p2.y));
    else                 return (p.x - p2.x);
  }
  else {
    if      (p.y < p1.y) return (p1.y - p.y);
    else if (p.y > p2.y) return (p.y - p2.y);
    else                 return 0;
  }
}

CQIllustratorShapeNearestPoint2D
CQIllustratorShape::
nearestPoint(const CPoint2D &p, ControlType type) const
{
  CQIllustratorShapeNearestPoint2D nearest;

  ControlPointList points;

  getControlPoints(points, type);

  uint num_points = points.size();

  for (uint i = 0; i < num_points; ++i) {
    double dist = points[i]->distance(this, p);

    nearest.updatePoint(points[i], dist);
  }

  return nearest;
}

void
CQIllustratorShape::
moveTo(const CPoint2D &pos)
{
  const CBBox2D &bbox = getFlatBBox();

  CPoint2D d = pos - bbox.getLL();

  moveBy(d);
}

void
CQIllustratorShape::
moveBy(const CPoint2D &d)
{
  assert(! getFixed());

  checkoutShape(CQIllustratorData::ChangeType::GEOMETRY);

  CMatrix2D m;

  m.setTranslation(d.x, d.y);

  transform(CPoint2D(0, 0), m);

  checkinShape(CQIllustratorData::ChangeType::GEOMETRY);
}

void
CQIllustratorShape::
resizeTo(double w, double h)
{
  const CBBox2D &bbox = getFlatBBox();

  double dw = w - bbox.getWidth ();
  double dh = h - bbox.getHeight();

  resizeBy(dw, dh);
}

void
CQIllustratorShape::
resizeBy(double dw, double dh)
{
  const CBBox2D &bbox = getFlatBBox();

  double w = bbox.getWidth ();
  double h = bbox.getHeight();

  scale(bbox.getLL(), (w + dw)/w, (h + dh)/h);
}

void
CQIllustratorShape::
movePointBy(const CQIllustratorShapeControlPoint *point, const CPoint2D &d)
{
  checkoutShape(CQIllustratorData::ChangeType::GEOMETRY);

  CQIllustratorShapeControlPoint *point1 = point->dup();

  point1->movePointBy(this, d);

  setControlPoint(point1);

  delete point1;

  checkinShape(CQIllustratorData::ChangeType::GEOMETRY);
}

void
CQIllustratorShape::
movePointTo(const CQIllustratorShapeControlPoint *point, const CPoint2D &pos)
{
  CQIllustratorShapeControlPoint *point1 = point->dup();

  point1->movePointTo(this, pos);

  setControlPoint(point1);

  delete point1;
}

bool
CQIllustratorShape::
removePoint(const CQIllustratorShapeControlPoint *point)
{
  checkoutShape(CQIllustratorData::ChangeType::GEOMETRY);

  bool rc = doRemovePoint(point);

  checkinShape(CQIllustratorData::ChangeType::GEOMETRY);

  return rc;
}

bool
CQIllustratorShape::
doRemovePoint(const CQIllustratorShapeControlPoint *)
{
  return false;
}

void
CQIllustratorShape::
scale(const CPoint2D &c, double sx, double sy)
{
  checkoutShape(CQIllustratorData::ChangeType::GEOMETRY);

  CMatrix2D m;

  m.setScale(sx, sy);

  transform(c, m);

  checkinShape(CQIllustratorData::ChangeType::GEOMETRY);
}

void
CQIllustratorShape::
rotate(double da)
{
  checkoutShape(CQIllustratorData::ChangeType::GEOMETRY);

  CMatrix2D m;

  m.setRotation(da);

  CMatrix2D m1, m2;

  CPoint2D rc = getRotateCenter();

  m1.setTranslation(-rc.x, -rc.y);
  m2.setTranslation( rc.x,  rc.y);

  CMatrix2D mm = m2*m*m1;

  setMatrix(mm*m_);

  checkinShape(CQIllustratorData::ChangeType::GEOMETRY);
}

void
CQIllustratorShape::
skew(double dx, double dy)
{
  checkoutShape(CQIllustratorData::ChangeType::GEOMETRY);

  CMatrix2D m;

  m.setSkew(dx, dy);

  CMatrix2D m1, m2;

  CPoint2D rc = getRotateCenter();

  m1.setTranslation(-rc.x, -rc.y);
  m2.setTranslation( rc.x,  rc.y);

  CMatrix2D mm = m2*m*m1;

  setMatrix(mm*m_);

  checkinShape(CQIllustratorData::ChangeType::GEOMETRY);
}

void
CQIllustratorShape::
transform(const CPoint2D &c, const CMatrix2D &m)
{
  CMatrix2D m1, m2;

  m1.setTranslation(-c.x, -c.y);
  m2.setTranslation( c.x,  c.y);

  CMatrix2D mm = m2*m*m1;

  setMatrix(mm*m_);
}

void
CQIllustratorShape::
setBBox(const CBBox2D &bbox)
{
  checkoutShape(CQIllustratorData::ChangeType::GEOMETRY);

  moveTo(bbox.getLL());

  resizeTo(bbox.getWidth(), bbox.getHeight());

  checkinShape(CQIllustratorData::ChangeType::GEOMETRY);
}

void
CQIllustratorShape::
draw(CQIllustratorShapeDrawer *drawer) const
{
  if (getFilter() != 0)
    drawGaussian(drawer);
  else
    drawShape(drawer);

  if (getFixed()) {
    const CBBox2D &bbox = getFlatBBox();

    drawer->pathInit();
    drawer->pathMoveTo(bbox.getLL());
    drawer->pathLineTo(bbox.getUR());
    drawer->pathStroke();

    drawer->pathInit();
    drawer->pathMoveTo(bbox.getLR());
    drawer->pathLineTo(bbox.getUL());
    drawer->pathStroke();
  }
}

void
CQIllustratorShape::
drawGaussian(CQIllustratorShapeDrawer *drawer) const
{
  CQIllustratorFilterShapeDrawer drawer1(drawer, this);

  drawShape(&drawer1);

  const CBBox2D &bbox = getBBox();

  //QTransform transform = drawer->getPainter()->worldTransform();
  QTransform transform = drawer->getPainter()->transform();

  QPointF p1 = transform.map(QPointF(bbox.getXMin(), bbox.getYMin()));
  QPointF p2 = transform.map(QPointF(bbox.getXMax(), bbox.getYMax()));

  int px1 = std::min(p1.x(), p2.x()) - 4;
  int py1 = std::min(p1.y(), p2.y()) - 4;
  int px2 = std::max(p1.x(), p2.x()) + 4;
  int py2 = std::max(p1.y(), p2.y()) + 4;

  //-----

  int w = px2 - px1 + 1;
  int h = py2 - py1 + 1;

  const QImage &qimage = drawer1.getImage().copy(px1, py1, w, h);

  QImage qimage1(w, h, QImage::Format_ARGB32);

  qimage1.fill(qRgba(0,0,0,0));

  //------

  double bx = 2.0;
  double by = 2.0;
  int    nx = 0;
  int    ny = 0;

  //------

  CQImageGaussianBlur blur(qimage);

  blur.blur(qimage1, bx, by, nx, ny);

  //------

  drawer->drawImage(px1, py1, qimage1);
}

void
CQIllustratorShape::
drawSelect(CQIllustratorShapeDrawer *drawer)
{
  CBBox2D bbox = getFlatBBox();

  if (bbox.isSet()) {
    CPoint2D p1 = bbox.getLL();
    CPoint2D p2 = bbox.getLR();
    CPoint2D p3 = bbox.getUR();
    CPoint2D p4 = bbox.getUL();

    drawer->drawControlLine(p1, p2);
    drawer->drawControlLine(p2, p3);
    drawer->drawControlLine(p3, p4);
    drawer->drawControlLine(p4, p1);
  }
}

bool
CQIllustratorShape::
flip(bool)
{
  return false;
}

void
CQIllustratorShape::
setGeometry(const CQIllustratorShapeGeometry *geom)
{
  checkoutShape(CQIllustratorData::ChangeType::GEOMETRY);

  doSetGeometry(geom);

  checkinShape(CQIllustratorData::ChangeType::GEOMETRY);
}

std::string
CQIllustratorShape::
getSVGStroke() const
{
  std::string str = "style=\"";

  const CQIllustratorShapeStroke &stroke = getStroke();

  const CRGBA     &scolor   = stroke.getColor();
  double           sopacity = stroke.getOpacity();
  double           width    = getStrokeWidth();
  const CLineDash &dash     = stroke.getLineDash();
  CLineCapType     cap      = stroke.getLineCap();
  CLineJoinType    join     = stroke.getLineJoin();

  if (sopacity > 0.0) {
    str += CStrUtil::strprintf("stroke: rgb(%d,%d,%d);", scolor.getRedI(),
                               scolor.getGreenI(), scolor.getBlueI());

    if (sopacity < 1.0)
      str += CStrUtil::strprintf(" stroke-opacity: %g;", sopacity);
  }
  else
    str += "stroke: none;";

  if (width != 1.0)
    str += CStrUtil::strprintf(" stroke-width: %g;", width);

  if (! dash.isSolid())
    str += CStrUtil::strprintf("stroke-dasharray: %s", dash.toString().c_str());

  if      (cap == LINE_CAP_TYPE_ROUND)
    str += CStrUtil::strprintf(" stroke-linecap: round;");
  else if (cap == LINE_CAP_TYPE_SQUARE)
    str += CStrUtil::strprintf(" stroke-linecap: square;");

  if      (join == LINE_JOIN_TYPE_ROUND)
    str += CStrUtil::strprintf(" stroke-linejoin: round;");
  else if (join == LINE_JOIN_TYPE_BEVEL)
    str += CStrUtil::strprintf(" stroke-linejoin: bevel;");

  const CQIllustratorShapeFill &fill = getFill();

  if (fill.hasGradient()) {
    // TODO
  }
  else {
    const CRGBA &fcolor   = fill.getColor();
    double       fopacity = fill.getOpacity();

    if (fopacity > 0.0) {
      str += CStrUtil::strprintf(" fill: rgb(%d,%d,%d);", fcolor.getRedI(),
                                 fcolor.getGreenI(), fcolor.getBlueI());

      if (fopacity < 1.0)
        str += CStrUtil::strprintf(" fill-opacity: %g;", fopacity);
    }
    else
      str += "fill: none;";
  }

  str += "\"";

  return str;
}

std::string
CQIllustratorShape::
getSVGMatrix() const
{
  return getSVGMatrix(m_);
}

std::string
CQIllustratorShape::
getSVGMatrix(const CMatrix2D &m) const
{
  if (m.isIdentity()) return "";

  std::string str = " transform=\"matrix(";

  double values[6];

  m.getValues(values, 6);

  for (uint i = 0; i < 6; ++i) {
    if (i > 0) str += ", ";

    str += CStrUtil::strprintf("%g", values[i]);
  }

  str += ")\"";

  return str;
}

double
CQIllustratorShape::
pointDist(const CPoint2D &p1, const CPoint2D &p2) const
{
  double dx = p1.x - p2.x;
  double dy = p1.y - p2.y;

  return sqrt(dx*dx + dy*dy);
}

void
CQIllustratorShape::
lockShape()
{
  ++lock_count_;
}

void
CQIllustratorShape::
unlockShape()
{
  assert(lock_count_ > 0);

  --lock_count_;
}

void
CQIllustratorShape::
checkoutShape(CQIllustratorData::ChangeType changeType)
{
  if (data_)
    data_->checkoutShape(this, changeType);
}

void
CQIllustratorShape::
checkinShape(CQIllustratorData::ChangeType changeType)
{
  if (data_)
    data_->checkinShape(this, changeType);
}
