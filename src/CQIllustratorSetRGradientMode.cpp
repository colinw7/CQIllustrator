#include <CQIllustratorSetRGradientMode.h>
#include <CQIllustrator.h>
#include <CQIllustratorHandle.h>
#include <CQIllustratorShapeDrawer.h>
#include <CQIllustratorUtil.h>

#include <QPainter>
#include <QBoxLayout>

#include <CRadialGradient.h>
#include <CQMenu.h>
#include <CQUtil.h>
#include <CQUtilGeom.h>
#include <CQRealEdit.h>
#include <CQSwatch.h>

#include <svg/rgradient_svg.h>

#include <svg/stop_point_svg.h>
#include <svg/stop_point_active_svg.h>

CQIllustratorSetRGradientMode::
CQIllustratorSetRGradientMode(CQIllustrator *illustrator) :
 CQIllustratorMode(illustrator, uint(CQIllustrator::Mode::RGRADIENT))
{
}

CQIllustratorSetRGradientToolbar *
CQIllustratorSetRGradientMode::
createToolbar()
{
  toolbar_ = new CQIllustratorSetRGradientToolbar(this);

  return toolbar_;
}

CQIllustratorRGradSizer *
CQIllustratorSetRGradientMode::
createSizer()
{
  sizer_ = new CQIllustratorRGradSizer(this);

  return sizer_;
}

CQMenuItem *
CQIllustratorSetRGradientMode::
createMenuItem(CQMenu *menu)
{
  QIcon icon = CQPixmapCacheInst->getIcon("RGRADIENT");

  menuItem_ = new CQMenuItem(menu, icon, "&Radial Gradient", CQMenuItem::CHECKABLE);

  menuItem_->setStatusTip("Create/Modify Radial Gradient");

  connect(menuItem_->getAction(), SIGNAL(toggled(bool)), this, SLOT(menuItemSlot()));

  return menuItem_;
}

void
CQIllustratorSetRGradientMode::
handleMouseRelease(const MouseEvent &e)
{
  CPoint2D p2 = CQUtil::fromQPoint(e.window);

  if (dragging_) {
    if (! moving_) {
      selectControlPoint(e);
    }
    // dragging finished so commit
    else {
      illustrator_->getSandbox()->commit(CQIllustratorData::ChangeType::FILL);
    }
  }
  // not dragging so do a select
  else {
    //CPoint2D p1 = CQUtil::fromQPoint(press_wpos_);

    // point click - select at point
    if (! moving_) {
      (void) illustrator_->selectAt(p2, e.event->isControlKey(), e.event->isShiftKey());
    }
    // drag then create rgradient ellipse using specified bbox
    else {
      illustrator_->addRadialGradient(CQUtil::fromQPoint(press_wpos_),
                                      CQUtil::fromQPoint(e.window));
    }
  }

  dragging_ = false;

  illustrator_->redraw();
}

void
CQIllustratorSetRGradientMode::
handleMouseDrag(const MouseEvent &e)
{
  if (dragging_) {
    CQIllustratorShape *shape = illustrator_->getSandbox()->frontShape();

    sizer_->updatePoint(shape, e.window);
  }
  else {
    //CPoint2D p1 = CQUtil::fromQPoint(press_wpos_);
    //CPoint2D p2 = CQUtil::fromQPoint(e.window);

    //toolbar_->setSize(CBBox2D(p1, p2));
  }

  illustrator_->redrawOverlay();

  prev_wpos_ = e.window;
}

