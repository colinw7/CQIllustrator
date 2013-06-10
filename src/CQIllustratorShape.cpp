#include <CQIllustratorShape.h>
#include <CQIllustratorData.h>
#include <CQIllustratorShapeDrawer.h>

#include <QPainter>
#include <CMathGeom2D.h>
#include <CLinearGradient.h>
#include <CRadialGradient.h>
#include <CQImageGaussianBlur.h>

uint CQIllustratorShape:: last_id_ = 1;

CQIllustratorShape::
CQIllustratorShape(CQIllustratorShape *parent) :
 data_      (NULL),
 id_        (last_id_++),
 parent_    (parent),
 shapes_    (),
 name_      (""),
 stroke_    (),
 fill_      (),
 filter_id_ (0),
 clip_      (false),
 fixed_     (false),
 visible_   (true),
 rcenter_   (),
 m_         (),
 lock_count_(0),
 bbox_      (),
 bbox_valid_(false)
{
  m_.setIdentity();
}

CQIllustratorShape::
CQIllustratorShape(const CQIllustratorShape &shape) :
 data_      (NULL),
 id_        (last_id_++),
 parent_    (shape.parent_),
 shapes_    (shape.shapes_),
 name_      (shape.name_),
 stroke_    (shape.stroke_),
 fill_      (shape.fill_),
 filter_id_ (shape.filter_id_),
 clip_      (shape.clip_),
 fixed_     (shape.fixed_),
 visible_   (shape.visible_),
 rcenter_   (shape.rcenter_),
 m_         (shape.m_),
 lock_count_(0),
 bbox_      (),
 bbox_valid_(false)
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
  checkoutShape(CQIllustratorData::CHANGE_GEOMETRY);

  m_ = m;

  invalidateBBox();

  checkinShape(CQIllustratorData::CHANGE_GEOMETRY);
}

const CMatrix2D &
CQIllustratorShape::
getFlatMatrix() const
{
  if (parent_) {
    static CMatrix2D m;

    m.setIdentity();

    CQIllustratorShape *parent = parent_;

    while (parent != NULL) {
      m = parent->m_*m;

      parent = parent->parent_;
    }

    return m;
  }
  else
    return m_;
}

CBBox2D
CQIllustratorShape::
getFlatBBox() const
{
  CBBox2D bbox = getBBox();

  if (parent_) {
    const CMatrix2D &m = getFlatMatrix();

    CPoint2D p1 = m*bbox.getLL();
    CPoint2D p2 = m*bbox.getLR();
    CPoint2D p3 = m*bbox.getUL();
    CPoint2D p4 = m*bbox.getUR();

    CBBox2D bbox1;

    bbox1 += p1;
    bbox1 += p2;
    bbox1 += p3;
    bbox1 += p4;

    return bbox1;
  }
  else
    return bbox;
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
  if      (type == CONTROL_GEOMETRY) {
  }
  else if (type == CONTROL_LGRADIENT) {
    const CQIllustratorShapeFill &fill = getFill();

    const CGenGradient *g = fill.getGradient();

    const CLinearGradient *lg = dynamic_cast<const CLinearGradient *>(g);

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
            CQIllustratorShapeLGradientControlPoint::START, p1);
    CQIllustratorShapeLGradientControlPoint *end   =
      new CQIllustratorShapeLGradientControlPoint(
            CQIllustratorShapeLGradientControlPoint::END  , p2);

    points.push_back(start);
    points.push_back(end);
  }
  else if (type == CONTROL_RGRADIENT) {
    const CQIllustratorShapeFill &fill = getFill();

    const CGenGradient *g = fill.getGradient();

    const CRadialGradient *rg = dynamic_cast<const CRadialGradient *>(g);

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
            CQIllustratorShapeRGradientControlPoint::CENTER, c);
    CQIllustratorShapeRGradientControlPoint *focus  =
      new CQIllustratorShapeRGradientControlPoint(
            CQIllustratorShapeRGradientControlPoint::FOCUS , f);
    CQIllustratorShapeRGradientControlPoint *radius =
      new CQIllustratorShapeRGradientControlPoint(
            CQIllustratorShapeRGradientControlPoint::RADIUS, rp);

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
  child->parent_ = NULL;

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
    parent_ = NULL;
}

void
CQIllustratorShape::
setName(const std::string &name)
{
  checkoutShape(CQIllustratorData::CHANGE_NAME);

  name_ = name;

  checkinShape(CQIllustratorData::CHANGE_NAME);
}

const CBBox2D &
CQIllustratorShape::
getBBox() const
{
  updateBBox();

  return bbox_;
}

CPoint2D
CQIllustratorShape::
getCenter() const
{
  return getBBox().getCenter();
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
  checkoutShape(CQIllustratorData::CHANGE_STROKE);

  stroke_ = stroke;

  invalidateBBox();

  checkinShape(CQIllustratorData::CHANGE_STROKE);
}

void
CQIllustratorShape::
setFill(const CQIllustratorShapeFill &fill)
{
  checkoutShape(CQIllustratorData::CHANGE_FILL);

  fill_ = fill;

  checkinShape(CQIllustratorData::CHANGE_FILL);
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
    return NULL;
}

void
CQIllustratorShape::
setFilter(uint num)
{
  checkoutShape(CQIllustratorData::CHANGE_GEOMETRY);

  CQIllustratorShapeFilter *filter = CQIllustratorShapeFilterMgrInst->getFilter(num);

  setFilter(filter);

  checkinShape(CQIllustratorData::CHANGE_GEOMETRY);
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
  checkoutShape(CQIllustratorData::CHANGE_GEOMETRY);

  setFilter((CQIllustratorShapeFilter *) NULL);

  checkinShape(CQIllustratorData::CHANGE_GEOMETRY);
}

void
CQIllustratorShape::
setClip(bool clip)
{
  checkoutShape(CQIllustratorData::CHANGE_GEOMETRY);

  clip_ = clip;

  checkinShape(CQIllustratorData::CHANGE_GEOMETRY);
}

bool
CQIllustratorShape::
inside(const CPoint2D &p) const
{
  const CBBox2D &bbox = getBBox();

  return bbox.inside(p);
}

CPoint2D
CQIllustratorShape::
getRotateCenter() const
{
  CMatrix2D save_m = m_;

  CQIllustratorShape *th = const_cast<CQIllustratorShape *>(this);

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

  CQIllustratorShape *th = const_cast<CQIllustratorShape *>(this);

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
  const CBBox2D &bbox = getBBox();

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
  const CBBox2D &bbox = getBBox();

  CPoint2D d = pos - bbox.getLL();

  moveBy(d);
}

void
CQIllustratorShape::
moveBy(const CPoint2D &d)
{
  assert(! getFixed());

  checkoutShape(CQIllustratorData::CHANGE_GEOMETRY);

  CMatrix2D m;

  m.setTranslation(d.x, d.y);

  transform(CPoint2D(0, 0), m);

  checkinShape(CQIllustratorData::CHANGE_GEOMETRY);
}

void
CQIllustratorShape::
resizeTo(double w, double h)
{
  const CBBox2D &bbox = getBBox();

  double dw = w - bbox.getWidth ();
  double dh = h - bbox.getHeight();

  resizeBy(dw, dh);
}

void
CQIllustratorShape::
resizeBy(double dw, double dh)
{
  const CBBox2D &bbox = getBBox();

  double w = bbox.getWidth ();
  double h = bbox.getHeight();

  scale(bbox.getLL(), (w + dw)/w, (h + dh)/h);
}

void
CQIllustratorShape::
movePointBy(const CQIllustratorShapeControlPoint *point, const CPoint2D &d)
{
  checkoutShape(CQIllustratorData::CHANGE_GEOMETRY);

  CQIllustratorShapeControlPoint *point1 = point->dup();

  point1->movePointBy(this, d);

  setControlPoint(point1);

  delete point1;

  checkinShape(CQIllustratorData::CHANGE_GEOMETRY);
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
  checkoutShape(CQIllustratorData::CHANGE_GEOMETRY);

  bool rc = doRemovePoint(point);

  checkinShape(CQIllustratorData::CHANGE_GEOMETRY);

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
  checkoutShape(CQIllustratorData::CHANGE_GEOMETRY);

  CMatrix2D m;

  m.setScale(sx, sy);

  transform(c, m);

  checkinShape(CQIllustratorData::CHANGE_GEOMETRY);
}

void
CQIllustratorShape::
rotate(double da)
{
  checkoutShape(CQIllustratorData::CHANGE_GEOMETRY);

  CMatrix2D m;

  m.setRotation(da);

  CMatrix2D m1, m2;

  CPoint2D rc = getRotateCenter();

  m1.setTranslation(-rc.x, -rc.y);
  m2.setTranslation( rc.x,  rc.y);

  CMatrix2D mm = m2*m*m1;

  setMatrix(mm*m_);

  checkinShape(CQIllustratorData::CHANGE_GEOMETRY);
}

void
CQIllustratorShape::
skew(double dx, double dy)
{
  checkoutShape(CQIllustratorData::CHANGE_GEOMETRY);

  CMatrix2D m;

  m.setSkew(dx, dy);

  CMatrix2D m1, m2;

  CPoint2D rc = getRotateCenter();

  m1.setTranslation(-rc.x, -rc.y);
  m2.setTranslation( rc.x,  rc.y);

  CMatrix2D mm = m2*m*m1;

  setMatrix(mm*m_);

  checkinShape(CQIllustratorData::CHANGE_GEOMETRY);
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
  checkoutShape(CQIllustratorData::CHANGE_GEOMETRY);

  moveTo(bbox.getLL());

  resizeTo(bbox.getWidth(), bbox.getHeight());

  checkinShape(CQIllustratorData::CHANGE_GEOMETRY);
}

