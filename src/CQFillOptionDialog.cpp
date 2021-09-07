#include <CQFillOptionDialog.h>
#include <CQFillOptionTool.h>
#include <CQIllustrator.h>
#include <CQGradientStopList.h>
#include <CQImageButton.h>
#include <CQPixmapCache.h>
#include <CQColorChooser.h>
#include <CQImagePreview.h>
#include <CQPointEdit.h>
#include <CQRealEdit.h>
#include <CQRealSpin.h>
#include <CQUtil.h>

#include <CRadialGradient.h>
#include <COSFile.h>

#include <QVBoxLayout>
#include <QFileDialog>
#include <QStackedWidget>
#include <QPushButton>
#include <QCheckBox>
#include <QComboBox>

CQFillOptionDialog::
CQFillOptionDialog(CQFillOptionTool *tool) :
 CQOptionToolDialog(), tool_(tool)
{
  initWidgets();
}

CQFillOptionDialog::
~CQFillOptionDialog()
{
}

void
CQFillOptionDialog::
setFill(const CQIllustratorShapeFill &fill)
{
  fill_ = fill;

  setActiveState();

  updateWidgets();
}

void
CQFillOptionDialog::
initWidgets()
{
  auto *layout = CQUtil::makeLayout<QVBoxLayout>(this, 0, 0);

  //-----

  auto *buttonLayout = CQUtil::makeLayout<QHBoxLayout>(2, 8);

  flatButton_  = new CQImageButton(CQPixmapCacheInst->getIcon("FLAT"     ));
  lgradButton_ = new CQImageButton(CQPixmapCacheInst->getIcon("LGRADIENT"));
  rgradButton_ = new CQImageButton(CQPixmapCacheInst->getIcon("RGRADIENT"));
  imageButton_ = new CQImageButton(CQPixmapCacheInst->getIcon("IMAGE"    ));

  flatButton_ ->setCheckable(true);
  lgradButton_->setCheckable(true);
  rgradButton_->setCheckable(true);
  imageButton_->setCheckable(true);

  buttonLayout->addWidget(flatButton_);
  buttonLayout->addWidget(lgradButton_);
  buttonLayout->addWidget(rgradButton_);
  buttonLayout->addWidget(imageButton_);
  buttonLayout->addStretch();

  layout->addLayout(buttonLayout);

  connect(flatButton_ , SIGNAL(clicked()), this, SLOT(flatSlot()));
  connect(lgradButton_, SIGNAL(clicked()), this, SLOT(lgradSlot()));
  connect(rgradButton_, SIGNAL(clicked()), this, SLOT(rgradSlot()));
  connect(imageButton_, SIGNAL(clicked()), this, SLOT(imageSlot()));

  //-----

  stack_ = CQUtil::makeWidget<QStackedWidget>("stack");

  layout->addWidget(stack_);

  //-----

  // Normal Fill
  auto *flatPanel = CQUtil::makeWidget<QFrame>("flatPanel");

  stack_->addWidget(flatPanel);

  auto *flatLayout = CQUtil::makeLayout<QGridLayout>(flatPanel, 2, 2);

  flatLayout->addWidget(new QLabel("Shown"  ), 0, 0);
  flatLayout->addWidget(new QLabel("Color"  ), 1, 0);
  flatLayout->addWidget(new QLabel("Opacity"), 2, 0);
  flatLayout->addWidget(new QLabel("Rule"   ), 3, 0);
  flatLayout->addWidget(new QLabel("Clip"   ), 4, 0);

  shownCheck_ = CQUtil::makeWidget<QCheckBox>("shownCheck");

  connect(shownCheck_, SIGNAL(stateChanged(int)), this, SLOT(shownSlot(int)));

  colorChooser_ = new CQColorChooser;

  colorChooser_->setStyles(CQColorChooser::ColorButton);

  connect(colorChooser_, SIGNAL(colorChanged(const QColor &)),
          this, SLOT(colorSlot(const QColor &)));

  opacityEdit_ = CQUtil::makeWidget<CQRealSpin>("opacityEdit");

  opacityEdit_->setRange(0.0, 1.0);
  opacityEdit_->setSingleStep(0.1);
  opacityEdit_->setDecimals(5);
//opacityEdit_->setRange(0.00, 1.0, 5);

  connect(opacityEdit_, SIGNAL(valueChanged(double)), this, SLOT(opacitySlot(double)));

  fillRule_ = CQUtil::makeWidget<QComboBox>("fillRule");

  fillRule_->addItems(QStringList() << "Winding" << "Even Odd");

  connect(fillRule_, SIGNAL(currentIndexChanged(const QString &)),
          this, SLOT(fillRuleSlot(const QString &)));

  clipCheck_ = CQUtil::makeLabelWidget<QCheckBox>("Yes", "clipCheck");

  connect(clipCheck_, SIGNAL(clicked(bool)), this, SLOT(clipSlot()));

  flatLayout->addWidget(shownCheck_  , 0, 1);
  flatLayout->addWidget(colorChooser_, 1, 1);
  flatLayout->addWidget(opacityEdit_ , 2, 1);
  flatLayout->addWidget(fillRule_    , 3, 1);
  flatLayout->addWidget(clipCheck_   , 4, 1);

  flatLayout->setRowStretch(5, 1);

  //-----

  // Linear Gradient
  auto *lgradPanel = CQUtil::makeWidget<QFrame>("lgradPanel");

  stack_->addWidget(lgradPanel);

  auto *lgradLayout = CQUtil::makeLayout<QVBoxLayout>(lgradPanel, 0, 0);

  auto *lgradEditLayout = CQUtil::makeLayout<QGridLayout>(2, 2);

  lgradLayout->addLayout(lgradEditLayout);

  lgradEditLayout->addWidget(new QLabel("Start"), 0, 0);
  lgradEditLayout->addWidget(new QLabel("End"  ), 1, 0);

  lgrad1_ = new CQPointEdit;
  lgrad2_ = new CQPointEdit;

  connect(lgrad1_, SIGNAL(valueChanged()), this, SLOT(updateLGrad()));
  connect(lgrad2_, SIGNAL(valueChanged()), this, SLOT(updateLGrad()));

  lgradEditLayout->addWidget(lgrad1_, 0, 1);
  lgradEditLayout->addWidget(lgrad2_, 1, 1);

  lgradStops_ = new CQGradientStopList;

  connect(lgradStops_, SIGNAL(stopsChanged()), this, SLOT(updateLGradStops()));

  lgradLayout->addWidget(lgradStops_);

  auto *lbuttonLayout = CQUtil::makeLayout<QHBoxLayout>(2, 2);

  lgradLayout->addLayout(lbuttonLayout);

  auto *addLGradStopButton    = CQUtil::makeLabelWidget<QPushButton>("Add", "addButton");
  auto *deleteLGradStopButton = CQUtil::makeLabelWidget<QPushButton>("Delete", "deleteButton");

  connect(addLGradStopButton   , SIGNAL(clicked()), this, SLOT(addLGradStop()));
  connect(deleteLGradStopButton, SIGNAL(clicked()), this, SLOT(removeLGradStop()));

  lbuttonLayout->addWidget(addLGradStopButton);
  lbuttonLayout->addWidget(deleteLGradStopButton);

  //-----

  // Radial Gradient
  auto *rgradPanel = CQUtil::makeWidget<QFrame>("rgradPanel");

  stack_->addWidget(rgradPanel);

  auto *rgradLayout = CQUtil::makeLayout<QVBoxLayout>(rgradPanel, 0, 0);

  auto *rgradEditLayout = CQUtil::makeLayout<QGridLayout>(2, 2);

  rgradLayout->addLayout(rgradEditLayout);

  rgradEditLayout->addWidget(new QLabel("Center"), 0, 0);
  rgradEditLayout->addWidget(new QLabel("Radius"), 1, 0);
  rgradEditLayout->addWidget(new QLabel("Focus" ), 2, 0);

  rgradC_ = new CQPointEdit;
  rgradR_ = new CQRealEdit;
  rgradF_ = new CQPointEdit;

  connect(rgradC_, SIGNAL(valueChanged()), this, SLOT(updateRGrad()));
  connect(rgradR_, SIGNAL(valueChanged(double)), this, SLOT(updateRGrad()));
  connect(rgradF_, SIGNAL(valueChanged()), this, SLOT(updateRGrad()));

  rgradEditLayout->addWidget(rgradC_, 0, 1);
  rgradEditLayout->addWidget(rgradR_, 1, 1);
  rgradEditLayout->addWidget(rgradF_, 2, 1);

  rgradStops_ = new CQGradientStopList;

  connect(rgradStops_, SIGNAL(stopsChanged()), this, SLOT(updateRGradStops()));

  rgradLayout->addWidget(rgradStops_);

  auto *rbuttonLayout = CQUtil::makeLayout<QHBoxLayout>(2, 2);

  rgradLayout->addLayout(rbuttonLayout);

  auto *addRGradStopButton    = CQUtil::makeLabelWidget<QPushButton>("Add", "addButton");
  auto *deleteRGradStopButton = CQUtil::makeLabelWidget<QPushButton>("Delete", "deleteButton");

  connect(addRGradStopButton   , SIGNAL(clicked()), this, SLOT(addRGradStop()));
  connect(deleteRGradStopButton, SIGNAL(clicked()), this, SLOT(removeRGradStop()));

  rbuttonLayout->addWidget(addRGradStopButton);
  rbuttonLayout->addWidget(deleteRGradStopButton);

  //-----

  // Image
  auto *imagePanel = CQUtil::makeWidget<QFrame>("imagePanel");

  stack_->addWidget(imagePanel);

  auto *imageLayout = CQUtil::makeLayout<QVBoxLayout>(imagePanel, 2, 2);

  imagePreview_ = new CQImagePreview;

  imagePreview_->setFixedSize(256, 256);

  imageLayout->addWidget(imagePreview_);

  auto *setImage = CQUtil::makeLabelWidget<QPushButton>("Set Image ...", "setImage");

  connect(setImage, SIGNAL(clicked()), this, SLOT(setImageSlot()));

  imageLayout->addWidget(setImage);

  imageScale_ = CQUtil::makeWidget<QComboBox>("imageScale");

  imageScale_->addItems(QStringList() << "None" << "Fit" << "Equal");

  connect(imageScale_, SIGNAL(currentIndexChanged(const QString &)),
          this, SLOT(imageScaleSlot(const QString &)));

  imageLayout->addWidget(imageScale_);

  //-----

  auto *buttonLayout1 = CQUtil::makeLayout<QHBoxLayout>(2, 2);

  auto *defButton = CQUtil::makeLabelWidget<QPushButton>("Set Default", "defButton");
  auto *bgButton  = CQUtil::makeLabelWidget<QPushButton>("Set Background", "bgButton");

  connect(defButton, SIGNAL(clicked()), this, SLOT(setDefaultSlot()));
  connect(bgButton , SIGNAL(clicked()), this, SLOT(setBackgroundSlot()));

  buttonLayout1->addWidget(defButton);
  buttonLayout1->addWidget(bgButton);

  layout->addLayout(buttonLayout1);

  //-----

  setActiveState();
}

