#include <CQIllustratorImageFill.h>

CQIllustratorImageFill::
CQIllustratorImageFill()
{
}

CQIllustratorImageFill::
CQIllustratorImageFill(const CQIllustratorImageFill &image) :
 image_(image.image_), scale_(image.scale_), halign_(image.halign_), valign_(image.valign_)
{
}

CQIllustratorImageFill::
~CQIllustratorImageFill()
{
}

void
CQIllustratorImageFill::
setImage(const CImagePtr &image)
{
  image_ = image;
}

void
CQIllustratorImageFill::
setScale(Scale scale)
{
  scale_ = scale;
}

void
CQIllustratorImageFill::
setHAlign(CHAlignType halign)
{
  halign_ = halign;
}

void
CQIllustratorImageFill::
setVAlign(CVAlignType valign)
{
  valign_ = valign;
}
