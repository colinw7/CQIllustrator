#ifndef CQIllustratorImageFill_H
#define CQIllustratorImageFill_H

#include <CImagePtr.h>
#include <CAlignType.h>

class CQIllustratorImageFill {
 public:
  enum class Scale {
    NONE,
    FIT,
    EQUAL
  };

 public:
  CQIllustratorImageFill() { }

  CQIllustratorImageFill(const CQIllustratorImageFill &image) :
   image_(image.image_), scale_(image.scale_), halign_(image.halign_), valign_(image.valign_) {
  }

  CImagePtr getImage() const { return image_; }
  void setImage(CImagePtr image) { image_ = image; }

  Scale getScale() const { return scale_; }
  void setScale(Scale scale) { scale_ = scale; }

  CHAlignType getHAlign() const { return halign_; }
  void setHAlign(CHAlignType halign) { halign_ = halign; }

  CVAlignType getVAlign() const { return valign_; }
  void setVAlign(CVAlignType valign) { valign_ = valign; }

 private:
  CImagePtr   image_;
  Scale       scale_  { Scale::NONE };
  CHAlignType halign_ { CHALIGN_TYPE_LEFT };
  CVAlignType valign_ { CVALIGN_TYPE_BOTTOM };
};

#endif