void
CQFillOptionDialog::
flatSlot()
{
  stack_->setCurrentIndex(0);

  if (fill_.hasGradient() || fill_.hasImage()) {
    fill_.setGradient(0);
    fill_.resetImage();

    emit valueChanged(fill_);

    updateWidgets();
  }

  setActiveState();
}

void
CQFillOptionDialog::
lgradSlot()
{
  stack_->setCurrentIndex(1);

  const CLinearGradient *lg = 0;

  if (fill_.hasGradient()) {
    const CGenGradient *g = fill_.getGradient();

    lg = dynamic_cast<const CLinearGradient *>(g);
  }

  if (lg == 0) {
    auto *lg1 = new CLinearGradient;

    lg1->addStop(0.0, CRGBA(1,1,1));
    lg1->addStop(1.0, CRGBA(0,0,0));

    fill_.setGradient(lg1);
    fill_.resetImage();

    emit valueChanged(fill_);

    updateWidgets();

    lg = lg1;
  }

  setActiveState();
}

void
CQFillOptionDialog::
rgradSlot()
{
  stack_->setCurrentIndex(2);

  const CRadialGradient *rg = 0;

  if (fill_.hasGradient()) {
    const CGenGradient *g = fill_.getGradient();

    rg = dynamic_cast<const CRadialGradient *>(g);
  }

  if (rg == 0) {
    auto *rg1 = new CRadialGradient;

    rg1->addStop(0.0, CRGBA(1,1,1));
    rg1->addStop(1.0, CRGBA(0,0,0));

    fill_.setGradient(rg1);
    fill_.resetImage();

    emit valueChanged(fill_);

    updateWidgets();

    rg = rg1;
  }

  setActiveState();
}

