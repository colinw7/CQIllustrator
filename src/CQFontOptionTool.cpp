#include <CQFontOptionTool.h>
#include <CQFontOptionDialog.h>
#include <CQFontChooser.h>
#include <CQUtil.h>

#include <QVBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPainter>

class CQFontOptionSwab : public QWidget {
 public:
  CQFontOptionSwab(CQFontOptionDialog *dialog);

 private:
  void paintEvent(QPaintEvent *e) override;

 private:
  CQFontOptionDialog *dialog_;
  int                 tw_, th_, fw_;
};

CQFontOptionTool::
CQFontOptionTool(QWidget *) :
 CQOptionTool(), dialog_(nullptr)
{
  dialog_ = new CQFontOptionDialog(this);

  QVBoxLayout *layout = new QVBoxLayout(this);
  layout->setMargin(0); layout->setSpacing(0);

  QLabel *label = new QLabel("<small><b>Font</b></small>");

  layout->addWidget(label);

  CQFontOptionSwab *swab = new CQFontOptionSwab(dialog_);

  layout->addWidget(swab);

  connect(dialog_, SIGNAL(valueChanged(const QFont &)),
          this, SIGNAL(valueChanged(const QFont &)));
}

CQOptionToolDialog *
CQFontOptionTool::
getDialog()
{
  return dialog_;
}

void
CQFontOptionTool::
setFont(QFont font)
{
  dialog_->setFont(font);
}

QFont
CQFontOptionTool::
getFont() const
{
  return dialog_->getFont();
}

//----------

CQFontOptionSwab::
CQFontOptionSwab(CQFontOptionDialog *dialog) :
 dialog_(dialog)
{
  QFontMetrics fm(font());

  tw_ = fm.horizontalAdvance("ABCabc");
  th_ = fm.ascent() + fm.descent();
  fw_ = 32;

  setFixedSize(tw_ + fw_ + 6, th_ + 4);
}

void
CQFontOptionSwab::
paintEvent(QPaintEvent *)
{
  QPainter painter(this);

  QFont font = dialog_->getFont();

  font.setPointSizeF(12);

  painter.setFont(font);

  painter.drawText(rect(), "ABCabc");
}