void
CQIllustratorSetRGradientMode::
drawOverlay(CQIllustratorShapeDrawer *drawer)
{
  QPainter *painter = drawer->getPainter();

  if (! dragging_) {
    // draw creation rubber band
    if (pressed_) {
      QPointF current_wpos = illustrator_->getITransform().map(QPointF(curr_ppos_));

      QPen pen;

      pen.setColor(QColor(0,0,0));
      pen.setStyle(Qt::DashLine);
      pen.setWidth(0);

      painter->setPen(pen);
      painter->setBrush(Qt::NoBrush);

      painter->drawLine(press_wpos_, current_wpos);
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
CQIllustratorSetRGradientMode::
getCursor() const
{
  return Qt::ArrowCursor;
}

//------------

CQIllustratorSetRGradientToolbar::
CQIllustratorSetRGradientToolbar(CQIllustratorSetRGradientMode *mode) :
 CQIllustratorToolbar(mode), mode_(mode)
{
}

QIcon
CQIllustratorSetRGradientToolbar::
getIcon()
{
  return CQPixmapCacheInst->getIcon("RGRADIENT");
}

void
CQIllustratorSetRGradientToolbar::
addWidgets()
{
  QBoxLayout *layout = qobject_cast<QBoxLayout *>(CQIllustratorToolbar::layout());

  //-----

  centerXEdit_ = new CQRealEdit(0.0);
  centerYEdit_ = new CQRealEdit(0.0);
  focusXEdit_  = new CQRealEdit(0.0);
  focusYEdit_  = new CQRealEdit(0.0);
  radiusEdit_  = new CQRealEdit(0.0);

  centerXEdit_->setFocusPolicy(Qt::ClickFocus);
  centerYEdit_->setFocusPolicy(Qt::ClickFocus);
  focusXEdit_ ->setFocusPolicy(Qt::ClickFocus);
  focusYEdit_ ->setFocusPolicy(Qt::ClickFocus);
  radiusEdit_ ->setFocusPolicy(Qt::ClickFocus);

  connect(centerXEdit_, SIGNAL(valueChanged(double)), this, SLOT(updateShape()));
  connect(centerYEdit_, SIGNAL(valueChanged(double)), this, SLOT(updateShape()));
  connect(focusXEdit_ , SIGNAL(valueChanged(double)), this, SLOT(updateShape()));
  connect(focusYEdit_ , SIGNAL(valueChanged(double)), this, SLOT(updateShape()));
  connect(radiusEdit_ , SIGNAL(valueChanged(double)), this, SLOT(updateShape()));

  CQSwatch *centerSwatch = new CQSwatch("Center", centerXEdit_, centerYEdit_);
  CQSwatch *focusSwatch  = new CQSwatch("Focus" , focusXEdit_ , focusYEdit_ );
  CQSwatch *radiusSwatch = new CQSwatch("Radius", radiusEdit_);

  layout->addWidget(centerSwatch);
  layout->addWidget(focusSwatch);
  layout->addWidget(radiusSwatch);

  centerXEdit_->setEnabled(false);
  centerYEdit_->setEnabled(false);
  focusXEdit_ ->setEnabled(false);
  focusYEdit_ ->setEnabled(false);
  radiusEdit_ ->setEnabled(false);
}

void
CQIllustratorSetRGradientToolbar::
setSelectedShape(const CQIllustratorShape *shape)
{
  const CRadialGradient *rg = nullptr;

  if (shape) {
    CQIllustratorShapeFill fill = shape->getFill();

    const CGenGradient *g = fill.getGradient();

    rg = dynamic_cast<const CRadialGradient *>(g);
  }

  centerXEdit_->setEnabled(rg != nullptr);
  centerYEdit_->setEnabled(rg != nullptr);
  focusXEdit_ ->setEnabled(rg != nullptr);
  focusYEdit_ ->setEnabled(rg != nullptr);
  radiusEdit_ ->setEnabled(rg != nullptr);

  if (rg) {
    centerXEdit_->setValue(rg->getCenterX());
    centerYEdit_->setValue(rg->getCenterY());
    focusXEdit_ ->setValue(rg->getFocusX());
    focusYEdit_ ->setValue(rg->getFocusY());
    radiusEdit_ ->setValue(rg->getRadius());
  }
}

void
CQIllustratorSetRGradientToolbar::
updateShape()
{
  CQIllustrator *illustrator = mode_->getIllustrator();

  CQIllustratorShape *shape = CQIllustratorUtil::getCurrentShape<CQIllustratorShape>(illustrator);

  if (! shape) return;

  CQIllustratorShapeFill fill = shape->getFill();

  CGenGradient *g = fill.getGradient();

  CRadialGradient *rg = dynamic_cast<CRadialGradient *>(g);

  if (! rg) return;

  double xc = centerXEdit_->getValue();
  double yc = centerYEdit_->getValue();
  double fx = focusXEdit_ ->getValue();
  double fy = focusYEdit_ ->getValue();
  double r  = radiusEdit_ ->getValue();

  rg->setCenter(xc, yc);
  rg->setFocus(fx, fy);
  rg->setRadius(r);

  shape->setFill(fill);

  illustrator->emitFillChanged();

  illustrator->redraw();
}

//----------

CQIllustratorRGradSizer::
CQIllustratorRGradSizer(CQIllustratorSetRGradientMode *mode) :
 CQIllustratorModeSizer(mode)
{
  CQIllustrator *illustrator = mode_->getIllustrator();

  center_handle_ = new CQIllustratorControlPointHandle(illustrator);
  focus_handle_  = new CQIllustratorControlPointHandle(illustrator);
  radius_handle_ = new CQIllustratorControlPointHandle(illustrator);

  center_handle_->setImage(CQPixmapCacheInst->getIcon("STOP_POINT"),
                           CQPixmapCacheInst->getIcon("STOP_POINT_ACTIVE"));
  focus_handle_ ->setImage(CQPixmapCacheInst->getIcon("STOP_POINT"),
                           CQPixmapCacheInst->getIcon("STOP_POINT_ACTIVE"));
  radius_handle_->setImage(CQPixmapCacheInst->getIcon("STOP_POINT"),
                           CQPixmapCacheInst->getIcon("STOP_POINT_ACTIVE"));

  addHandle(center_handle_);
  addHandle(focus_handle_ );
  addHandle(radius_handle_);
}

void
CQIllustratorRGradSizer::
drawHandles(QPainter *painter, const CQIllustratorShape *shape)
{
  CQIllustratorShape::ControlPointList controlPoints;

  shape->getControlPoints(controlPoints, CQIllustratorShape::ControlType::RGRADIENT);

  if (controlPoints.size() == 3) {
    center_handle_->draw(shape, controlPoints[0], painter);
    focus_handle_ ->draw(shape, controlPoints[1], painter);
    radius_handle_->draw(shape, controlPoints[2], painter);

    CPoint2D p1 = controlPoints[0]->getPoint(shape);
    CPoint2D p2 = controlPoints[1]->getPoint(shape);
    CPoint2D p3 = controlPoints[2]->getPoint(shape);

    QPen pen;

    pen.setColor(QColor(0,0,0));
    pen.setStyle(Qt::DashLine);
    pen.setWidth(0);

    painter->setPen(pen);
    painter->setBrush(Qt::NoBrush);

    painter->drawLine(CQUtil::toQPoint(p1), CQUtil::toQPoint(p2));
    painter->drawLine(CQUtil::toQPoint(p1), CQUtil::toQPoint(p3));
  }
}
