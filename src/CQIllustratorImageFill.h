#ifndef CQIllustratorImageFill_H
#define CQIllustratorImageFill_H

#include <CImageLib.h>
#include <CAlignType.h>

class CQIllustratorImageFill {
 public:
  enum class Scale {
    NONE,
    FIT,
    EQUAL
  };

 public:
  CQIllustratorImageFill();

  CQIllustratorImageFill(const CQIllustratorImageFill &image);

 ~CQIllustratorImageFill();

  const CImagePtr &getImage() const { return image_; }
  void setImage(const CImagePtr &image);

  Scale getScale() const { return scale_; }
  void setScale(Scale scale);

  CHAlignType getHAlign() const { return halign_; }
  void setHAlign(CHAlignType halign);

  CVAlignType getVAlign() const { return valign_; }
  void setVAlign(CVAlignType valign);

 private:
  CImagePtr   image_;
  Scale       scale_  { Scale::NONE };
  CHAlignType halign_ { CHALIGN_TYPE_LEFT };
  CVAlignType valign_ { CVALIGN_TYPE_BOTTOM };
};

#endif
