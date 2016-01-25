#include <CQRadioButtons.h>

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QRadioButton>

CQHRadioButtons::
CQHRadioButtons(QWidget *parent, const char *name, ...) :
 CQRadioButtons(parent)
{
  va_list vargs;

  va_start(vargs, name);

  std::vector<QString> names;

  while (name != 0) {
    names.push_back(name);

    name = va_arg(vargs, char *);
  }

  va_end(vargs);

  addButtons(names);
}

CQHRadioButtons::
CQHRadioButtons(QWidget *parent) :
 CQRadioButtons(parent)
{
}

QLayout *
CQHRadioButtons::
createLayout(QWidget *parent)
{
  QHBoxLayout *l = new QHBoxLayout(parent);
  l->setMargin(0); l->setSpacing(0);

  return l;
}

void
CQHRadioButtons::
addWidget(QWidget *widget)
{
  QHBoxLayout *l = qobject_cast<QHBoxLayout *>(layout());

  l->addWidget(widget);
}

//-----------

CQVRadioButtons::
CQVRadioButtons(QWidget *parent, const char *name, ...) :
 CQRadioButtons(parent)
{
  va_list vargs;

  va_start(vargs, name);

  std::vector<QString> names;

  while (name != 0) {
    names.push_back(name);

    name = va_arg(vargs, char *);
  }

  va_end(vargs);

  addButtons(names);
}

CQVRadioButtons::
CQVRadioButtons(QWidget *parent) :
 CQRadioButtons(parent)
{
}

QLayout *
CQVRadioButtons::
createLayout(QWidget *parent)
{
  QVBoxLayout *l = new QVBoxLayout(parent);
  l->setMargin(0); l->setSpacing(0);

  return l;
}

void
CQVRadioButtons::
addWidget(QWidget *widget)
{
  QVBoxLayout *l = qobject_cast<QVBoxLayout *>(layout());

  l->addWidget(widget);
}

//-----------

CQToolbarRadioButtons::
CQToolbarRadioButtons(QWidget *parent, const char *title, const char *name, ...) :
 CQRadioButtons(parent), title_(title)
{
  va_list vargs;

  va_start(vargs, name);

  std::vector<QString> names;

  while (name != 0) {
    names.push_back(name);

    name = va_arg(vargs, char *);
  }

  va_end(vargs);

  addButtons(names);
}

CQToolbarRadioButtons::
CQToolbarRadioButtons(QWidget *parent, const char *title) :
 CQRadioButtons(parent), title_(title)
{
}

QLayout *
CQToolbarRadioButtons::
createLayout(QWidget *parent)
{
  QGridLayout *l = new QGridLayout(parent);
  l->setMargin(0); l->setSpacing(0);

  return l;
}

void
CQToolbarRadioButtons::
addWidget(QWidget *widget)
{
  QGridLayout *l = qobject_cast<QGridLayout *>(layout());

  if (buttons_.empty()) {
    QLabel *titleLabel = new QLabel("<small><b>" + title_ + "</b></mode>");

    l->addWidget(titleLabel, 0, 0);
  }

  int pos = buttons_.size();

  l->addWidget(widget, 1, pos);
}

//-----------

CQRadioButtons::
CQRadioButtons(QWidget *parent) :
 QWidget(parent), hasLayout_(false)
{
}

CQRadioButtons::
~CQRadioButtons()
{
}

void
CQRadioButtons::
addButtons(const std::vector<QString> &names)
{
  std::vector<QString>::const_iterator p1, p2;

  for (p1 = names.begin(), p2 = names.end() ; p1 != p2; ++p1)
    addButton(*p1);

  qobject_cast<QBoxLayout *>(layout())->addStretch();
}

void
CQRadioButtons::
addButton(const QString &name)
{
  if (! hasLayout_) {
    (void) createLayout(this);

    hasLayout_ = true;
  }

  QRadioButton *button = new QRadioButton(name);

  addWidget(button);

  if (buttons_.empty())
    button->setChecked(true);

  buttons_.push_back(button);

  connect(button, SIGNAL(toggled(bool)), this, SLOT(valueChangedI(bool)));
}

QLayout *
CQRadioButtons::
createLayout(QWidget *parent)
{
  QHBoxLayout *l = new QHBoxLayout(parent);
  l->setMargin(0); l->setSpacing(0);

  return l;
}

void
CQRadioButtons::
addWidget(QWidget *widget)
{
  QHBoxLayout *l = qobject_cast<QHBoxLayout *>(layout());

  l->addWidget(widget);
}

void
CQRadioButtons::
valueChangedI(bool value)
{
  if (! value) return;

  int id = getCurrentValue();

  emit valueChanged(id);

  emit valueChanged(buttons_[id]->text());
}

int
CQRadioButtons::
getCurrentValue() const
{
  std::vector<QRadioButton *>::const_iterator p1 = buttons_.begin();
  std::vector<QRadioButton *>::const_iterator p2 = buttons_.end  ();

  for (int i = 0; p1 != p2; ++p1, ++i)
    if ((*p1)->isChecked())
      return i;

  return -1;
}

QString
CQRadioButtons::
getCurrentName() const
{
  std::vector<QRadioButton *>::const_iterator p1 = buttons_.begin();
  std::vector<QRadioButton *>::const_iterator p2 = buttons_.end  ();

  for (int i = 0; p1 != p2; ++p1, ++i)
    if ((*p1)->isChecked())
      return (*p1)->text();

  return "";
}
