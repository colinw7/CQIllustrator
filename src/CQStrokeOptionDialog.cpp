#include <CQStrokeOptionDialog.h>
#include <CQStrokeOptionTool.h>
#include <CQIllustrator.h>

#include <CQColorChooser.h>
#include <CQLineDash.h>
#include <CQLineCap.h>
#include <CQLineJoin.h>
#include <CQRealSpin.h>
#include <CQUtil.h>
#include <CQUtilRGBA.h>

#include <QVBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QCheckBox>
#include <QPainter>

CQStrokeOptionDialog::
CQStrokeOptionDialog(CQStrokeOptionTool *tool) :
 CQOptionToolDialog(), tool_(tool)
{
  setObjectName("dialog");

  initWidgets();
}

void
CQStrokeOptionDialog::
setStroke(const CQIllustratorShapeStroke &stroke)
{
  stroke_ = stroke;

  updateWidgets();
}

void
CQStrokeOptionDialog::
initWidgets()
{
  auto *layout = CQUtil::makeLayout<QVBoxLayout>(this, 0, 0);

  //---

  auto *gridLayout = CQUtil::makeLayout<QGridLayout>(2, 2);

  gridLayout->addWidget(new QLabel("Shown"      ), 0, 0);
  gridLayout->addWidget(new QLabel("Color"      ), 1, 0);
  gridLayout->addWidget(new QLabel("Width"      ), 2, 0);
  gridLayout->addWidget(new QLabel("Opacity"    ), 3, 0);
  gridLayout->addWidget(new QLabel("Dash"       ), 4, 0);
  gridLayout->addWidget(new QLabel("Line Cap"   ), 5, 0);
  gridLayout->addWidget(new QLabel("Line Join"  ), 6, 0);
  gridLayout->addWidget(new QLabel("Mitre Limit"), 7, 0);

  shownCheck_   = new QCheckBox;
  colorChooser_ = new CQColorChooser;
  widthEdit_    = new CQRealSpin;
  opacityEdit_  = new CQRealSpin;
  dashEdit_     = new CQLineDash;
  capEdit_      = new CQLineCap;
  joinEdit_     = new CQLineJoin;
  mitreEdit_    = new CQRealSpin;

  widthEdit_->setRange(0.01, 1000);
  widthEdit_->setSingleStep(1.0);
  widthEdit_->setDecimals(3);
//widthEdit_->setValidator(new QDoubleValidator(0.01, 1000.0, 3, widthEdit_));

  opacityEdit_->setRange(0.0, 1.0);
  opacityEdit_->setSingleStep(0.1);
  opacityEdit_->setDecimals(5);
//opacityEdit_->setValidator(new QDoubleValidator(0.00,    1.0, 5, opacityEdit_));

  mitreEdit_->setRange(0.01, 100.0);
  mitreEdit_->setSingleStep(1.0);
  mitreEdit_->setDecimals(3);
//mitreEdit_->setValidator(new QDoubleValidator(0.01,  100.0, 3, mitreEdit_));

  colorChooser_->setStyles(CQColorChooser::ColorButton);

  connect(shownCheck_  , SIGNAL(stateChanged(int)),
          this, SLOT(shownSlot(int)));
  connect(colorChooser_, SIGNAL(colorChanged(const QColor &)),
          this, SLOT(colorSlot(const QColor &)));
  connect(widthEdit_   , SIGNAL(valueChanged(double)),
          this, SLOT(widthSlot(double)));
  connect(opacityEdit_ , SIGNAL(valueChanged(double)),
          this, SLOT(opacitySlot(double)));
  connect(dashEdit_    , SIGNAL(valueChanged(const CLineDash &)),
          this, SLOT(dashSlot(const CLineDash &)));
  connect(capEdit_     , SIGNAL(valueChanged(CLineCapType)),
          this, SLOT(capSlot(CLineCapType)));
  connect(joinEdit_    , SIGNAL(valueChanged(CLineJoinType)),
          this, SLOT(joinSlot(CLineJoinType)));
  connect(mitreEdit_   , SIGNAL(valueChanged(double)),
          this, SLOT(mitreSlot(double)));

  gridLayout->addWidget(shownCheck_  , 0, 1);
  gridLayout->addWidget(colorChooser_, 1, 1);
  gridLayout->addWidget(widthEdit_   , 2, 1);
  gridLayout->addWidget(opacityEdit_ , 3, 1);
  gridLayout->addWidget(dashEdit_    , 4, 1);
  gridLayout->addWidget(capEdit_     , 5, 1);
  gridLayout->addWidget(joinEdit_    , 6, 1);
  gridLayout->addWidget(mitreEdit_   , 7, 1);

  layout->addLayout(gridLayout);

  //------

  auto *defButton = CQUtil::makeLabelWidget<QPushButton>("Set Default", "defButton");

  connect(defButton, SIGNAL(clicked()), this, SLOT(setDefaultSlot()));

  layout->addWidget(defButton);
}

