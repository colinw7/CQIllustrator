#include <CQFillOption.h>
#include <CQIllustrator.h>
#include <CQColorChooser.h>
#include <CQUtil.h>
#include <CQRealEdit.h>
#include <CQPointEdit.h>
#include <CQImageButton.h>
#include <CQImagePreview.h>
#include <CLinearGradient.h>
#include <CRadialGradient.h>
#include <COSFile.h>

#include <CQGradientStopList.h>

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QComboBox>
#include <QCheckBox>
#include <QPushButton>
#include <QDoubleSpinBox>
#include <QStackedWidget>
#include <QFileDialog>
#include <QPainter>

#include <svg/flat_svg.h>
#include <svg/lgradient_svg.h>
#include <svg/rgradient_svg.h>
#include <svg/image_svg.h>

class CQFillOptionSwab : public QWidget {
 public:
  CQFillOptionSwab(CQFillOptionDialog *dialog);

 private:
  void paintEvent(QPaintEvent *e);

 private:
  CQFillOptionDialog *dialog_;
};

//----------

CQFillOptionTool::
CQFillOptionTool(CQIllustrator *illustrator) :
 CQOptionTool(), illustrator_(illustrator)
{
  setObjectName("fill");

  dialog_ = new CQFillOptionDialog(this);

  QVBoxLayout *layout = new QVBoxLayout(this);
  layout->setMargin(0); layout->setSpacing(0);

  QLabel *label = new QLabel("<small><b>Fill</b></small>");

  layout->addWidget(label);

  swab_ = new CQFillOptionSwab(dialog_);

  layout->addWidget(swab_);

  connect(dialog_, SIGNAL(valueChanged(const CQIllustratorShapeFill &)),
          this, SIGNAL(valueChanged(const CQIllustratorShapeFill &)));

  connect(dialog_, SIGNAL(clipChanged(bool)),
          this, SIGNAL(clipChanged(bool)));

  connect(illustrator_, SIGNAL(selectionChanged()), this, SLOT(selectionChangedSlot()));
  connect(illustrator_, SIGNAL(fillChanged()), this, SLOT(selectionChangedSlot()));
}

CQOptionToolDialog *
CQFillOptionTool::
getDialog()
{
  return dialog_;
}

void
CQFillOptionTool::
selectionChangedSlot()
{
  const CQIllustratorShape *shape = 0;

  const CQIllustratorSelectedShapes *selection = illustrator_->getSelection();

  uint num_selected = selection->size();

  if (num_selected == 1) {
    const CQIllustratorSelectedShape &sshape = selection->front();

    shape = sshape.getShape();
  }

  CQIllustratorShapeFill fill;

  if (shape)
    fill = shape->getFill();
  else
    fill = illustrator_->getDefaultFill();

  dialog_->setFill(fill);

  swab_->update();
}

//----------

CQFillOptionSwab::
CQFillOptionSwab(CQFillOptionDialog *dialog) :
 dialog_(dialog)
{
  setObjectName("swab");

  QFontMetrics fm(font());

  int th = fm.height() + 4;

  setFixedSize(2*th, th);
}

void
CQFillOptionSwab::
paintEvent(QPaintEvent *)
{
  QPainter painter(this);

  const CQIllustratorShapeFill &fill = dialog_->getFill();

  if (fill.hasGradient()) {
    const CGenGradient *g = fill.getGradient();

    const CLinearGradient *lg = 0;
    const CRadialGradient *rg = 0;

    if      ((lg = dynamic_cast<const CLinearGradient *>(g)) != 0) {
      QBrush brush(CQUtil::toQGradient(lg));

      painter.setBrush(brush);
    }
    else if ((rg = dynamic_cast<const CRadialGradient *>(g)) != 0) {
      QBrush brush(CQUtil::toQGradient(rg));

      painter.setBrush(brush);
    }
  }
  else {
    CRGBA color = fill.getColor();

    QBrush brush(CQUtil::rgbaToColor(color));

    painter.setBrush(brush);
  }

  painter.drawRect(rect());
}

//----------

