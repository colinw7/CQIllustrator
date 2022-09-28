#include <CQIllustratorPropertiesDlg.h>
#include <CQIllustrator.h>
#include <CQIllustratorShape.h>
#include <CQIllustratorEllipseShape.h>
#include <CQIllustratorNPolyShape.h>
#include <CQIllustratorRectShape.h>
#include <CQIllustratorStarShape.h>
#include <CQIllustratorTextShape.h>
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

  connect(tree_, SIGNAL(valueChanged(QObject *, const QString &)),
          illustrator, SLOT(redraw()));
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
  tree_->addProperty("", illustrator_, "flipY");
  tree_->addProperty("", illustrator_, "snapEnabled");
  tree_->addProperty("", illustrator_, "snapXPitch");
  tree_->addProperty("", illustrator_, "snapYPitch");

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
  tree_->addProperty(objName, shape, "flatBBox");
  tree_->addProperty(objName, shape, "fixed");
  tree_->addProperty(objName, shape, "visible");
  tree_->addProperty(objName, shape, "clip");

  auto *ellipse = dynamic_cast<CQIllustratorEllipseShape *>(shape);
  auto *npoly   = dynamic_cast<CQIllustratorNPolyShape *>(shape);
  auto *rect    = dynamic_cast<CQIllustratorRectShape *>(shape);
  auto *star    = dynamic_cast<CQIllustratorStarShape *>(shape);
  auto *text    = dynamic_cast<CQIllustratorTextShape *>(shape);

  if      (ellipse) {
    tree_->addProperty(objName, ellipse, "angle1");
    tree_->addProperty(objName, ellipse, "angle2");
  }
  else if (npoly) {
    tree_->addProperty(objName, npoly, "numPoints");
    tree_->addProperty(objName, npoly, "radius");
    tree_->addProperty(objName, npoly, "angle");
  }
  else if (rect) {
    tree_->addProperty(objName, rect, "radiusX");
    tree_->addProperty(objName, rect, "radiusY");
  }
  else if (star) {
    tree_->addProperty(objName, star, "numPoints");
    tree_->addProperty(objName, star, "innerRadius");
    tree_->addProperty(objName, star, "outerRadius");
    tree_->addProperty(objName, star, "innerAngle");
    tree_->addProperty(objName, star, "outerAngle");
  }
  else if (text) {
    tree_->addProperty(objName, text, "text");
    tree_->addProperty(objName, text, "ll"  );
    tree_->addProperty(objName, text, "ur"  );
  }

  for (const auto &child : shape->getChildren())
    loadShape(objName, child);
}

void
CQIllustratorPropertiesDlg::
itemSelectedSlot(QObject *obj, const QString & /*path*/)
{
  auto *shape = qobject_cast<CQIllustratorShape *>(obj);
  if (! obj) return;

  illustrator_->setSelectShape(shape);
}

QSize
CQIllustratorPropertiesDlg::
sizeHint() const
{
  QFontMetrics fm(font());

  int w = fm.horizontalAdvance("X")*50;
  int h = fm.height()*25;

  return QSize(w, h);
}
