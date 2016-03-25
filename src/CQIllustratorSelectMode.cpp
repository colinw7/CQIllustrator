#include <CQIllustratorSelectMode.h>
#include <CQIllustrator.h>
#include <CQIllustratorShapeDrawer.h>

#include <QPainter>
#include <QHBoxLayout>

#include <CQMenu.h>
#include <CQUtil.h>
#include <CQSwatch.h>
#include <CQRadioButtons.h>
#include <CQImageButton.h>

#include <svg/select_svg.h>
#include <svg/select_all_svg.h>
#include <svg/select_none_svg.h>

#include <cursors/select.xbm>
#include <cursors/selectmask.xbm>

enum { ROTATE_SMALL = 1, ROTATE_NORMAL = 15, ROTATE_LARGE = 90 };

CQIllustratorSelectMode::
CQIllustratorSelectMode(CQIllustrator *illustrator) :
 CQIllustratorMode(illustrator, CQIllustrator::MODE_SELECT), sizing_(false), inside_(true)
{
  setCursor(select_bits, selectmask_bits, 2, 2);

  sizer_ = new CQIllustratorSizer(illustrator_);
}

CQIllustratorSelectToolbar *
CQIllustratorSelectMode::
createToolbar()
{
  toolbar_ = new CQIllustratorSelectToolbar(this);

  return toolbar_;
}

CQMenuItem *
CQIllustratorSelectMode::
createMenuItem(CQMenu *menu)
{
  QIcon icon = CQPixmapCacheInst->getIcon("SELECT");

  menuItem_ = new CQMenuItem(menu, icon, "&Select Shape", CQMenuItem::CHECKABLE);

  menuItem_->setStatusTip("Select Shape");

  connect(menuItem_->getAction(), SIGNAL(toggled(bool)), this, SLOT(menuItemSlot()));

  return menuItem_;
}

void
CQIllustratorSelectMode::
setInside(bool inside)
{
  inside_ = inside;
}

void
CQIllustratorSelectMode::
handleMousePress(const MouseEvent &)
{
  sizing_   = false;
  dragging_ = false;

  // if in sizer handle then sizing
  if (sizer_->mousePress(press_wpos_)) {
    sizing_ = true;
  }
  // if press point inside a shape then dragging
  else {
    CPoint2D p = CQUtil::fromQPoint(press_wpos_);

    CQIllustratorSelectedShapes *selection = illustrator_->getSelection();

    CQIllustratorSelectedShapes::iterator ps1, ps2;

    for (ps1 = selection->begin(), ps2 = selection->end(); ps1 != ps2; ++ps1) {
      CQIllustratorShape *shape = (*ps1).getShape();

      const CBBox2D &bbox = shape->getBBox();

      if (bbox.inside(p)) {
        dragging_ = true;
        break;
      }
    }
  }

  if (dragging_ || sizing_) {
    illustrator_->getSandbox()->addSelection();

    if (illustrator_->getSandbox()->getNumShapes() == 1) {
      CQIllustratorShape *shape = illustrator_->getSandbox()->frontShape();

      drag_bbox_   = shape->getBBox();
      drag_center_ = shape->getRotateCenter();
      drag_matrix_ = shape->getMatrix();
    }
    else {
      drag_bbox_   = illustrator_->getSandbox()->getBBox();
      drag_center_ = drag_bbox_.getCenter();
      drag_matrix_ = CMatrix2D();
    }

    illustrator_->redraw();
  }
}

void
CQIllustratorSelectMode::
handleMouseRelease(const MouseEvent &e)
{
  if ((dragging_ || sizing_) && ! moving_) {
    illustrator_->getSandbox()->clear();

    dragging_ = false;
    sizing_   = false;
  }

  // not sizing (move/resize/rotate) and not dragging so do a select
  if (! sizing_ && ! dragging_) {
    CPoint2D p = CQUtil::fromQPoint(e.window);

    // point click - select at point and toggle sizer mode if same shape selected
    if (! moving_) {
      CQIllustratorSelectedShapes *selection = illustrator_->getSelection();

      bool single_shape = (selection->size() == 1);

      CQIllustratorShape *old_select_shape = 0;

      if (single_shape)
        old_select_shape = selection->front().getShape();

      CQIllustratorShape *select_shape =
        illustrator_->selectAt(p, e.event->isControlKey(), e.event->isShiftKey());

      if (single_shape && select_shape == old_select_shape)
        sizer_->toggleOp();
    }
    // box drag - select inside or touching range
    else {
      CBBox2D bbox(CQUtil::fromQPoint(press_wpos_), p);

      if (inside_)
        illustrator_->selectIn(bbox, e.event->isControlKey(), e.event->isShiftKey());
      else
        illustrator_->selectOverlap(bbox, e.event->isControlKey(), e.event->isShiftKey());
    }

    illustrator_->redraw();
  }
  // sizing or dragging finished so commit
  else {
    if (sizing_)
      sizer_->mouseRelease(e.window);

    illustrator_->getSandbox()->commit(CQIllustratorData::CHANGE_GEOMETRY);
  }

  dragging_ = false;
  sizing_   = false;
}

