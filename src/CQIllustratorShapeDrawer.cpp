#include <CQIllustratorShapeDrawer.h>
#include <CQIllustratorCanvas.h>
#include <CQIllustrator.h>

#include <CQImage.h>
#include <CQUtil.h>
#include <QPainter>

#include <xpm/control_point.xpm>
#include <xpm/control_point_active.xpm>
#include <xpm/curve_point.xpm>
#include <xpm/curve_point_active.xpm>

#define IMAGE_DATA(I) I, sizeof(I)/sizeof(char *)

CQIllustratorShapeDrawer::
CQIllustratorShapeDrawer(CQIllustrator *illustrator, QPainter *painter) :
 illustrator_(illustrator),
 painter_    (painter),
 bbox_       (),
 path_       (0),
 qfont_      (),
 m_          (CMATRIX_TYPE_IDENTITY),
 matrices_   ()
{
  path_ = new QPainterPath;

  control_point_image_ =
    CImageMgrInst->createImage(CImageXPMSrc(IMAGE_DATA(control_point_data)));
  control_point_active_image_ =
    CImageMgrInst->createImage(CImageXPMSrc(IMAGE_DATA(control_point_active_data)));

  curve_point_image_ =
    CImageMgrInst->createImage(CImageXPMSrc(IMAGE_DATA(curve_point_data)));
  curve_point_active_image_ =
    CImageMgrInst->createImage(CImageXPMSrc(IMAGE_DATA(curve_point_active_data)));

  qi1_control_ = control_point_image_       .cast<CQImage>()->getQImage();
  qi2_control_ = control_point_active_image_.cast<CQImage>()->getQImage();
  qi1_curve_   = curve_point_image_         .cast<CQImage>()->getQImage();
  qi2_curve_   = curve_point_active_image_  .cast<CQImage>()->getQImage();
}

CQIllustratorShapeDrawer::
~CQIllustratorShapeDrawer()
{
  delete path_;
}

void
CQIllustratorShapeDrawer::
setPainter(QPainter *painter)
{
  painter_ = painter;
}

void
CQIllustratorShapeDrawer::
setBBox(const CBBox2D &bbox)
{
  bbox_ = bbox;
}

void
CQIllustratorShapeDrawer::
setStroke(const CQIllustratorShape *shape, const CQIllustratorShapeStroke &stroke)
{
  illustrator_->setStroke(painter_, shape, stroke);
}

void
CQIllustratorShapeDrawer::
setFill(const CQIllustratorShape *shape, const CQIllustratorShapeFill &fill)
{
  bbox_ = shape->getBBox();

  illustrator_->setFill(painter_, shape, fill, bbox_);
}

void
CQIllustratorShapeDrawer::
setFont(CFontPtr font)
{
  qfont_ = CQUtil::toQFont(font);

  painter_->setFont(qfont_);
}

void
CQIllustratorShapeDrawer::
pushMatrix(const CMatrix2D &m, bool combine)
{
  matrices_.push_back(m_);

  QTransform t1;

  if (combine)
    m_ = m_*m;
  else
    m_ = m;

  QTransform t = CQUtil::toQTransform(m_);

  //painter_->setWorldTransform(t);
  painter_->setTransform(t);
}

void
CQIllustratorShapeDrawer::
popMatrix()
{
  m_ = matrices_.back();

  matrices_.pop_back();

  QTransform t = CQUtil::toQTransform(m_);

  //painter_->setWorldTransform(t);
  painter_->setTransform(t);
}

void
CQIllustratorShapeDrawer::
drawText(const CBBox2D &bbox, const std::string &str, CHAlignType halign, CVAlignType valign)
{
  //QTransform transform = painter_->worldTransform();
  QTransform transform = painter_->transform();

  const CPoint2D &ll = bbox.getLL();
  const CPoint2D &ur = bbox.getUR();

  QPointF p1 = transform.map(QPointF(ll.x, ll.y));
  QPointF p2 = transform.map(QPointF(ur.x, ur.y));

  double fs = qfont_.pointSizeF();

  int pixels_per_inch = painter_->device()->physicalDpiX();
//int pixels_per_inch = painter_->device()->logicalDpiX();

  fs = pixels_per_inch*fs/72.0;

  QPointF pf1 = transform.map(QPointF(0 , 0 ));
  QPointF pf2 = transform.map(QPointF(fs, fs));

  double fs1 = std::max(fabs(pf2.x() - pf1.x()), fabs(pf2.y() - pf1.y()));

  QString qstr(str.c_str());

  QRectF qrect(std::min(p1.x(), p2.x()), std::min(p1.y(), p2.y()),
               fabs(p2.x() - p1.x()), fabs(p2.y() - p1.y()));

  painter_->setWorldMatrixEnabled(false);

  QFont qfont = qfont_;

  int ifs = std::max(1, int(fs1));

  qfont.setPixelSize(ifs);

  int flags = CQUtil::toQAlign(halign) | CQUtil::toQAlign(valign);

  painter_->setFont(qfont);

  painter_->drawText(qrect, flags, qstr);

  painter_->setWorldMatrixEnabled(true);
}

