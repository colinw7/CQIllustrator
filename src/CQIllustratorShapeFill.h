#ifndef CQIllustratorShapeFill_H
#define CQIllustratorShapeFill_H

#include <CQIllustratorImageFill.h>
#include <CFillType.h>
#include <CLinearGradient.h>
#include <CImagePtr.h>
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
  CQIllustratorShapeFill();

  CQIllustratorShapeFill(const CQIllustratorShapeFill &fill);

  CQIllustratorShapeFill &operator=(const CQIllustratorShapeFill &fill);

 ~CQIllustratorShapeFill();

  bool isFilled() const { return filled_; }
  void setFilled(bool b) { filled_ = b; }

  const CRGBA &getColor() const { return color_; }
  void setColor(const CRGBA &color) { color_ = color; }

  double getOpacity() const { return opacity_; }
  void setOpacity(double opacity) { opacity_ = opacity; }

  double getFillRule() const { return rule_; }
  void setFillRule(CFillType rule) { rule_ = rule; }

  bool hasGradient() const { return (gradient_ != nullptr); }

  void setGradient(const CGenGradient *g);

  const CGenGradient *getGradient() const { return gradient_; }
  CGenGradient *getGradient() { return gradient_; }

  bool hasImage() const { return (image_ != nullptr); }

  CImagePtr getImage() const;

  void setImage(CImagePtr image);

  void resetImage();

  ImageScale getImageScale() const;

  void setImageScale(ImageScale scale);

  CHAlignType getImageHAlign() const;

  void setImageHAlign(CHAlignType halign);

  CVAlignType getImageVAlign() const;

  void setImageVAlign(CVAlignType valign);

  void draw(const CQIllustratorShape *shape, CQIllustratorShapeDrawer *drawer) const;

 private:
  bool                    filled_ { true };
  CRGBA                   color_ { 1, 1, 1 };
  double                  opacity_ { 1 };
  CFillType               rule_ { FILL_TYPE_EVEN_ODD };
  CGenGradient           *gradient_ { nullptr };
  CQIllustratorImageFill *image_ { nullptr };
};

#endif
