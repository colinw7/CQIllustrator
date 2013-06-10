#include <CQFont.h>
#include <CConfig.h>
#include <CFontMgr.h>
#include <CQImage.h>

#include <QPainter>

CQFontMgr *
CQFontMgr::
getInstance()
{
  static CQFontMgr *instance;

  if (! instance)
    instance = new CQFontMgr;

  return instance;
}

CQFontMgr::
CQFontMgr()
{
  config_ = new CConfig("CQFont");
}

void
CQFontMgr::
setPrototype()
{
  CFontPtr font(new CQFont("courier", CFONT_STYLE_NORMAL, 12));

  CFontMgrInst->setPrototype(font);
}

CFontPtr
CQFontMgr::
lookupFont(const QFont &qfont)
{
  CFontStyle style = CFONT_STYLE_NORMAL;

  if      (qfont.bold() && qfont.italic())
    style = CFONT_STYLE_BOLD_ITALIC;
  else if (qfont.bold())
    style = CFONT_STYLE_BOLD;
  else if (qfont.italic())
    style = CFONT_STYLE_ITALIC;

  return lookupFont(qfont.family().toStdString(), style, getPixelSize(qfont));
}

CFontPtr
CQFontMgr::
lookupFont(const std::string &family, CFontStyle style, double size, double angle,
           double char_angle, uint x_res, uint y_res)
{
  CFontPtr font = CFontMgrInst->lookupFont(family, style, size, angle, char_angle, x_res, y_res);

  assert(font.cast<CQFont>() != NULL);

  return font;
}

std::string
CQFontMgr::
lookupFamily(const std::string &family)
{
  std::string value;

  if (config_->getValue(family, value) && value != "")
    return value;

  return family;
}

CFontStyle
CQFontMgr::
fontStyle(const QFont &qfont)
{
  CFontStyle fontStyle = CFONT_STYLE_NORMAL;

  if (qfont.bold()) {
    if (qfont.italic())
      fontStyle = CFONT_STYLE_BOLD_ITALIC;
    else
      fontStyle = CFONT_STYLE_BOLD;
  }
  else {
    if (qfont.italic())
      fontStyle = CFONT_STYLE_ITALIC;
  }

  return fontStyle;
}

int
CQFontMgr::
getPixelSize(const QFont &qfont)
{
  int size = qfont.pixelSize();

  if (size <= 0)
    size = qfont.pointSize();

  return size;
}

//----------

CQFont::
CQFont(const std::string &family, CFontStyle style, double size, double angle, double char_angle,
       int x_res, int y_res) :
 CFont(family, style, size, angle, char_angle, x_res, y_res)
{
  int weight = QFont::Normal;

  if (style & CFONT_STYLE_BOLD)
    weight = QFont::Bold;

  bool italic = (style & CFONT_STYLE_ITALIC);

  std::string qfamily = CQFontMgrInst->lookupFamily(family);

  qfont_ = new QFont(qfamily.c_str(), int(size), weight, italic);

  qmetrics_ = new QFontMetricsF(*qfont_);
}

CQFont::
CQFont(const std::string &full_name) :
 CFont(full_name), qfont_(NULL), qmetrics_(NULL)
{
}

CQFont::
CQFont(const QFont &qfont) :
 CFont(qfont.family().toStdString(), CQFontMgr::fontStyle(qfont), CQFontMgr::getPixelSize(qfont))
{
  qfont_ = new QFont(qfont);

  qmetrics_ = new QFontMetricsF(*qfont_);
}

CQFont::
CQFont(const CQFont &qfont) :
 CFont(qfont)
{
  qfont_ = new QFont(*qfont.qfont_);

  qmetrics_ = new QFontMetricsF(*qfont_);
}

CQFont::
~CQFont()
{
  delete qfont_;
  delete qmetrics_;
}

CFontPtr
CQFont::
dup(const std::string &family, CFontStyle style, double size, double angle, double char_angle,
    int x_res, int y_res) const
{
  CQFont *qfont = new CQFont(family, style, size, angle, char_angle, x_res, y_res);

  return CFontPtr(qfont);
}

const CQFont &
CQFont::
operator=(const CQFont &qfont)
{
  qfont_ = new QFont(*qfont.qfont_);

  qmetrics_ = new QFontMetricsF(*qfont_);

  return *this;
}

double
CQFont::
getCharWidth() const
{
  return getQMetrics().maxWidth();
}

double
CQFont::
getCharAscent() const
{
  return getQMetrics().ascent();
}

double
CQFont::
getCharDescent() const
{
  return getQMetrics().descent();
}

double
CQFont::
getCharHeight() const
{
  return getQMetrics().height();
}

double
CQFont::
getStringWidth(const std::string &str) const
{
  QString qstr(str.c_str());

  return getQMetrics().size(0, qstr).width();
}

bool
CQFont::
isProportional() const
{
  return ! qfont_->fixedPitch();
}

#if 0
CImagePtr
CQFont::
getStringImage(const std::string &str)
{
  uint w = getIStringWidth(str);
  uint h = getICharHeight();

  CImageNameSrc src("CQFont::getStringImage");

  CImagePtr image = CImageMgrInst->createImage(src);

  image->setDataSize(w, h);

  CQImage *qimage = image.cast<CQImage>();

  qimage->updateCImage();

  QPainter painter(&qimage->getQImage());

  qimage->updateCImage();

  QPen   pen;
  QBrush brush;

  brush.setColor(Qt::transparent);

  brush.setStyle(Qt::NoBrush);

  painter.fillRect(0, 0, w, h, brush);

  qimage->updateCImage();

  pen.setColor(QColor(0,0,0));

  painter.setPen  (pen);
  painter.setBrush(brush);

  painter.setFont(*qfont_);

  int ascent = getICharAscent();

  painter.drawText(0, ascent, QString(str.c_str()));

  qimage->updateCImage();

  return image;
}
#endif
