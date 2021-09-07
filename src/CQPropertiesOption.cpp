#include <CQPropertiesOption.h>
#include <CQIllustrator.h>
#include <CQIllustratorTextShape.h>
#include <CQIllustratorShapeFilter.h>

#include <CQPointEdit.h>
#include <CQMatrix2D.h>
#include <CQRealEdit.h>
#include <CQAngleSpinBox.h>

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QTabWidget>

CQPropertiesOptionTool::
CQPropertiesOptionTool(CQIllustrator *illustrator) :
 CQOptionTool(), illustrator_(illustrator), dialog_(0)
{
  dialog_ = new CQPropertiesOptionDialog(this);

  QVBoxLayout *layout = new QVBoxLayout(this);
  layout->setMargin(0); layout->setSpacing(0);

  QLabel *label = new QLabel("<small><b>Properties</b></small>");

  layout->addWidget(label);

  label_ = new QLabel;

  QFontMetrics fm(font());

  int width = fm.width("ABCDEFGHIJKLM");

  label_->setMinimumWidth(width);

  layout->addWidget(label_);

  connect(illustrator_, SIGNAL(selectionChanged()),
          this, SLOT(selectionChangedSlot()));
}

CQOptionToolDialog *
CQPropertiesOptionTool::
getDialog()
{
  return dialog_;
}

void
CQPropertiesOptionTool::
selectionChangedSlot()
{
  const CQIllustratorShape *shape = 0;

  const CQIllustratorSelectedShapes *selection = illustrator_->getSelection();

  uint num_selected = selection->size();

  if (num_selected == 1) {
    const CQIllustratorSelectedShape &sshape = selection->front();

    shape = sshape.getShape();
  }

  if (shape)
    label_->setText(shape->getName().c_str());
  else
    label_->setText("");

  dialog_->update(shape);
}

void
CQPropertiesOptionTool::
emitValueChanged()
{
  emit valueChanged();
}

//----------

CQPropertiesOptionDialog::
CQPropertiesOptionDialog(CQPropertiesOptionTool *tool) :
 CQOptionToolDialog(), tool_(tool)
{
  initWidgets();
}

CQIllustrator *
CQPropertiesOptionDialog::
getIllustrator() const
{
  return tool_->getIllustrator();
}

void
CQPropertiesOptionDialog::
initWidgets()
{
  QVBoxLayout *layout = new QVBoxLayout(this);

  layout->setMargin(0); layout->setSpacing(0);

  tab_ = new QTabWidget;

  layout->addWidget(tab_);

  //-----

  QWidget *valuesPanel = new QWidget;

  QGridLayout *valuesLayout = new QGridLayout(valuesPanel);

  valuesLayout->setMargin(2); valuesLayout->setSpacing(2);

  valuesLayout->addWidget(new QLabel("Name"    ), 0, 0);
  valuesLayout->addWidget(new QLabel("Class"   ), 1, 0);
  valuesLayout->addWidget(new QLabel("Position"), 2, 0);
  valuesLayout->addWidget(new QLabel("Size"    ), 3, 0);
  valuesLayout->addWidget(new QLabel("Text"    ), 4, 0);

  nameEdit_  = new QLineEdit;
  classEdit_ = new QLineEdit;
  posEdit_   = new CQPointEdit;
  sizeEdit_  = new CQPointEdit;
  textEdit_  = new QLineEdit;

  connect(nameEdit_, SIGNAL(returnPressed()), this, SLOT(updateShape()));
  connect(posEdit_ , SIGNAL(valueAccepted()), this, SLOT(updateShape()));
  connect(sizeEdit_, SIGNAL(valueAccepted()), this, SLOT(updateShape()));
  connect(textEdit_, SIGNAL(returnPressed()), this, SLOT(updateShape()));

  valuesLayout->addWidget(nameEdit_ , 0, 1);
  valuesLayout->addWidget(classEdit_, 1, 1);
  valuesLayout->addWidget(posEdit_  , 2, 1);
  valuesLayout->addWidget(sizeEdit_ , 3, 1);
  valuesLayout->addWidget(textEdit_ , 4, 1);

  valuesLayout->setRowStretch(5, 1);

  tab_->addTab(valuesPanel, "Values");

  //-----

  QWidget *pointsPanel = new QWidget;

  QVBoxLayout *pointsLayout = new QVBoxLayout(pointsPanel);

  pointsLayout->setMargin(2); pointsLayout->setSpacing(2);

  table_ = new CQControlPointsTable(this);

  pointsLayout->addWidget(table_);

  tab_->addTab(pointsPanel, "Points");

  //-----

  transformPanel_ = new CQTransformPanel(this);

  tab_->addTab(transformPanel_, "Transform");

  //-----

  QWidget *filterPanel = new QWidget;

  QGridLayout *filterLayout = new QGridLayout(filterPanel);

  filterLayout->setMargin(2); filterLayout->setSpacing(2);

  filterLayout->addWidget(new QLabel("Filter"), 0, 0);

  filterEdit_ = new QLineEdit;

  connect(filterEdit_, SIGNAL(returnPressed()), this, SLOT(updateShape()));

  filterLayout->addWidget(filterEdit_, 0, 1);

  filterLayout->setRowStretch(2, 1);

  tab_->addTab(filterPanel, "Filter");
}

