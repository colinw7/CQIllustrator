#include <CQFillOptionTool.h>
#include <CQFillOptionDialog.h>
#include <CQIllustrator.h>
#include <CQUtil.h>
#include <CLinearGradient.h>
#include <CRadialGradient.h>

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
