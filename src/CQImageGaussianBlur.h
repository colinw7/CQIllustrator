#ifndef CQIMAGE_GAUSSIAN_BLUR_H
#define CQIMAGE_GAUSSIAN_BLUR_H

#include <QImage>
#include <CGaussianBlur.h>

class CQImageWrapper : public QImage {
 public:
  CQImageWrapper(QImage &image) :
   image_(image) {
  }

  void getPixelRange(int *x1, int *y1, int *x2, int *y2) const {
   *x1 = 0;
   *y1 = 0;
   *x2 = image_.width () - 1;
   *y2 = image_.height() - 1;
  }

  void getRGBA(int x, int y, double *r, double *g, double *b, double *a) const {
    QRgb rgb = image_.pixel(x, y);

    *r = qRed  (rgb)/255.0;
    *g = qGreen(rgb)/255.0;
    *b = qBlue (rgb)/255.0;
    *a = qAlpha(rgb)/255.0;
  }

  void setRGBA(int x, int y, double r, double g, double b, double a) {
    QRgb rgb = qRgba(int(r*255), int(g*255), int(b*255), int(a*255));

    image_.setPixel(x, y, rgb);
  }

 private:
  friend class CQImageGaussianBlur;

  QImage &image_;
};

class CQImageGaussianBlur {
 public:
  CQImageGaussianBlur(QImage image) :
   image_(image) {
  }

  void blur(QImage &dst, double bx, double by, int nx, int ny) {
    CGaussianBlur<CQImageWrapper> blur;

    CQImageWrapper dst1(dst);

    blur.blur(image_, dst1, bx, by, nx, ny);
  }

 private:
  CQImageWrapper image_;
};

#endif
