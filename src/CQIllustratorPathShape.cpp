#include <CQIllustratorPathShape.h>
#include <CQIllustratorPathGeometry.h>
#include <CQIllustratorPathShapeControlLine.h>
#include <CQIllustratorPathShapeControlPoint.h>
#include <CQIllustratorShapeDrawer.h>

class CPathShapePolygonProcess : public CPathShapeProcess {
 public:
  CPathShapePolygonProcess(CPolygon2D &polygon, const CQIllustratorPathShape *path) :
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
    // TODO ?
  }

  void close() {
  }

  void term() {
  }

 private:
  CPolygon2D&                   polygon_;
  const CQIllustratorPathShape *path_ { nullptr };
  CMatrix2D                     m_;
};

//-----

CQIllustratorPathShape::
CQIllustratorPathShape(const CPathShapePartList &parts) :
 CQIllustratorShape(), parts_(parts)
{
}

CQIllustratorPathShape::
CQIllustratorPathShape(const CQIllustratorPathShape &path) :
 CQIllustratorShape(path), parts_(path.parts_)
{
}

CQIllustratorPathShape *
CQIllustratorPathShape::
dup() const
{
  return new CQIllustratorPathShape(*this);
}

bool
CQIllustratorPathShape::
getPolygon(CPolygon2D &polygon) const
{
  CPathShapePolygonProcess process(polygon, this);

  process.init();

  auto *th = const_cast<CQIllustratorPathShape *>(this);

  th->processPath(&process);

  process.term();

  return true;
}

bool
CQIllustratorPathShape::
getPath(CPathShapePartList &parts) const
{
  parts = parts_;

  return true;
}

void
CQIllustratorPathShape::
moveBy(const CPoint2D &d)
{
  assert(! getFixed());

  checkoutShape(CQIllustratorData::ChangeType::GEOMETRY);

  const CMatrix2D &m = getFlatMatrix();

  CMatrix2D im = m.inverse();

  uint num_parts = parts_.size();

  for (uint i = 0; i < num_parts; ++i) {
    CPathShapePart *part = parts_[i];

    CPathPartType type = part->getType();

    if      (type == CPATH_PART_TYPE_MOVE_TO) {
      auto *moveToPart = dynamic_cast<CPathShapeMoveTo *>(part);

      moveToPart->setPoint(im*(m*moveToPart->getPoint() + d));
    }
    else if (type == CPATH_PART_TYPE_LINE_TO) {
      auto *lineToPart = dynamic_cast<CPathShapeLineTo *>(part);

      lineToPart->setPoint(im*(m*lineToPart->getPoint() + d));
    }
    else if (type == CPATH_PART_TYPE_CURVE2_TO) {
      auto *curveToPart = dynamic_cast<CPathShapeCurve2To *>(part);

      curveToPart->setPoint1(im*(m*curveToPart->getPoint1() + d));
      curveToPart->setPoint2(im*(m*curveToPart->getPoint2() + d));
    }
    else if (type == CPATH_PART_TYPE_CURVE3_TO) {
      auto *curveToPart = dynamic_cast<CPathShapeCurve3To *>(part);

      curveToPart->setPoint1(im*(m*curveToPart->getPoint1() + d));
      curveToPart->setPoint2(im*(m*curveToPart->getPoint2() + d));
      curveToPart->setPoint3(im*(m*curveToPart->getPoint3() + d));
    }
    else if (type == CPATH_PART_TYPE_ARC) {
      auto *arcToPart = dynamic_cast<CPathShapeArc *>(part);

      arcToPart->setCenter(im*(m*arcToPart->getCenter() + d));
    }
    else
      continue;
  }

  invalidateBBox();

  checkinShape(CQIllustratorData::ChangeType::GEOMETRY);
}

void
CQIllustratorPathShape::
startGroup()
{
  ++group_;
}

