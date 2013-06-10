#include <CQAngleSpinBox.h>

namespace {
const QChar Degree(0xB0);
}

CQAngleSpinBox::
CQAngleSpinBox(QWidget *parent, double value) :
 QDoubleSpinBox(parent)
{
  init("edit", value);
}

CQAngleSpinBox::
CQAngleSpinBox(double value) :
 QDoubleSpinBox(NULL)
{
  init("edit", value);
}

double
CQAngleSpinBox::
getValue() const
{
  return value();
}

void
CQAngleSpinBox::
init(const QString &name, double value)
{
  if (name.length()) setObjectName(name);

  setRange(-360.0, 360.0);

  setAlignment(Qt::AlignVCenter|Qt::AlignRight);

  setDecimals(1);

  setSuffix(Degree);

  connect(this, SIGNAL(valueChanged(double)), this, SIGNAL(angleChanged(double)));

  setValue(value);
}
