#include <CQIllustratorTransformMode.h>
#include <CQIllustrator.h>
#include <CQIllustratorShapeDrawer.h>

#include <QGridLayout>
#include <QLabel>
#include <QTabWidget>
#include <QPushButton>
#include <QToolButton>
#include <QComboBox>
#include <QCheckBox>
#include <QPainter>
#include <QMenu>
#include <QAction>
#include <QIcon>
#include <QPixmap>
#include <QEvent>

#include <CQRealEdit.h>
#include <CQAngleSpinBox.h>
#include <CQUtil.h>
#include <CQMenu.h>
#include <CQDockWidget.h>

#include <xpm/transform.xpm>

CQIllustratorTransformMode::
CQIllustratorTransformMode(CQIllustrator *illustrator) :
 CQIllustratorMode(illustrator, CQIllustrator::MODE_TRANSFORM)
{
}

CQIllustratorTransformToolbar *
CQIllustratorTransformMode::
createToolbar()
{
  toolbar_ = new CQIllustratorTransformToolbar(this);

  return toolbar_;
}

CQMenuItem *
CQIllustratorTransformMode::
createMenuItem(CQMenu *menu)
{
  menuItem_ = new CQMenuItem(menu, "&Transform", CQMenuItem::CHECKABLE);

  menuItem_->setStatusTip("Transform Object");
  menuItem_->setXPMIcon(transform_data);

  connect(menuItem_->getAction(), SIGNAL(toggled(bool)), this, SLOT(menuItemSlot()));

  return menuItem_;
}

void
CQIllustratorTransformMode::
handleMouseRelease(const MouseEvent &e)
{
  if (dragging_) {
    // not moving so select control point
    if (! moving_) {
      selectControlPoint(e);
    }
    // dragging finished so commit
    else {
    }
  }
  // not dragging so do a select
  else {
    CPoint2D p1 = CQUtil::fromQPoint(press_wpos_);
    CPoint2D p2 = CQUtil::fromQPoint(release_wpos_);

    // point click - select at point
    if (! moving_) {
      (void) illustrator_->selectAt(p2, e.event->isControlKey(), e.event->isShiftKey());
    }
    // drag then create rectangle using specified bbox
    else {
      (void) illustrator_->selectIn(CBBox2D(p1, p2), e.event->isControlKey(),
                                    e.event->isShiftKey());
    }
  }

  CQIllustratorMode::handleMouseRelease(e);
}

void
CQIllustratorTransformMode::
handleMouseDrag(const MouseEvent &)
{
  illustrator_->redrawOverlay();
}

void
CQIllustratorTransformMode::
drawOverlay(CQIllustratorShapeDrawer *)
{
}

QCursor
CQIllustratorTransformMode::
getCursor() const
{
  return Qt::OpenHandCursor;
}

//-------------------

CQIllustratorTransformToolbar::
CQIllustratorTransformToolbar(CQIllustratorTransformMode *mode) :
 CQIllustratorToolbar(mode), mode_(mode)
{
}

CQIllustrator *
CQIllustratorTransformToolbar::
getIllustrator() const
{
  return mode_->getIllustrator();
}

QIcon
CQIllustratorTransformToolbar::
getIcon()
{
  return QIcon(QPixmap(transform_data));
}

void
CQIllustratorTransformToolbar::
addWidgets()
{
  QBoxLayout *layout = qobject_cast<QBoxLayout *>(CQIllustratorToolbar::layout());

  QGridLayout *grid = new QGridLayout;
  grid->setMargin(0); grid->setSpacing(2);

  layout->addLayout(grid);

  //-----

  tab_ = new QTabWidget;

  QFont font = tab_->font();

  font.setPointSizeF(font.pointSizeF()*0.7);

  tab_->setFont(font);

  //------

  moveTool_ = new CQTransformMoveTool(this);

  tab_->addTab(moveTool_, "Move");

  //------

  scaleTool_ = new CQTransformScaleTool(this);

  tab_->addTab(scaleTool_, "Scale");

  //------

  rotateTool_ = new CQTransformRotateTool(this);

  tab_->addTab(rotateTool_, "Rotate");

  //------

  skewTool_ = new CQTransformSkewTool(this);

  tab_->addTab(skewTool_, "Skew");

  //------

  grid->addWidget(tab_, 0, 0, 2, 1);

  //------

  QPushButton *resetButton = new QPushButton("Reset");

  connect(resetButton, SIGNAL(clicked()), this, SLOT(resetSlot()));

  grid->addWidget(resetButton, 0, 1, 2, 1);

  //------

  QPushButton *applyButton = new QPushButton("Apply");

  connect(applyButton, SIGNAL(clicked()), this, SLOT(applySlot()));

  grid->addWidget(applyButton, 0, 2, 2, 1);

  //------

  grid->setColumnStretch(3, 1);
}