void
CQIllustratorPathShape::
endGroup()
{
  --group_;

  assert(group_ >= 0);

  if (group_ == 0)
    invalidateBBox();
}

bool
CQIllustratorPathShape::
empty() const
{
  return parts_.empty();
}

double
CQIllustratorPathShape::
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
      const auto *moveToPart = dynamic_cast<const CPathShapeMoveTo *>(part);

      p1 = m*moveToPart->getPoint();
    }
    else if (type == CPATH_PART_TYPE_LINE_TO) {
      const auto *lineToPart = dynamic_cast<const CPathShapeLineTo *>(part);

      p1 = m*lineToPart->getPoint();
    }
    else if (type == CPATH_PART_TYPE_CURVE2_TO) {
      const auto *curveToPart = dynamic_cast<const CPathShapeCurve2To *>(part);

      p1 = m*curveToPart->getPoint2();
    }
    else if (type == CPATH_PART_TYPE_CURVE3_TO) {
      const auto *curveToPart = dynamic_cast<const CPathShapeCurve3To *>(part);

      p1 = m*curveToPart->getPoint3();
    }
    else if (type == CPATH_PART_TYPE_ARC) {
      const auto *arcToPart = dynamic_cast<const CPathShapeArc *>(part);

      p1 = m*arcToPart->getCenter();
    }
    else
      continue;

    dist = std::min(fabs(p1.x - p.x), fabs(p1.y - p.y));
  }

  return dist;
}

bool
CQIllustratorPathShape::
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
      const auto *moveToPart = dynamic_cast<const CPathShapeMoveTo *>(part);

      p1 = m*moveToPart->getPoint();
    }
    else if (type == CPATH_PART_TYPE_LINE_TO) {
      const auto *lineToPart = dynamic_cast<const CPathShapeLineTo *>(part);

      p1 = m*lineToPart->getPoint();
    }
    else if (type == CPATH_PART_TYPE_CURVE2_TO) {
      const auto *curveToPart = dynamic_cast<const CPathShapeCurve2To *>(part);

      p1 = m*curveToPart->getPoint2();
    }
    else if (type == CPATH_PART_TYPE_CURVE3_TO) {
      const auto *curveToPart = dynamic_cast<const CPathShapeCurve3To *>(part);

      p1 = m*curveToPart->getPoint3();
    }
    else if (type == CPATH_PART_TYPE_ARC) {
      // TODO: use arc end points
      const auto *arcToPart = dynamic_cast<const CPathShapeArc *>(part);

      p1 = m*arcToPart->getCenter();
    }
    else
      continue;

    points.push_back(p1);
  }

  return CMathGeom2D::PointInsideEvenOdd(p, &points[0], points.size());
}

void
CQIllustratorPathShape::
getControlLines(ControlLineList &clines) const
{
  std::vector<CLine2D> lines;

  getLines(lines);

  uint num_lines = lines.size();

  for (uint i = 0; i < num_lines; ++i)
    clines.push_back(new CQIllustratorPathShapeControlLine(i, lines[i]));
}

