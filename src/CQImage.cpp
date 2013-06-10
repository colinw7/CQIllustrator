#include <CQImage.h>
#include <CQUtil.h>
#include <CImageLib.h>
#include <QImage>

void
CQImage::
setPrototype()
{
  CImagePtr image(new CQImage());

  CImageMgrInst->setPrototype(image);
}

CQImage::
CQImage() :
 initialized_(false)
{
}

CQImage::
CQImage(int width, int height) :
 CImage(width, height), initialized_(false)
{
}

CQImage::
CQImage(const CImage &image) :
 CImage(image), initialized_(false)
{
}

void
CQImage::
initQImage()
{
  int x1, y1, x2, y2;

  getWindow(&x1, &y1, &x2, &y2);

  uint width  = x2 - x1 + 1;
  uint height = y2 - y1 + 1;

  if (qimage_.isNull() ||
      qimage_.width () != (int) width ||
      qimage_.height() != (int) height)
    qimage_ = QImage(width, height, QImage::Format_ARGB32);

  CRGBA rgba;

  for (int y = y1; y <= y2; ++y) {
    for (int x = x1; x <= x2; ++x) {
      getRGBAPixel(x, y, rgba);

      qimage_.setPixel(x - x1, y - y1, CQUtil::rgbaToColor(rgba).rgba());
    }
  }

  initialized_ = true;
}

void
CQImage::
updateCImage()
{
  if (! initialized_)
    return;

  uint width  = qimage_.width ();
  uint height = qimage_.height();

  if (getWidth() != width || getHeight() != height)
    CImage::setDataSizeV(width, height);

  CRGBA rgba;

  for (uint y = 0; y < height; ++y) {
    for (uint x = 0; x < width; ++x) {
      rgba = CQUtil::colorToRGBA(QColor::fromRgba(qimage_.pixel(x, y)));

      setRGBAPixel(x, y, rgba);
    }
  }
}

CQImage::
CQImage(const QImage &qimage)
{
  qimage_      = qimage;
  initialized_ = true;

  updateCImage();
}

CQImage::
CQImage(const CQImage &qimage) :
 CImage(qimage)
{
  qimage_      = qimage.qimage_;
  initialized_ = qimage.initialized_;
}

CQImage::
~CQImage()
{
}

CQImage &
CQImage::
operator=(const CImage &image)
{
  const CQImage &qimage = dynamic_cast<const CQImage &>(image);

  qimage_      = qimage.qimage_;
  initialized_ = qimage.initialized_;

  updateCImage();

  return *this;
}

CQImage &
CQImage::
operator=(const CQImage &qimage)
{
  qimage_      = qimage.qimage_;
  initialized_ = qimage.initialized_;

  updateCImage();

  return *this;
}

CQImage &
CQImage::
operator=(const QImage &qimage)
{
  qimage_      = qimage;
  initialized_ = true;

  updateCImage();

  return *this;
}

CImagePtr
CQImage::
dup() const
{
  return CImagePtr(new CQImage(*this));
}

void
CQImage::
setImage(const QImage &qimage)
{
  qimage_      = qimage;
  initialized_ = true;

  updateCImage();
}

QImage &
CQImage::
getQImage()
{
  if (! initialized_)
    initQImage();

  return qimage_;
}

QImage *
CQImage::
getQImageP()
{
  if (! initialized_)
    initQImage();

  return &qimage_;
}

void
CQImage::
setDataSizeV(int width, int height)
{
  qimage_ = QImage(width, height, QImage::Format_ARGB32);

  CImage::setDataSizeV(width, height);
}

bool
CQImage::
setColorIndexPixel(int pos, uint pixel)
{
  int w = getWidth();

  setColorIndexPixel(pos % w, pos / w, pixel);

  return true;
}

bool
CQImage::
setColorIndexPixel(int x, int y, uint pixel)
{
  int w = getWidth();

  if (initialized_)
    qimage_.setPixel(QPoint(x, y), pixel);

  CImage::setColorIndexPixel(y*w + x, pixel);

  return true;
}

bool
CQImage::
setRGBAPixel(int pos, const CRGBA &rgba)
{
  int w = getWidth();

  setRGBAPixel(pos % w, pos / w, rgba);

  return true;
}

bool
CQImage::
setRGBAPixel(int x, int y, const CRGBA &rgba)
{
  int w = getWidth();

  if (initialized_)
    qimage_.setPixel(QPoint(x, y), CQUtil::rgbaToColor(rgba).rgba());

  CImage::setRGBAPixel(y*w + x, rgba);

  return true;
}

void
CQImage::
dataChanged()
{
  initialized_ = false;
}
