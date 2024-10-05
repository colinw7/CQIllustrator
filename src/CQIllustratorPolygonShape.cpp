#include <CQIllustratorPolygonShape.h>
#include <CQIllustratorPolygonGeometry.h>
#include <CQIllustratorShapeDrawer.h>

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
  if (type == ControlType::GEOMETRY) {
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

  if (before_point != nullptr) {
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

    const CMatrix2D &m = getMatrix();

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

  checkoutShape(CQIllustratorData::ChangeType::GEOMETRY);

  const CMatrix2D &m = getFlatMatrix();

  CMatrix2D im = m.inverse();

  uint num_points = points_.size();

  for (uint i = 0; i < num_points; ++i)
    points_[i] = im*(m*points_[i] + d);

  invalidateBBox();

  checkinShape(CQIllustratorData::ChangeType::GEOMETRY);
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
