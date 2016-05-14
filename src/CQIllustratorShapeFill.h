#ifndef CQIllustratorShapeFill_H
#define CQIllustratorShapeFill_H

#include <CQIllustratorImageFill.h>
#include <CFillType.h>
#include <CLinearGradient.h>
#include <CRGBA.h>

class CQIllustratorShape;
class CQIllustratorShapeDrawer;

class CQIllustratorShapeFill {
 public:
  enum class ImageScale {
    NONE,
    FIT,
    EQUAL
  };

 public:
  CQIllustratorShapeFill() { }

  CQIllustratorShapeFill(const CQIllustratorShapeFill &fill) :
   filled_(fill.filled_), color_(fill.color_), opacity_(fill.opacity_), rule_(fill.rule_) {
    if (fill.gradient_)
      gradient_ = fill.gradient_->dup();

    if (fill.image_)
      image_ = new CQIllustratorImageFill(*fill.image_);
  }

  const CQIllustratorShapeFill &operator=(const CQIllustratorShapeFill &fill) {
    filled_  = fill.filled_;
    color_   = fill.color_;
    opacity_ = fill.opacity_;
    rule_    = fill.rule_;

    delete gradient_;

    if (fill.gradient_)
      gradient_ = fill.gradient_->dup();
    else
      gradient_ = 0;

    delete image_;

    if (fill.image_)
      image_ = new CQIllustratorImageFill(*fill.image_);
    else
      image_ = 0;

    return *this;
  }

 ~CQIllustratorShapeFill() {
    delete gradient_;
    delete image_;
  }

  bool isFilled() const { return filled_; }
  void setFilled(bool b) { filled_ = b; }

  const CRGBA &getColor() const { return color_; }
  void setColor(const CRGBA &color) { color_ = color; }

  double getOpacity() const { return opacity_; }
  void setOpacity(double opacity) { opacity_ = opacity; }

  double getFillRule() const { return rule_; }
  void setFillRule(CFillType rule) { rule_ = rule; }

  bool hasGradient() const { return (gradient_ != 0); }

  void setGradient(const CGenGradient *g) {
    delete gradient_;

    if (g)
      gradient_ = g->dup();
    else
      gradient_ = 0;
  }

  const CGenGradient *getGradient() const { return gradient_; }
  CGenGradient *getGradient() { return gradient_; }

  bool hasImage() const { return (image_ != 0); }

  CImagePtr getImage() const {
    if (! image_) {
      CQIllustratorShapeFill *th = const_cast<CQIllustratorShapeFill *>(this);

      th->image_ = new CQIllustratorImageFill;
    }

    return image_->getImage();
  }

  void setImage(CImagePtr image) {
    if (! image_) {
      CQIllustratorShapeFill *th = const_cast<CQIllustratorShapeFill *>(this);

      th->image_ = new CQIllustratorImageFill;
    }

    image_->setImage(image);
  }

  void resetImage() {
    delete image_;

    image_ = 0;
  }

  ImageScale getImageScale() const {
    if (! image_)
      return ImageScale::NONE;

    return (ImageScale) image_->getScale();
  }

  void setImageScale(ImageScale scale) {
    if (! image_) {
      CQIllustratorShapeFill *th = const_cast<CQIllustratorShapeFill *>(this);

      th->image_ = new CQIllustratorImageFill;
    }

    image_->setScale((CQIllustratorImageFill::Scale) scale);
  }

  CHAlignType getImageHAlign() const {
    if (! image_)
      return CHALIGN_TYPE_LEFT;

    return image_->getHAlign();
  }

  void setImageHAlign(CHAlignType halign) {
    if (! image_) {
      CQIllustratorShapeFill *th = const_cast<CQIllustratorShapeFill *>(this);

      th->image_ = new CQIllustratorImageFill;
    }

    image_->setHAlign(halign);
  }

  CVAlignType getImageVAlign() const {
    if (! image_)
      return CVALIGN_TYPE_BOTTOM;

    return image_->getVAlign();
  }

  void setImageVAlign(CVAlignType valign) {
    if (! image_) {
      CQIllustratorShapeFill *th = const_cast<CQIllustratorShapeFill *>(this);

      th->image_ = new CQIllustratorImageFill;
    }

    image_->setVAlign(valign);
  }

  void draw(const CQIllustratorShape *shape, CQIllustratorShapeDrawer *drawer) const;

 private:
  bool                    filled_ { true };
  CRGBA                   color_ { 1, 1, 1 };
  double                  opacity_ { 1 };
  CFillType               rule_ { FILL_TYPE_EVEN_ODD };
  CGenGradient           *gradient_ { 0 };
  CQIllustratorImageFill *image_ { 0 };
};

#endif