void
CQIllustratorPathShape::
getLines(std::vector<CLine2D> &lines) const
{
  CPoint2D p0, p1, p2;

  const CMatrix2D &m = getFlatMatrix();

  uint num_parts = parts_.size();

  for (uint i = 0; i < num_parts; ++i) {
    const CPathShapePart *part = parts_[i];

    CPathPartType type = part->getType();

    if      (type == CPATH_PART_TYPE_MOVE_TO) {
      const auto *moveToPart = dynamic_cast<const CPathShapeMoveTo *>(part);

      p1 = m*moveToPart->getPoint();

      p0 = p1;
    }
    else if (type == CPATH_PART_TYPE_LINE_TO) {
      const auto *lineToPart = dynamic_cast<const CPathShapeLineTo *>(part);

      p2 = m*lineToPart->getPoint();

      lines.push_back(CLine2D(p1, p2));

      p1 = p2;
    }
    else if (type == CPATH_PART_TYPE_CURVE2_TO) {
      const auto *curveToPart = dynamic_cast<const CPathShapeCurve2To *>(part);

      p2 = m*curveToPart->getPoint2();

      lines.push_back(CLine2D(p1, p2));

      p1 = p2;
    }
    else if (type == CPATH_PART_TYPE_CURVE3_TO) {
      const auto *curveToPart = dynamic_cast<const CPathShapeCurve3To *>(part);

      p2 = m*curveToPart->getPoint3();

      lines.push_back(CLine2D(p1, p2));
    }
    else if (type == CPATH_PART_TYPE_ARC) {
      const auto *arcToPart = dynamic_cast<const CPathShapeArc *>(part);

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
CQIllustratorPathShape::
getLine(uint i) const
{
  std::vector<CLine2D> lines;

  getLines(lines);

  return lines[i];
}

void
CQIllustratorPathShape::
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
      auto *moveToPart = dynamic_cast<CPathShapeMoveTo *>(part);

      if      (sline_num == int(i))
        moveToPart->setPoint(im*l.start());
      else if (eline_num == int(i))
        moveToPart->setPoint(im*l.end  ());

      eline_num = sline_num++;
    }
    else if (type == CPATH_PART_TYPE_LINE_TO) {
      auto *lineToPart = dynamic_cast<CPathShapeLineTo *>(part);

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
CQIllustratorPathShape::
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
      auto *moveToPart = dynamic_cast<CPathShapeMoveTo *>(part);

      p2 = moveToPart->getPoint();
      p0 = p2;

      line_num = -1;
    }
    else if (partType2 == CPATH_PART_TYPE_LINE_TO) {
      auto *lineToPart = dynamic_cast<CPathShapeLineTo *>(part);

      p2 = lineToPart->getPoint();

      ++line_num;
    }
    else if (partType2 == CPATH_PART_TYPE_CURVE2_TO) {
      auto *curve2Part = dynamic_cast<CPathShapeCurve2To *>(part);

      p2 = curve2Part->getPoint2();

      ++line_num;
    }
    else if (partType2 == CPATH_PART_TYPE_CURVE3_TO) {
      auto *curve3Part = dynamic_cast<CPathShapeCurve3To *>(part);

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

    CPathShapePart *part1 = 0;

    if (line_num == int(i)) {
      CPoint2D pp1, pp2;

      bool pp1_set = false, pp2_set = false;

      if      (partType2 == CPATH_PART_TYPE_LINE_TO) {
      }
      else if (partType2 == CPATH_PART_TYPE_CURVE2_TO) {
        auto *curve2Part = dynamic_cast<CPathShapeCurve2To *>(part);

        pp2 = curve2Part->getPoint1(); pp2_set = true;
      }
      else if (partType2 == CPATH_PART_TYPE_CURVE3_TO) {
        auto *curve3Part = dynamic_cast<CPathShapeCurve3To *>(part);

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
CQIllustratorPathShape::
getControlPoints(ControlPointList &points, ControlType type) const
{
  if (type == ControlType::GEOMETRY) {
    const CMatrix2D &m = getFlatMatrix();

    uint num_parts = parts_.size();

    for (uint i = 0; i < num_parts; ++i) {
      const CPathShapePart *part = parts_[i];

      CPathPartType type = part->getType();

      if      (type == CPATH_PART_TYPE_MOVE_TO) {
        const auto *moveToPart = dynamic_cast<const CPathShapeMoveTo *>(part);

        CPoint2D p1 = m*moveToPart->getPoint();

        points.push_back(new CQIllustratorPathShapeControlPoint(i, p1));
      }
      else if (type == CPATH_PART_TYPE_LINE_TO) {
        const auto *lineToPart = dynamic_cast<const CPathShapeLineTo *>(part);

        CPoint2D p1 = m*lineToPart->getPoint();

        points.push_back(new CQIllustratorPathShapeControlPoint(i, p1));
      }
      else if (type == CPATH_PART_TYPE_CURVE2_TO) {
        const auto *curveToPart = dynamic_cast<const CPathShapeCurve2To *>(part);

        CPoint2D pp1 = m*curveToPart->getPoint1();
        CPoint2D pp2 = m*curveToPart->getPoint2();

        points.push_back(new CQIllustratorPathShapeControlPoint(i, 1, pp1));
        points.push_back(new CQIllustratorPathShapeControlPoint(i, pp2));
      }
      else if (type == CPATH_PART_TYPE_CURVE3_TO) {
        const auto *curveToPart = dynamic_cast<const CPathShapeCurve3To *>(part);

        CPoint2D pp1 = m*curveToPart->getPoint1();
        CPoint2D pp2 = m*curveToPart->getPoint2();
        CPoint2D pp3 = m*curveToPart->getPoint3();

        points.push_back(new CQIllustratorPathShapeControlPoint(i, 1, pp1));
        points.push_back(new CQIllustratorPathShapeControlPoint(i, 2, pp2));
        points.push_back(new CQIllustratorPathShapeControlPoint(i, pp3));
      }
      else if (type == CPATH_PART_TYPE_ARC) {
        const auto *arcToPart = dynamic_cast<const CPathShapeArc *>(part);

        const CPoint2D &c = arcToPart->getCenter();

        CPoint2D p1, p2;

        arcToPart->getEndPoints(p1, p2);

        CPoint2D c1  = m*c;
        CPoint2D pp1 = m*p1;
        CPoint2D pp2 = m*p2;

        points.push_back(new CQIllustratorPathShapeControlPoint(i, c1 ));
        points.push_back(new CQIllustratorPathShapeControlPoint(i, 1, pp1));
        points.push_back(new CQIllustratorPathShapeControlPoint(i, 2, pp2));
      }
      else if (type == CPATH_PART_TYPE_CLOSE) {
      }
    }
  }
  else
    CQIllustratorShape::getControlPoints(points, type);
}

void
CQIllustratorPathShape::
setControlPoint(const CQIllustratorShapeControlPoint *point)
{
  const auto *ppoint = dynamic_cast<const CQIllustratorPathShapeControlPoint *>(point);
  assert(ppoint);

  const CMatrix2D &m = getFlatMatrix();

  uint ind  = ppoint->getInd();
  uint ind1 = ppoint->getInd1();

  CMatrix2D im = m.inverse();

  CPoint2D p = im*point->getPoint(this);

  setPoint(ind, ind1, p);
}

CPoint2D
CQIllustratorPathShape::
getPoint(uint ind, uint ind1) const
{
  uint num_parts = parts_.size();

  assert(ind < num_parts);

  const CPathShapePart *part = parts_[ind];

  CPathPartType type = part->getType();

  CPoint2D p;

  if      (type == CPATH_PART_TYPE_MOVE_TO) {
    assert(ind1 == 0);

    const auto *moveToPart = dynamic_cast<const CPathShapeMoveTo *>(part);

    p = moveToPart->getPoint();
  }
  else if (type == CPATH_PART_TYPE_LINE_TO) {
    assert(ind1 == 0);

    const auto *lineToPart = dynamic_cast<const CPathShapeLineTo *>(part);

    p = lineToPart->getPoint();
  }
  else if (type == CPATH_PART_TYPE_CURVE2_TO) {
    assert(ind1 < 2);

    const auto *curveToPart = dynamic_cast<const CPathShapeCurve2To *>(part);

    if (ind1 == 0)
      p = curveToPart->getPoint2();
    else
      p = curveToPart->getPoint1();
  }
  else if (type == CPATH_PART_TYPE_CURVE3_TO) {
    assert(ind1 < 3);

    const auto *curveToPart = dynamic_cast<const CPathShapeCurve3To *>(part);

    if      (ind1 == 0)
      p = curveToPart->getPoint3();
    else if (ind1 == 1)
      p = curveToPart->getPoint1();
    else
      p = curveToPart->getPoint2();
  }
  else if (type == CPATH_PART_TYPE_ARC) {
    assert(ind1 < 3);

    const auto *arcToPart = dynamic_cast<const CPathShapeArc *>(part);

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
CQIllustratorPathShape::
setPoint(uint ind, uint ind1, const CPoint2D &p)
{
  uint num_parts = parts_.size();

  assert(ind < num_parts);

  CPathShapePart *part = parts_[ind];

  CPathPartType type = part->getType();

  if      (type == CPATH_PART_TYPE_MOVE_TO) {
    assert(ind1 == 0);

    auto *moveToPart = dynamic_cast<CPathShapeMoveTo *>(part);

    moveToPart->setPoint(p);
  }
  else if (type == CPATH_PART_TYPE_LINE_TO) {
    assert(ind1 == 0);

    auto *lineToPart = dynamic_cast<CPathShapeLineTo *>(part);

    lineToPart->setPoint(p);
  }
  else if (type == CPATH_PART_TYPE_CURVE2_TO) {
    assert(ind1 < 2);

    auto *curveToPart = dynamic_cast<CPathShapeCurve2To *>(part);

    if (ind1 == 0)
      curveToPart->setPoint2(p);
    else
      curveToPart->setPoint1(p);
  }
  else if (type == CPATH_PART_TYPE_CURVE3_TO) {
    assert(ind1 < 3);

    auto *curveToPart = dynamic_cast<CPathShapeCurve3To *>(part);

    if      (ind1 == 0)
      curveToPart->setPoint3(p);
    else if (ind1 == 1)
      curveToPart->setPoint1(p);
    else
      curveToPart->setPoint2(p);
  }
  else if (type == CPATH_PART_TYPE_ARC) {
    assert(ind1 < 3);

    auto *arcToPart = dynamic_cast<CPathShapeArc *>(part);

    if (ind1 == 0)
      arcToPart->setCenter(p);
  }
  else
    return;

  invalidateBBox();
}

void
CQIllustratorPathShape::
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
CQIllustratorPathShape::
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
CQIllustratorPathShape::
doRemovePoint(const CQIllustratorShapeControlPoint *point)
{
  const auto *ppoint = dynamic_cast<const CQIllustratorPathShapeControlPoint *>(point);
  assert(ppoint);

  uint ind = ppoint->getInd();

  parts_.removePart(ind);

  return true;
}

void
CQIllustratorPathShape::
updateBBox() const
{
  if (! bbox_valid_) {
    const CMatrix2D &m = getMatrix();

    bbox_.reset();

    uint num_parts = parts_.size();

    for (uint i = 0; i < num_parts; ++i) {
      const CPathShapePart *part = parts_[i];

      CPathPartType type = part->getType();

      if      (type == CPATH_PART_TYPE_MOVE_TO) {
        const auto *moveToPart = dynamic_cast<const CPathShapeMoveTo *>(part);

        CPoint2D p1 = m*moveToPart->getPoint();

        bbox_ += p1;
      }
      else if (type == CPATH_PART_TYPE_LINE_TO) {
        const auto *lineToPart = dynamic_cast<const CPathShapeLineTo *>(part);

        CPoint2D p1 = m*lineToPart->getPoint();

        bbox_ += p1;
      }
      else if (type == CPATH_PART_TYPE_CURVE2_TO) {
        const auto *curveToPart = dynamic_cast<const CPathShapeCurve2To *>(part);

        CPoint2D pp1 = m*curveToPart->getPoint1();
        CPoint2D pp2 = m*curveToPart->getPoint2();

        bbox_ += pp1;
        bbox_ += pp2;
      }
      else if (type == CPATH_PART_TYPE_CURVE3_TO) {
        const auto *curveToPart = dynamic_cast<const CPathShapeCurve3To *>(part);

        CPoint2D pp1 = m*curveToPart->getPoint1();
        CPoint2D pp2 = m*curveToPart->getPoint2();
        CPoint2D pp3 = m*curveToPart->getPoint3();

        bbox_ += pp1;
        bbox_ += pp2;
        bbox_ += pp3;
      }
      else if (type == CPATH_PART_TYPE_ARC) {
        const auto *arcToPart = dynamic_cast<const CPathShapeArc *>(part);

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
CQIllustratorPathShape::
setParts(const CPathShapePartList &parts)
{
  parts_ = parts;

  invalidateBBox();
}

void
CQIllustratorPathShape::
addMoveTo(const CPoint2D &p)
{
  parts_.addPart(new CPathShapeMoveTo(p));

  if (group_ == 0) invalidateBBox();
}

void
CQIllustratorPathShape::
addLineTo(const CPoint2D &p)
{
  parts_.addPart(new CPathShapeLineTo(p));

  if (group_ == 0) invalidateBBox();
}

void
CQIllustratorPathShape::
addLineTo(uint ind, const CPoint2D &p)
{
  parts_.addPart(ind, new CPathShapeLineTo(p));

  if (group_ == 0) invalidateBBox();
}

void
CQIllustratorPathShape::
addCurveTo(const CPoint2D &p1, const CPoint2D &p2)
{
  parts_.addPart(new CPathShapeCurve2To(p1, p2));

  if (group_ == 0) invalidateBBox();
}

void
CQIllustratorPathShape::
addCurveTo(const CPoint2D &p1, const CPoint2D &p2, const CPoint2D &p3)
{
  parts_.addPart(new CPathShapeCurve3To(p1, p2, p3));

  if (group_ == 0) invalidateBBox();
}

void
CQIllustratorPathShape::
addArcTo(const CPoint2D &c, double xr, double yr, double angle1, double angle2)
{
  parts_.addPart(new CPathShapeArc(c, xr, yr, angle1, angle2));

  if (group_ == 0) invalidateBBox();
}

void
CQIllustratorPathShape::
addClose()
{
  parts_.addPart(new CPathShapeClose());

  if (group_ == 0) invalidateBBox();
}

bool
CQIllustratorPathShape::
flip(bool x_axis)
{
  uint num_parts = parts_.size();

  if (num_parts == 0) return true;

  const CMatrix2D &m = getMatrix();

  CMatrix2D im = m.inverse();

  CPoint2D rc = getRotateCenter();

  for (uint i = 0; i < num_parts; ++i) {
    CPathShapePart *part = parts_[i];

    CPathPartType type = part->getType();

    if      (type == CPATH_PART_TYPE_MOVE_TO) {
      auto *moveToPart = dynamic_cast<CPathShapeMoveTo *>(part);

      CPoint2D p = m*moveToPart->getPoint();

      moveToPart->setPoint(im*p.flip(rc, x_axis));
    }
    else if (type == CPATH_PART_TYPE_LINE_TO) {
      auto *lineToPart = dynamic_cast<CPathShapeLineTo *>(part);

      CPoint2D p = m*lineToPart->getPoint();

      lineToPart->setPoint(im*p.flip(rc, x_axis));
    }
    else if (type == CPATH_PART_TYPE_CURVE2_TO) {
      auto *curveToPart = dynamic_cast<CPathShapeCurve2To *>(part);

      CPoint2D p1 = m*curveToPart->getPoint1();
      CPoint2D p2 = m*curveToPart->getPoint2();

      curveToPart->setPoint1(im*p1.flip(rc, x_axis));
      curveToPart->setPoint2(im*p2.flip(rc, x_axis));
    }
    else if (type == CPATH_PART_TYPE_CURVE3_TO) {
      auto *curveToPart = dynamic_cast<CPathShapeCurve3To *>(part);

      CPoint2D p1 = m*curveToPart->getPoint1();
      CPoint2D p2 = m*curveToPart->getPoint2();
      CPoint2D p3 = m*curveToPart->getPoint3();

      curveToPart->setPoint1(im*p1.flip(rc, x_axis));
      curveToPart->setPoint2(im*p2.flip(rc, x_axis));
      curveToPart->setPoint3(im*p3.flip(rc, x_axis));
    }
    else if (type == CPATH_PART_TYPE_ARC) {
      auto *arcToPart = dynamic_cast<CPathShapeArc *>(part);

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
CQIllustratorPathShape::
drawShape(CQIllustratorShapeDrawer *drawer) const
{
  uint num_parts = parts_.size();

  if (num_parts == 0) return;

  const CMatrix2D &m = getFlatMatrix();

  drawer->pathInit();

  for (uint i = 0; i < num_parts; ++i) {
    const auto *part = parts_[i];

    auto type = part->getType();

    if      (type == CPATH_PART_TYPE_MOVE_TO) {
      const auto *moveToPart = dynamic_cast<const CPathShapeMoveTo *>(part);

      CPoint2D p1 = m*moveToPart->getPoint();

      drawer->pathMoveTo(p1);
    }
    else if (type == CPATH_PART_TYPE_LINE_TO) {
      const auto *lineToPart = dynamic_cast<const CPathShapeLineTo *>(part);

      CPoint2D p1 = m*lineToPart->getPoint();

      drawer->pathLineTo(p1);
    }
    else if (type == CPATH_PART_TYPE_CURVE2_TO) {
      const auto *curveToPart = dynamic_cast<const CPathShapeCurve2To *>(part);

      CPoint2D p11 = m*curveToPart->getPoint1();
      CPoint2D p22 = m*curveToPart->getPoint2();

      drawer->pathBezierTo(p11, p22);
    }
    else if (type == CPATH_PART_TYPE_CURVE3_TO) {
      const auto *curveToPart = dynamic_cast<const CPathShapeCurve3To *>(part);

      const CPoint2D &p1 = curveToPart->getPoint1();
      const CPoint2D &p2 = curveToPart->getPoint2();
      const CPoint2D &p3 = curveToPart->getPoint3();

      CPoint2D p11 = m*p1;
      CPoint2D p22 = m*p2;
      CPoint2D p33 = m*p3;

      drawer->pathBezierTo(p11, p22, p33);
    }
    else if (type == CPATH_PART_TYPE_ARC) {
      const auto *arcToPart = dynamic_cast<const CPathShapeArc *>(part);

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
CQIllustratorPathShape::
getGeometry()
{
  return new CQIllustratorPathGeometry(fixed_, rcenter_, m_, parts_);
}

void
CQIllustratorPathShape::
doSetGeometry(const CQIllustratorShapeGeometry *geom)
{
  const auto *geom1 = dynamic_cast<const CQIllustratorPathGeometry *>(geom);

  if (! geom1) return;

  fixed_   = geom1->fixed_;
  rcenter_ = geom1->rcenter_;
  m_       = geom1->m_;
  parts_   = geom1->parts_;

  invalidateBBox();
}

class CPathShapeSVGProcess : public CPathShapeProcess {
 public:
  CPathShapeSVGProcess(const CQIllustratorSaveData &saveData, CQIllustratorPathShape *path) :
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
  CFile                  *file_;
  CQIllustratorPathShape *path_;
  std::string             sep_;
};

void
CQIllustratorPathShape::
saveSVG(const CQIllustratorSaveData &saveData)
{
  CPathShapeSVGProcess process(saveData, this);

  process.init();

  processPath(&process);

  process.term();
}

void
CQIllustratorPathShape::
saveCmd(const CQIllustratorSaveData & /*saveData*/)
{
}

void
CQIllustratorPathShape::
processPath(CPathShapeProcess *process)
{
  parts_.processPath(process);
}
