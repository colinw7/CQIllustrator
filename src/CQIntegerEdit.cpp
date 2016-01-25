#include <CQIntegerEdit.h>
#include <CStrUtil.h>

#include <QIntValidator>

CQIntegerEdit::
CQIntegerEdit(QWidget *parent, int value) :
 QLineEdit(parent)
{
  init("edit", value);
}

CQIntegerEdit::
CQIntegerEdit(int value) :
 QLineEdit(0)
{
  init("edit", value);
}

void
CQIntegerEdit::
init(const QString &name, int value)
{
  if (name.length()) setObjectName(name);

  QValidator *validator = new QIntValidator(this);

  //setMinimumSize(QSize(60,0));
  //setMaximumSize(QSize(60,22));

  //setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

  setValidator(validator);

  connect(this, SIGNAL(editingFinished()), this, SLOT(editingFinishedI()));

  setValue(value);
}

void
CQIntegerEdit::
setValue(int value)
{
  value_ = value;

  valueToWidget();
}

int
CQIntegerEdit::
getValue() const
{
  CQIntegerEdit *th = const_cast<CQIntegerEdit *>(this);

  th->widgetToValue();

  return value_;
}

void
CQIntegerEdit::
editingFinishedI()
{
  if (widgetToValue())
    emit valueChanged(value_);
}

void
CQIntegerEdit::
valueToWidget()
{
  setText(CStrUtil::toString(value_).c_str());
}

bool
CQIntegerEdit::
widgetToValue()
{
  int value = CStrUtil::toInteger(text().toStdString());

  bool changed = (value != value_);

  value_ = value;

  return changed;
}
