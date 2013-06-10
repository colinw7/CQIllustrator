#ifndef CDISPLAY_RANGE_2D_H
#define CDISPLAY_RANGE_2D_H

#include <CMathGen.h>
#include <CRange2D.h>
#include <CIRange2D.h>
#include <CMatrix2D.h>
#include <CBBox2D.h>
#include <CAlignType.h>

// Class to represent a 2D mapping from window to pixel coordinates
class CDisplayRange2D {
 public:
  struct IRange {
   int xmin, ymin, xmax, ymax;

   IRange(int xmin1=0, int ymin1=0, int xmax1=0, int ymax1=0) :
    xmin(xmin1), ymin(ymin1), xmax(xmax1), ymax(ymax1) {
   }

   void set(int xmin1, int ymin1, int xmax1, int ymax1) {
     xmin = xmin1; ymin = ymin1; xmax = xmax1; ymax = ymax1;
   }

   void get(int *xmin1, int *ymin1, int *xmax1, int *ymax1) const {
     *xmin1 = xmin; *ymin1 = ymin; *xmax1 = xmax; *ymax1 = ymax;
   }

   int dx() const { return xmax - xmin; }
   int dy() const { return ymax - ymin; }

   int xmid() const { return (xmin + xmax)/2 ; }
   int ymid() const { return (ymin + ymax)/2; }

   void incX(int dx) { xmin += dx; xmax += dx; }
   void incY(int dy) { ymin += dy; ymax += dy; }
  };

  struct RRange {
   double xmin, ymin, xmax, ymax;

   RRange(double xmin1=0, double ymin1=0, double xmax1=0, double ymax1=0) :
    xmin(xmin1), ymin(ymin1), xmax(xmax1), ymax(ymax1) {
   }

   void set(double xmin1, double ymin1, double xmax1, double ymax1) {
     xmin = xmin1; ymin = ymin1; xmax = xmax1; ymax = ymax1;
   }

   void get(double *xmin1, double *ymin1, double *xmax1, double *ymax1) const {
     *xmin1 = xmin; *ymin1 = ymin; *xmax1 = xmax; *ymax1 = ymax;
   }

   double dx() const { return xmax - xmin; }
   double dy() const { return ymax - ymin; }

   double xmid() const { return (xmin + xmax)/2 ; }
   double ymid() const { return (ymin + ymax)/2; }

   void incX(double dx) { xmin += dx; xmax += dx; }
   void incY(double dy) { ymin += dy; ymax += dy; }
  };

 public:
  CDisplayRange2D(int    pixel_xmin  =   0, int    pixel_ymin  =   0,
                  int    pixel_xmax  = 100, int    pixel_ymax  = 100,
                  double window_xmin = 0.0, double window_ymin = 0.0,
                  double window_xmax = 1.0, double window_ymax = 1.0) :
   pixel_ (pixel_xmin , pixel_ymin , pixel_xmax , pixel_ymax ),
   window_(window_xmin, window_ymin, window_xmax, window_ymax),
   window1_(window_), window_xc1_(0.0), window_yc1_(0.0), window_width1_(1.0), window_height1_(1.0),
   factor_x_(1.0), factor_y_(1.0), factor_x1_(1.0), factor_y1_(1.0), pdx_(0.0), pdy_(0.0),
   equal_scale_(false), scale_min_(true),
   halign_(CHALIGN_TYPE_CENTER), valign_(CVALIGN_TYPE_CENTER),
   flip_x_(false), flip_y_(false), matrix_(), imatrix_() {
    reset();
  }

  void setPixelRange(int pixel_xmin, int pixel_ymin, int pixel_xmax, int pixel_ymax) {
    pixel_.set(pixel_xmin, pixel_ymin, pixel_xmax, pixel_ymax);

    reset();
  }

  void setWindowRange(double window_xmin, double window_ymin,
                      double window_xmax, double window_ymax) {
    window_.set(window_xmin, window_ymin, window_xmax, window_ymax);

    reset();
  }

  void getPixelRange(int *pixel_xmin, int *pixel_ymin, int *pixel_xmax, int *pixel_ymax) const {
    pixel_.get(pixel_xmin, pixel_ymin, pixel_xmax, pixel_ymax);
  }

  int getPixelWidth() const {
    return pixel_.dx();
  }

  int getPixelHeight() const {
    return pixel_.dy();
  }