void
CQIllustratorShapeDrawer::
pathInit()
{
  delete path_;

  path_ = new QPainterPath;
}

void
CQIllustratorShapeDrawer::
pathMoveTo(const CPoint2D &p)
{
  path_->moveTo(p.x, p.y);
}

void
CQIllustratorShapeDrawer::
pathLineTo(const CPoint2D &p)
{
  path_->lineTo(p.x, p.y);
}

void
CQIllustratorShapeDrawer::
pathBezierTo(const CPoint2D &p1, const CPoint2D &p2)
{
  path_->quadTo(CQUtil::toQPoint(p1), CQUtil::toQPoint(p2));
}

void
CQIllustratorShapeDrawer::
pathBezierTo(const CPoint2D &p1, const CPoint2D &p2, const CPoint2D &p3)
{
  path_->cubicTo(CQUtil::toQPoint(p1), CQUtil::toQPoint(p2), CQUtil::toQPoint(p3));
}

void
CQIllustratorShapeDrawer::
pathArc(const CPoint2D &c, double xr, double yr, double angle1, double angle2)
{
  QRectF rect(QPointF(c.x - xr, c.y - yr), QPointF(c.x + xr, c.y + yr));

  double a1 = -CMathGen::RadToDeg(angle1);
  double a2 = -CMathGen::RadToDeg(angle2);

  path_->arcTo(rect, a1, a2 - a1);
}

void
CQIllustratorShapeDrawer::
pathText(const CBBox2D &rect, const std::string &str, CHAlignType halign, CVAlignType valign)
{
  double fs = qfont_.pointSizeF();

  int pixels_per_inch = painter_->device()->physicalDpiX();

  fs = pixels_per_inch*fs/24.0;

  //QTransform transform = painter_->worldTransform();
  QTransform transform = painter_->transform();

  QPointF pf1 = transform.map(QPointF(0 , 0 ));
  QPointF pf2 = transform.map(QPointF(fs, fs));

  double fs1 = std::max(fabs(pf2.x() - pf1.x()), fabs(pf2.y() - pf1.y()));

  int ifs = std::max(1, int(fs1));

  QFont qfont = qfont_;

  qfont.setPixelSize(ifs);

  std::vector<std::string> words;

  CStrUtil::addFields(str, words, "\n", false);

  uint num_words = words.size();

  std::vector<double> widths, heights;

  QPainterPath path1;

  path1.addText(QPoint(0, 0), qfont, "qQ");

  QRectF qrect = path1.boundingRect();

  double hq = qrect.height();

  double w = 0;
  double h = 0;

  w  = std::max(w, qrect.width());
  h += qrect.height();

  for (uint i = 0; i < num_words; ++i) {
    QString qstr(words[num_words - i - 1].c_str());

    QPainterPath path1;

    path1.addText(QPoint(0, 0), qfont, qstr);

    QRectF qrect = path1.boundingRect();

    w = std::max(w, qrect.width());

    if (i == 0 || i == num_words - 1)
      h += qrect.height();
    else
      h += hq;

    widths .push_back(qrect.width ());
    heights.push_back(qrect.height());
  }

  double x = rect.getXMin();
  double y = rect.getYMin();

  double dx = rect.getWidth () - w;
  double dy = rect.getHeight() - h;

  if      (halign == CHALIGN_TYPE_LEFT  ) { }
  else if (halign == CHALIGN_TYPE_CENTER) x += dx/2;
  else if (halign == CHALIGN_TYPE_RIGHT ) x += dx;

  if      (valign == CVALIGN_TYPE_BOTTOM) { }
  else if (valign == CVALIGN_TYPE_CENTER) y += dy/2;
  else if (valign == CVALIGN_TYPE_TOP   ) y += dy;

  QTransform transform1;

  transform1.scale(1, -1);

  //painter_->setWorldTransform(transform1, true);
  painter_->setTransform(transform1, true);

  for (uint i = 0; i < num_words; ++i) {
    QString qstr(words[num_words - i - 1].c_str());

    double dw1 = w  - widths [i];
    //double dh1 = hq - heights[i];

    double dx1 = 0;

    if      (halign == CHALIGN_TYPE_LEFT  ) { }
    else if (halign == CHALIGN_TYPE_CENTER) dx1 = dw1/2;
    else if (halign == CHALIGN_TYPE_RIGHT ) dx1 = dw1;

    path_->addText(QPoint(x + dx1, -y), qfont, qstr);

    y += hq;
  }
}