void
CQFillOptionDialog::
imageSlot()
{
  stack_->setCurrentIndex(3);

  if (! fill_.hasImage()) {
    fill_.setImage(CImagePtr());
    fill_.setGradient(0);

    emit valueChanged(fill_);

    updateWidgets();
  }

  setActiveState();
}

void
CQFillOptionDialog::
setActiveState()
{
  const CLinearGradient *lg = 0;
  const CRadialGradient *rg = 0;

  bool is_image = false;

  if      (fill_.hasGradient()) {
    const CGenGradient *g = fill_.getGradient();

    lg = dynamic_cast<const CLinearGradient *>(g);
    rg = dynamic_cast<const CRadialGradient *>(g);
  }
  else if (fill_.hasImage()) {
    is_image = true;
  }

  flatButton_ ->setChecked(! is_image && ! lg && ! rg);
  lgradButton_->setChecked(lg);
  rgradButton_->setChecked(rg);
  imageButton_->setChecked(is_image);
}

void
CQFillOptionDialog::
updateWidgets()
{
  const CLinearGradient *lg = 0;
  const CRadialGradient *rg = 0;

  bool is_image = false;

  if      (fill_.hasGradient()) {
    const CGenGradient *g = fill_.getGradient();

    lg = dynamic_cast<const CLinearGradient *>(g);

    if (lg != 0) {
      stack_->setCurrentIndex(1);

      lgrad1_->setValue(lg->getPoint1());
      lgrad2_->setValue(lg->getPoint2());

      lgradStops_->init(*lg);
    }

    rg = dynamic_cast<const CRadialGradient *>(g);

    if (rg != 0) {
      stack_->setCurrentIndex(2);

      rgradC_->setValue(rg->getCenter());
      rgradR_->setValue(rg->getRadius());
      rgradF_->setValue(rg->getFocus ());

      rgradStops_->init(*rg);
    }
  }
  else if (fill_.hasImage()) {
    stack_->setCurrentIndex(3);

    imagePreview_->setImage(fill_.getImage());

    is_image = true;
  }
  else {
    stack_->setCurrentIndex(0);

    shownCheck_  ->setChecked     (fill_.isFilled());
    colorChooser_->setColor       (CQUtil::rgbaToColor(fill_.getColor()));
    opacityEdit_ ->setValue       (fill_.getOpacity());
    fillRule_    ->setCurrentIndex(fill_.getFillRule() == FILL_TYPE_WINDING ? 0 : 1);
  }

  colorChooser_->setEnabled(lg == 0 && rg == 0 && ! is_image);
  opacityEdit_ ->setEnabled(lg == 0 && rg == 0 && ! is_image);
  fillRule_    ->setEnabled(lg == 0 && rg == 0 && ! is_image);

  lgrad1_     ->setEnabled(lg != 0);
  lgrad2_     ->setEnabled(lg != 0);
  lgradStops_ ->setEnabled(lg != 0);

  rgradC_    ->setEnabled(rg != 0);
  rgradR_    ->setEnabled(rg != 0);
  rgradF_    ->setEnabled(rg != 0);
  rgradStops_->setEnabled(rg != 0);

  imagePreview_->setEnabled(is_image);
}

