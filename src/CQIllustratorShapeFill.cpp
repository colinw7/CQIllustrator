#include <CQIllustratorShapeFill.h>
#include <CQIllustratorShapeDrawer.h>

CQIllustratorShapeFill::
CQIllustratorShapeFill()
{
}

CQIllustratorShapeFill::
CQIllustratorShapeFill(const CQIllustratorShapeFill &fill) :
 filled_(fill.filled_), color_(fill.color_), opacity_(fill.opacity_), rule_(fill.rule_)
{
  if (fill.gradient_)
    gradient_ = fill.gradient_->dup();

  if (fill.image_)
    image_ = new CQIllustratorImageFill(*fill.image_);
}

CQIllustratorShapeFill &
CQIllustratorShapeFill::
operator=(const CQIllustratorShapeFill &fill)
{
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

CQIllustratorShapeFill::
~CQIllustratorShapeFill()
{
  delete gradient_;
  delete image_;
}

void
CQIllustratorShapeFill::
setGradient(const CGenGradient *g)
{
  delete gradient_;

  if (g)
    gradient_ = g->dup();
  else
    gradient_ = 0;
}

CImagePtr
CQIllustratorShapeFill::
getImage() const
{
  if (! image_) {
    auto *th = const_cast<CQIllustratorShapeFill *>(this);

    th->image_ = new CQIllustratorImageFill;
  }

  return image_->getImage();
}

void
CQIllustratorShapeFill::
setImage(CImagePtr image)
{
  if (! image_) {
    auto *th = const_cast<CQIllustratorShapeFill *>(this);

    th->image_ = new CQIllustratorImageFill;
  }

  image_->setImage(image);
}

void
CQIllustratorShapeFill::
resetImage()
{
  delete image_;

  image_ = 0;
}

CQIllustratorShapeFill::ImageScale
CQIllustratorShapeFill::
getImageScale() const
{
  if (! image_)
    return ImageScale::NONE;

  return ImageScale(image_->getScale());
}

void
CQIllustratorShapeFill::
setImageScale(ImageScale scale)
{
  if (! image_) {
    auto *th = const_cast<CQIllustratorShapeFill *>(this);

    th->image_ = new CQIllustratorImageFill;
  }

  image_->setScale(CQIllustratorImageFill::Scale(scale));
}

CHAlignType
CQIllustratorShapeFill::
getImageHAlign() const
{
  if (! image_)
    return CHALIGN_TYPE_LEFT;

  return image_->getHAlign();
}

void
CQIllustratorShapeFill::
setImageHAlign(CHAlignType halign)
{
  if (! image_) {
    auto *th = const_cast<CQIllustratorShapeFill *>(this);

    th->image_ = new CQIllustratorImageFill;
  }

  image_->setHAlign(halign);
}

CVAlignType
CQIllustratorShapeFill::
getImageVAlign() const
{
  if (! image_)
    return CVALIGN_TYPE_BOTTOM;

  return image_->getVAlign();
}

void
CQIllustratorShapeFill::
setImageVAlign(CVAlignType valign)
{
  if (! image_) {
    auto *th = const_cast<CQIllustratorShapeFill *>(this);

    th->image_ = new CQIllustratorImageFill;
  }

  image_->setVAlign(valign);
}

void
CQIllustratorShapeFill::
draw(const CQIllustratorShape *shape, CQIllustratorShapeDrawer *drawer) const
{
  if (! isFilled())
    return;

  drawer->setFill(shape, *this);

  drawer->pathFill(rule_);
}
