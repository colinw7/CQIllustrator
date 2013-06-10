#include <CQIllustratorInfo.h>
#include <CQIllustratorCanvas.h>
#include <CQIllustratorShape.h>

#include <QVBoxLayout>
#include <QLabel>

#include <CQUtil.h>

CQIllustratorInfo::
CQIllustratorInfo(CQIllustratorCanvas *canvas) :
 QWidget(canvas), canvas_(canvas)
{
  setAutoFillBackground(true);

  CQUtil::setBackground(this, QColor(255,255,204));

  QVBoxLayout *layout = new QVBoxLayout(this);

  label_ = new QLabel("Info");

  layout->addWidget(label_);

  hide();
}

void
CQIllustratorInfo::
setShape(const CQIllustratorShape *shape)
{
  const CBBox2D &bbox = shape->getBBox();

  QStringList lines;

  lines.push_back(QString("<b>%1</b>: %2").
    arg(shape->getClassName()).arg(shape->getName().c_str()));
  lines.push_back(QString("<b>BBox</b>: {{%1 %2} {%3 %4}}").
    arg(bbox.getXMin()).arg(bbox.getYMin()).arg(bbox.getXMax()).arg(bbox.getYMax()));

  int num_lines = lines.length();

  QString text;

  text += "<small>";

  for (int i = 0; i < num_lines; ++i) {
    if (i > 0) text += "<br>";

    text += lines[i];
  }

  text += "</small>";

  label_->setText(text);
}
