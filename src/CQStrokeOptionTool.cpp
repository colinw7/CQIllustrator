#include <CQStrokeOptionTool.h>
#include <CQStrokeOptionDialog.h>
#include <CQIllustrator.h>
#include <CQUtil.h>

#include <QVBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QPainter>

class CQStrokeOptionSwab : public QWidget {
 public:
  CQStrokeOptionSwab(CQStrokeOptionDialog *dialog);

 private:
  void paintEvent(QPaintEvent *e) override;

 private:
  CQStrokeOptionDialog *dialog_ { nullptr };
  int                   tw_ { 1 }, th_ { 1 }, fw_ { 1 };
};

//----------

CQStrokeOptionTool::
CQStrokeOptionTool(CQIllustrator *illustrator) :
 CQOptionTool(), illustrator_(illustrator), dialog_(0)
{
  setObjectName("stroke");

  dialog_ = new CQStrokeOptionDialog(this);

  auto *layout = CQUtil::makeLayout<QVBoxLayout>(this, 0, 0);

  auto *label = CQUtil::makeLabelWidget<QLabel>("<small><b>Stroke</b></small>", "label");

  layout->addWidget(label);

  swab_ = new CQStrokeOptionSwab(dialog_);

  layout->addWidget(swab_);

  connect(dialog_, SIGNAL(valueChanged(const CQIllustratorShapeStroke &)),
          this, SIGNAL(valueChanged(const CQIllustratorShapeStroke &)));

  connect(illustrator_, SIGNAL(selectionChanged()),
          this, SLOT(selectionChangedSlot()));
}

CQOptionToolDialog *
CQStrokeOptionTool::
getDialog()
{
  return dialog_;
}

void
CQStrokeOptionTool::
selectionChangedSlot()
{
  const CQIllustratorShape *shape = 0;

  const CQIllustratorSelectedShapes *selection = illustrator_->getSelection();

  uint num_selected = selection->size();

  if (num_selected == 1) {
    const CQIllustratorSelectedShape &sshape = selection->front();

    shape = sshape.getShape();
  }

  CQIllustratorShapeStroke stroke;

  if (shape)
    stroke = shape->getStroke();
  else
    stroke = illustrator_->getDefaultStroke();

  dialog_->setStroke(stroke);

  swab_->update();
}

//----------

CQStrokeOptionSwab::
CQStrokeOptionSwab(CQStrokeOptionDialog *dialog) :
 dialog_(dialog)
{
  setObjectName("swab");

  QFontMetrics fm(font());

  tw_ = fm.width("XX.XX");
  th_ = fm.ascent() + fm.descent();
  fw_ = 32;

  setFixedSize(tw_ + fw_ + 6, th_ + 4);
}

void
CQStrokeOptionSwab::
paintEvent(QPaintEvent *)
{
  QPainter painter(this);

  const CQIllustratorShapeStroke &stroke = dialog_->getStroke();

  QString wstr = QString("%1").arg(stroke.getWidth());

  QFontMetrics fm(font());

  //int w = fm.width(wstr);

  int x1 = 2;
  int x2 = x1 + fw_;
  int x3 = x2 + 2;

  int y1 = 2;
  int y2 = y1 + th_;

  painter.eraseRect(rect());

  painter.setPen(QColor(0,0,0));

  painter.drawText(QRect(x3, y1, tw_, y2 - y1), wstr);

  painter.setBrush(CQUtil::rgbaToColor(stroke.getColor()));

  painter.drawRect(QRect(x1, y1, x2 - x1, y2 - y1));
}