void
CQIllustratorShapeDrawer::
pathClose()
{
  path_->closeSubpath();
}

void
CQIllustratorShapeDrawer::
pathStroke()
{
  painter_->strokePath(*path_, painter_->pen());
}

void
CQIllustratorShapeDrawer::
pathFill(CFillType type)
{
  if (type == FILL_TYPE_EVEN_ODD)
    path_->setFillRule(Qt::OddEvenFill);
  else
    path_->setFillRule(Qt::WindingFill);

  painter_->fillPath(*path_, painter_->brush());
}

void
CQIllustratorShapeDrawer::
pathClip()
{
  painter_->setClipPath(*path_);
}

void
CQIllustratorShapeDrawer::
drawControlLine(const CPoint2D &p1, const CPoint2D &p2)
{
  QPen pen;

  pen.setColor(QColor(255,0,0));
  pen.setStyle(Qt::DashLine);

  painter_->setPen(pen);
  painter_->setBrush(Qt::NoBrush);

  painter_->drawLine(CQUtil::toQPoint(p1), CQUtil::toQPoint(p2));
}

void
CQIllustratorShapeDrawer::
drawGridLine(const CPoint2D &p1, const CPoint2D &p2)
{
  QPen pen;

  pen.setColor(QColor(100,100,255));
  pen.setStyle(Qt::DashLine);

  painter_->setPen(pen);
  painter_->setBrush(Qt::NoBrush);

  painter_->drawLine(CQUtil::toQPoint(p1), CQUtil::toQPoint(p2));
}

void
CQIllustratorShapeDrawer::
drawGridSubLine(const CPoint2D &p1, const CPoint2D &p2)
{
  QPen pen;

  pen.setColor(QColor(200,200,255));
  pen.setStyle(Qt::DashLine);

  painter_->setPen(pen);
  painter_->setBrush(Qt::NoBrush);

  painter_->drawLine(CQUtil::toQPoint(p1), CQUtil::toQPoint(p2));
}

#if 0
void
CQIllustratorShapeDrawer::
drawControlPointNormal(const CPoint2D &point, bool selected)
{
  int ys = (illustrator_->getFlipY() ? -1 : 1);

  //QTransform transform = painter_->worldTransform();
  QTransform transform = painter_->transform();

  QPointF p = transform.map(CQUtil::toQPoint(point)) - QPointF(3, -3*ys);

  painter_->setWorldMatrixEnabled(false);

  if (selected)
    painter_->drawImage(p, qi2_control_);
  else
    painter_->drawImage(p, qi1_control_);

  painter_->setWorldMatrixEnabled(true);
}

void
CQIllustratorShapeDrawer::
drawControlPointControl(const CPoint2D &point, bool selected)
{
  int ys = (illustrator_->getFlipY() ? -1 : 1);

  //QTransform transform = painter_->worldTransform();
  QTransform transform = painter_->transform();

  QPointF p = transform.map(CQUtil::toQPoint(point)) - QPointF(2, -2*ys);

  painter_->setWorldMatrixEnabled(false);

  if (selected)
    painter_->drawImage(p, qi2_curve_);
  else
    painter_->drawImage(p, qi1_curve_);

  painter_->setWorldMatrixEnabled(true);
}
#endif

void
CQIllustratorShapeDrawer::
drawImage(int x, int y, QImage image)
{
  painter_->setWorldMatrixEnabled(false);

  painter_->drawImage(x, y, image);

  painter_->setWorldMatrixEnabled(true);
}

//----------------

CQIllustratorFilterShapeDrawer::
CQIllustratorFilterShapeDrawer(CQIllustratorShapeDrawer *drawer, const CQIllustratorShape *shape) :
 CQIllustratorShapeDrawer(drawer->getIllustrator()), shape_(shape)
{
  QPainter *painter = drawer->getPainter();

  qimage_ = QImage(painter->device()->width(), painter->device()->height(), QImage::Format_ARGB32);

  qimage_.fill(qRgba(0,0,0,0));

  painter_ = new QPainter(&qimage_);

  //painter_->setWorldTransform(painter->worldTransform());
  painter_->setTransform(painter->transform());

  bbox_ = shape_->getBBox();
}

CQIllustratorFilterShapeDrawer::
~CQIllustratorFilterShapeDrawer()
{
  delete painter_;
}

QImage &
CQIllustratorFilterShapeDrawer::
getImage()
{
  return qimage_;
}