void
CQIllustratorTransformToolbar::
resetSlot()
{
}

void
CQIllustratorTransformToolbar::
applySlot()
{
  int ind = tab_->currentIndex();

  if      (ind == 0) moveTool_  ->apply();
  else if (ind == 1) scaleTool_ ->apply();
  else if (ind == 2) rotateTool_->apply();
  else if (ind == 3) skewTool_  ->apply();
}

//-------

CQTransformMoveTool::
CQTransformMoveTool(CQIllustratorTransformToolbar *toolbar) :
 QWidget(NULL), toolbar_(toolbar)
{
  QGridLayout *grid = new QGridLayout(this);
  grid->setMargin(0); grid->setSpacing(8);

  QLabel *hlabel = new QLabel("H");
  QLabel *vlabel = new QLabel("V");

  hedit_ = new CQRealEdit(0.0);
  vedit_ = new CQRealEdit(0.0);

  relativeCheck_ = new QCheckBox("Relative");
  groupCheck_    = new QCheckBox("As Group");

  relativeCheck_->setChecked(true);
  groupCheck_   ->setChecked(true);

  grid->addWidget(hlabel, 0, 0);
  grid->addWidget(hedit_, 0, 1);
  grid->addWidget(vlabel, 0, 2);
  grid->addWidget(vedit_, 0, 3);

  grid->addWidget(relativeCheck_, 0, 4);
  grid->addWidget(groupCheck_   , 0, 5);

  grid->setColumnStretch(6, 1);
}

void
CQTransformMoveTool::
apply()
{
  bool relative = relativeCheck_->isChecked();

  CQIllustrator *illustrator = toolbar_->getIllustrator();

  CPoint2D d(hedit_->getValue(), vedit_->getValue());

  bool snap_x = (relative ? fabs(d.x) > 0 : true);
  bool snap_y = (relative ? fabs(d.y) > 0 : true);

  illustrator->startUndoGroup("Move Current");

  CQIllustratorSelectedShapes *selection = illustrator->getSelection();

  CQIllustratorSelectedShapes::iterator ps1, ps2;

  for (ps1 = selection->begin(), ps2 = selection->end(); ps1 != ps2; ++ps1) {
    CQIllustratorShape *shape = (*ps1).getShape();

    if (shape->getFixed()) continue;

    const CBBox2D &bbox = shape->getBBox();

    CPoint2D ll;

    if (relative)
      ll = bbox.getLL() + d;
    else
      ll = d;

    if (snap_x) ll.x = CMathGen::Round(ll.x);
    if (snap_y) ll.y = CMathGen::Round(ll.y);

    shape->moveTo(ll);
  }

  illustrator->endUndoGroup();

  illustrator->redraw();
}

//-------

CQTransformScaleTool::
CQTransformScaleTool(CQIllustratorTransformToolbar *toolbar) :
 QWidget(NULL), toolbar_(toolbar)
{
  QGridLayout *grid = new QGridLayout(this);
  grid->setMargin(0); grid->setSpacing(8);

  QLabel *wlabel = new QLabel("W");
  QLabel *hlabel = new QLabel("H");

  wedit_ = new CQRealEdit(1.0);
  hedit_ = new CQRealEdit(1.0);

  equalCheck_ = new QCheckBox("Equal Scale");
  groupCheck_ = new QCheckBox("As Group");

  grid->addWidget(wlabel, 0, 0);
  grid->addWidget(wedit_, 0, 1);
  grid->addWidget(hlabel, 0, 2);
  grid->addWidget(hedit_, 0, 3);

  grid->addWidget(equalCheck_, 0, 4);
  grid->addWidget(groupCheck_, 0, 5);

  grid->setColumnStretch(5, 1);
}