CQFillOptionDialog::
CQFillOptionDialog(CQFillOptionTool *tool) :
 CQOptionToolDialog(), tool_(tool)
{
  initWidgets();
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
  QVBoxLayout *layout = new QVBoxLayout(this);
  layout->setMargin(0); layout->setSpacing(0);

  //-----

  QHBoxLayout *buttonLayout = new QHBoxLayout;
  buttonLayout->setMargin(2); buttonLayout->setSpacing(8);

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

  stack_ = new QStackedWidget;

  layout->addWidget(stack_);

  //-----

  // Normal Fill
  QWidget *flatPanel = new QWidget;

  stack_->addWidget(flatPanel);

  QGridLayout *flatLayout = new QGridLayout(flatPanel);
  flatLayout->setMargin(2); flatLayout->setSpacing(2);

  flatLayout->addWidget(new QLabel("Shown"  ), 0, 0);
  flatLayout->addWidget(new QLabel("Color"  ), 1, 0);
  flatLayout->addWidget(new QLabel("Opacity"), 2, 0);
  flatLayout->addWidget(new QLabel("Rule"   ), 3, 0);
  flatLayout->addWidget(new QLabel("Clip"   ), 4, 0);

  shownCheck_ = new QCheckBox;

  connect(shownCheck_, SIGNAL(stateChanged(int)), this, SLOT(shownSlot(int)));

  colorChooser_ = new CQColorChooser;

  colorChooser_->setStyles(CQColorChooser::ColorButton);

  connect(colorChooser_, SIGNAL(colorChanged(const QColor &)),
          this, SLOT(colorSlot(const QColor &)));

  opacityEdit_ = new QDoubleSpinBox;

  opacityEdit_->setRange(0.0, 1.0);
  opacityEdit_->setSingleStep(0.1);
  opacityEdit_->setDecimals(5);
//opacityEdit_->setRange(0.00, 1.0, 5);

  connect(opacityEdit_, SIGNAL(valueChanged(double)),
          this, SLOT(opacitySlot(double)));

  fillRule_ = new QComboBox;

  fillRule_->addItems(QStringList() << "Winding" << "Even Odd");

  connect(fillRule_, SIGNAL(currentIndexChanged(const QString &)),
          this, SLOT(fillRuleSlot(const QString &)));

  clipCheck_ = new QCheckBox("Yes");

  connect(clipCheck_, SIGNAL(clicked(bool)), this, SLOT(clipSlot()));

  flatLayout->addWidget(shownCheck_  , 0, 1);
  flatLayout->addWidget(colorChooser_, 1, 1);
  flatLayout->addWidget(opacityEdit_ , 2, 1);
  flatLayout->addWidget(fillRule_    , 3, 1);
  flatLayout->addWidget(clipCheck_   , 4, 1);

  flatLayout->setRowStretch(5, 1);

  //-----

  // Linear Gradient

  QWidget *lgradPanel = new QWidget;

  stack_->addWidget(lgradPanel);

  QVBoxLayout *lgradLayout = new QVBoxLayout(lgradPanel);

  lgradLayout->setMargin(0); lgradLayout->setSpacing(0);

  QGridLayout *lgradEditLayout = new QGridLayout;

  lgradEditLayout->setMargin(2); lgradEditLayout->setSpacing(2);

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

  QHBoxLayout *lbuttonLayout = new QHBoxLayout;

  lgradLayout->addLayout(lbuttonLayout);

  QPushButton *addLGradStopButton    = new QPushButton("Add");
  QPushButton *deleteLGradStopButton = new QPushButton("Delete");

  connect(addLGradStopButton   , SIGNAL(clicked()), this, SLOT(addLGradStop()));
  connect(deleteLGradStopButton, SIGNAL(clicked()), this, SLOT(removeLGradStop()));

  lbuttonLayout->addWidget(addLGradStopButton);
  lbuttonLayout->addWidget(deleteLGradStopButton);

  //-----

  // Radial Gradient

  QWidget *rgradPanel = new QWidget;

  stack_->addWidget(rgradPanel);

  QVBoxLayout *rgradLayout = new QVBoxLayout(rgradPanel);

  rgradLayout->setMargin(0); rgradLayout->setSpacing(0);

  QGridLayout *rgradEditLayout = new QGridLayout;

  rgradEditLayout->setMargin(2); rgradEditLayout->setSpacing(2);

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

  QHBoxLayout *rbuttonLayout = new QHBoxLayout;

  rgradLayout->addLayout(rbuttonLayout);

  QPushButton *addRGradStopButton    = new QPushButton("Add");
  QPushButton *deleteRGradStopButton = new QPushButton("Delete");

  connect(addRGradStopButton   , SIGNAL(clicked()), this, SLOT(addRGradStop()));
  connect(deleteRGradStopButton, SIGNAL(clicked()), this, SLOT(removeRGradStop()));

  rbuttonLayout->addWidget(addRGradStopButton);
  rbuttonLayout->addWidget(deleteRGradStopButton);

  //-----

  // Image

  QWidget *imagePanel = new QWidget;

  stack_->addWidget(imagePanel);

  QVBoxLayout *imageLayout = new QVBoxLayout(imagePanel);

  imagePreview_ = new CQImagePreview;

  imagePreview_->setFixedSize(256, 256);

  imageLayout->addWidget(imagePreview_);

  QPushButton *setImage = new QPushButton("Set Image ...");

  connect(setImage, SIGNAL(clicked()), this, SLOT(setImageSlot()));

  imageLayout->addWidget(setImage);

  imageScale_ = new QComboBox;

  imageScale_->addItems(QStringList() << "None" << "Fit" << "Equal");

  connect(imageScale_, SIGNAL(currentIndexChanged(const QString &)),
          this, SLOT(imageScaleSlot(const QString &)));

  imageLayout->addWidget(imageScale_);

  //-----

  QHBoxLayout *buttonLayout1 = new QHBoxLayout;

  QPushButton *defButton = new QPushButton("Set Default");
  QPushButton *bgButton  = new QPushButton("Set Background");

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
    CLinearGradient *lg1 = new CLinearGradient;

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
    CRadialGradient *rg1 = new CRadialGradient;

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
  CGenGradient *g = fill_.getGradient();

  CLinearGradient *lg = dynamic_cast<CLinearGradient *>(g);

  if (! lg) return;

  lg->setPoint1(lgrad1_->getValue());
  lg->setPoint2(lgrad2_->getValue());

  emit valueChanged(fill_);
}