  void getWindowRange(double *window_xmin, double *window_ymin,
                      double *window_xmax, double *window_ymax) const {
    window_.get(window_xmin, window_ymin, window_xmax, window_ymax);
  }

  void getWindowRange(CBBox2D &bbox) const {
    double window_xmin, window_ymin, window_xmax, window_ymax;

    getWindowRange(&window_xmin, &window_ymin, &window_xmax, &window_ymax);

    bbox = CBBox2D(CPoint2D(window_xmin, window_ymin), CPoint2D(window_xmax, window_ymax));
  }

  double getWindowWidth() const {
    return window_.dx();
  }

  double getWindowHeight() const {
    return window_.dy();
  }

  CPoint2D getWindowCenter() const {
    return CPoint2D(window_.xmid(), window_.ymid());
  }

  void setEqualScale(bool flag) {
    equal_scale_ = flag;

    recalc();
  }

  bool getEqualScale() const { return equal_scale_; }

  void setScaleMin(bool flag) {
    scale_min_ = flag;

    recalc();
  }

  bool getScaleMin() const { return scale_min_; }

  void setHAlign(CHAlignType halign) {
    halign_ = halign;

    recalc();
  }

  void setVAlign(CVAlignType valign) {
    valign_ = valign;

    recalc();
  }

  void setAlign(CHAlignType halign, CVAlignType valign) {
    halign_ = halign;
    valign_ = valign;

    recalc();
  }

  CHAlignType getHAlign() const { return halign_; }
  CVAlignType getVAlign() const { return valign_; }

  bool getFlipX() const { return flip_x_; }
  bool getFlipY() const { return flip_y_; }

  void zoomIn(double factor) {
    zoomOut(1.0/factor);
  }

  void zoomOut(double factor) {
    double window_hwidth  = 0.5*window_width1_ *factor;
    double window_hheight = 0.5*window_height1_*factor;

    zoomTo(window_xc1_ - window_hwidth, window_yc1_ - window_hheight,
           window_xc1_ + window_hwidth, window_yc1_ + window_hheight);
  }

  void zoomTo(double window_xmin, double window_ymin, double window_xmax, double window_ymax) {
    window1_.set(window_xmin, window_ymin, window_xmax, window_ymax);

    recalc();
  }

  void scroll(double offset_x, double offset_y) {
    scrollX(offset_x);
    scrollY(offset_y);
  }

  void scrollX(double offset_x) {
    window1_.incX(offset_x);

    recalc();
  }

  void scrollY(double offset_y) {
    window1_.incY(offset_y);

    recalc();
  }

  void reset() {
    window1_ = window_;

    recalc();
  }

  void recalc() {
    window_xc1_ = window1_.xmid();
    window_yc1_ = window1_.ymid();

    window_width1_  = window1_.dx();
    window_height1_ = window1_.dy();

    factor_x_ =  pixel_.dx()/window_width1_ ;
    factor_y_ = -pixel_.dy()/window_height1_;

    if (equal_scale_) {
      factor_x1_ = factor_x_;
      factor_y1_ = factor_y_;

      if (scale_min_) {
        if (fabs(factor_x1_) > fabs(factor_y1_)) {
          if (factor_x1_ < 0)
            factor_x1_ = -fabs(factor_y1_);
          else
            factor_x1_ =  fabs(factor_y1_);
        }
        else {
          if (factor_y1_ < 0)
            factor_y1_ = -fabs(factor_x1_);
          else
            factor_y1_ =  fabs(factor_x1_);
        }
      }
      else {
        if (fabs(factor_x1_) < fabs(factor_y1_)) {
          if (factor_x1_ < 0)
            factor_x1_ = -fabs(factor_y1_);
          else
            factor_x1_ =  fabs(factor_y1_);
        }
        else {
          if (factor_y1_ < 0)
            factor_y1_ = -fabs(factor_x1_);
          else
            factor_y1_ =  fabs(factor_x1_);
        }
      }

      double px, py;

      pdx_ = 0;
      pdy_ = 0;

      windowToPixel(window1_.xmax, window1_.ymin, &px, &py);

      if      (halign_ == CHALIGN_TYPE_LEFT)
        pdx_ = 0;
      else if (halign_ == CHALIGN_TYPE_CENTER)
        pdx_ = (pixel_.xmax - px)/2;
      else if (halign_ == CHALIGN_TYPE_RIGHT)
        pdx_ = pixel_.xmax - px;

      if      (valign_ == CVALIGN_TYPE_TOP)
        pdy_ = 0;
      else if (valign_ == CVALIGN_TYPE_CENTER)
        pdy_ = (pixel_.ymax - py)/2;
      else if (valign_ == CVALIGN_TYPE_BOTTOM)
        pdy_ = pixel_.ymax - py;
    }
    else {
      pdx_ = 0;
      pdy_ = 0;
    }

    flip_x_ = ((pixel_ .xmax - pixel_ .xmin)*(window_.xmax - window_.xmin) < 0);
    flip_y_ = ((pixel_ .ymax - pixel_ .ymin)*(window_.ymax - window_.ymin) < 0);

    //------

    CMatrix2D matrix1, matrix2, matrix3;

    if (equal_scale_) {
      matrix1.setTranslation(pixel_.xmin + pdx_, pixel_.ymin + pdy_);
      matrix2.setScale      (factor_x1_, factor_y1_);
      matrix3.setTranslation(-window1_.xmin, -window1_.ymax);
    }
    else {
      matrix1.setTranslation(pixel_.xmin, pixel_.ymin);
      matrix2.setScale      (factor_x_, factor_y_);
      matrix3.setTranslation(-window1_.xmin, -window1_.ymax);
    }

    matrix_  = matrix1*matrix2*matrix3;
    imatrix_ = matrix_.inverse();
  }

