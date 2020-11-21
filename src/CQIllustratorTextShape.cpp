#include <CQIllustratorTextShape.h>
#include <CQIllustratorTextGeometry.h>
#include <CQIllustratorShapeDrawer.h>
#include <CQIllustrator.h>
#include <CQUtil.h>
#include <cassert>

CQIllustratorTextShape::
CQIllustratorTextShape(const CPoint2D &p1, const CPoint2D &p2, const std::string &str,
                       CFontPtr font, CHAlignType halign, CVAlignType valign) :
 CQIllustratorShape(), p1_(p1), p2_(p2), str_(str), font_(font), halign_(halign), valign_(valign)
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

QString
CQIllustratorTextShape::
getQText() const
{
  return str_.c_str();
}

void
CQIllustratorTextShape::
setQText(const QString &text)
{
  str_ = text.toStdString();
}

void
CQIllustratorTextShape::
getControlPoints(ControlPointList &points, ControlType type) const
{
  if (type == ControlType::GEOMETRY) {
    const CMatrix2D &m = getFlatMatrix();

    CPoint2D p1 = m*p1_;
    CPoint2D p2 = m*p2_;

    CQIllustratorTextShapeControlPoint *ll =
      new CQIllustratorTextShapeControlPoint(ControlPointType::LL, p1);
    CQIllustratorTextShapeControlPoint *ur =
      new CQIllustratorTextShapeControlPoint(ControlPointType::UR, p2);

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

  if      (type == CQIllustratorTextShape::ControlPointType::LL)
    p1_ = p;
  else if (type == CQIllustratorTextShape::ControlPointType::UR)
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

QPointF
CQIllustratorTextShape::
getQLL() const
{
  return CQUtil::toQPoint(getLL());
}

void
CQIllustratorTextShape::
setQLL(const QPointF &p)
{
  setLL(CQUtil::fromQPoint(p));
}

QPointF
CQIllustratorTextShape::
getQUR() const
{
  return CQUtil::toQPoint(getUR());
}

void
CQIllustratorTextShape::
setQUR(const QPointF &p)
{
  setUR(CQUtil::fromQPoint(p));
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

  checkoutShape(CQIllustratorData::ChangeType::GEOMETRY);

  const CMatrix2D &m = getFlatMatrix();

  CMatrix2D im = m.inverse();

  p1_ = im*(m*p1_ + d);
  p2_ = im*(m*p2_ + d);

  invalidateBBox();

  checkinShape(CQIllustratorData::ChangeType::GEOMETRY);
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

  if (drawer->getIllustrator()->getFlipY()) {
    CPoint2D pb1 = p4 + f*(p1 - p4);
    CPoint2D pb2 = p3 + f*(p2 - p3);

    drawer->drawControlLine(pb1, pb2);
  }
  else {
    CPoint2D pb1 = p1 + f*(p4 - p1);
    CPoint2D pb2 = p2 + f*(p3 - p2);

    drawer->drawControlLine(pb1, pb2);
  }
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

  if      (type_ == CQIllustratorTextShape::ControlPointType::LL)
    return text->getLL();
  else if (type_ == CQIllustratorTextShape::ControlPointType::UR)
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

  if      (type_ == CQIllustratorTextShape::ControlPointType::LL)
    text->setLL(point);
  else if (type_ == CQIllustratorTextShape::ControlPointType::UR)
    text->setUR(point);
  else
    assert(false);
}
