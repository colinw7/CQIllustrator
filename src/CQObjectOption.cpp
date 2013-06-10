#include <CQObjectOption.h>
#include <CQIllustrator.h>

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QTreeWidget>

CQObjectOptionTool::
CQObjectOptionTool(CQIllustrator *illustrator) :
 CQOptionTool(), illustrator_(illustrator), dialog_(NULL)
{
  dialog_ = new CQObjectOptionDialog(this);

  QVBoxLayout *layout = new QVBoxLayout(this);
  layout->setMargin(0); layout->setSpacing(0);

  QLabel *label = new QLabel("<small><b>Selection</b></small>");

  layout->addWidget(label);

  label_ = new QLabel;

  QFontMetrics fm(font());

  int width = fm.width("ABCDEFGHIJKLM");

  label_->setMinimumWidth(width);

  layout->addWidget(label_);

  connect(illustrator_, SIGNAL(selectionChanged()),
          this, SLOT(selectionChangedSlot()));

  connect(dialog_, SIGNAL(valueChanged(const QString &)),
          this, SIGNAL(valueChanged(const QString &)));

  connect(this, SIGNAL(poppingUp()), this, SLOT(populateSlot()));
}

CQOptionToolDialog *
CQObjectOptionTool::
getDialog()
{
  return dialog_;
}

void
CQObjectOptionTool::
selectionChangedSlot()
{
  const CQIllustratorSelectedShapes *selection = illustrator_->getSelection();

  uint num_selected = selection->size();

  if      (num_selected == 0)
    label_->setText("None");
  else if (num_selected == 1) {
    const CQIllustratorSelectedShape &sshape = selection->front();

    const CQIllustratorShape *shape = sshape.getShape();

    uint num_points = sshape.numPoints();
    uint num_lines  = sshape.numLines ();

    std::string str = shape->getName();

    if (num_points > 0)
      str += " P#" + CStrUtil::toString(num_points);

    if (num_lines > 0)
      str += " L#" + CStrUtil::toString(num_lines);

    label_->setText(str.c_str());
  }
  else
    label_->setText(QString("#%1").arg(num_selected));
}

void
CQObjectOptionTool::
populateSlot()
{
  dialog_->populate(illustrator_);
}

void
CQObjectOptionTool::
selectShape(const std::string &name)
{
  illustrator_->setSelectShape(name);
}

//----------

CQObjectOptionDialog::
CQObjectOptionDialog(CQObjectOptionTool *tool) :
 CQOptionToolDialog(), tool_(tool)
{
  initWidgets();
}

void
CQObjectOptionDialog::
initWidgets()
{
  QVBoxLayout *layout = new QVBoxLayout(this);

  list_ = new QTreeWidget;

  layout->addWidget(list_);

  list_->headerItem()->setHidden(true);

  connect(list_, SIGNAL(itemClicked(QTreeWidgetItem *, int)),
          this, SLOT(itemSelectedSlot(QTreeWidgetItem *, int)));
}

void
CQObjectOptionDialog::
updateWidgets()
{
}

void
CQObjectOptionDialog::
itemSelectedSlot(QTreeWidgetItem *item, int pos)
{
  QString name = item->text(pos);

  tool_->selectShape(name.toStdString());
}

void
CQObjectOptionDialog::
populate(CQIllustrator *illustrator)
{
  list_->clear();

  const CQIllustratorData::ShapeStack &shapes = illustrator->getShapes();

  CQIllustratorData::ShapeStack::const_iterator ps1, ps2;

  for (ps1 = shapes.begin(), ps2 = shapes.end(); ps1 != ps2; ++ps1) {
    const CQIllustratorShape *shape = *ps1;

    if (shape->getParent()) continue;

    QStringList strs;

    strs << shape->getName().c_str();

    QTreeWidgetItem *item = new QTreeWidgetItem(strs);

    list_->addTopLevelItem(item);

    populateChildren(illustrator, item, shape);
  }
}

void
CQObjectOptionDialog::
populateChildren(CQIllustrator *illustrator, QTreeWidgetItem *pitem,
                 const CQIllustratorShape *shape)
{
  const CQIllustratorShape::ShapeList &children = shape->getChildren();

  CQIllustratorShape::ShapeList::const_iterator pc1, pc2;

  for (pc1 = children.begin(), pc2 = children.end(); pc1 != pc2; ++pc1) {
    const CQIllustratorShape *child = *pc1;

    QStringList strs;

    strs << child->getName().c_str();

    QTreeWidgetItem *item = new QTreeWidgetItem(strs);

    pitem->addChild(item);

    populateChildren(illustrator, item, child);
  }
}