  void windowToPixel(const CPoint2D &window, CPoint2D &pixel) const {
    windowToPixel(window.x, window.y, &pixel.x, &pixel.y);
  }

  void windowToPixel(double window_x, double window_y, double *pixel_x, double *pixel_y) const {
    if (equal_scale_) {
      *pixel_x = (window_x - window1_.xmin)*factor_x1_ + pixel_.xmin + pdx_;
      *pixel_y = (window_y - window1_.ymin)*factor_y1_ + pixel_.ymax + pdy_;
    }
    else {
      *pixel_x = (window_x - window1_.xmin)*factor_x_  + pixel_.xmin;
      *pixel_y = (window_y - window1_.ymin)*factor_y_  + pixel_.ymax;
    }
  }

  void windowToPixel(double window_x, double window_y, int *pixel_x, int *pixel_y) const {
    double pixel_x1, pixel_y1;

    windowToPixel(window_x, window_y, &pixel_x1, &pixel_y1);

    *pixel_x = CMathGen::Round(pixel_x1);
    *pixel_y = CMathGen::Round(pixel_y1);
  }

  void pixelToWindow(const CPoint2D &pixel, CPoint2D &window) const {
    pixelToWindow(pixel.x, pixel.y, &window.x, &window.y);
  }

  void pixelToWindow(double pixel_x, double pixel_y, double *window_x, double *window_y) const {
    if (equal_scale_) {
      *window_x = (pixel_x - pixel_.xmin - pdx_)/factor_x1_ + window1_.xmin;
      *window_y = (pixel_y - pixel_.ymax - pdy_)/factor_y1_ + window1_.ymin;
    }
    else {
      *window_x = (pixel_x - pixel_.xmin)/factor_x_  + window1_.xmin;
      *window_y = (pixel_y - pixel_.ymax)/factor_y_  + window1_.ymin;
    }
  }

  void pixelToWindow(int pixel_x, int pixel_y, double *window_x, double *window_y) const {
    pixelToWindow(double(pixel_x), double(pixel_y), window_x, window_y);
  }

  void pixelLengthToWindowLength(double pixel, double *window) const {
    double window_x, window_y;

    pixelToWindow(pixel, pixel, &window_x, &window_y);

    *window = window_x;
  }

  bool checkPixel(int x, int y) {
    return (x >= pixel_.xmin && x <= pixel_.xmin &&
            y >= pixel_.ymin && y <= pixel_.ymin);
  }

  const CMatrix2D &getMatrix () const { return matrix_ ; }
  const CMatrix2D &getIMatrix() const { return imatrix_; }

 private:
  IRange pixel_;
  RRange window_;

  RRange window1_;

  double window_xc1_, window_yc1_;
  double window_width1_, window_height1_;

  double factor_x_, factor_y_;
  double factor_x1_, factor_y1_;

  double pdx_, pdy_;

  bool        equal_scale_;
  bool        scale_min_;
  CHAlignType halign_;
  CVAlignType valign_;

  bool flip_x_, flip_y_;

  CMatrix2D matrix_;
  CMatrix2D imatrix_;
};

#endif
