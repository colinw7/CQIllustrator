#include <CQIllustratorSetLGradientMode.h>
#include <CQIllustrator.h>
#include <CQIllustratorHandle.h>
#include <CQIllustratorShapeDrawer.h>

#include <QPainter>
#include <QBoxLayout>
#include <QRadioButton>

#include <CLinearGradient.h>
#include <CQMenu.h>
#include <CQUtil.h>
#include <CQRealEdit.h>
#include <CQSwatch.h>

#include "xpm/lgradient.xpm"

#include "xpm/stop_point.xpm"
#include "xpm/stop_point_active.xpm"

#define IMAGE_DATA(I) I, sizeof(I)/sizeof(char *)

CQIllustratorSetLGradientMode::
CQIllustratorSetLGradientMode(CQIllustrator *illustrator) :
 CQIllustratorMode(illustrator, CQIllustrator::MODE_LGRADIENT)
{
}

CQIllustratorSetLGradientToolbar *
CQIllustratorSetLGradientMode::
createToolbar()
{
  toolbar_ = new CQIllustratorSetLGradientToolbar(this);

  return toolbar_;
}

CQIllustratorLGradSizer *
CQIllustratorSetLGradientMode::
createSizer()
{
  sizer_ = new CQIllustratorLGradSizer(this);

  return sizer_;
}

CQMenuItem *
CQIllustratorSetLGradientMode::
createMenuItem(CQMenu *menu)
{
  menuItem_ = new CQMenuItem(menu, "&Linear Gradient", CQMenuItem::CHECKABLE);

  menuItem_->setStatusTip("Create/Modify Linear Gradient");
  menuItem_->setXPMIcon(lgradient_data);

  connect(menuItem_->getAction(), SIGNAL(toggled(bool)), this, SLOT(menuItemSlot()));

  return menuItem_;
}

void
CQIllustratorSetLGradientMode::
handleMouseRelease(const MouseEvent &e)
{
  CPoint2D p1 = CQUtil::fromQPoint(press_wpos_);
  CPoint2D p2 = CQUtil::fromQPoint(e.window);

  if (dragging_) {
    if (! moving_) {
      selectControlPoint(e);
    }
    // dragging finished so commit
    else {
      illustrator_->getSandbox()->commit(CQIllustratorData::CHANGE_FILL);
    }
  }
  // not dragging so do a select
  else {
    // point click - select at point
    if (! moving_) {
      (void) illustrator_->selectAt(p2, e.event->isControlKey(), e.event->isShiftKey());
    }
    else {
      if (editMode_ == CREATE_MODE) {
        // drag then create lgradient rectangle using specified bbox
        illustrator_->addLinearGradient(CQUtil::fromQPoint(press_wpos_),
                                        CQUtil::fromQPoint(e.window));
      }
      else {
        // drag - select points in rectangle
        CBBox2D bbox(p1, p2);

        illustrator_->selectPointsIn(bbox, CQIllustratorShape::CONTROL_LGRADIENT,
                                     e.event->isControlKey(), e.event->isShiftKey());
      }
    }
  }

  dragging_ = false;

  illustrator_->redraw();
}

void
CQIllustratorSetLGradientMode::
handleMouseDrag(const MouseEvent &e)
{
  if (dragging_) {
    CQIllustratorShape *shape = illustrator_->getSandbox()->frontShape();

    sizer_->updatePoint(shape, e.window);
  }
  else {
    if (editMode_ == CREATE_MODE) {
      //CPoint2D p1 = CQUtil::fromQPoint(press_wpos_);
      //CPoint2D p2 = CQUtil::fromQPoint(e.window);

      //toolbar_->setSize(CBBox2D(p1, p2));
    }
    else {
    }
  }

  illustrator_->redrawOverlay();

  prev_wpos_ = e.window;
}