void
CQTransformScaleTool::
apply()
{
  //bool equal = equalCheck_->isChecked();

  CQIllustrator *illustrator = toolbar_->getIllustrator();

  double dw = wedit_->getValue();
  double dh = hedit_->getValue();

  illustrator->startUndoGroup("Resize Current");

  CQIllustratorSelectedShapes *selection = illustrator->getSelection();

  CQIllustratorSelectedShapes::iterator ps1, ps2;

  for (ps1 = selection->begin(), ps2 = selection->end(); ps1 != ps2; ++ps1) {
    CQIllustratorShape *shape = (*ps1).getShape();

    if (shape->getFixed()) continue;

    CBBox2D bbox = shape->getBBox();

    bbox.setWidth (bbox.getWidth ()*dw);
    bbox.setHeight(bbox.getHeight()*dh);

    shape->setBBox(bbox);
  }

  illustrator->endUndoGroup();

  illustrator->redraw();
}

//-------

CQTransformRotateTool::
CQTransformRotateTool(CQIllustratorTransformToolbar *toolbar) :
 QWidget(NULL), toolbar_(toolbar)
{
  QGridLayout *grid = new QGridLayout(this);
  grid->setMargin(0); grid->setSpacing(8);

  angleEdit_ = new CQAngleSpinBox(0.0);

  groupCheck_ = new QCheckBox("As Group");

  grid->addWidget(angleEdit_, 0, 0);

  grid->addWidget(groupCheck_, 0, 1);

  grid->setColumnStretch(2, 1);
}

void
CQTransformRotateTool::
apply()
{
  CQIllustrator *illustrator = toolbar_->getIllustrator();

  double a = M_PI*angleEdit_->getValue()/180.0;

  illustrator->startUndoGroup("Rotate Current");

  CQIllustratorSelectedShapes *selection = illustrator->getSelection();

  CQIllustratorSelectedShapes::iterator ps1, ps2;

  for (ps1 = selection->begin(), ps2 = selection->end(); ps1 != ps2; ++ps1) {
    CQIllustratorShape *shape = (*ps1).getShape();

    if (shape->getFixed()) continue;

    shape->rotate(a);
  }

  illustrator->endUndoGroup();

  illustrator->redraw();
}

//-------

CQTransformSkewTool::
CQTransformSkewTool(CQIllustratorTransformToolbar *toolbar) :
 QWidget(NULL), toolbar_(toolbar)
{
  QGridLayout *grid = new QGridLayout(this);
  grid->setMargin(0); grid->setSpacing(8);

  QLabel *hlabel = new QLabel("H");
  QLabel *vlabel = new QLabel("V");

  hedit_ = new CQRealEdit(0.0);
  vedit_ = new CQRealEdit(0.0);

  groupCheck_ = new QCheckBox("As Group");

  grid->addWidget(hlabel, 0, 0);
  grid->addWidget(hedit_, 0, 1);
  grid->addWidget(vlabel, 0, 2);
  grid->addWidget(vedit_, 0, 3);

  grid->addWidget(groupCheck_, 0, 4);

  grid->setColumnStretch(5, 1);
}

void
CQTransformSkewTool::
apply()
{
  CQIllustrator *illustrator = toolbar_->getIllustrator();

  double dx = hedit_->getValue();
  double dy = vedit_->getValue();

  illustrator->startUndoGroup("Rotate Current");

  CQIllustratorSelectedShapes *selection = illustrator->getSelection();

  CQIllustratorSelectedShapes::iterator ps1, ps2;

  for (ps1 = selection->begin(), ps2 = selection->end(); ps1 != ps2; ++ps1) {
    CQIllustratorShape *shape = (*ps1).getShape();

    if (shape->getFixed()) continue;

    shape->skew(dx, dy);
  }

  illustrator->endUndoGroup();

  illustrator->redraw();
}
