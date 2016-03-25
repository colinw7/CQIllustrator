#include <CQAngleSpinBox.h>

namespace {
const QChar Degree(0xB0);
}

CQAngleSpinBox::
CQAngleSpinBox(QWidget *parent, const CAngle &angle) :
 QDoubleSpinBox(parent), angle_(angle)
{
  init();
}

CQAngleSpinBox::
CQAngleSpinBox(const CAngle &angle) :
 QDoubleSpinBox(0), angle_(angle)
{
  init();
}

void
CQAngleSpinBox::
setAngle(const CAngle &angle)
{
  angle_ = angle;

  setValue(angle_.degrees());
}

void
CQAngleSpinBox::
init()
{
  setObjectName("angle");

  setRange(-360.0, 360.0);

  setAlignment(Qt::AlignVCenter|Qt::AlignRight);

  setDecimals(1);

  setSuffix(Degree);

  connect(this, SIGNAL(valueChanged(double)), this, SLOT(valueChangedSlot(double)));

  setValue(angle_.degrees());
}

void
CQAngleSpinBox::
valueChangedSlot(double a)
{
  angle_ = CAngle::makeDegrees(a);

  emit angleChanged(angle_);
}