void
CQIllustratorSetLGradientMode::
drawOverlay(CQIllustratorShapeDrawer *drawer)
{
  QPainter *painter = drawer->getPainter();

  if (! dragging_) {
    // draw rubber band
    if (pressed_) {
      QPen pen;

      pen.setColor(QColor(0,0,0));
      pen.setStyle(Qt::DashLine);

      painter->setPen(pen);
      painter->setBrush(Qt::NoBrush);

      if (editMode_ == CREATE_MODE)
        painter->drawLine(press_wpos_, curr_wpos_);
      else
        painter->drawRect(QRectF(press_wpos_, curr_wpos_));
    }

    //------

    // draw handles for single selected shape
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
CQIllustratorSetLGradientMode::
getCursor() const
{
  return Qt::ArrowCursor;
}

//------------

CQIllustratorSetLGradientToolbar::
CQIllustratorSetLGradientToolbar(CQIllustratorSetLGradientMode *mode) :
 CQIllustratorToolbar(mode), mode_(mode)
{
}

QIcon
CQIllustratorSetLGradientToolbar::
getIcon()
{
  return QIcon(QPixmap(lgradient_data));
}

void
CQIllustratorSetLGradientToolbar::
addWidgets()
{
  QBoxLayout *layout = qobject_cast<QBoxLayout *>(CQIllustratorToolbar::layout());

  //-----

  QWidget *modeFrame = new QWidget;

  QVBoxLayout *modeLayout = new QVBoxLayout(modeFrame);

  createRadio_ = new QRadioButton("Create");
  editRadio_   = new QRadioButton("Edit");

  createRadio_->setChecked(true);

  createRadio_->setFocusPolicy(Qt::NoFocus);
  editRadio_  ->setFocusPolicy(Qt::NoFocus);

  modeLayout->addWidget(createRadio_);
  modeLayout->addWidget(editRadio_);

  connect(createRadio_, SIGNAL(toggled(bool)), this, SLOT(modeChangedSlot()));

  CQSwatch *modeSwatch = new CQSwatch(modeFrame);

  layout->addWidget(modeSwatch);

  //-----

  startXEdit_ = new CQRealEdit(0.0);
  startYEdit_ = new CQRealEdit(0.0);
  endXEdit_   = new CQRealEdit(0.0);
  endYEdit_   = new CQRealEdit(0.0);

  startXEdit_->setFocusPolicy(Qt::ClickFocus);
  startYEdit_->setFocusPolicy(Qt::ClickFocus);
  endXEdit_  ->setFocusPolicy(Qt::ClickFocus);
  endYEdit_  ->setFocusPolicy(Qt::ClickFocus);

  connect(startXEdit_, SIGNAL(valueChanged(double)), this, SLOT(updateShape()));
  connect(startYEdit_, SIGNAL(valueChanged(double)), this, SLOT(updateShape()));
  connect(endXEdit_  , SIGNAL(valueChanged(double)), this, SLOT(updateShape()));
  connect(endYEdit_  , SIGNAL(valueChanged(double)), this, SLOT(updateShape()));

  CQSwatch *startSwatch = new CQSwatch("Start", startXEdit_, startYEdit_);
  CQSwatch *endSwatch   = new CQSwatch("End"  , endXEdit_  , endYEdit_  );

  layout->addWidget(startSwatch);
  layout->addWidget(endSwatch);

  startXEdit_->setEnabled(false);
  startYEdit_->setEnabled(false);
  endXEdit_  ->setEnabled(false);
  endYEdit_  ->setEnabled(false);
}

void
CQIllustratorSetLGradientToolbar::
modeChangedSlot()
{
  if (createRadio_->isChecked())
    mode_->setEditMode(CQIllustratorMode::CREATE_MODE);
  else
    mode_->setEditMode(CQIllustratorMode::EDIT_MODE);
}

void
CQIllustratorSetLGradientToolbar::
setSelectedShape(const CQIllustratorShape *shape)
{
  if (! shape) return;

  CQIllustratorShapeFill fill = shape->getFill();

  const CGenGradient *g = fill.getGradient();

  const CLinearGradient *lg = dynamic_cast<const CLinearGradient *>(g);

  startXEdit_->setEnabled(lg != NULL);
  startYEdit_->setEnabled(lg != NULL);
  endXEdit_  ->setEnabled(lg != NULL);
  endYEdit_  ->setEnabled(lg != NULL);

  if (lg) {
    startXEdit_->setValue(lg->getX1());
    startYEdit_->setValue(lg->getY1());
    endXEdit_  ->setValue(lg->getX2());
    endYEdit_  ->setValue(lg->getY2());
  }
}

void
CQIllustratorSetLGradientToolbar::
updateShape()
{
  CQIllustrator *illustrator = mode_->getIllustrator();

  CQIllustratorShape *shape = CQIllustratorUtil::getCurrentShape<CQIllustratorShape>(illustrator);

  if (! shape) return;

  CQIllustratorShapeFill fill = shape->getFill();

  CGenGradient *g = fill.getGradient();

  CLinearGradient *lg = dynamic_cast<CLinearGradient *>(g);

  if (! lg) return;

  double x1 = startXEdit_->getValue();
  double y1 = startXEdit_->getValue();
  double x2 = endXEdit_  ->getValue();
  double y2 = endXEdit_  ->getValue();

  lg->setX1(x1); lg->setY1(y1);
  lg->setX2(x2); lg->setY2(y2);

  shape->setFill(fill);

  illustrator->emitFillChanged();

  illustrator->redraw();
}

//----------

CQIllustratorLGradSizer::
CQIllustratorLGradSizer(CQIllustratorSetLGradientMode *mode) :
 CQIllustratorModeSizer(mode)
{
  CQIllustrator *illustrator = mode_->getIllustrator();

  start_handle_ = new CQIllustratorControlPointHandle(illustrator);
  end_handle_   = new CQIllustratorControlPointHandle(illustrator);

  start_handle_->setImage(IMAGE_DATA(stop_point_data), IMAGE_DATA(stop_point_active_data));
  end_handle_  ->setImage(IMAGE_DATA(stop_point_data), IMAGE_DATA(stop_point_active_data));

  addHandle(start_handle_);
  addHandle(end_handle_  );
}

void
CQIllustratorLGradSizer::
drawHandles(QPainter *painter, const CQIllustratorShape *shape)
{
  CQIllustratorShape::ControlPointList controlPoints;

  shape->getControlPoints(controlPoints, CQIllustratorShape::CONTROL_LGRADIENT);

  if (controlPoints.size() == 2) {
    start_handle_->draw(shape, controlPoints[0], painter);
    end_handle_  ->draw(shape, controlPoints[1], painter);

    CPoint2D p1 = controlPoints[0]->getPoint(shape);
    CPoint2D p2 = controlPoints[1]->getPoint(shape);

    QPen pen;

    pen.setColor(QColor(0,0,0));
    pen.setStyle(Qt::DashLine);

    painter->setPen(pen);
    painter->setBrush(Qt::NoBrush);

    painter->drawLine(CQUtil::toQPoint(p1), CQUtil::toQPoint(p2));
  }
}