void
CQIllustratorShape::
draw(CQIllustratorShapeDrawer *drawer) const
{
  if (getFilter() != NULL)
    drawGaussian(drawer);
  else
    drawShape(drawer);

  if (getFixed()) {
    const CBBox2D &bbox = getBBox();

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

  CPoint2D p1 = bbox.getLL();
  CPoint2D p2 = bbox.getLR();
  CPoint2D p3 = bbox.getUR();
  CPoint2D p4 = bbox.getUL();

  drawer->drawControlLine(p1, p2);
  drawer->drawControlLine(p2, p3);
  drawer->drawControlLine(p3, p4);
  drawer->drawControlLine(p4, p1);
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
  checkoutShape(CQIllustratorData::CHANGE_GEOMETRY);

  doSetGeometry(geom);

  checkinShape(CQIllustratorData::CHANGE_GEOMETRY);
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

//----------

CQIllustratorShapeLGradientControlPoint::
CQIllustratorShapeLGradientControlPoint(Position pos, const CPoint2D &p) :
 CQIllustratorShapeControlPoint((uint) pos, p), pos_(pos)
{
}

CQIllustratorShapeLGradientControlPoint *
CQIllustratorShapeLGradientControlPoint::
dup() const
{
  return new CQIllustratorShapeLGradientControlPoint(pos_, point_);
}

CPoint2D
CQIllustratorShapeLGradientControlPoint::
getPoint(const CQIllustratorShape *shape) const
{
  CBBox2D bbox = shape->getFlatBBox();

  const CQIllustratorShapeFill &fill = shape->getFill();

  const CGenGradient *g = fill.getGradient();

  const CLinearGradient *lg = dynamic_cast<const CLinearGradient *>(g);

  assert(lg);

  CPoint2D p;

  if (lg) {
    if (pos_ == START)
      p = CPoint2D(lg->getX1(), lg->getY1());
    else
      p = CPoint2D(lg->getX2(), lg->getY2());
  }
  else {
    if (pos_ == START)
      p = CPoint2D(0, 0);
    else
      p = CPoint2D(1, 1);
  }

  double x1 = p.x*(bbox.getXMax() - bbox.getXMin()) + bbox.getXMin();
  double y1 = p.y*(bbox.getYMax() - bbox.getYMin()) + bbox.getYMin();

  return CPoint2D(x1, y1);
}

void
CQIllustratorShapeLGradientControlPoint::
setPoint(CQIllustratorShape *shape, const CPoint2D &point)
{
  CQIllustratorShapeFill &fill = shape->getFill();

  CGenGradient *g = fill.getGradient();

  CLinearGradient *lg = dynamic_cast<CLinearGradient *>(g);

  if (! lg) return;

  CBBox2D bbox = shape->getFlatBBox();

  CPoint2D p;

  p.x = (point.x - bbox.getXMin())/(bbox.getXMax() - bbox.getXMin());
  p.y = (point.y - bbox.getYMin())/(bbox.getYMax() - bbox.getYMin());

  if (pos_ == START) {
    lg->setX1(p.x);
    lg->setY1(p.y);
  }
  else {
    lg->setX2(p.x);
    lg->setY2(p.y);
  }

  point_ = p;
}

//----------

CQIllustratorShapeRGradientControlPoint::
CQIllustratorShapeRGradientControlPoint(Position pos, const CPoint2D &p) :
 CQIllustratorShapeControlPoint((uint) pos, p), pos_(pos)
{
}

CQIllustratorShapeRGradientControlPoint *
CQIllustratorShapeRGradientControlPoint::
dup() const
{
  return new CQIllustratorShapeRGradientControlPoint(pos_, point_);
}

CPoint2D
CQIllustratorShapeRGradientControlPoint::
getPoint(const CQIllustratorShape *shape) const
{
  CBBox2D bbox = shape->getFlatBBox();

  const CQIllustratorShapeFill &fill = shape->getFill();

  const CGenGradient *g = fill.getGradient();

  const CRadialGradient *rg = dynamic_cast<const CRadialGradient *>(g);

  assert(rg);

  CPoint2D p;

  if (rg) {
    if      (pos_ == CENTER)
      p = CPoint2D(rg->getCenterX(), rg->getCenterY());
    else if (pos_ == FOCUS)
      p = CPoint2D(rg->getFocusX(), rg->getFocusY());
    else {
      double r1 = rg->getRadius()/sqrt(2);

      p = CPoint2D(rg->getCenterX() + r1, rg->getCenterY() + r1);
    }
  }
  else {
    if      (pos_ == CENTER)
      p = CPoint2D(0.5, 0.5);
    else if (pos_ == FOCUS)
      p = CPoint2D(0.5, 0.5);
    else
      p = CPoint2D(1.0, 0.0);
  }

  double x1 = p.x*(bbox.getXMax() - bbox.getXMin()) + bbox.getXMin();
  double y1 = p.y*(bbox.getYMax() - bbox.getYMin()) + bbox.getYMin();

  return CPoint2D(x1, y1);
}

void
CQIllustratorShapeRGradientControlPoint::
setPoint(CQIllustratorShape *shape, const CPoint2D &point)
{
  CQIllustratorShapeFill &fill = shape->getFill();

  CGenGradient *g = fill.getGradient();

  CRadialGradient *rg = dynamic_cast<CRadialGradient *>(g);

  if (! rg) return;

  CBBox2D bbox = shape->getFlatBBox();

  CPoint2D p;

  p.x = (point.x - bbox.getXMin())/(bbox.getXMax() - bbox.getXMin());
  p.y = (point.y - bbox.getYMin())/(bbox.getYMax() - bbox.getYMin());

  if      (pos_ == CENTER)
    rg->setCenter(p);
  else if (pos_ == FOCUS)
    rg->setFocus(p);
  else {
    double d = p.distanceTo(CPoint2D(rg->getCenterX(), rg->getCenterY()));

    rg->setRadius(d);
  }

  rg->init(1, 1);

  point_ = p;
}

//----------

CQIllustratorRectShape::
CQIllustratorRectShape(const CPoint2D &p1, const CPoint2D &p2, double rx, double ry) :
 CQIllustratorShape(), p1_(std::min(p1.x, p2.x), std::min(p1.y, p2.y)),
 p2_(std::max(p1.x, p2.x), std::max(p1.y, p2.y)),
 rx_(rx), ry_(ry)
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
  if (type == CONTROL_GEOMETRY) {
    const CMatrix2D &m = getFlatMatrix();

    CPoint2D p1 = m*p1_;
    CPoint2D p2 = m*p2_;

    CPoint2D rx1(rx_, 0);
    CPoint2D ry1(0, ry_);

    CQIllustratorRectShapeControlPoint *ll = new CQIllustratorRectShapeControlPoint(TYPE_LL, p1);
    CQIllustratorRectShapeControlPoint *ur = new CQIllustratorRectShapeControlPoint(TYPE_UR, p2);
    CQIllustratorRectShapeControlPoint *rx = new CQIllustratorRectShapeControlPoint(TYPE_RX, rx1);
    CQIllustratorRectShapeControlPoint *ry = new CQIllustratorRectShapeControlPoint(TYPE_RY, ry1);

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

  if      (type == CQIllustratorRectShape::TYPE_LL)
    p1_ = p;
  else if (type == CQIllustratorRectShape::TYPE_UR)
    p2_ = p;
  else if (type == CQIllustratorRectShape::TYPE_RX)
    rx_ = std::min(std::max(p.x - p1_.x, 0.0), fabs(p2_.x - p1_.x)/2);
  else if (type == CQIllustratorRectShape::TYPE_RY)
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

    const CMatrix2D &m = getFlatMatrix();

    CPoint2D p1 = m*CPoint2D(p1_.x, p1_.y);
    CPoint2D p2 = m*CPoint2D(p2_.x, p1_.y);
    CPoint2D p3 = m*CPoint2D(p2_.x, p2_.y);
    CPoint2D p4 = m*CPoint2D(p1_.x, p2_.y);

    bbox_ += p1;
    bbox_ += p2;
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

  checkoutShape(CQIllustratorData::CHANGE_GEOMETRY);

  const CMatrix2D &m = getFlatMatrix();

  CMatrix2D im = m.inverse();

  p1_ = im*(m*p1_ + d);
  p2_ = im*(m*p2_ + d);

  invalidateBBox();

  checkinShape(CQIllustratorData::CHANGE_GEOMETRY);
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
    drawer->pathArc   (CPoint2D(p2.x - rx_, p2.y + ry_), rx_, ry_, 3*M_PI/2, 2*M_PI      );
//  drawer->pathLineTo(CPoint2D(p3.x      , p3.y - ry_));
    drawer->pathArc   (CPoint2D(p3.x - rx_, p3.y - ry_), rx_, ry_, 0        , M_PI/2 );
//  drawer->pathLineTo(CPoint2D(p4.x + rx_, p4.y      ));
    drawer->pathArc   (CPoint2D(p4.x + rx_, p4.y - ry_), rx_, ry_, M_PI/2 , M_PI     );
//  drawer->pathLineTo(CPoint2D(p1.x      , p1.y + ry_));
    drawer->pathArc   (CPoint2D(p1.x + rx_, p1.y + ry_), rx_, ry_, M_PI     , 3*M_PI/2);
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
 CQIllustratorShapeControlPoint((uint) type, p), type_(type)
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

  if      (type_ == CQIllustratorRectShape::TYPE_LL)
    return rect->getLL();
  else if (type_ == CQIllustratorRectShape::TYPE_UR)
    return rect->getUR();
  else if (type_ == CQIllustratorRectShape::TYPE_RX)
    return rect->getRXPoint();
  else if (type_ == CQIllustratorRectShape::TYPE_RY)
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

  if      (type_ == CQIllustratorRectShape::TYPE_LL)
    rect->setLL(point);
  else if (type_ == CQIllustratorRectShape::TYPE_UR)
    rect->setUR(point);
  else if (type_ == CQIllustratorRectShape::TYPE_RX)
    rect->setRXPoint(point);
  else if (type_ == CQIllustratorRectShape::TYPE_RY)
    rect->setRYPoint(point);
  else
    assert(false);
}

//--------------

CQIllustratorEllipseShape::
CQIllustratorEllipseShape(const CPoint2D &p1, const CPoint2D &p2,
                          double angle1, double angle2, CEllipseConnectType connectType) :
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
  if (type == CONTROL_GEOMETRY) {
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
      new CQIllustratorEllipseShapeControlPoint(TYPE_RX, pxr);
    CQIllustratorEllipseShapeControlPoint *cyr =
      new CQIllustratorEllipseShapeControlPoint(TYPE_RY, pyr);
    CQIllustratorEllipseShapeControlPoint *ca1 =
      new CQIllustratorEllipseShapeControlPoint(TYPE_A1, pa1);
    CQIllustratorEllipseShapeControlPoint *ca2 =
      new CQIllustratorEllipseShapeControlPoint(TYPE_A2, pa2);

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

  if      (type == CQIllustratorEllipseShape::TYPE_RX) {
    double rx = std::max(0.0, p.x);

    p1_.x = c.x - rx/2.0;
    p2_.x = c.x + rx/2.0;
  }
  else if (type == CQIllustratorEllipseShape::TYPE_RY) {
    double ry = std::max(0.0, p.y);

    p1_.y = c.y - ry/2.0;
    p2_.y = c.y + ry/2.0;
  }
  else if (type == CQIllustratorEllipseShape::TYPE_A1) {
    setAngle1(CMathGen::RadToDeg(atan2(p.y, p.x)));
  }
  else if (type == CQIllustratorEllipseShape::TYPE_A2) {
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

    const CMatrix2D &m = getFlatMatrix();

    CPoint2D p1 = m*CPoint2D(p1_.x, p1_.y);
    CPoint2D p2 = m*CPoint2D(p2_.x, p1_.y);
    CPoint2D p3 = m*CPoint2D(p2_.x, p2_.y);
    CPoint2D p4 = m*CPoint2D(p1_.x, p2_.y);

    bbox_ += p1;
    bbox_ += p2;
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

  if (connectType_ == CELLIPSE_CONNECT_LINE) {
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

  checkoutShape(CQIllustratorData::CHANGE_GEOMETRY);

  const CMatrix2D &m = getFlatMatrix();

  CMatrix2D im = m.inverse();

  p1_ = im*(m*p1_ + d);
  p2_ = im*(m*p2_ + d);

  invalidateBBox();

  checkinShape(CQIllustratorData::CHANGE_GEOMETRY);
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

  if (connectType_ == CELLIPSE_CONNECT_LINE) {
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

  if      (type_ == CQIllustratorEllipseShape::TYPE_RX)
    return ellipse->getRXPoint();
  else if (type_ == CQIllustratorEllipseShape::TYPE_RY)
    return ellipse->getRYPoint();
  else if (type_ == CQIllustratorEllipseShape::TYPE_A1)
    return ellipse->getAngle1Point();
  else if (type_ == CQIllustratorEllipseShape::TYPE_A2)
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

  if      (type_ == CQIllustratorEllipseShape::TYPE_RX)
    ellipse->setRXPoint(point);
  else if (type_ == CQIllustratorEllipseShape::TYPE_RY)
    ellipse->setRYPoint(point);
  else if (type_ == CQIllustratorEllipseShape::TYPE_A1)
    ellipse->setAngle1Point(point);
  else if (type_ == CQIllustratorEllipseShape::TYPE_A2)
    ellipse->setAngle2Point(point);
  else
    assert(false);
}

//--------------

CQIllustratorNPolyShape::
CQIllustratorNPolyShape(const CPoint2D &c, uint n, double r, double a) :
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
  if (type == CONTROL_GEOMETRY) {
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
setNum(uint n)
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

    const CMatrix2D &m = getFlatMatrix();

    double da = 2*M_PI/n_;

    for (uint i = 0; i < n_; ++i) {
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

  for (uint i = 0; i < n_; ++i) {
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

  for (uint i = 0; i < n_; ++i) {
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

  checkoutShape(CQIllustratorData::CHANGE_GEOMETRY);

  const CMatrix2D &m = getFlatMatrix();

  CMatrix2D im = m.inverse();

  c_ = im*(m*c_ + d);

  invalidateBBox();

  checkinShape(CQIllustratorData::CHANGE_GEOMETRY);
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

  for (uint i = 0; i < n_; ++i) {
    double a = a_ + i*da;

    double c = cos(a), s = sin(a);

    p[i] = CPoint2D(c_.x + r_*c, c_.y + r_*s);
  }

  drawer->pathInit();

  drawer->pathMoveTo(p[0]);

  for (uint i = 1; i < n_; ++i)
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

  for (uint i = 0; i < n_; ++i) {
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

//--------------

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
  if (type == CONTROL_GEOMETRY) {
    CPoint2D pc = getCenterPoint();
    CPoint2D p1 = getInnerPoint();
    CPoint2D p2 = getOuterPoint();

    CQIllustratorStarShapeControlPoint *c  =
      new CQIllustratorStarShapeControlPoint(TYPE_CENTER, pc);
    CQIllustratorStarShapeControlPoint *ri =
      new CQIllustratorStarShapeControlPoint(TYPE_IR    , p1);
    CQIllustratorStarShapeControlPoint *ro =
      new CQIllustratorStarShapeControlPoint(TYPE_OR    , p2);

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

  if      (type == CQIllustratorStarShape::TYPE_CENTER)
    setCenterPoint(p);
  else if (type == CQIllustratorStarShape::TYPE_IR)
    setInnerPoint(p);
  else if (type == CQIllustratorStarShape::TYPE_OR)
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

    const CMatrix2D &m = getFlatMatrix();

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

  checkoutShape(CQIllustratorData::CHANGE_GEOMETRY);

  const CMatrix2D &m = getFlatMatrix();

  CMatrix2D im = m.inverse();

  c_ = im*(m*c_ + d);

  invalidateBBox();

  checkinShape(CQIllustratorData::CHANGE_GEOMETRY);
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

  if      (type_ == CQIllustratorStarShape::TYPE_CENTER)
    return star->getCenterPoint();
  else if (type_ == CQIllustratorStarShape::TYPE_IR)
    return star->getInnerPoint();
  else if (type_ == CQIllustratorStarShape::TYPE_OR)
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

  if      (type_ == CQIllustratorStarShape::TYPE_CENTER)
    star->setCenterPoint(point);
  else if (type_ == CQIllustratorStarShape::TYPE_IR)
    star->setInnerPoint(point);
  else if (type_ == CQIllustratorStarShape::TYPE_OR)
    star->setOuterPoint(point);
  else
    assert(false);
}

//--------------

CQIllustratorPolygonShape::
CQIllustratorPolygonShape(const PointList &points) :
 CQIllustratorShape(), points_(points)
{
}

CQIllustratorPolygonShape::
CQIllustratorPolygonShape(const CQIllustratorPolygonShape &polygon) :
 CQIllustratorShape(polygon), points_(polygon.points_)
{
}

CQIllustratorPolygonShape *
CQIllustratorPolygonShape::
dup() const
{
  return new CQIllustratorPolygonShape(*this);
}

CPoint2D
CQIllustratorPolygonShape::
getPoint(uint i) const
{
  assert(i < points_.size());

  const CMatrix2D &m = getFlatMatrix();

  return m*points_[i];
}

void
CQIllustratorPolygonShape::
setPoint(uint i, const CPoint2D &p)
{
  assert(i < points_.size());

  const CMatrix2D &m = getFlatMatrix();

  CMatrix2D im = m.inverse();

  points_[i] = im*p;
}

double
CQIllustratorPolygonShape::
distance(const CPoint2D &p) const
{
  double dist = 1E50;

  const CMatrix2D &m = getFlatMatrix();

  uint num_points = points_.size();

  for (uint i = 0; i < num_points; ++i) {
    CPoint2D p1 = m*points_[i];

    dist = std::min(fabs(p1.x - p.x), fabs(p1.y - p.y));
  }

  return dist;
}

bool
CQIllustratorPolygonShape::
inside(const CPoint2D &p) const
{
  PointList points;

  const CMatrix2D &m = getFlatMatrix();

  uint num_points = points_.size();

  points.resize(num_points);

  for (uint i = 0; i < num_points; ++i)
    points[i] = m*points_[i];

  return CMathGeom2D::PointInsideEvenOdd(p, &points[0], num_points);
}

void
CQIllustratorPolygonShape::
getControlPoints(ControlPointList &points, ControlType type) const
{
  if (type == CONTROL_GEOMETRY) {
    const CMatrix2D &m = getFlatMatrix();

    uint num_points = points_.size();

    for (uint i = 0; i < num_points; ++i) {
      CPoint2D p1 = m*points_[i];

      points.push_back(new CQIllustratorPolygonShapeControlPoint(i, p1));
    }
  }
  else
    CQIllustratorShape::getControlPoints(points, type);
}

void
CQIllustratorPolygonShape::
setControlPoint(const CQIllustratorShapeControlPoint *point)
{
  const CQIllustratorPolygonShapeControlPoint *ppoint =
    dynamic_cast<const CQIllustratorPolygonShapeControlPoint *>(point);
  if (! ppoint) assert(false);

  const CMatrix2D &m = getFlatMatrix();

  CMatrix2D im = m.inverse();

  points_[ppoint->getInd()] = im*point->getPoint(this);

  invalidateBBox();
}

bool
CQIllustratorPolygonShape::
addPoint(const CQIllustratorShapeControlPoint *before_point, const CPoint2D &p)
{
  points_.push_back(p);

  if (before_point != NULL) {
    const CQIllustratorPolygonShapeControlPoint *before_point1 =
      dynamic_cast<const CQIllustratorPolygonShapeControlPoint *>(before_point);

    int num_points = points_.size();

    if (before_point1->getInd() < 0 || before_point1->getInd() >= int(num_points)) return false;

    for (int i = num_points - 1; i >= before_point1->getInd(); --i)
      points_[i + 1] = points_[i];

    points_[before_point1->getInd()] = p;
  }

  invalidateBBox();

  return true;
}

bool
CQIllustratorPolygonShape::
doRemovePoint(const CQIllustratorShapeControlPoint *point)
{
  const CQIllustratorPolygonShapeControlPoint *ppoint =
    dynamic_cast<const CQIllustratorPolygonShapeControlPoint *>(point);
  if (! ppoint) assert(false);

  uint num_points = points_.size();

  if (ppoint->getInd() < 0 || ppoint->getInd() >= int(num_points)) return false;

  for (uint i = ppoint->getInd() + 1; i < num_points; ++i)
    points_[i - 1] = points_[i];

  points_.pop_back();

  return true;
}

void
CQIllustratorPolygonShape::
updateBBox() const
{
  if (! bbox_valid_) {
    bbox_.reset();

    const CMatrix2D &m = getFlatMatrix();

    uint num_points = points_.size();

    for (uint i = 0; i < num_points; ++i) {
      CPoint2D p = m*points_[i];

      bbox_ += p;
    }

    bbox_valid_ = true;
  }
}

void
CQIllustratorPolygonShape::
setPoints(const CPolygon2D &polygon)
{
  const CPolygon2D::PointList &points = polygon.getPoints();

  uint num_points = points.size();

  for (uint i = 0; i < num_points; ++i)
    addPoint(points[i]);
}

void
CQIllustratorPolygonShape::
addPoint(const CPoint2D &p)
{
  const CMatrix2D &m = getFlatMatrix();

  CMatrix2D im = m.inverse();

  points_.push_back(im*p);

  invalidateBBox();
}

bool
CQIllustratorPolygonShape::
getPolygon(CPolygon2D &polygon) const
{
  const CMatrix2D &m = getFlatMatrix();

  uint num_points = points_.size();

  for (uint i = 0; i < num_points; ++i)
    polygon.addPoint(m*points_[i]);

  return true;
}

bool
CQIllustratorPolygonShape::
getPath(CPathShapePartList &parts) const
{
  const CMatrix2D &m = getFlatMatrix();

  uint num_points = points_.size();

  for (uint i = 0; i < num_points; ++i) {
    CPoint2D p = m*points_[i];

    if (i == 0)
      parts.addPart(new CPathShapeMoveTo(p));
    else
      parts.addPart(new CPathShapeLineTo(p));
  }

  parts.addPart(new CPathShapeClose());

  return true;
}

void
CQIllustratorPolygonShape::
moveBy(const CPoint2D &d)
{
  assert(! getFixed());

  checkoutShape(CQIllustratorData::CHANGE_GEOMETRY);

  const CMatrix2D &m = getFlatMatrix();

  CMatrix2D im = m.inverse();

  uint num_points = points_.size();

  for (uint i = 0; i < num_points; ++i)
    points_[i] = im*(m*points_[i] + d);

  invalidateBBox();

  checkinShape(CQIllustratorData::CHANGE_GEOMETRY);
}

void
CQIllustratorPolygonShape::
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

  uint num_points = points_.size();

  for (uint i = 0; i < num_points; ++i) {
    CPoint2D p = m*points_[i];

    p.x = x + (p.x - x)*xs;
    p.y = y + (p.y - y)*ys;

    points_[i] = im*p;
  }

  invalidateBBox();
}

bool
CQIllustratorPolygonShape::
flip(bool x_axis)
{
  const CMatrix2D &m = getFlatMatrix();

  CMatrix2D im = m.inverse();

  CPoint2D rc = getRotateCenter();

  uint num_points = points_.size();

  for (uint i = 0; i < num_points; ++i) {
    CPoint2D p = m*points_[i];

    points_[i] = im*p.flip(rc, x_axis);
  }

  return true;
}

void
CQIllustratorPolygonShape::
drawShape(CQIllustratorShapeDrawer *drawer) const
{
  if (points_.empty()) return;

  const CMatrix2D &m = getFlatMatrix();

  drawer->pushMatrix(m, true);

  uint num_points = points_.size();

  drawer->pathInit();

  drawer->pathMoveTo(points_[num_points - 1]);

  for (uint i = 0; i < num_points; ++i)
    drawer->pathLineTo(points_[i]);

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
CQIllustratorPolygonShape::
getGeometry()
{
  return new CQIllustratorPolygonGeometry(fixed_, rcenter_, m_, points_);
}

void
CQIllustratorPolygonShape::
doSetGeometry(const CQIllustratorShapeGeometry *geom)
{
  const CQIllustratorPolygonGeometry *geom1 =
    dynamic_cast<const CQIllustratorPolygonGeometry *>(geom);

  if (! geom1) return;

  fixed_   = geom1->fixed_;
  rcenter_ = geom1->rcenter_;
  m_       = geom1->m_;
  points_  = geom1->points_;

  invalidateBBox();
}

void
CQIllustratorPolygonShape::
saveSVG(const CQIllustratorSaveData &saveData)
{
  CFile *file = saveData.file;

  file->printf("<polygon points=\"");

  uint num_points = points_.size();

  for (uint i = 0; i < num_points; ++i) {
    CPoint2D p = points_[i];

    if (i > 0) file->printf(" ");

    file->printf("%g,%g", p.x, p.y);
  }

  file->printf("\"%s %s/>\n", getSVGMatrix().c_str(), getSVGStroke().c_str());
}

void
CQIllustratorPolygonShape::
saveCmd(const CQIllustratorSaveData & /*saveData*/)
{
}

//-----

CQIllustratorPolygonShapeControlPoint::
CQIllustratorPolygonShapeControlPoint(uint ind, const CPoint2D &p) :
 CQIllustratorShapeControlPoint(ind, p), ind_(ind)
{
}

CQIllustratorPolygonShapeControlPoint *
CQIllustratorPolygonShapeControlPoint::
dup() const
{
  return new CQIllustratorPolygonShapeControlPoint(ind_, point_);
}

CPoint2D
CQIllustratorPolygonShapeControlPoint::
getPoint(const CQIllustratorShape *shape) const
{
  const CQIllustratorPolygonShape *polygon =
    dynamic_cast<const CQIllustratorPolygonShape *>(shape);
  assert(polygon);

  return polygon->getPoint(ind_);
}

void
CQIllustratorPolygonShapeControlPoint::
setPoint(CQIllustratorShape *shape, const CPoint2D &point)
{
  CQIllustratorPolygonShape *polygon = dynamic_cast<CQIllustratorPolygonShape *>(shape);
  assert(polygon);

  polygon->setPoint(ind_, point);
}

//--------------

class CPathShapePolygonProcess : public CPathShapeProcess {
 public:
  CPathShapePolygonProcess(CPolygon2D &polygon, const CPathShape *path) :
   CPathShapeProcess(), polygon_(polygon), path_(path) {
    m_ = path_->getFlatMatrix();
  }

  void init() {
  }

  void moveTo(const CPoint2D &p) {
    polygon_.addPoint(m_*p);
  }

  void lineTo(const CPoint2D &p) {
    polygon_.addPoint(m_*p);
  }

  void curve2To(const CPoint2D &p1, const CPoint2D &p2) {
    polygon_.addPoint(m_*p1);
    polygon_.addPoint(m_*p2);
  }

  void curve3To(const CPoint2D &p1, const CPoint2D &p2, const CPoint2D &p3) {
    polygon_.addPoint(m_*p1);
    polygon_.addPoint(m_*p2);
    polygon_.addPoint(m_*p3);
  }

  void arcTo(const CPoint2D &/*c*/, double /*xr*/, double /*yr*/,
             double /*angle1*/, double /*angle2*/) {
  }

  void close() {
  }

  void term() {
  }

 private:
  CPolygon2D                   &polygon_;
  const CPathShape *path_;
  CMatrix2D                     m_;
};

CPathShape::
CPathShape(const CPathShapePartList &parts) :
 CQIllustratorShape(), parts_(parts), group_(0)
{
}

CPathShape::
CPathShape(const CPathShape &path) :
 CQIllustratorShape(path), parts_(path.parts_)
{
}

CPathShape *
CPathShape::
dup() const
{
  return new CPathShape(*this);
}

bool
CPathShape::
getPolygon(CPolygon2D &polygon) const
{
  CPathShapePolygonProcess process(polygon, this);

  process.init();

  CPathShape *th = const_cast<CPathShape *>(this);

  th->processPath(&process);

  process.term();

  return true;
}

bool
CPathShape::
getPath(CPathShapePartList &parts) const
{
  parts = parts_;

  return true;
}

void
CPathShape::
moveBy(const CPoint2D &d)
{
  const CMatrix2D &m = getFlatMatrix();

  CMatrix2D im = m.inverse();

  uint num_parts = parts_.size();

  for (uint i = 0; i < num_parts; ++i) {
    CPathShapePart *part = parts_[i];

    CPathPartType type = part->getType();

    CPoint2D p1;

    if      (type == CPATH_PART_TYPE_MOVE_TO) {
      CPathShapeMoveTo *moveToPart = dynamic_cast<CPathShapeMoveTo *>(part);

      moveToPart->setPoint(im*(m*moveToPart->getPoint() + d));
    }
    else if (type == CPATH_PART_TYPE_LINE_TO) {
      CPathShapeLineTo *lineToPart = dynamic_cast<CPathShapeLineTo *>(part);

      lineToPart->setPoint(im*(m*lineToPart->getPoint() + d));
    }
    else if (type == CPATH_PART_TYPE_CURVE2_TO) {
      CPathShapeCurve2To *curveToPart = dynamic_cast<CPathShapeCurve2To *>(part);

      curveToPart->setPoint1(im*(m*curveToPart->getPoint1() + d));
      curveToPart->setPoint2(im*(m*curveToPart->getPoint2() + d));
    }
    else if (type == CPATH_PART_TYPE_CURVE3_TO) {
      CPathShapeCurve3To *curveToPart = dynamic_cast<CPathShapeCurve3To *>(part);

      curveToPart->setPoint1(im*(m*curveToPart->getPoint1() + d));
      curveToPart->setPoint2(im*(m*curveToPart->getPoint2() + d));
      curveToPart->setPoint3(im*(m*curveToPart->getPoint3() + d));
    }
    else if (type == CPATH_PART_TYPE_ARC) {
      CPathShapeArc *arcToPart = dynamic_cast<CPathShapeArc *>(part);

      arcToPart->setCenter(im*(m*arcToPart->getCenter() + d));
    }
    else
      continue;
  }
}

void
CPathShape::
startGroup()
{
  ++group_;
}

void
CPathShape::
endGroup()
{
  --group_;

  assert(group_ >= 0);

  if (group_ == 0)
    invalidateBBox();
}

bool
CPathShape::
empty() const
{
  return parts_.empty();
}

double
CPathShape::
distance(const CPoint2D &p) const
{
  const CMatrix2D &m = getFlatMatrix();

  double dist = 1E50;

  uint num_parts = parts_.size();

  for (uint i = 0; i < num_parts; ++i) {
    const CPathShapePart *part = parts_[i];

    CPathPartType type = part->getType();

    CPoint2D p1;

    if      (type == CPATH_PART_TYPE_MOVE_TO) {
      const CPathShapeMoveTo *moveToPart = dynamic_cast<const CPathShapeMoveTo *>(part);

      p1 = m*moveToPart->getPoint();
    }
    else if (type == CPATH_PART_TYPE_LINE_TO) {
      const CPathShapeLineTo *lineToPart = dynamic_cast<const CPathShapeLineTo *>(part);

      p1 = m*lineToPart->getPoint();
    }
    else if (type == CPATH_PART_TYPE_CURVE2_TO) {
      const CPathShapeCurve2To *curveToPart = dynamic_cast<const CPathShapeCurve2To *>(part);

      p1 = m*curveToPart->getPoint2();
    }
    else if (type == CPATH_PART_TYPE_CURVE3_TO) {
      const CPathShapeCurve3To *curveToPart = dynamic_cast<const CPathShapeCurve3To *>(part);

      p1 = m*curveToPart->getPoint3();
    }
    else if (type == CPATH_PART_TYPE_ARC) {
      const CPathShapeArc *arcToPart = dynamic_cast<const CPathShapeArc *>(part);

      p1 = m*arcToPart->getCenter();
    }
    else
      continue;

    dist = std::min(fabs(p1.x - p.x), fabs(p1.y - p.y));
  }

  return dist;
}

bool
CPathShape::
inside(const CPoint2D &p) const
{
  const CMatrix2D &m = getFlatMatrix();

  std::vector<CPoint2D> points;

  uint num_parts = parts_.size();

  for (uint i = 0; i < num_parts; ++i) {
    const CPathShapePart *part = parts_[i];

    CPathPartType type = part->getType();

    CPoint2D p1;

    if      (type == CPATH_PART_TYPE_MOVE_TO) {
      const CPathShapeMoveTo *moveToPart = dynamic_cast<const CPathShapeMoveTo *>(part);

      p1 = m*moveToPart->getPoint();
    }
    else if (type == CPATH_PART_TYPE_LINE_TO) {
      const CPathShapeLineTo *lineToPart = dynamic_cast<const CPathShapeLineTo *>(part);

      p1 = m*lineToPart->getPoint();
    }
    else if (type == CPATH_PART_TYPE_CURVE2_TO) {
      const CPathShapeCurve2To *curveToPart = dynamic_cast<const CPathShapeCurve2To *>(part);

      p1 = m*curveToPart->getPoint2();
    }
    else if (type == CPATH_PART_TYPE_CURVE3_TO) {
      const CPathShapeCurve3To *curveToPart = dynamic_cast<const CPathShapeCurve3To *>(part);

      p1 = m*curveToPart->getPoint3();
    }
    else if (type == CPATH_PART_TYPE_ARC) {
      // TODO: use arc end points
      const CPathShapeArc *arcToPart = dynamic_cast<const CPathShapeArc *>(part);

      p1 = m*arcToPart->getCenter();
    }
    else
      continue;

    points.push_back(p1);
  }

  return CMathGeom2D::PointInsideEvenOdd(p, &points[0], points.size());
}

void
CPathShape::
getControlLines(ControlLineList &clines) const
{
  std::vector<CLine2D> lines;

  getLines(lines);

  uint num_lines = lines.size();

  for (uint i = 0; i < num_lines; ++i)
    clines.push_back(new CPathShapeControlLine(i, lines[i]));
}

void
CPathShape::
getLines(std::vector<CLine2D> &lines) const
{
  CPoint2D p0, p1, p2;

  const CMatrix2D &m = getFlatMatrix();

  uint num_parts = parts_.size();

  for (uint i = 0; i < num_parts; ++i) {
    const CPathShapePart *part = parts_[i];

    CPathPartType type = part->getType();

    if      (type == CPATH_PART_TYPE_MOVE_TO) {
      const CPathShapeMoveTo *moveToPart = dynamic_cast<const CPathShapeMoveTo *>(part);

      p1 = m*moveToPart->getPoint();

      p0 = p1;
    }
    else if (type == CPATH_PART_TYPE_LINE_TO) {
      const CPathShapeLineTo *lineToPart = dynamic_cast<const CPathShapeLineTo *>(part);

      p2 = m*lineToPart->getPoint();

      lines.push_back(CLine2D(p1, p2));

      p1 = p2;
    }
    else if (type == CPATH_PART_TYPE_CURVE2_TO) {
      const CPathShapeCurve2To *curveToPart = dynamic_cast<const CPathShapeCurve2To *>(part);

      p2 = m*curveToPart->getPoint2();

      lines.push_back(CLine2D(p1, p2));

      p1 = p2;
    }
    else if (type == CPATH_PART_TYPE_CURVE3_TO) {
      const CPathShapeCurve3To *curveToPart = dynamic_cast<const CPathShapeCurve3To *>(part);

      p2 = m*curveToPart->getPoint3();

      lines.push_back(CLine2D(p1, p2));
    }
    else if (type == CPATH_PART_TYPE_ARC) {
      const CPathShapeArc *arcToPart = dynamic_cast<const CPathShapeArc *>(part);

      CPoint2D pa1, pa2;

      arcToPart->getEndPoints(pa1, pa2);

      p2 = m*pa2;

      lines.push_back(CLine2D(p1, p2));

      p1 = p2;
    }
    else if (type == CPATH_PART_TYPE_CLOSE) {
      lines.push_back(CLine2D(p1, p0));

      p1 = p0;
    }
    else
      continue;
  }
}

CLine2D
CPathShape::
getLine(uint i) const
{
  std::vector<CLine2D> lines;

  getLines(lines);

  return lines[i];
}

void
CPathShape::
setLine(uint i, const CLine2D &l)
{
  std::vector<CLine2D> lines;

  getLines(lines);

  int sline_num = 0;
  int eline_num = lines.size() - 1;

  const CMatrix2D &m = getFlatMatrix();

  CMatrix2D im = m.inverse();

  uint num_parts = parts_.size();

  for (uint j = 0; j < num_parts; ++j) {
    CPathShapePart *part = parts_[j];

    CPathPartType type = part->getType();

    if      (type == CPATH_PART_TYPE_MOVE_TO) {
      CPathShapeMoveTo *moveToPart =
        dynamic_cast<CPathShapeMoveTo *>(part);

      if      (sline_num == int(i))
        moveToPart->setPoint(im*l.start());
      else if (eline_num == int(i))
        moveToPart->setPoint(im*l.end  ());

      eline_num = sline_num++;
    }
    else if (type == CPATH_PART_TYPE_LINE_TO) {
      CPathShapeLineTo *lineToPart =
        dynamic_cast<CPathShapeLineTo *>(part);

      if      (sline_num == int(i))
        lineToPart->setPoint(im*l.start());
      else if (eline_num == int(i))
        lineToPart->setPoint(im*l.end  ());

      eline_num = sline_num++;
    }
    else if (type == CPATH_PART_TYPE_CURVE2_TO) {
      eline_num = sline_num++;
    }
    else if (type == CPATH_PART_TYPE_CURVE3_TO) {
      eline_num = sline_num++;
    }
    else if (type == CPATH_PART_TYPE_ARC) {
      eline_num = sline_num++;
    }
    else if (type == CPATH_PART_TYPE_CLOSE) {
      eline_num = sline_num++;
    }
    else
      continue;
  }
}

void
CPathShape::
setLineType(uint i, CPathPartType type)
{
  CPathShapePartList parts;

  int line_num = -1;

  uint num_parts = parts_.size();

  CPoint2D p0, p1, p2;

  for (uint j = 0; j < num_parts; ++j) {
    CPathShapePart *part = parts_[j];

    CPathPartType partType2 = part->getType();

    if      (partType2 == CPATH_PART_TYPE_MOVE_TO) {
      CPathShapeMoveTo *moveToPart =
        dynamic_cast<CPathShapeMoveTo *>(part);

      p2 = moveToPart->getPoint();
      p0 = p2;

      line_num = -1;
    }
    else if (partType2 == CPATH_PART_TYPE_LINE_TO) {
      CPathShapeLineTo *lineToPart =
        dynamic_cast<CPathShapeLineTo *>(part);

      p2 = lineToPart->getPoint();

      ++line_num;
    }
    else if (partType2 == CPATH_PART_TYPE_CURVE2_TO) {
      CPathShapeCurve2To *curve2Part =
        dynamic_cast<CPathShapeCurve2To *>(part);

      p2 = curve2Part->getPoint2();

      ++line_num;
    }
    else if (partType2 == CPATH_PART_TYPE_CURVE3_TO) {
      CPathShapeCurve3To *curve3Part =
        dynamic_cast<CPathShapeCurve3To *>(part);

      p2 = curve3Part->getPoint3();

      ++line_num;
    }
    else if (partType2 == CPATH_PART_TYPE_ARC) {
      // TODO

      ++line_num;
    }
    else if (partType2 == CPATH_PART_TYPE_CLOSE) {
      p2 = p0;

      ++line_num;
    }
    else
      continue;

    CPathShapePart *part1 = NULL;

    if (line_num == int(i)) {
      CPoint2D pp1, pp2;

      bool pp1_set = false, pp2_set = false;

      if      (partType2 == CPATH_PART_TYPE_LINE_TO) {
      }
      else if (partType2 == CPATH_PART_TYPE_CURVE2_TO) {
        CPathShapeCurve2To *curve2Part =
          dynamic_cast<CPathShapeCurve2To *>(part);

        pp2 = curve2Part->getPoint1(); pp2_set = true;
      }
      else if (partType2 == CPATH_PART_TYPE_CURVE3_TO) {
        CPathShapeCurve3To *curve3Part =
          dynamic_cast<CPathShapeCurve3To *>(part);

        pp1 = curve3Part->getPoint1(); pp1_set = true;
        pp2 = curve3Part->getPoint2(); pp2_set = true;
      }

      if      (type == CPATH_PART_TYPE_LINE_TO) {
        part1 = new CPathShapeLineTo(p2);
      }
      else if (type == CPATH_PART_TYPE_CURVE2_TO) {
        if (! pp2_set) pp2 = (p1 + p2)/2;

        part1 = new CPathShapeCurve2To(pp2, p2);
      }
      else if (type == CPATH_PART_TYPE_CURVE3_TO) {
        if (! pp1_set) pp1 = p1 +   (p2 - p1)/3;
        if (! pp2_set) pp2 = p1 + 2*(p2 + p1)/3;

        part1 = new CPathShapeCurve3To(pp1, pp2, p2);
      }
      else
        part1 = part->dup();

      parts.addPart(part1);

      if (partType2 == CPATH_PART_TYPE_CLOSE) {
        part1 = part->dup();

        parts.addPart(part1);
      }
    }
    else {
      part1 = part->dup();

      parts.addPart(part1);
    }

    p1 = p2;
  }

  parts_ = parts;
}

void
CPathShape::
getControlPoints(ControlPointList &points, ControlType type) const
{
  if (type == CONTROL_GEOMETRY) {
    const CMatrix2D &m = getFlatMatrix();

    uint num_parts = parts_.size();

    for (uint i = 0; i < num_parts; ++i) {
      const CPathShapePart *part = parts_[i];

      CPathPartType type = part->getType();

      if      (type == CPATH_PART_TYPE_MOVE_TO) {
        const CPathShapeMoveTo *moveToPart = dynamic_cast<const CPathShapeMoveTo *>(part);

        CPoint2D p1 = m*moveToPart->getPoint();

        points.push_back(new CPathShapeControlPoint(i, p1));
      }
      else if (type == CPATH_PART_TYPE_LINE_TO) {
        const CPathShapeLineTo *lineToPart = dynamic_cast<const CPathShapeLineTo *>(part);

        CPoint2D p1 = m*lineToPart->getPoint();

        points.push_back(new CPathShapeControlPoint(i, p1));
      }
      else if (type == CPATH_PART_TYPE_CURVE2_TO) {
        const CPathShapeCurve2To *curveToPart = dynamic_cast<const CPathShapeCurve2To *>(part);

        CPoint2D pp1 = m*curveToPart->getPoint1();
        CPoint2D pp2 = m*curveToPart->getPoint2();

        points.push_back(new CPathShapeControlPoint(i, 1, pp1));
        points.push_back(new CPathShapeControlPoint(i, pp2));
      }
      else if (type == CPATH_PART_TYPE_CURVE3_TO) {
        const CPathShapeCurve3To *curveToPart = dynamic_cast<const CPathShapeCurve3To *>(part);

        CPoint2D pp1 = m*curveToPart->getPoint1();
        CPoint2D pp2 = m*curveToPart->getPoint2();
        CPoint2D pp3 = m*curveToPart->getPoint3();

        points.push_back(new CPathShapeControlPoint(i, 1, pp1));
        points.push_back(new CPathShapeControlPoint(i, 2, pp2));
        points.push_back(new CPathShapeControlPoint(i, pp3));
      }
      else if (type == CPATH_PART_TYPE_ARC) {
        const CPathShapeArc *arcToPart = dynamic_cast<const CPathShapeArc *>(part);

        const CPoint2D &c = arcToPart->getCenter();

        CPoint2D p1, p2;

        arcToPart->getEndPoints(p1, p2);

        CPoint2D c1  = m*c;
        CPoint2D pp1 = m*p1;
        CPoint2D pp2 = m*p2;

        points.push_back(new CPathShapeControlPoint(i, c1 ));
        points.push_back(new CPathShapeControlPoint(i, 1, pp1));
        points.push_back(new CPathShapeControlPoint(i, 2, pp2));
      }
      else if (type == CPATH_PART_TYPE_CLOSE) {
      }
    }
  }
  else
    CQIllustratorShape::getControlPoints(points, type);
}

void
CPathShape::
setControlPoint(const CQIllustratorShapeControlPoint *point)
{
  const CPathShapeControlPoint *ppoint =
    dynamic_cast<const CPathShapeControlPoint *>(point);
  assert(ppoint);

  const CMatrix2D &m = getFlatMatrix();

  uint ind  = ppoint->getInd();
  uint ind1 = ppoint->getInd1();

  CMatrix2D im = m.inverse();

  CPoint2D p = im*point->getPoint(this);

  setPoint(ind, ind1, p);
}

CPoint2D
CPathShape::
getPoint(uint ind, uint ind1) const
{
  uint num_parts = parts_.size();

  assert(ind < num_parts);

  const CPathShapePart *part = parts_[ind];

  CPathPartType type = part->getType();

  CPoint2D p;

  if      (type == CPATH_PART_TYPE_MOVE_TO) {
    assert(ind1 == 0);

    const CPathShapeMoveTo *moveToPart = dynamic_cast<const CPathShapeMoveTo *>(part);

    p = moveToPart->getPoint();
  }
  else if (type == CPATH_PART_TYPE_LINE_TO) {
    assert(ind1 == 0);

    const CPathShapeLineTo *lineToPart = dynamic_cast<const CPathShapeLineTo *>(part);

    p = lineToPart->getPoint();
  }
  else if (type == CPATH_PART_TYPE_CURVE2_TO) {
    assert(ind1 < 2);

    const CPathShapeCurve2To *curveToPart = dynamic_cast<const CPathShapeCurve2To *>(part);

    if (ind1 == 0)
      p = curveToPart->getPoint2();
    else
      p = curveToPart->getPoint1();
  }
  else if (type == CPATH_PART_TYPE_CURVE3_TO) {
    assert(ind1 < 3);

    const CPathShapeCurve3To *curveToPart = dynamic_cast<const CPathShapeCurve3To *>(part);

    if      (ind1 == 0)
      p = curveToPart->getPoint3();
    else if (ind1 == 1)
      p = curveToPart->getPoint1();
    else
      p = curveToPart->getPoint2();
  }
  else if (type == CPATH_PART_TYPE_ARC) {
    assert(ind1 < 3);

    const CPathShapeArc *arcToPart = dynamic_cast<const CPathShapeArc *>(part);

    CPoint2D p1, p2;

    arcToPart->getEndPoints(p1, p2);

    if      (ind1 == 0)
      p = arcToPart->getCenter();
    else if (ind1 == 0)
      p = p1;
    else
      p = p2;
  }
  else
    assert(false);

  const CMatrix2D &m = getFlatMatrix();

  return m*p;
}

void
CPathShape::
setPoint(uint ind, uint ind1, const CPoint2D &p)
{
  uint num_parts = parts_.size();

  assert(ind < num_parts);

  CPathShapePart *part = parts_[ind];

  CPathPartType type = part->getType();

  if      (type == CPATH_PART_TYPE_MOVE_TO) {
    assert(ind1 == 0);

    CPathShapeMoveTo *moveToPart = dynamic_cast<CPathShapeMoveTo *>(part);

    moveToPart->setPoint(p);
  }
  else if (type == CPATH_PART_TYPE_LINE_TO) {
    assert(ind1 == 0);

    CPathShapeLineTo *lineToPart = dynamic_cast<CPathShapeLineTo *>(part);

    lineToPart->setPoint(p);
  }
  else if (type == CPATH_PART_TYPE_CURVE2_TO) {
    assert(ind1 < 2);

    CPathShapeCurve2To *curveToPart = dynamic_cast<CPathShapeCurve2To *>(part);

    if (ind1 == 0)
      curveToPart->setPoint2(p);
    else
      curveToPart->setPoint1(p);
  }
  else if (type == CPATH_PART_TYPE_CURVE3_TO) {
    assert(ind1 < 3);

    CPathShapeCurve3To *curveToPart = dynamic_cast<CPathShapeCurve3To *>(part);

    if      (ind1 == 0)
      curveToPart->setPoint3(p);
    else if (ind1 == 1)
      curveToPart->setPoint1(p);
    else
      curveToPart->setPoint2(p);
  }
  else if (type == CPATH_PART_TYPE_ARC) {
    assert(ind1 < 3);

    CPathShapeArc *arcToPart = dynamic_cast<CPathShapeArc *>(part);

    if (ind1 == 0)
      arcToPart->setCenter(p);
  }
  else
    return;

  invalidateBBox();
}

void
CPathShape::
setCornerPoint(uint ind)
{
  uint num_parts = parts_.size();

  assert(ind < num_parts);

  CMatrix2D save_m = m_;

  m_.setIdentity();

  CPoint2D p2 = getPoint(ind, 0);

  delete parts_[ind];

  parts_.setPart(ind, new CPathShapeLineTo(p2));

  m_ = save_m;
}

void
CPathShape::
setCurvePoint(uint ind)
{
  uint num_parts = parts_.size();

  assert(ind < num_parts);

  CMatrix2D save_m = m_;

  m_.setIdentity();

  CPoint2D p1(0,0), p3(0, 0);

  CPoint2D p2 = getPoint(ind, 0);

  if (ind > 0            ) p1 = getPoint(ind - 1, 0);
  if (ind < num_parts - 1) p3 = getPoint(ind + 1, 0);

  delete parts_[ind];

  parts_.setPart(ind, new CPathShapeCurve2To(p2, (p1 + p3)/2));

  m_ = save_m;
}

bool
CPathShape::
doRemovePoint(const CQIllustratorShapeControlPoint *point)
{
  const CPathShapeControlPoint *ppoint =
      dynamic_cast<const CPathShapeControlPoint *>(point);
  assert(ppoint);

  uint ind = ppoint->getInd();

  parts_.removePart(ind);

  return true;
}

void
CPathShape::
updateBBox() const
{
  if (! bbox_valid_) {
    const CMatrix2D &m = getFlatMatrix();

    bbox_.reset();

    uint num_parts = parts_.size();

    for (uint i = 0; i < num_parts; ++i) {
      const CPathShapePart *part = parts_[i];

      CPathPartType type = part->getType();

      if      (type == CPATH_PART_TYPE_MOVE_TO) {
        const CPathShapeMoveTo *moveToPart = dynamic_cast<const CPathShapeMoveTo *>(part);

        CPoint2D p1 = m*moveToPart->getPoint();

        bbox_ += p1;
      }
      else if (type == CPATH_PART_TYPE_LINE_TO) {
        const CPathShapeLineTo *lineToPart = dynamic_cast<const CPathShapeLineTo *>(part);

        CPoint2D p1 = m*lineToPart->getPoint();

        bbox_ += p1;
      }
      else if (type == CPATH_PART_TYPE_CURVE2_TO) {
        const CPathShapeCurve2To *curveToPart = dynamic_cast<const CPathShapeCurve2To *>(part);

        CPoint2D pp1 = m*curveToPart->getPoint1();
        CPoint2D pp2 = m*curveToPart->getPoint2();

        bbox_ += pp1;
        bbox_ += pp2;
      }
      else if (type == CPATH_PART_TYPE_CURVE3_TO) {
        const CPathShapeCurve3To *curveToPart = dynamic_cast<const CPathShapeCurve3To *>(part);

        CPoint2D pp1 = m*curveToPart->getPoint1();
        CPoint2D pp2 = m*curveToPart->getPoint2();
        CPoint2D pp3 = m*curveToPart->getPoint3();

        bbox_ += pp1;
        bbox_ += pp2;
        bbox_ += pp3;
      }
      else if (type == CPATH_PART_TYPE_ARC) {
        const CPathShapeArc *arcToPart = dynamic_cast<const CPathShapeArc *>(part);

        CPoint2D c = m*arcToPart->getCenter();

        double xr = arcToPart->getRadiusX();
        double yr = arcToPart->getRadiusY();

        double a1 = arcToPart->getAngle1();
        double a2 = arcToPart->getAngle2();
        double a3 = (a1 + a2)/2;

        double x1, y1, x2, y2, x3, y3;

        CMathGeom2D::EllipsePointAtAngle(c.x, c.y, xr, yr, a1, &x1, &y1);
        CMathGeom2D::EllipsePointAtAngle(c.x, c.y, xr, yr, a2, &x2, &y2);
        CMathGeom2D::EllipsePointAtAngle(c.x, c.y, xr, yr, a3, &x3, &y3);

        bbox_ += CPoint2D(x1, y1);
        bbox_ += CPoint2D(x2, y2);
        bbox_ += CPoint2D(x3, y3);
      }
      else
        continue;
    }

    double sopacity = getStroke().getOpacity();

    if (sopacity > 0.0) {
      double lw2 = getStrokeWidth()/2;

      bbox_.expand(-lw2, -lw2, lw2, lw2);
    }

    bbox_valid_ = true;
  }
}

void
CPathShape::
setParts(const CPathShapePartList &parts)
{
  parts_ = parts;

  invalidateBBox();
}

void
CPathShape::
addMoveTo(const CPoint2D &p)
{
  parts_.addPart(new CPathShapeMoveTo(p));

  if (group_ == 0) invalidateBBox();
}

void
CPathShape::
addLineTo(const CPoint2D &p)
{
  parts_.addPart(new CPathShapeLineTo(p));

  if (group_ == 0) invalidateBBox();
}

void
CPathShape::
addLineTo(uint ind, const CPoint2D &p)
{
  parts_.addPart(ind, new CPathShapeLineTo(p));

  if (group_ == 0) invalidateBBox();
}

void
CPathShape::
addCurveTo(const CPoint2D &p1, const CPoint2D &p2)
{
  parts_.addPart(new CPathShapeCurve2To(p1, p2));

  if (group_ == 0) invalidateBBox();
}

void
CPathShape::
addCurveTo(const CPoint2D &p1, const CPoint2D &p2, const CPoint2D &p3)
{
  parts_.addPart(new CPathShapeCurve3To(p1, p2, p3));

  if (group_ == 0) invalidateBBox();
}

void
CPathShape::
addArcTo(const CPoint2D &c, double xr, double yr, double angle1, double angle2)
{
  parts_.addPart(new CPathShapeArc(c, xr, yr, angle1, angle2));

  if (group_ == 0) invalidateBBox();
}

void
CPathShape::
addClose()
{
  parts_.addPart(new CPathShapeClose());

  if (group_ == 0) invalidateBBox();
}

bool
CPathShape::
flip(bool x_axis)
{
  uint num_parts = parts_.size();

  if (num_parts == 0) return true;

  const CMatrix2D &m = getFlatMatrix();

  CMatrix2D im = m.inverse();

  CPoint2D rc = getRotateCenter();

  for (uint i = 0; i < num_parts; ++i) {
    CPathShapePart *part = parts_[i];

    CPathPartType type = part->getType();

    if      (type == CPATH_PART_TYPE_MOVE_TO) {
      CPathShapeMoveTo *moveToPart = dynamic_cast<CPathShapeMoveTo *>(part);

      CPoint2D p = m*moveToPart->getPoint();

      moveToPart->setPoint(im*p.flip(rc, x_axis));
    }
    else if (type == CPATH_PART_TYPE_LINE_TO) {
      CPathShapeLineTo *lineToPart = dynamic_cast<CPathShapeLineTo *>(part);

      CPoint2D p = m*lineToPart->getPoint();

      lineToPart->setPoint(im*p.flip(rc, x_axis));
    }
    else if (type == CPATH_PART_TYPE_CURVE2_TO) {
      CPathShapeCurve2To *curveToPart = dynamic_cast<CPathShapeCurve2To *>(part);

      CPoint2D p1 = m*curveToPart->getPoint1();
      CPoint2D p2 = m*curveToPart->getPoint2();

      curveToPart->setPoint1(im*p1.flip(rc, x_axis));
      curveToPart->setPoint2(im*p2.flip(rc, x_axis));
    }
    else if (type == CPATH_PART_TYPE_CURVE3_TO) {
      CPathShapeCurve3To *curveToPart = dynamic_cast<CPathShapeCurve3To *>(part);

      CPoint2D p1 = m*curveToPart->getPoint1();
      CPoint2D p2 = m*curveToPart->getPoint2();
      CPoint2D p3 = m*curveToPart->getPoint3();

      curveToPart->setPoint1(im*p1.flip(rc, x_axis));
      curveToPart->setPoint2(im*p2.flip(rc, x_axis));
      curveToPart->setPoint3(im*p3.flip(rc, x_axis));
    }
    else if (type == CPATH_PART_TYPE_ARC) {
      CPathShapeArc *arcToPart = dynamic_cast<CPathShapeArc *>(part);

      CPoint2D c      = m*arcToPart->getCenter();
    //double   xr     = arcToPart->getRadiusX();
    //double   yr     = arcToPart->getRadiusY();
      double   angle1 = arcToPart->getAngle1();
      double   angle2 = arcToPart->getAngle2();

      arcToPart->setCenter(im*c.flip(rc, x_axis));
      arcToPart->setAngle1(360 - angle1);
      arcToPart->setAngle2(360 - angle2);
    }
    else if (type == CPATH_PART_TYPE_CLOSE) {
    }
  }

  return true;
}

void
CPathShape::
drawShape(CQIllustratorShapeDrawer *drawer) const
{
  uint num_parts = parts_.size();

  if (num_parts == 0) return;

  const CMatrix2D &m = getFlatMatrix();

  drawer->pathInit();

  for (uint i = 0; i < num_parts; ++i) {
    const CPathShapePart *part = parts_[i];

    CPathPartType type = part->getType();

    if      (type == CPATH_PART_TYPE_MOVE_TO) {
      const CPathShapeMoveTo *moveToPart = dynamic_cast<const CPathShapeMoveTo *>(part);

      CPoint2D p1 = m*moveToPart->getPoint();

      drawer->pathMoveTo(p1);
    }
    else if (type == CPATH_PART_TYPE_LINE_TO) {
      const CPathShapeLineTo *lineToPart = dynamic_cast<const CPathShapeLineTo *>(part);

      CPoint2D p1 = m*lineToPart->getPoint();

      drawer->pathLineTo(p1);
    }
    else if (type == CPATH_PART_TYPE_CURVE2_TO) {
      const CPathShapeCurve2To *curveToPart = dynamic_cast<const CPathShapeCurve2To *>(part);

      CPoint2D p11 = m*curveToPart->getPoint1();
      CPoint2D p22 = m*curveToPart->getPoint2();

      drawer->pathBezierTo(p11, p22);
    }
    else if (type == CPATH_PART_TYPE_CURVE3_TO) {
      const CPathShapeCurve3To *curveToPart = dynamic_cast<const CPathShapeCurve3To *>(part);

      const CPoint2D &p1 = curveToPart->getPoint1();
      const CPoint2D &p2 = curveToPart->getPoint2();
      const CPoint2D &p3 = curveToPart->getPoint3();

      CPoint2D p11 = m*p1;
      CPoint2D p22 = m*p2;
      CPoint2D p33 = m*p3;

      drawer->pathBezierTo(p11, p22, p33);
    }
    else if (type == CPATH_PART_TYPE_ARC) {
      const CPathShapeArc *arcToPart = dynamic_cast<const CPathShapeArc *>(part);

      const CPoint2D &c      = arcToPart->getCenter();
      double          xr     = arcToPart->getRadiusX();
      double          yr     = arcToPart->getRadiusY();
      double          angle1 = arcToPart->getAngle1();
      double          angle2 = arcToPart->getAngle2();

      CPoint2D c1 = m*c;

      drawer->pathArc(c1, xr, yr, angle1, angle2);
    }
    else if (type == CPATH_PART_TYPE_CLOSE) {
      drawer->pathClose();
    }
  }

  if (! clip_) {
    fill_  .draw(this, drawer);
    stroke_.draw(this, drawer);
  }
  else
    drawer->pathClip();
}

CQIllustratorShapeGeometry *
CPathShape::
getGeometry()
{
  return new CQIllustratorPathGeometry(fixed_, rcenter_, m_, parts_);
}

void
CPathShape::
doSetGeometry(const CQIllustratorShapeGeometry *geom)
{
  const CQIllustratorPathGeometry *geom1 = dynamic_cast<const CQIllustratorPathGeometry *>(geom);

  if (! geom1) return;

  fixed_   = geom1->fixed_;
  rcenter_ = geom1->rcenter_;
  m_       = geom1->m_;
  parts_   = geom1->parts_;

  invalidateBBox();
}

class CPathShapeSVGProcess : public CPathShapeProcess {
 public:
  CPathShapeSVGProcess(const CQIllustratorSaveData &saveData, CPathShape *path) :
   CPathShapeProcess(), file_(saveData.file), path_(path) {
  }

  void init() {
    file_->printf("<path d=\"");
  }

  void moveTo(const CPoint2D &p) {
    file_->printf("%sM %g,%g", sep_.c_str(), p.x, p.y);

    sep_ = " ";
  }

  void lineTo(const CPoint2D &p) {
    file_->printf("%sL %g,%g", sep_.c_str(), p.x, p.y);

    sep_ = " ";
  }

  void curve2To(const CPoint2D &p1, const CPoint2D &p2) {
    file_->printf("%sQ %g,%g %g,%g", sep_.c_str(), p1.x, p1.y, p2.x, p2.y);

    sep_ = " ";
  }

  void curve3To(const CPoint2D &p1, const CPoint2D &p2, const CPoint2D &p3) {
    file_->printf("%sC %g,%g %g,%g %g,%g", sep_.c_str(), p1.x, p1.y, p2.x, p2.y, p3.x, p3.y);

    sep_ = " ";
  }

  void arcTo(const CPoint2D &c, double xr, double yr, double angle1, double angle2) {
    double a1 = CMathGen::RadToDeg(angle1);
    double a2 = CMathGen::RadToDeg(angle2);

    double phi = 0;

    int    fa, fs;
    double x0, y0, x1, y1;

    CMathGeom2D::ConvertToSVGArc(c.x, c.y, xr, yr, a1, a2 - a1, phi,
                                 &x0, &y0, &x1, &y1, &fa, &fs);

    file_->printf("%sA %g,%g %g %d %d %g %g", sep_.c_str(), xr, yr, phi, fa, fs, x1, y1);

    sep_ = " ";
  }

  void close() {
    file_->printf("%sz", sep_.c_str());

    sep_ = " ";
  }

  void term() {
    file_->printf("\"%s %s/>\n", path_->getSVGMatrix().c_str(), path_->getSVGStroke().c_str());
  }

 private:
  CFile       *file_;
  CPathShape  *path_;
  std::string  sep_;
};

void
CPathShape::
saveSVG(const CQIllustratorSaveData &saveData)
{
  CPathShapeSVGProcess process(saveData, this);

  process.init();

  processPath(&process);

  process.term();
}

void
CPathShape::
saveCmd(const CQIllustratorSaveData & /*saveData*/)
{
}

void
CPathShape::
processPath(CPathShapeProcess *process)
{
  parts_.processPath(process);
}

//-----

CPathShapeControlLine::
CPathShapeControlLine(uint ind, const CLine2D &p) :
 CQIllustratorShapeControlLine(ind, p), ind_(ind)
{
}

CPathShapeControlLine *
CPathShapeControlLine::
dup() const
{
  return new CPathShapeControlLine(ind_, line_);
}

CLine2D
CPathShapeControlLine::
getLine(const CQIllustratorShape *shape) const
{
  const CPathShape *path = dynamic_cast<const CPathShape *>(shape);
  assert(path);

  return path->getLine(ind_);
}

void
CPathShapeControlLine::
setLine(CQIllustratorShape *shape, const CLine2D &line)
{
  CPathShape *path = dynamic_cast<CPathShape *>(shape);
  assert(path);

  path->setLine(ind_, line);
}

void
CPathShapeControlLine::
setType(CQIllustratorShape *shape, CPathPartType type)
{
  CPathShape *path = dynamic_cast<CPathShape *>(shape);
  assert(path);

  path->setLineType(ind_, type);
}

//-----

CPathShapeControlPoint::
CPathShapeControlPoint(uint ind, const CPoint2D &p) :
 CQIllustratorShapeControlPoint((ind << 4), p), ind_(ind), ind1_(0)
{
}

CPathShapeControlPoint::
CPathShapeControlPoint(uint ind, uint ind1, const CPoint2D &p) :
 CQIllustratorShapeControlPoint((ind << 4) | ind1, p), ind_(ind), ind1_(ind1)
{
}

CPathShapeControlPoint *
CPathShapeControlPoint::
dup() const
{
  return new CPathShapeControlPoint(ind_, ind1_, point_);
}

CPoint2D
CPathShapeControlPoint::
getPoint(const CQIllustratorShape *shape) const
{
  const CPathShape *path = dynamic_cast<const CPathShape *>(shape);
  assert(path);

  return path->getPoint(ind_, ind1_);
}

void
CPathShapeControlPoint::
setPoint(CQIllustratorShape *shape, const CPoint2D &point)
{
  CPathShape *path = dynamic_cast<CPathShape *>(shape);
  assert(path);

  path->setPoint(ind_, ind1_, point);
}

void
CPathShapeControlPoint::
setCornerNode(CQIllustratorShape *shape) const
{
  CPathShape *path = dynamic_cast<CPathShape *>(shape);
  assert(path);

  path->setCornerPoint(ind_);
}

void
CPathShapeControlPoint::
setCurveNode(CQIllustratorShape *shape) const
{
  CPathShape *path = dynamic_cast<CPathShape *>(shape);
  assert(path);

  path->setCurvePoint(ind_);
}

//----------

CQIllustratorTextShape::
CQIllustratorTextShape(const CPoint2D &p1, const CPoint2D &p2, const std::string &str,
                       CFontPtr font, CHAlignType halign, CVAlignType valign) :
 CQIllustratorShape(), p1_(p1), p2_(p2), str_(str), font_(font),
 halign_(halign), valign_(valign), cursorPos_(-1)
{
}

CQIllustratorTextShape::
CQIllustratorTextShape(const CQIllustratorTextShape &text) :
 CQIllustratorShape(text), p1_(text.p1_), p2_(text.p2_), str_(text.str_), font_(text.font_),
 halign_(text.halign_), valign_(text.valign_), cursorPos_(text.cursorPos_)
{
}

CQIllustratorTextShape *
CQIllustratorTextShape::
dup() const
{
  return new CQIllustratorTextShape(*this);
}

void
CQIllustratorTextShape::
getControlPoints(ControlPointList &points, ControlType type) const
{
  if (type == CONTROL_GEOMETRY) {
    const CMatrix2D &m = getFlatMatrix();

    CPoint2D p1 = m*p1_;
    CPoint2D p2 = m*p2_;

    CQIllustratorTextShapeControlPoint *ll = new CQIllustratorTextShapeControlPoint(TYPE_LL, p1);
    CQIllustratorTextShapeControlPoint *ur = new CQIllustratorTextShapeControlPoint(TYPE_UR, p2);

    points.push_back(ll);
    points.push_back(ur);
  }
  else
    CQIllustratorShape::getControlPoints(points, type);
}

void
CQIllustratorTextShape::
setControlPoint(const CQIllustratorShapeControlPoint *point)
{
  const CQIllustratorTextShapeControlPoint *tpoint =
    dynamic_cast<const CQIllustratorTextShapeControlPoint *>(point);

  assert(tpoint);

  const CMatrix2D &m = getFlatMatrix();

  CMatrix2D im = m.inverse();

  CPoint2D p = im*point->getPoint(this);

  CQIllustratorTextShape::ControlPointType type = tpoint->getType();

  if      (type == CQIllustratorTextShape::TYPE_LL)
    p1_ = p;
  else if (type == CQIllustratorTextShape::TYPE_UR)
    p2_ = p;

  invalidateBBox();
}

CPoint2D
CQIllustratorTextShape::
getLL() const
{
  const CMatrix2D &m = getFlatMatrix();

  return m*p1_;
}

CPoint2D
CQIllustratorTextShape::
getUR() const
{
  const CMatrix2D &m = getFlatMatrix();

  return m*p2_;
}

void
CQIllustratorTextShape::
setLL(const CPoint2D &p)
{
  const CMatrix2D &m = getFlatMatrix();

  CMatrix2D im = m.inverse();

  CPoint2D p1 = im*p;

  p1_ = p1;

  invalidateBBox();
}

void
CQIllustratorTextShape::
setUR(const CPoint2D &p)
{
  const CMatrix2D &m = getFlatMatrix();

  CMatrix2D im = m.inverse();

  CPoint2D p1 = im*p;

  p2_ = p1;

  invalidateBBox();
}

void
CQIllustratorTextShape::
setText(const std::string &str)
{
  str_ = str;

  invalidateBBox();
}

void
CQIllustratorTextShape::
setFont(CFontPtr font)
{
  font_ = font;
}

void
CQIllustratorTextShape::
setHAlign(CHAlignType halign)
{
  halign_ = halign;
}


void
CQIllustratorTextShape::
setVAlign(CVAlignType valign)
{
  valign_ = valign;
}

void
CQIllustratorTextShape::
updateBBox() const
{
  if (! bbox_valid_) {
    bbox_.reset();

    const CMatrix2D &m = getFlatMatrix();

    CPoint2D p1 = m*CPoint2D(p1_.x, p1_.y);
    CPoint2D p2 = m*CPoint2D(p2_.x, p1_.y);
    CPoint2D p3 = m*CPoint2D(p2_.x, p2_.y);
    CPoint2D p4 = m*CPoint2D(p1_.x, p2_.y);

    bbox_ += p1;
    bbox_ += p2;
    bbox_ += p3;
    bbox_ += p4;

    bbox_valid_ = true;
  }
}

bool
CQIllustratorTextShape::
getPolygon(CPolygon2D &) const
{
  return false;
}

bool
CQIllustratorTextShape::
getPath(CPathShapePartList &) const
{
  return false;
}

void
CQIllustratorTextShape::
moveBy(const CPoint2D &d)
{
  assert(! getFixed());

  checkoutShape(CQIllustratorData::CHANGE_GEOMETRY);

  const CMatrix2D &m = getFlatMatrix();

  CMatrix2D im = m.inverse();

  p1_ = im*(m*p1_ + d);
  p2_ = im*(m*p2_ + d);

  invalidateBBox();

  checkinShape(CQIllustratorData::CHANGE_GEOMETRY);
}

void
CQIllustratorTextShape::
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
CQIllustratorTextShape::
flip(bool)
{
  return false;
}

void
CQIllustratorTextShape::
drawShape(CQIllustratorShapeDrawer *drawer) const
{
  const CMatrix2D &m = getFlatMatrix();

  //drawer->setStroke(this, getStroke());
  //drawer->setFill  (this, getFill  ());

  drawer->setFont(font_);

  drawer->pushMatrix(m, true);

  drawer->pathInit();

  drawer->pathText(CBBox2D(p1_, p2_), str_, halign_, valign_);
//drawer->drawText(CBBox2D(p1_, p2_), str_, halign_, valign_, cursorPos_);

  if (! clip_) {
    fill_  .draw(this, drawer);
    stroke_.draw(this, drawer);
  }
  else
    drawer->pathClip();

  drawer->popMatrix();
}

CQIllustratorShapeGeometry *
CQIllustratorTextShape::
getGeometry()
{
  return new CQIllustratorTextGeometry(fixed_, rcenter_, m_, p1_, p2_, str_,
                                       font_, halign_, valign_);
}

void
CQIllustratorTextShape::
doSetGeometry(const CQIllustratorShapeGeometry *geom)
{
  const CQIllustratorTextGeometry *geom1 =
    dynamic_cast<const CQIllustratorTextGeometry *>(geom);

  if (! geom1) return;

  fixed_   = geom1->fixed_;
  rcenter_ = geom1->rcenter_;
  m_       = geom1->m_;
  p1_      = geom1->p1_;
  p2_      = geom1->p2_;
  str_     = geom1->str_;
  font_    = geom1->font_;
  halign_  = geom1->halign_;
  valign_  = geom1->valign_;

  invalidateBBox();
}

void
CQIllustratorTextShape::
saveSVG(const CQIllustratorSaveData &saveData)
{
  CFile *file = saveData.file;

  file->printf("<text x=\"%g\" y=\"%g\"%s %s>%s</text>\n",
               p1_.x, p1_.y, getSVGMatrix().c_str(), getSVGStroke().c_str(), str_.c_str());
}

void
CQIllustratorTextShape::
saveCmd(const CQIllustratorSaveData & /*saveData*/)
{
}

void
CQIllustratorTextShape::
drawSelect(CQIllustratorShapeDrawer *drawer)
{
  CBBox2D bbox = getFlatBBox();

  CPoint2D p1 = bbox.getLL();
  CPoint2D p2 = bbox.getLR();
  CPoint2D p3 = bbox.getUR();
  CPoint2D p4 = bbox.getUL();

  drawer->drawControlLine(p1, p2);
  drawer->drawControlLine(p2, p3);
  drawer->drawControlLine(p3, p4);
  drawer->drawControlLine(p4, p1);

//const CMatrix2D &m = getFlatMatrix();

  double a = font_->getCharAscent();
  double d = font_->getCharDescent();

  double f = d/(a + d);

  CPoint2D pb1 = p1 + f*(p4 - p1);
  CPoint2D pb2 = p2 + f*(p3 - p2);

  drawer->drawControlLine(pb1, pb2);
}

//-----

CQIllustratorTextShapeControlPoint::
CQIllustratorTextShapeControlPoint(CQIllustratorTextShape::ControlPointType type,
                                   const CPoint2D &p) :
 CQIllustratorShapeControlPoint((uint) type, p), type_(type)
{
}

CQIllustratorTextShapeControlPoint *
CQIllustratorTextShapeControlPoint::
dup() const
{
  return new CQIllustratorTextShapeControlPoint(type_, point_);
}

CPoint2D
CQIllustratorTextShapeControlPoint::
getPoint(const CQIllustratorShape *shape) const
{
  const CQIllustratorTextShape *text = dynamic_cast<const CQIllustratorTextShape *>(shape);
  assert(text);

  if      (type_ == CQIllustratorTextShape::TYPE_LL)
    return text->getLL();
  else if (type_ == CQIllustratorTextShape::TYPE_UR)
    return text->getUR();
  else
    assert(false);
}

void
CQIllustratorTextShapeControlPoint::
setPoint(CQIllustratorShape *shape, const CPoint2D &point)
{
  CQIllustratorTextShape *text = dynamic_cast<CQIllustratorTextShape *>(shape);
  assert(text);

  if      (type_ == CQIllustratorTextShape::TYPE_LL)
    text->setLL(point);
  else if (type_ == CQIllustratorTextShape::TYPE_UR)
    text->setUR(point);
  else
    assert(false);
}

//--------------

CQIllustratorGroupShape::
CQIllustratorGroupShape() :
 CQIllustratorShape()
{
}

CQIllustratorGroupShape::
CQIllustratorGroupShape(const CQIllustratorGroupShape &group) :
 CQIllustratorShape(group)
{
  // TODO: dup children ?
}

CQIllustratorGroupShape *
CQIllustratorGroupShape::
dup() const
{
  return new CQIllustratorGroupShape(*this);
}

void
CQIllustratorGroupShape::
childrenChanged()
{
  invalidateBBox();
}

void
CQIllustratorGroupShape::
getControlPoints(ControlPointList &points, ControlType type) const
{
  if (type == CONTROL_GEOMETRY) {
    const CBBox2D &bbox = getBBox();

    CPoint2D p1 = bbox.getLL();
    CPoint2D p2 = bbox.getUR();

    points.push_back(new CQIllustratorGroupShapeControlPoint(0, p1));
    points.push_back(new CQIllustratorGroupShapeControlPoint(1, p2));
  }
  else
    CQIllustratorShape::getControlPoints(points, type);
}

void
CQIllustratorGroupShape::
setControlPoint(const CQIllustratorShapeControlPoint *)
{
  // TODO
}

void
CQIllustratorGroupShape::
scale(const CPoint2D &c, double sx, double sy)
{
  CQIllustratorShape::scale(c, sx, sy);

  invalidateBBox();
}

void
CQIllustratorGroupShape::
rotate(double da)
{
  CQIllustratorShape::rotate(da);

  invalidateBBox();
}

void
CQIllustratorGroupShape::
transform(const CPoint2D &c, const CMatrix2D &m)
{
  CQIllustratorShape::transform(c, m);

  invalidateBBox();
}

void
CQIllustratorGroupShape::
updateBBox() const
{
  if (! bbox_valid_) {
    CBBox2D bbox;

    ShapeList::const_iterator ps1, ps2;

    for (ps1 = shapes_.begin(), ps2 = shapes_.end(); ps1 != ps2; ++ps1)
      bbox += (*ps1)->getBBox();

    CPoint2D p1 = bbox.getLL();
    CPoint2D p2 = bbox.getLR();
    CPoint2D p3 = bbox.getUL();
    CPoint2D p4 = bbox.getUR();

    bbox_ = CBBox2D(p1, p3);

    bbox_ += p2;
    bbox_ += p4;

    bbox_valid_ = true;
  }
}

bool
CQIllustratorGroupShape::
getPolygon(CPolygon2D &) const
{
  return false;
}

bool
CQIllustratorGroupShape::
getPath(CPathShapePartList &) const
{
  return false;
}

void
CQIllustratorGroupShape::
getPolygons(std::vector<CPolygon2D> &polygons) const
{
  ShapeList::const_iterator ps1, ps2;

  for (ps1 = shapes_.begin(), ps2 = shapes_.end(); ps1 != ps2; ++ps1) {
    CPolygon2D polygon;

    if (! (*ps1)->getPolygon(polygon))
      continue;

    polygons.push_back(polygon);
  }
}

void
CQIllustratorGroupShape::
moveBy(const CPoint2D &d)
{
  assert(! getFixed());

  checkoutShape(CQIllustratorData::CHANGE_GEOMETRY);

  //CQIllustratorShape::moveBy(d);

  ShapeList::const_iterator ps1, ps2;

  for (ps1 = shapes_.begin(), ps2 = shapes_.end(); ps1 != ps2; ++ps1) {
    CQIllustratorShape *shape = *ps1;

    shape->moveBy(d);
  }

  invalidateBBox();

  checkinShape(CQIllustratorData::CHANGE_GEOMETRY);
}

bool
CQIllustratorGroupShape::
flip(bool x_axis)
{
  bool rc = true;

  ShapeList::const_iterator ps1, ps2;

  for (ps1 = shapes_.begin(), ps2 = shapes_.end(); ps1 != ps2; ++ps1) {
    CQIllustratorShape *shape = *ps1;

    bool rc1 = shape->flip(x_axis);

    // if fails replace with polygon ?
    if (! rc1)
      rc = false;
  }

  return rc;
}

void
CQIllustratorGroupShape::
drawShape(CQIllustratorShapeDrawer *drawer) const
{
  //const CMatrix2D &m = getFlatMatrix();

  ShapeList::const_iterator ps1, ps2;

  for (ps1 = shapes_.begin(), ps2 = shapes_.end(); ps1 != ps2; ++ps1) {
    CQIllustratorShape *shape = *ps1;

    //CMatrix2D m1 = shape->getMatrix();

    //shape->setMatrix(m*m1);

    shape->draw(drawer);

    //shape->setMatrix(m1);
  }
}

CQIllustratorShapeGeometry *
CQIllustratorGroupShape::
getGeometry()
{
  return new CQIllustratorGroupGeometry(fixed_, rcenter_, m_);
}

void
CQIllustratorGroupShape::
doSetGeometry(const CQIllustratorShapeGeometry *geom)
{
  const CQIllustratorGroupGeometry *geom1 = dynamic_cast<const CQIllustratorGroupGeometry *>(geom);

  if (! geom1) return;

  fixed_   = geom1->fixed_;
  rcenter_ = geom1->rcenter_;
  m_       = geom1->m_;

  invalidateBBox();
}

void
CQIllustratorGroupShape::
saveSVG(const CQIllustratorSaveData &saveData)
{
  CFile *file = saveData.file;

  file->printf("<g>\n");

  ShapeList::const_iterator ps1, ps2;

  for (ps1 = shapes_.begin(), ps2 = shapes_.end(); ps1 != ps2; ++ps1)
    (*ps1)->saveSVG(saveData);

  file->printf("</g>\n");
}

void
CQIllustratorGroupShape::
saveCmd(const CQIllustratorSaveData &saveData)
{
  ShapeList::const_iterator ps1, ps2;

  for (ps1 = shapes_.begin(), ps2 = shapes_.end(); ps1 != ps2; ++ps1)
    (*ps1)->saveCmd(saveData);
}

//-----

CQIllustratorGroupShapeControlPoint::
CQIllustratorGroupShapeControlPoint(uint ind, const CPoint2D &p) :
 CQIllustratorShapeControlPoint(ind, p)
{
}

CQIllustratorGroupShapeControlPoint *
CQIllustratorGroupShapeControlPoint::
dup() const
{
  return new CQIllustratorGroupShapeControlPoint(ind_, point_);
}

CPoint2D
CQIllustratorGroupShapeControlPoint::
getPoint(const CQIllustratorShape *) const
{
  return CPoint2D(0,0);
}

void
CQIllustratorGroupShapeControlPoint::
setPoint(CQIllustratorShape *, const CPoint2D &)
{
}

//----------

void
CQIllustratorShapeStroke::
draw(const CQIllustratorShape *shape, CQIllustratorShapeDrawer *drawer) const
{
  drawer->setStroke(shape, *this);

  drawer->pathStroke();
}

void
CQIllustratorShapeFill::
draw(const CQIllustratorShape *shape, CQIllustratorShapeDrawer *drawer) const
{
  drawer->setFill(shape, *this);

  drawer->pathFill(rule_);
}

//----------

CQIllustratorShapeControlLine::
CQIllustratorShapeControlLine(uint id, const CLine2D &line) :
 id_(id), line_(line)
{
}

CLine2D
CQIllustratorShapeControlLine::
getLine(const CQIllustratorShape *) const
{
  return line_;
}

void
CQIllustratorShapeControlLine::
setLine(CQIllustratorShape *, const CLine2D &line)
{
  line_ = line;
}

void
CQIllustratorShapeControlLine::
setLine(const CLine2D &line)
{
  line_ = line;
}

void
CQIllustratorShapeControlLine::
updateLine(CQIllustratorShape *shape, CQIllustratorShapeControlLine *line)
{
  setLine(shape, line->line_);
}

void
CQIllustratorShapeControlLine::
setType(CQIllustratorShape *, CPathPartType)
{
}

bool
operator==(const CQIllustratorShapeControlLine &p1, const CQIllustratorShapeControlLine &p2)
{
  return (p1.id_ == p2.id_);
}

bool
operator!=(const CQIllustratorShapeControlLine &p1, const CQIllustratorShapeControlLine &p2)
{
  return ! (p1 == p2);
}

void
CQIllustratorShapeControlLine::
moveLineTo(CQIllustratorShape *shape, const CLine2D &pos)
{
  setLine(shape, pos);
}

//----------

CQIllustratorShapeControlPoint::
CQIllustratorShapeControlPoint(uint id, const CPoint2D &point, ControlPointType type) :
 id_(id), point_(point), type_(type)
{
}

CPoint2D
CQIllustratorShapeControlPoint::
getPoint(const CQIllustratorShape *) const
{
  return point_;
}

void
CQIllustratorShapeControlPoint::
setPoint(CQIllustratorShape *, const CPoint2D &point)
{
  point_ = point;
}

void
CQIllustratorShapeControlPoint::
setPoint(const CPoint2D &point)
{
  point_ = point;
}

void
CQIllustratorShapeControlPoint::
updatePoint(CQIllustratorShape *shape, CQIllustratorShapeControlPoint *point)
{
  setPoint(shape, point->point_);
}

bool
operator==(const CQIllustratorShapeControlPoint &p1, const CQIllustratorShapeControlPoint &p2)
{
  return (p1.id_ == p2.id_);
}

bool
operator!=(const CQIllustratorShapeControlPoint &p1, const CQIllustratorShapeControlPoint &p2)
{
  return ! (p1 == p2);
}

double
CQIllustratorShapeControlPoint::
distance(const CQIllustratorShape *shape, const CPoint2D &point) const
{
  return pointDist(getPoint(shape), point);
}

void
CQIllustratorShapeControlPoint::
movePointBy(CQIllustratorShape *shape, const CPoint2D &d)
{
  setPoint(shape, getPoint(shape) + d);
}

void
CQIllustratorShapeControlPoint::
movePointTo(CQIllustratorShape *shape, const CPoint2D &pos)
{
  setPoint(shape, pos);
}

double
CQIllustratorShapeControlPoint::
pointDist(const CPoint2D &p1, const CPoint2D &p2)
{
  double dx = p1.x - p2.x;
  double dy = p1.y - p2.y;

  return sqrt(dx*dx + dy*dy);
}

//----------

CQIllustratorShapeFilterMgr *
CQIllustratorShapeFilterMgr::
getInstance()
{
  static CQIllustratorShapeFilterMgr *instance;

  if (! instance)
    instance = new CQIllustratorShapeFilterMgr;

  return instance;
}

CQIllustratorShapeFilterMgr::
CQIllustratorShapeFilterMgr() :
 id_(0)
{
  addFilter(new CQIllustratorShapeGaussianFilter(2.0));
}

CQIllustratorShapeFilterMgr::
~CQIllustratorShapeFilterMgr()
{
}

CQIllustratorShapeFilter *
CQIllustratorShapeFilterMgr::
getFilter(uint id)
{
  FilterMap::const_iterator p = filterMap_.find(id);

  if (p == filterMap_.end())
    return NULL;

  return (*p).second;
}

void
CQIllustratorShapeFilterMgr::
addFilter(CQIllustratorShapeFilter *filter)
{
  filter->setId(++id_);

  filterMap_[id_] = filter;
}

//---

CQIllustratorShapeFilter::
CQIllustratorShapeFilter()
{
}

CQIllustratorShapeFilter::
~CQIllustratorShapeFilter()
{
}