void
CQStrokeOptionDialog::
updateWidgets()
{
  shownCheck_  ->setChecked (stroke_.isStroked());
  colorChooser_->setColor   (CQUtil::rgbaToColor(stroke_.getColor()));
  widthEdit_   ->setValue   (stroke_.getWidth());
  opacityEdit_ ->setValue   (stroke_.getOpacity());
  dashEdit_    ->setLineDash(stroke_.getLineDash());
  capEdit_     ->setLineCap (stroke_.getLineCap());
  joinEdit_    ->setLineJoin(stroke_.getLineJoin());
  mitreEdit_   ->setValue   (stroke_.getMitreLimit());
}

void
CQStrokeOptionDialog::
shownSlot(int state)
{
  bool shown = state;

  if (shown == stroke_.isStroked())
    return;

  stroke_.setStroked(shown);

  tool_->update();

  emit valueChanged(stroke_);
}

void
CQStrokeOptionDialog::
colorSlot(const QColor &color)
{
  CRGBA rgba = CQUtil::colorToRGBA(color);

  if (rgba == stroke_.getColor())
    return;

  stroke_.setColor(rgba);

  tool_->update();

  emit valueChanged(stroke_);
}

void
CQStrokeOptionDialog::
widthSlot(double new_width)
{
  if (fabs(new_width - stroke_.getWidth()) < 1E-4)
    return;

  stroke_.setWidth(new_width);

  tool_->update();

  emit valueChanged(stroke_);
}

void
CQStrokeOptionDialog::
opacitySlot(double new_opacity)
{
  if (new_opacity == stroke_.getOpacity())
    return;

  stroke_.setOpacity(new_opacity);

  tool_->update();

  emit valueChanged(stroke_);
}

void
CQStrokeOptionDialog::
dashSlot(const CLineDash &dash)
{
  if (dash == stroke_.getLineDash())
    return;

  stroke_.setLineDash(dash);

  emit valueChanged(stroke_);
}

void
CQStrokeOptionDialog::
capSlot(CLineCapType cap)
{
  if (cap == stroke_.getLineCap())
    return;

  stroke_.setLineCap(cap);

  emit valueChanged(stroke_);
}

void
CQStrokeOptionDialog::
joinSlot(CLineJoinType join)
{
  if (join == stroke_.getLineJoin())
    return;

  stroke_.setLineJoin(join);

  emit valueChanged(stroke_);
}

void
CQStrokeOptionDialog::
mitreSlot(double new_mitre_limit)
{
  if (new_mitre_limit == stroke_.getMitreLimit())
    return;

  stroke_.setMitreLimit(new_mitre_limit);

  tool_->update();

  emit valueChanged(stroke_);
}

void
CQStrokeOptionDialog::
setDefaultSlot()
{
  tool_->getIllustrator()->setDefaultStroke(stroke_);
}
