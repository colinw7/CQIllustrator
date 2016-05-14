#include <CQIllustratorGroupShape.h>
#include <CQIllustratorGroupGeometry.h>

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
  if (type == ControlType::GEOMETRY) {
    const CBBox2D &bbox = getFlatBBox();

    if (bbox.isSet()) {
      CPoint2D p1 = bbox.getLL();
      CPoint2D p2 = bbox.getUR();

      points.push_back(new CQIllustratorGroupShapeControlPoint(0, p1));
      points.push_back(new CQIllustratorGroupShapeControlPoint(1, p2));
    }
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
    bbox_.reset();

    const CMatrix2D &m = getMatrix();

    CBBox2D bbox;

    for (const auto &shape : shapes_) {
      const CBBox2D &shapeBBox = shape->getBBox();

      if (shapeBBox.isSet())
        bbox += shapeBBox;
    }

    if (bbox.isSet()) {
      CPoint2D p1 = m*bbox.getLL();
      CPoint2D p2 = m*bbox.getLR();
      CPoint2D p3 = m*bbox.getUL();
      CPoint2D p4 = m*bbox.getUR();

      CBBox2D bbox1(p1, p2);

      bbox1 += p3;
      bbox1 += p4;

      bbox_ = bbox1;
    }

    bbox_valid_ = bbox_.isSet();
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

  checkoutShape(CQIllustratorData::ChangeType::GEOMETRY);

  //CQIllustratorShape::moveBy(d);

  ShapeList::const_iterator ps1, ps2;

  for (ps1 = shapes_.begin(), ps2 = shapes_.end(); ps1 != ps2; ++ps1) {
    CQIllustratorShape *shape = *ps1;

    shape->moveBy(d);
  }

  invalidateBBox();

  checkinShape(CQIllustratorData::ChangeType::GEOMETRY);
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

    if (! shape->getVisible()) continue;

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