void
CQFillOptionDialog::
shownSlot(int state)
{
  bool shown = state;

  if (shown == fill_.isFilled())
    return;

  fill_.setFilled(shown);

  tool_->update();

  emit valueChanged(fill_);
}

void
CQFillOptionDialog::
colorSlot(const QColor &color)
{
  fill_.setColor(CQUtil::colorToRGBA(color));

  tool_->update();

  emit valueChanged(fill_);
}

void
CQFillOptionDialog::
opacitySlot(double value)
{
  fill_.setOpacity(value);

  tool_->update();

  emit valueChanged(fill_);
}

void
CQFillOptionDialog::
fillRuleSlot(const QString &rule)
{
  fill_.setFillRule(rule == "Winding" ? FILL_TYPE_WINDING : FILL_TYPE_EVEN_ODD);

  tool_->update();

  emit valueChanged(fill_);
}

void
CQFillOptionDialog::
clipSlot()
{
  bool value = clipCheck_->isChecked();

  emit clipChanged(value);
}

void
CQFillOptionDialog::
updateLGrad()
{
  auto *g  = fill_.getGradient();
  auto *lg = dynamic_cast<CLinearGradient *>(g);
  if (! lg) return;

  lg->setPoint1(lgrad1_->getValue());
  lg->setPoint2(lgrad2_->getValue());

  emit valueChanged(fill_);
}

