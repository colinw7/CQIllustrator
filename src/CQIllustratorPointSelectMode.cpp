#include <CQIllustratorPointSelectMode.h>
#include <CQIllustrator.h>
#include <CQIllustratorHandle.h>
#include <CQIllustratorShapeDrawer.h>

#include <QPainter>
#include <QHBoxLayout>
#include <QPushButton>

#include <CQMenu.h>
#include <CQUtil.h>
#include <CQSwatch.h>

#include "xpm/point_select.xpm"

#include "cursors/select.xbm"
#include "cursors/selectmask.xbm"

#include "xpm/selected_point.xpm"

#define IMAGE_DATA(I) I, sizeof(I)/sizeof(char *)

CQIllustratorPointSelectMode::
CQIllustratorPointSelectMode(CQIllustrator *illustrator) :
 CQIllustratorMode(illustrator, CQIllustrator::MODE_POINT_SELECT)
{
  setCursor(select_bits, selectmask_bits, 2, 2);
}

CQIllustratorPointSelectToolbar *
CQIllustratorPointSelectMode::
createToolbar()
{
  toolbar_ = new CQIllustratorPointSelectToolbar(this);

  return toolbar_;
}
CQIllustratorPointSelectSizer *
CQIllustratorPointSelectMode::
createSizer()
{
  sizer_ = new CQIllustratorPointSelectSizer(this);

  return sizer_;
}

CQMenuItem *
CQIllustratorPointSelectMode::
createMenuItem(CQMenu *menu)
{
  menuItem_ = new CQMenuItem(menu, "&Select Point", CQMenuItem::CHECKABLE);

  menuItem_->setStatusTip("Select Shape Point");
  menuItem_->setXPMIcon(point_select_data);

  connect(menuItem_->getAction(), SIGNAL(toggled(bool)), this, SLOT(menuItemSlot()));

  return menuItem_;
}

void
CQIllustratorPointSelectMode::
handleMouseRelease(const MouseEvent &e)
{
  CPoint2D p2 = CQUtil::fromQPoint(e.window);

  if (dragging_) {
    if (! moving_) {
      selectControlPoint(e);
    }
    // dragging finished so commit
    else {
      illustrator_->getSandbox()->commit(CQIllustratorData::CHANGE_GEOMETRY);
    }
  }
  // not dragging so do a select
  else {
    CPoint2D p1 = CQUtil::fromQPoint(press_wpos_);

    // point click - select at point
    if (! moving_) {
      (void) illustrator_->selectPointAt(p2, getControlType(), e.event->isControlKey(),
                                         e.event->isShiftKey());
    }
    // drag - select points in rectangle
    else {
      CBBox2D bbox(p1, p2);

      illustrator_->selectPointsIn(bbox, CQIllustratorShape::CONTROL_GEOMETRY,
                                   e.event->isControlKey(), e.event->isShiftKey());
    }
  }

  dragging_ = false;

  illustrator_->redraw();
}

void
CQIllustratorPointSelectMode::
handleMouseDrag(const MouseEvent &e)
{
  if (dragging_) {
    CQIllustratorShape *shape = illustrator_->getSandbox()->frontShape();

    sizer_->updatePoint(shape, e.window);
  }
  else {
  }

  illustrator_->redrawOverlay();

  prev_wpos_ = e.window;
}

void
CQIllustratorPointSelectMode::
drawOverlay(CQIllustratorShapeDrawer *drawer)
{
  QPainter *painter = drawer->getPainter();

  if (! dragging_) {
    // draw rubber band
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

    // draw handles for selected shapes
    CQIllustratorSelectedShapes *selection = illustrator_->getSelection();

    if (selection->size() == 1) {
      const CQIllustratorSelectedShape &sshape = selection->front();

      const CQIllustratorShape *shape = sshape.getShape();

      sizer_->draw(painter, shape);

      sizer_->drawSelectionHandles(painter, sshape);
    }
  }
  else {
    CQIllustratorShape *shape = illustrator_->getSandbox()->frontShape();

    sizer_->draw(painter, shape);
  }
}

QCursor
CQIllustratorPointSelectMode::
getCursor() const
{
  return Qt::ArrowCursor;
}

//---------------

CQIllustratorPointSelectToolbar::
CQIllustratorPointSelectToolbar(CQIllustratorPointSelectMode *mode) :
 CQIllustratorToolbar(mode), mode_(mode)
{
}

QIcon
CQIllustratorPointSelectToolbar::
getIcon()
{
  return QIcon(QPixmap(point_select_data));
}

void
CQIllustratorPointSelectToolbar::
addWidgets()
{
  QBoxLayout *layout = qobject_cast<QBoxLayout *>(CQIllustratorToolbar::layout());

  //-----

  QPushButton *addButton  = new QPushButton("Add");

  CQSwatch *selectSwatch = new CQSwatch("Select", addButton);

  layout->addWidget(selectSwatch);
}

void
CQIllustratorPointSelectToolbar::
setSelectedShape(const CQIllustratorShape *)
{
}

//------

CQIllustratorPointSelectSizer::
CQIllustratorPointSelectSizer(CQIllustratorPointSelectMode *mode) :
 CQIllustratorModeSizer(mode)
{
}

void
CQIllustratorPointSelectSizer::
drawHandles(QPainter *painter, const CQIllustratorShape *shape)
{
  if (shape) {
    CQIllustratorShape::ControlPointList controlPoints;

    shape->getControlPoints(controlPoints);

    uint numControlPoints = controlPoints.size();

    updateHandles(numControlPoints);

    for (uint i = 0; i < numControlPoints; ++i)
      controlPointHandles_[i]->draw(shape, controlPoints[i], painter);
  }
}

void
CQIllustratorPointSelectSizer::
updateHandles(uint numHandles)
{
  CQIllustrator *illustrator = mode_->getIllustrator();

  while (controlPointHandles_.size() < numHandles) {
    CQIllustratorControlPointHandle *handle = new CQIllustratorControlPointHandle(illustrator);

    addHandle(handle);
  }

  while (controlPointHandles_.size() > numHandles) {
    CQIllustratorControlPointHandle *handle = NULL;

    removeHandle(handle);
  }

  for (uint i = 0; i < numHandles; ++i) {
    CQIllustratorControlPointHandle *handle = new CQIllustratorControlPointHandle(illustrator);

    handle->setImage(IMAGE_DATA(selected_point_data));
  }
}