void
CQPropertiesOptionDialog::
updateWidgets()
{
}

void
CQPropertiesOptionDialog::
update(const CQIllustratorShape *shape)
{
  table_->clear();

  if (shape) {
    name_ = shape->getName();

    const CBBox2D &bbox = shape->getBBox();

    nameEdit_ ->setText (name_.c_str());
    classEdit_->setText (shape->getClassName());

    if (bbox.isSet()) {
      posEdit_ ->setValue(bbox.getLL());
      sizeEdit_->setValue(CPoint2D(bbox.getWidth(), bbox.getHeight()));
    }
    else {
      posEdit_ ->setValue(CPoint2D(0,0));
      sizeEdit_->setValue(CPoint2D(0,0));
    }

    const CQIllustratorTextShape *text = dynamic_cast<const CQIllustratorTextShape *>(shape);

    if (text)
      textEdit_->setText(text->getText().c_str());
    else
      textEdit_->setText("");

    CQIllustratorShape::ControlPointList points;

    shape->getControlPoints(points);

    uint num_points = points.size();

    table_->setRowCount(num_points);

    for (uint i = 0; i < num_points; ++i) {
      CQIllustratorShapeControlPoint *point = points[i];

      table_->addPoint(shape, i, point);
    }

    transformPanel_->getMatrix()->setValue(shape->getMatrix());

    CQIllustratorShapeFilter *filter = shape->getFilter();

    if (filter)
      filterEdit_->setText(QString("%1").arg(filter->getId()));
    else
      filterEdit_->setText("<none>");
  }
  else {
    name_ = "";

    nameEdit_ ->setText("");
    classEdit_->setText("");
    posEdit_  ->setText("");
    sizeEdit_ ->setText("");
    textEdit_ ->setText("");

    transformPanel_->getMatrix()->setValue(CMatrix2D(CMatrix2D::Type::IDENTITY));

    filterEdit_->setText("");
  }
}

void
CQPropertiesOptionDialog::
updateShape()
{
  CQIllustrator *illustrator = getIllustrator();

  CQIllustratorShape *shape = illustrator->getShape(name_);

  if (shape) {
    illustrator->checkoutShape(shape, CQIllustratorData::ChangeType::GEOMETRY);

    shape->setName(nameEdit_->text().toStdString());

    CPoint2D pos = posEdit_->getValue();

    shape->moveTo(pos);

    CQIllustratorTextShape *text = dynamic_cast<CQIllustratorTextShape *>(shape);

    if (text)
      text->setText(textEdit_->text().toStdString());

    shape->setMatrix(transformPanel_->getMatrix()->getValue());

    bool ok;

    int filterNum = filterEdit_->text().toInt(&ok);

    if (ok)
      shape->setFilter(filterNum);
    else
      shape->unsetFilter();

    illustrator->checkinShape(shape, CQIllustratorData::ChangeType::GEOMETRY);
  }

  tool_->emitValueChanged();
}

void
CQPropertiesOptionDialog::
replaceMatrixSlot()
{
  CQIllustratorShape *shape = tool_->getIllustrator()->getShape(name_);

  if (! shape) return;

  CMatrix2D m = transformPanel_->getNewMatrix();

  shape->setMatrix(m);

  tool_->emitValueChanged();
}