void
CQFillOptionDialog::
updateLGradStops()
{
  const auto &stops = lgradStops_->getStops();

  auto *g  = fill_.getGradient();
  auto *lg = dynamic_cast<CLinearGradient *>(g);
  if (! lg) return;

  lg->setStops(stops);

  emit valueChanged(fill_);
}

void
CQFillOptionDialog::
addLGradStop()
{
  auto *g  = fill_.getGradient();
  auto *lg = dynamic_cast<CLinearGradient *>(g);
  if (! lg) return;

  lg->addStop(1.0, CRGBA(1,1,1));

  updateWidgets();

  emit valueChanged(fill_);
}

void
CQFillOptionDialog::
removeLGradStop()
{
}

void
CQFillOptionDialog::
updateRGrad()
{
  auto *g  = fill_.getGradient();
  auto *rg = dynamic_cast<CRadialGradient *>(g);
  if (! rg) return;

  rg->setCenter(rgradC_->getValue());
  rg->setRadius(rgradR_->getValue());
  rg->setFocus (rgradF_->getValue());

  emit valueChanged(fill_);
}

void
CQFillOptionDialog::
updateRGradStops()
{
  const CGenGradient::StopList &stops = rgradStops_->getStops();

  auto *g  = fill_.getGradient();
  auto *rg = dynamic_cast<CRadialGradient *>(g);
  if (! rg) return;

  rg->setStops(stops);

  emit valueChanged(fill_);
}

void
CQFillOptionDialog::
addRGradStop()
{
  auto *g  = fill_.getGradient();
  auto *rg = dynamic_cast<CRadialGradient *>(g);
  if (! rg) return;

  rg->addStop(1.0, CRGBA(1,1,1));

  updateWidgets();

  emit valueChanged(fill_);
}

void
CQFillOptionDialog::
removeRGradStop()
{
}

void
CQFillOptionDialog::
setImageSlot()
{
  auto title  = QString("Load Image");
  auto cwd    = QString(COSFile::getCurrentDir().c_str());
  auto filter = QString("Image Files (*)");

  auto filenames = QFileDialog::getOpenFileNames(this, title, cwd, filter);

  if (filenames.size() == 0)
    return;

  QStringListIterator fi(filenames);

  while (fi.hasNext()) {
    CImageFileSrc src(fi.next().toStdString());

    auto image = CImageMgrInst->lookupImage(src);

    if (image.isValid()) {
      fill_.setImage(image);

      updateWidgets();

      emit valueChanged(fill_);

      return;
    }
  }
}

void
CQFillOptionDialog::
imageScaleSlot(const QString &scale)
{
  if      (scale == "None")
    fill_.setImageScale(CQIllustratorShapeFill::ImageScale::NONE);
  else if (scale == "Fit")
    fill_.setImageScale(CQIllustratorShapeFill::ImageScale::FIT);
  else if (scale == "Equal")
    fill_.setImageScale(CQIllustratorShapeFill::ImageScale::EQUAL);

  tool_->update();

  emit valueChanged(fill_);
}

void
CQFillOptionDialog::
setDefaultSlot()
{
  tool_->getIllustrator()->setDefaultFill(fill_);
}

void
CQFillOptionDialog::
setBackgroundSlot()
{
  tool_->getIllustrator()->setBackground(fill_.getColor());
}
