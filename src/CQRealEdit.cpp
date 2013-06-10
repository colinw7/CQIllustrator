#include <CQRealEdit.h>
#include <CStrUtil.h>

#include <QMenu>
#include <QHBoxLayout>
#include <QDoubleValidator>
#include <QContextMenuEvent>
#include <cmath>

CQRealEdit::
CQRealEdit(QWidget *parent, double value) :
 QLineEdit(parent), menu_(NULL)
{
  init("edit", value);
}

CQRealEdit::
CQRealEdit(double value) :
 QLineEdit(NULL), menu_(NULL)
{
  init("edit", value);
}

void
CQRealEdit::
init(const QString &name, double value)
{
  if (name.length()) setObjectName(name);

  validator_ = new QDoubleValidator(this);

  setValidator(validator_);

  //setMinimumSize(QSize(60,0));
  //setMaximumSize(QSize(60,22));

  //setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

  connect(this, SIGNAL(editingFinished()), this, SLOT(editingFinishedI()));

  setValue(value);
}

void
CQRealEdit::
addMenuValue(double value)
{
  if (menu_ == NULL) {
    menu_ = new QMenu;

    connect(menu_, SIGNAL(triggered(QAction *)), this, SLOT(menuItemActivated(QAction *)));
  }

  CQRealEditAction *action = new CQRealEditAction(value);

  menu_->addAction(action);
}

void
CQRealEdit::
setValue(double value)
{
  value_ = value;

  valueToWidget();
}

double
CQRealEdit::
getValue() const
{
  return value_;
}

void
CQRealEdit::
setRange(double lower, double upper, int decimals)
{
  validator_->setRange(lower, upper, decimals);
}

void
CQRealEdit::
editingFinishedI()
{
  if (widgetToValue())
    emit valueChanged(value_);
}

void
CQRealEdit::
valueToWidget()
{
  setText(CStrUtil::toString(value_).c_str());
}

bool
CQRealEdit::
widgetToValue()
{
  double value = CStrUtil::toReal(text().toStdString());

  bool changed = (fabs(value - value_) > 1E-6);

  value_ = value;

  return changed;
}

void
CQRealEdit::
contextMenuEvent(QContextMenuEvent *e)
{
  if (menu_)
    menu_->exec(e->globalPos());
}

void
CQRealEdit::
menuItemActivated(QAction *action)
{
  CQRealEditAction *raction = dynamic_cast<CQRealEditAction *>(action);
  if (! raction) return;

  double value = raction->getValue();

  bool changed = (fabs(value - value_) > 1E-6);

  setValue(value);

  if (changed)
    emit valueChanged(value_);
}

//------

CQRealEditAction::
CQRealEditAction(double value) :
 QAction(QString("%1").arg(value), NULL), value_(value)
{
}