void
CQFillOptionDialog::
updateLGradStops()
{
  const CGenGradient::StopList &stops = lgradStops_->getStops();

  CGenGradient *g = fill_.getGradient();

  CLinearGradient *lg = dynamic_cast<CLinearGradient *>(g);

  if (! lg) return;

  lg->setStops(stops);

  emit valueChanged(fill_);
}

void
CQFillOptionDialog::
addLGradStop()
{
  CGenGradient *g = fill_.getGradient();

  CLinearGradient *lg = dynamic_cast<CLinearGradient *>(g);

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
  CGenGradient *g = fill_.getGradient();

  CRadialGradient *rg = dynamic_cast<CRadialGradient *>(g);

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

  CGenGradient *g = fill_.getGradient();

  CRadialGradient *rg = dynamic_cast<CRadialGradient *>(g);

  if (! rg) return;

  rg->setStops(stops);

  emit valueChanged(fill_);
}

void
CQFillOptionDialog::
addRGradStop()
{
  CGenGradient *g = fill_.getGradient();

  CRadialGradient *rg = dynamic_cast<CRadialGradient *>(g);

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
  QString title  = "Load Image";
  QString cwd    = QString(COSFile::getCurrentDir().c_str());
  QString filter = "Image Files (*)";

  QStringList filenames = QFileDialog::getOpenFileNames(this, title, cwd, filter);

  if (filenames.size() == 0)
    return;

  QStringListIterator fi(filenames);

  while (fi.hasNext()) {
    CImageFileSrc src(fi.next().toStdString());

    CImagePtr image = CImageMgrInst->lookupImage(src);

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
