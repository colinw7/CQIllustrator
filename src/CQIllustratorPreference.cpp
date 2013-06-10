#include <CQIllustratorPreference.h>
#include <CQIllustrator.h>

#include <QLabel>
#include <QVBoxLayout>
#include <QGridLayout>
#include <CQPointEdit.h>

CQIllustratorPreferenceDock::
CQIllustratorPreferenceDock(CQIllustrator *illustrator) :
 QWidget(NULL), illustrator_(illustrator)
{
  QVBoxLayout *layout = new QVBoxLayout(this);
  layout->setMargin(0); layout->setSpacing(0);

  fullMin_ = new CQPointEdit;
  fullMax_ = new CQPointEdit;

  connect(fullMin_, SIGNAL(valueChanged()), this, SLOT(updateFillBBox()));
  connect(fullMax_, SIGNAL(valueChanged()), this, SLOT(updateFillBBox()));

  QGridLayout *fullLayout = new QGridLayout;

  fullLayout->addWidget(new QLabel("Min"), 0, 0);
  fullLayout->addWidget(fullMin_         , 0, 1);
  fullLayout->addWidget(new QLabel("Max"), 1, 0);
  fullLayout->addWidget(fullMax_         , 1, 1);

  layout->addLayout(fullLayout);

  layout->addStretch();

  connect(illustrator_, SIGNAL(fullBBoxChanged()), this, SLOT(syncFillBBox()));

  syncFillBBox();
}

void
CQIllustratorPreferenceDock::
syncFillBBox()
{
  fullMin_->setValue(illustrator_->getFullBBox().getMin());
  fullMax_->setValue(illustrator_->getFullBBox().getMax());
}

void
CQIllustratorPreferenceDock::
updateFillBBox()
{
  const CPoint2D &fullMin = fullMin_->getValue();
  const CPoint2D &fullMax = fullMax_->getValue();

  CBBox2D fullBBox;

  fullBBox += fullMin;
  fullBBox += fullMax;

  illustrator_->setFullBBox(fullBBox);
}
