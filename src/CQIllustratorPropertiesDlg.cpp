#include <CQIllustratorPropertiesDlg.h>
#include <CQIllustrator.h>
#include <CQIllustratorShape.h>
#include <CQPropertyTree.h>
#include <QVBoxLayout>

CQIllustratorPropertiesDlg::
CQIllustratorPropertiesDlg(CQIllustrator *illustrator) :
 illustrator_(illustrator)
{
  setWindowTitle("Properties");

  QVBoxLayout *layout = new QVBoxLayout(this);
  layout->setMargin(0); layout->setSpacing(2);

  tree_ = new CQPropertyTree;

#if 0
  connect(tree_, SIGNAL(valueChanged(QObject *, const QString &)),
          illustrator, SLOT(redraw()));
#endif

  connect(tree_, SIGNAL(itemSelected(QObject *, const QString &)),
          this, SLOT(itemSelectedSlot(QObject *, const QString &)));

  layout->addWidget(tree_);
}

void
CQIllustratorPropertiesDlg::
load()
{
  tree_->clear();

  tree_->addProperty("", illustrator_, "background");
  tree_->addProperty("", illustrator_, "rect");
  tree_->addProperty("", illustrator_, "fullRect");

  for (const auto &shape : illustrator_->getShapes()) {
    if (shape->getParent()) continue;

    loadShape("", shape);
  }
}

void
CQIllustratorPropertiesDlg::
loadShape(const QString &parentName, CQIllustratorShape *shape)
{
  QString objName;

  if (parentName != "")
    objName = parentName + "/" + shape->getName().c_str();
  else
    objName = shape->getName().c_str();

  tree_->addProperty(objName, shape, "id");
  tree_->addProperty(objName, shape, "name");
  tree_->addProperty(objName, shape, "bbox");
  tree_->addProperty(objName, shape, "fixed");
  tree_->addProperty(objName, shape, "visible");

  for (const auto &child : shape->getChildren())
    loadShape(objName, child);
}

void
CQIllustratorPropertiesDlg::
itemSelectedSlot(QObject *obj, const QString & /*path*/)
{
  CQIllustratorShape *shape = qobject_cast<CQIllustratorShape *>(obj);
  if (! obj) return;

  illustrator_->setSelectShape(shape);
}

QSize
CQIllustratorPropertiesDlg::
sizeHint() const
{
  QFontMetrics fm(font());

  int w = fm.width("X")*50;
  int h = fm.height()  *25;

  return QSize(w, h);
}