void
CQPropertiesOptionDialog::
appendMatrixSlot()
{
  CQIllustratorShape *shape = tool_->getIllustrator()->getShape(name_);

  if (! shape) return;

  CMatrix2D m = transformPanel_->getNewMatrix();

  shape->setMatrix(m*shape->getMatrix());

  tool_->emitValueChanged();
}

void
CQPropertiesOptionDialog::
resetMatrixSlot()
{
  CQIllustratorShape *shape = tool_->getIllustrator()->getShape(name_);

  if (! shape) return;

  shape->setMatrix(CMatrix2D(CMatrix2D::Type::IDENTITY));

  tool_->emitValueChanged();
}

//------

CQControlPointsTable::
CQControlPointsTable(CQPropertiesOptionDialog *dialog) :
 CQTableWidget(), dialog_(dialog)
{
  clear();

  connect(this, SIGNAL(valueChanged(int, int)), this, SLOT(updatePointsSlot()));
}

CQIllustrator *
CQControlPointsTable::
getIllustrator() const
{
  return dialog_->getIllustrator();
}

void
CQControlPointsTable::
clear()
{
  CQTableWidget::clear();

  setColumnCount(2);

  setColumnLabel(0, "Index");
  setColumnLabel(1, "Point");
}

void
CQControlPointsTable::
addPoint(const CQIllustratorShape *shape, uint pos, CQIllustratorShapeControlPoint *point)
{
  CQControlPointItem *item = new CQControlPointItem(this, shape, point);

  int ind = point->getId();

  setItem(pos, 0, QString("%1").arg(ind));
  setItem(pos, 1, item);
}

void
CQControlPointsTable::
updatePointsSlot()
{
#if 0
  int ind      = 0;
  int num_rows = rowCount();

  CGenGradient::StopList &stops = list_->getStops();

  for (const auto &stop : stops) {
    if (ind >= num_rows)
      break;

    CQControlPointItem *item = getItem<CQControlPointItem >(ind, 1);

    CPoint2D p = item->getValue();

    shape->setControlPoint(p);

    ++ind;
  }

  list_->emitStopsChanged();
#endif
}

//------

CQControlPointItem::
CQControlPointItem(CQControlPointsTable *t, const CQIllustratorShape *shape,
                   CQIllustratorShapeControlPoint *point) :
 CQTableWidgetItem(t), table_(t), shape_(shape), point_(0), edit_(0)
{
  point_ = point->dup();
}

CQControlPointItem::
~CQControlPointItem()
{
  delete point_;
}

CQControlPointItem *
CQControlPointItem::
clone() const
{
  return new CQControlPointItem(table_, shape_, point_);
}

QString
CQControlPointItem::
getString() const
{
  return CQPointEdit::pointToString(point_->getPoint(shape_)).c_str();
}

QWidget *
CQControlPointItem::
createEditor(QWidget *parent) const
{
  edit_ = new CQPointEdit(parent);

  return edit_;
}

void
CQControlPointItem::
setEditorData()
{
  if (edit_.isNull()) return;

  edit_->setValue(point_->getPoint(shape_));
}

void
CQControlPointItem::
getEditorData(QString &str)
{
  if (edit_.isNull()) return;

  CQIllustrator *illustrator = table_->getIllustrator();

  CPoint2D point = edit_->getValue();

  str = CQPointEdit::pointToString(point).c_str();

  CPoint2D point1 = point_->getPoint(shape_);

  if (point != point1) {
    CQIllustratorShape *shape = const_cast<CQIllustratorShape *>(shape_);

    illustrator->checkoutShape(shape, CQIllustratorData::ChangeType::GEOMETRY);

    point_->setPoint(shape, point);

    illustrator->checkinShape(shape, CQIllustratorData::ChangeType::GEOMETRY);
  }
}

bool
CQControlPointItem::
sizeHint(const QStyleOptionViewItem &option, QSize &size) const
{
  size = QSize(option.rect.width(), option.rect.height());

  return true;
}

//---------------