void
CQIllustratorSelectMode::
handleMouseDrag(const MouseEvent &e)
{
  if      (sizing_) {
    CQIllustratorShape *shape = illustrator_->getSandbox()->frontShape();

    if (shape) {
      shape->setBBox        (drag_bbox_);
      shape->setRotateCenter(drag_center_);
      shape->setMatrix      (drag_matrix_);

      sizer_->updateShape(shape, press_wpos_, e.window, e.event->isShiftKey());
    }
  }
  else if (dragging_) {
    double dx = e.window.x() - press_wpos_.x();
    double dy = e.window.y() - press_wpos_.y();

    if (e.event->isShiftKey()) {
      if (fabs(dx) >= fabs(dy)) dy = 0;
      else                      dx = 0;
    }

    CBBox2D bbox = illustrator_->getSandbox()->getBBox();

    CPoint2D d = drag_bbox_.getLL() - bbox.getLL() + CPoint2D(dx, dy);

    illustrator_->getSandbox()->moveBy(d);
  }

  illustrator_->redrawOverlay();
}

void
CQIllustratorSelectMode::
handleMouseMove(const MouseEvent &e)
{
  if (sizer_->updateActive(e.window))
    illustrator_->redrawOverlay();
}

bool
CQIllustratorSelectMode::
handleKeyPress(const KeyEvent &e)
{
  CKeyType key = e.event->getType();

  if (key == CKEY_TYPE_Less) {
    illustrator_->getSandbox()->addSelection();

    if (illustrator_->getSandbox()->getNumShapes() == 1) {
      CQIllustratorShape *shape = illustrator_->getSandbox()->frontShape();

      if (! e.event->isControlKey())
        shape->rotate( ROTATE_NORMAL*M_PI/180.0);
      else
        shape->rotate( ROTATE_SMALL *M_PI/180.0);

      illustrator_->getSandbox()->commit(CQIllustratorData::CHANGE_GEOMETRY);

      return true;
    }
  }
  else if (key == CKEY_TYPE_Greater) {
    illustrator_->getSandbox()->addSelection();

    if (illustrator_->getSandbox()->getNumShapes() == 1) {
      CQIllustratorShape *shape = illustrator_->getSandbox()->frontShape();

      if (! e.event->isControlKey())
        shape->rotate(-ROTATE_NORMAL*M_PI/180.0);
      else
        shape->rotate(-ROTATE_SMALL *M_PI/180.0);

      illustrator_->getSandbox()->commit(CQIllustratorData::CHANGE_GEOMETRY);

      return true;
    }
  }

  return CQIllustratorMode::handleKeyPress(e);
}

void
CQIllustratorSelectMode::
drawOverlay(CQIllustratorShapeDrawer *drawer)
{
  QPainter *painter = drawer->getPainter();

  // draw sizer if not dragging
  if (! dragging_ && ! sizing_) {
    // draw selection rubber band
    if (pressed_) {
      QPointF current_wpos = illustrator_->getITransform().map(QPointF(curr_ppos_));

      QPen pen;

      pen.setColor(QColor(0,0,0));
      pen.setStyle(Qt::DashLine);

      painter->setPen(pen);
      painter->setBrush(Qt::NoBrush);

      painter->drawRect(QRectF(press_wpos_, current_wpos));
    }

    //------

    // draw sizer for single selected shape
    CQIllustratorSelectedShapes *selection = illustrator_->getSelection();

    if (selection->size() == 1) {
      CQIllustratorShape *shape = selection->front().getShape();

      sizer_->draw(drawer, shape);
    }
  }
  else {
    if (sizing_) {
      CQIllustratorShape *shape = illustrator_->getSandbox()->frontShape();

      if (shape)
        sizer_->draw(drawer, shape);
    }
  }
}

//---------------

CQIllustratorSelectToolbar::
CQIllustratorSelectToolbar(CQIllustratorSelectMode *mode) :
 CQIllustratorToolbar(mode), mode_(mode)
{
}

QIcon
CQIllustratorSelectToolbar::
getIcon()
{
  return CQPixmapCacheInst->getIcon("SELECT");
}

void
CQIllustratorSelectToolbar::
addWidgets()
{
  QBoxLayout *layout = qobject_cast<QBoxLayout *>(CQIllustratorToolbar::layout());

  selectAllButton_  = new CQImageButton(CQPixmapCacheInst->getIcon("SELECT_ALL" ));
  selectNoneButton_ = new CQImageButton(CQPixmapCacheInst->getIcon("SELECT_NONE"));

  selectAllButton_ ->setToolTip("Select All");
  selectNoneButton_->setToolTip("Select None");

  connect(selectAllButton_ , SIGNAL(clicked()), this, SLOT(selectAllSlot ()));
  connect(selectNoneButton_, SIGNAL(clicked()), this, SLOT(selectNoneSlot()));

  CQSwatch *selectSwatch = new CQSwatch("Select", selectAllButton_, selectNoneButton_);

  layout->addWidget(selectSwatch);

  //-----

  CQHRadioButtons *buttons = new CQHRadioButtons;

  buttons->addButton("Inside");
  buttons->addButton("Overlap");

  connect(buttons, SIGNAL(valueChanged(int)), this, SLOT(setMode(int)));

  CQSwatch *modeSwatch = new CQSwatch("Mode", buttons);

  layout->addWidget(modeSwatch);
}

void
CQIllustratorSelectToolbar::
selectAllSlot()
{
  mode_->getIllustrator()->selectAll();
}

void
CQIllustratorSelectToolbar::
selectNoneSlot()
{
  mode_->getIllustrator()->selectNone();
}

void
CQIllustratorSelectToolbar::
setMode(int ind)
{
  mode_->setInside(ind == 0);
}
