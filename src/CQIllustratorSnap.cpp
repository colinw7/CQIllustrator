#include <CQIllustratorSnap.h>
#include <CQIllustrator.h>

#include <QVBoxLayout>
#include <QLabel>
#include <QCheckBox>
#include <QDoubleSpinBox>

CQIllustratorSnapDock::
CQIllustratorSnapDock(CQIllustrator *illustrator) :
 QWidget(0), illustrator_(illustrator)
{
  QVBoxLayout *layout = new QVBoxLayout(this);
  layout->setMargin(2); layout->setSpacing(2);

  enabledCheck_ = new QCheckBox("Enabled");

  connect(enabledCheck_, SIGNAL(clicked(bool)), this, SLOT(enabledSlot()));

  layout->addWidget(enabledCheck_);

  QGridLayout *gridLayout = new QGridLayout;

  xPitch_ = new QDoubleSpinBox;

  xPitch_->setRange(0.01, 1000);
  xPitch_->setSingleStep(1.0);
  xPitch_->setDecimals(3);

  connect(xPitch_, SIGNAL(valueChanged(double)), this, SLOT(xPitchSlot(double)));

  yPitch_ = new QDoubleSpinBox;

  yPitch_->setRange(0.01, 1000);
  yPitch_->setSingleStep(1.0);
  yPitch_->setDecimals(3);

  connect(yPitch_, SIGNAL(valueChanged(double)), this, SLOT(yPitchSlot(double)));

  gridLayout->addWidget(new QLabel("X Pitch"), 0, 0);
  gridLayout->addWidget(xPitch_              , 0, 1);
  gridLayout->addWidget(new QLabel("Y Pitch"), 1, 0);
  gridLayout->addWidget(yPitch_              , 1, 1);

  gridLayout->setColumnStretch(2, 1);
  gridLayout->setRowStretch   (2, 1);

  layout->addLayout(gridLayout);

  enabledCheck_->setChecked(illustrator_->getSnapEnabled());

  xPitch_->setValue(illustrator_->getSnapXPitch());
  yPitch_->setValue(illustrator_->getSnapYPitch());
}

void
CQIllustratorSnapDock::
enabledSlot()
{
  illustrator_->setSnapEnabled(enabledCheck_->isChecked());
}

void
CQIllustratorSnapDock::
xPitchSlot(double xpitch)
{
  illustrator_->setSnapXPitch(xpitch);
}

void
CQIllustratorSnapDock::
yPitchSlot(double ypitch)
{
  illustrator_->setSnapYPitch(ypitch);
}