CQTransformPanel::
CQTransformPanel(CQPropertiesOptionDialog *dialog) :
 dialog_(dialog)
{
  QVBoxLayout *layout = new QVBoxLayout(this);

  layout->setMargin(2); layout->setSpacing(2);

  matrix_ = new CQMatrix2D(this);

  connect(matrix_, SIGNAL(valueChanged()), dialog, SLOT(updateShape()));

  layout->addWidget(matrix_);

  tabWidget_ = new QTabWidget;

  //------

  QWidget *translatePanel = new QWidget;

  QGridLayout *translateLayout = new QGridLayout(translatePanel);

  QLabel *translateXLabel = new QLabel("X");
  QLabel *translateYLabel = new QLabel("Y");

  translateXEdit_ = new CQRealEdit(0.0);
  translateYEdit_ = new CQRealEdit(0.0);

  translateLayout->addWidget(translateXLabel, 0, 0);
  translateLayout->addWidget(translateXEdit_, 0, 1);
  translateLayout->addWidget(translateYLabel, 1, 0);
  translateLayout->addWidget(translateYEdit_, 1, 1);

  tabWidget_->addTab(translatePanel, "Translate");

  //------

  QWidget *scalePanel = new QWidget;

  QGridLayout *scaleLayout = new QGridLayout(scalePanel);

  QLabel *scaleXLabel = new QLabel("X");
  QLabel *scaleYLabel = new QLabel("Y");

  scaleXEdit_ = new CQRealEdit(1.0);
  scaleYEdit_ = new CQRealEdit(1.0);

  scaleLayout->addWidget(scaleXLabel, 0, 0);
  scaleLayout->addWidget(scaleXEdit_, 0, 1);
  scaleLayout->addWidget(scaleYLabel, 1, 0);
  scaleLayout->addWidget(scaleYEdit_, 1, 1);

  tabWidget_->addTab(scalePanel, "Scale");

  //------

  QWidget *rotatePanel = new QWidget;

  QGridLayout *rotateLayout = new QGridLayout(rotatePanel);

  QLabel *rotateAngleLabel = new QLabel("Angle");

  rotateAngleEdit_ = new CQAngleSpinBox(CAngle(0.0));

  rotateLayout->addWidget(rotateAngleLabel, 0, 0);
  rotateLayout->addWidget(rotateAngleEdit_, 0, 1);

  tabWidget_->addTab(rotatePanel, "Rotate");

  //------

  QWidget *skewPanel = new QWidget;

  QGridLayout *skewLayout = new QGridLayout(skewPanel);

  QLabel *skewXLabel = new QLabel("X");
  QLabel *skewYLabel = new QLabel("Y");

  skewXEdit_ = new CQRealEdit(0.0);
  skewYEdit_ = new CQRealEdit(0.0);

  skewLayout->addWidget(skewXLabel, 0, 0);
  skewLayout->addWidget(skewXEdit_, 0, 1);
  skewLayout->addWidget(skewYLabel, 1, 0);
  skewLayout->addWidget(skewYEdit_, 1, 1);

  tabWidget_->addTab(skewPanel, "Skew");

  //------

  layout->addWidget(tabWidget_);

  QHBoxLayout *buttonsLayout = new QHBoxLayout;

  QPushButton *replaceMatrix = new QPushButton("Replace");
  QPushButton *appendMatrix  = new QPushButton("Append");
  QPushButton *resetMatrix   = new QPushButton("Reset");

  buttonsLayout->addWidget(replaceMatrix);
  buttonsLayout->addWidget(appendMatrix);
  buttonsLayout->addWidget(resetMatrix);

  layout->addLayout(buttonsLayout);

  connect(replaceMatrix, SIGNAL(clicked()), dialog, SLOT(replaceMatrixSlot()));
  connect(appendMatrix , SIGNAL(clicked()), dialog, SLOT(appendMatrixSlot()));
  connect(resetMatrix  , SIGNAL(clicked()), dialog, SLOT(resetMatrixSlot()));
}

CMatrix2D
CQTransformPanel::
getNewMatrix() const
{
  int ind = tabWidget_->currentIndex();

  switch (ind) {
    case 0:
      return CMatrix2D::translation(translateXEdit_->getValue(), translateYEdit_->getValue());
    case 1:
      return CMatrix2D::scale(scaleXEdit_->getValue(), scaleYEdit_->getValue());
    case 2:
      return CMatrix2D::rotation(M_PI*rotateAngleEdit_->getAngle().degrees()/180.0);
    case 3:
      return CMatrix2D::skew(skewXEdit_->getValue(), skewYEdit_->getValue());
  }

  return CMatrix2D::identity();
}
