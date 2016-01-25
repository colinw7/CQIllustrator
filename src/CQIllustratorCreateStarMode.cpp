#include <CQIllustratorCreateStarMode.h>
#include <CQIllustrator.h>
#include <CQIllustratorHandle.h>
#include <CQIllustratorShapeDrawer.h>

#include <QPainter>
#include <QHBoxLayout>

#include <CQMenu.h>
#include <CQUtil.h>
#include <CQImageButton.h>
#include <CQPointEdit.h>
#include <CQIntegerEdit.h>
#include <CQRealEdit.h>
#include <CQAngleSpinBox.h>
#include <CQSwatch.h>

#include <xpm/star_poly.xpm>
#include <xpm/poly.xpm>
#include <xpm/star.xpm>
#include <xpm/center.xpm>
#include <xpm/center_active.xpm>

#include <cursors/star.xbm>
#include <cursors/starmask.xbm>

#define IMAGE_DATA(I) I, sizeof(I)/sizeof(char *)

CQIllustratorCreateStarMode::
CQIllustratorCreateStarMode(CQIllustrator *illustrator) :
 CQIllustratorMode(illustrator, CQIllustrator::MODE_STAR)
{
  setCursor(star_bits, starmask_bits, 2, 2);
}

CQIllustratorCreateStarToolbar *
CQIllustratorCreateStarMode::
createToolbar()
{
  toolbar_ = new CQIllustratorCreateStarToolbar(this);

  return toolbar_;
}

CQIllustratorCreateStarSizer *
CQIllustratorCreateStarMode::
createSizer()
{
  sizer_ = new CQIllustratorCreateStarSizer(this);

  return sizer_;
}

CQMenuItem *
CQIllustratorCreateStarMode::
createMenuItem(CQMenu *menu)
{
  menuItem_ = new CQMenuItem(menu, "&Star", CQMenuItem::CHECKABLE);

  menuItem_->setStatusTip("Create Star or Polygon");
  menuItem_->setXPMIcon(star_poly_data);

  connect(menuItem_->getAction(), SIGNAL(toggled(bool)), this, SLOT(menuItemSlot()));

  return menuItem_;
}

void
CQIllustratorCreateStarMode::
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

    // point click - select at point and toggle sizer mode if same shape selected
    if (! moving_) {
      (void) illustrator_->selectAt(p1, e.event->isControlKey(), e.event->isShiftKey());
    }
    // drag then create star or rectangle using specified bbox
    else {
      double r = CVector2D(p2, p1).length()/2.0;

      uint n = 5;

      double da = 2.0*M_PI/n;

      double a2 = CMathGen::RadToDeg(M_PI/2.0);
      double a1 = a2 - CMathGen::RadToDeg(da/2.0);

      if (toolbar_->getCreateMode() == CQIllustratorCreateStarToolbar::CREATE_STAR_MODE) {
        CQIllustratorStarShape *star =
          illustrator_->createStarShape((p1 + p2)/2, n, r/2.0, r, a1, a2);

        illustrator_->addShape(star);

        illustrator_->setSelectShape(star);
      }
      else {
        CQIllustratorNPolyShape *poly =
          illustrator_->createNPolyShape((p1 + p2)/2, n, r, a2);

        illustrator_->addShape(poly);

        illustrator_->setSelectShape(poly);
      }
    }
  }

  dragging_ = false;

  illustrator_->redraw();
}

void
CQIllustratorCreateStarMode::
handleMouseDrag(const MouseEvent &e)
{
  if (dragging_) {
    CQIllustratorShape *shape = illustrator_->getSandbox()->frontShape();

    sizer_->updatePoint(shape, e.window);
  }
  else {
    CPoint2D p1 = CQUtil::fromQPoint(press_wpos_);
    CPoint2D p2 = CQUtil::fromQPoint(e.window);

    toolbar_->setSize(CBBox2D(p1, p2));
  }

  illustrator_->redrawOverlay();

  prev_wpos_ = e.window;
}

void
CQIllustratorCreateStarMode::
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

      painter->setPen(pen);
      painter->setBrush(Qt::NoBrush);

      painter->drawRect(QRectF(press_wpos_, current_wpos));
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

//------------

CQIllustratorCreateStarToolbar::
CQIllustratorCreateStarToolbar(CQIllustratorCreateStarMode *mode) :
 CQIllustratorToolbar(mode), mode_(mode), createMode_(CREATE_STAR_MODE)
{
}

QIcon
CQIllustratorCreateStarToolbar::
getIcon()
{
  return QIcon(QPixmap(star_poly_data));
}

void
CQIllustratorCreateStarToolbar::
addWidgets()
{
  QBoxLayout *layout = qobject_cast<QBoxLayout *>(CQIllustratorToolbar::layout());

  //-----

  starButton_ = new CQImageButton(QPixmap(star_data));
  polyButton_ = new CQImageButton(QPixmap(poly_data));

  connect(starButton_, SIGNAL(toggled(bool)), this, SLOT(starModeSlot(bool)));
  connect(polyButton_, SIGNAL(toggled(bool)), this, SLOT(polyModeSlot(bool)));

  starButton_->setCheckable(true);
  polyButton_->setCheckable(true);

  starButton_->setChecked(true);

  CQSwatch *modeSwatch = new CQSwatch("Mode", starButton_, polyButton_);

  layout->addWidget(modeSwatch);

  //-----

  centerEdit_  = new CQPointEdit;
  numEdit_     = new QSpinBox(0);
  radius1Edit_ = new CQRealEdit(0.0);
  radius2Edit_ = new CQRealEdit(0.0);
  angle1Edit_  = new CQAngleSpinBox(0.0);
  angle2Edit_  = new CQAngleSpinBox(0.0);

  numEdit_->setRange(0, 100);

  centerEdit_ ->setFocusPolicy(Qt::ClickFocus);
  numEdit_    ->setFocusPolicy(Qt::ClickFocus);
  radius1Edit_->setFocusPolicy(Qt::ClickFocus);
  radius2Edit_->setFocusPolicy(Qt::ClickFocus);
  angle1Edit_ ->setFocusPolicy(Qt::ClickFocus);
  angle2Edit_ ->setFocusPolicy(Qt::ClickFocus);

  connect(centerEdit_ , SIGNAL(valueChanged()), this, SLOT(updateShape()));
  connect(numEdit_    , SIGNAL(valueChanged(int)), this, SLOT(updateShape()));
  connect(radius1Edit_, SIGNAL(valueChanged(double)), this, SLOT(updateShape()));
  connect(radius2Edit_, SIGNAL(valueChanged(double)), this, SLOT(updateShape()));
  connect(angle1Edit_ , SIGNAL(angleChanged(double)), this, SLOT(updateShape()));
  connect(angle2Edit_ , SIGNAL(angleChanged(double)), this, SLOT(updateShape()));

  CQSwatch *centerSwatch  = new CQSwatch("Center"       , centerEdit_ );
  CQSwatch *numSwatch     = new CQSwatch("Num Sides"    , numEdit_    );
  CQSwatch *radius1Swatch = new CQSwatch("Innner Radius", radius1Edit_);
  CQSwatch *radius2Swatch = new CQSwatch("Outer Radius" , radius2Edit_);
  CQSwatch *angle1Swatch  = new CQSwatch("Inner Angle"  , angle1Edit_ );
  CQSwatch *angle2Swatch  = new CQSwatch("Outer Angle"  , angle2Edit_ );

  layout->addWidget(centerSwatch);
  layout->addWidget(numSwatch);
  layout->addWidget(radius1Swatch);
  layout->addWidget(radius2Swatch);
  layout->addWidget(angle1Swatch);
  layout->addWidget(angle2Swatch);

  centerEdit_ ->setEnabled(false);
  numEdit_    ->setEnabled(false);
  radius1Edit_->setEnabled(false);
  radius2Edit_->setEnabled(false);
  angle1Edit_ ->setEnabled(false);
  angle2Edit_ ->setEnabled(false);
}

void
CQIllustratorCreateStarToolbar::
setSelectedShape(const CQIllustratorShape *shape)
{
  const CQIllustratorStarShape  *star = dynamic_cast<const CQIllustratorStarShape  *>(shape);
  const CQIllustratorNPolyShape *poly = dynamic_cast<const CQIllustratorNPolyShape *>(shape);

  centerEdit_ ->setEnabled(star != 0 || poly != 0);
  numEdit_    ->setEnabled(star != 0 || poly != 0);
  radius1Edit_->setEnabled(star != 0 || poly != 0);
  radius2Edit_->setEnabled(star != 0);
  angle1Edit_ ->setEnabled(star != 0 || poly != 0);
  angle2Edit_ ->setEnabled(star != 0);

  disconnect(numEdit_   , SIGNAL(valueChanged(int)), this, SLOT(updateShape()));
  disconnect(angle1Edit_, SIGNAL(angleChanged(double)), this, SLOT(updateShape()));
  disconnect(angle2Edit_, SIGNAL(angleChanged(double)), this, SLOT(updateShape()));

  if      (star) {
    const CBBox2D &bbox = star->getBBox();

    setSize(bbox);

    numEdit_    ->setValue(star->getNum());
    radius1Edit_->setValue(star->getInnerRadius());
    radius2Edit_->setValue(star->getOuterRadius());
    angle1Edit_ ->setValue(star->getInnerAngle());
    angle2Edit_ ->setValue(star->getOuterAngle());
  }
  else if (poly) {
    const CBBox2D &bbox = poly->getBBox();

    setSize(bbox);

    numEdit_    ->setValue(poly->getNum());
    radius1Edit_->setValue(poly->getRadius());
    angle1Edit_ ->setValue(poly->getAngle());
  }
  else {
    numEdit_    ->setValue(0);
    radius1Edit_->setValue(0.0);
    radius2Edit_->setValue(0.0);
    angle1Edit_ ->setValue(0.0);
    angle2Edit_ ->setValue(0.0);
  }

  connect(numEdit_   , SIGNAL(valueChanged(int)), this, SLOT(updateShape()));
  connect(angle1Edit_, SIGNAL(angleChanged(double)), this, SLOT(updateShape()));
  connect(angle2Edit_, SIGNAL(angleChanged(double)), this, SLOT(updateShape()));
}

void
CQIllustratorCreateStarToolbar::
setSize(const CBBox2D &bbox)
{
  CPoint2D c = bbox.getCenter();

  centerEdit_->setValue(c);
}

void
CQIllustratorCreateStarToolbar::
starModeSlot(bool state)
{
  if (state) {
    createMode_ = CREATE_STAR_MODE;

    polyButton_->setChecked(false);
  }
}

void
CQIllustratorCreateStarToolbar::
polyModeSlot(bool state)
{
  if (state) {
    createMode_ = CREATE_POLY_MODE;

    starButton_->setChecked(false);
  }
}

void
CQIllustratorCreateStarToolbar::
updateShape()
{
  CQIllustrator *illustrator = mode_->getIllustrator();

  CQIllustratorStarShape  *star =
    CQIllustratorUtil::getCurrentShape<CQIllustratorStarShape>(illustrator);
  CQIllustratorNPolyShape *poly =
    CQIllustratorUtil::getCurrentShape<CQIllustratorNPolyShape>(illustrator);

  if      (star) {
    CPoint2D c  = centerEdit_ ->getValue();
    int      n  = numEdit_    ->value();
    double   r1 = radius1Edit_->getValue();
    double   r2 = radius2Edit_->getValue();
    double   a1 = angle1Edit_ ->value();
    double   a2 = angle2Edit_ ->value();

    illustrator->checkoutShape(star, CQIllustratorData::CHANGE_GEOMETRY);

    star->setCenterPoint(c);
    star->setNum        (n);
    star->setInnerRadius(r1);
    star->setOuterRadius(r2);
    star->setInnerAngle (a1);
    star->setOuterAngle (a2);

    illustrator->checkinShape(star, CQIllustratorData::CHANGE_GEOMETRY);
  }
  else if (poly) {
    CPoint2D c = centerEdit_ ->getValue();
    int      n = numEdit_    ->value();
    double   r = radius1Edit_->getValue();
    double   a = angle1Edit_ ->value();

    illustrator->checkoutShape(poly, CQIllustratorData::CHANGE_GEOMETRY);

    poly->setCenterPoint(c);
    poly->setNum        (n);
    poly->setRadius     (r);
    poly->setAngle      (a);

    illustrator->checkinShape(poly, CQIllustratorData::CHANGE_GEOMETRY);
  }

  illustrator->redraw();
}

//------

CQIllustratorCreateStarSizer::
CQIllustratorCreateStarSizer(CQIllustratorCreateStarMode *mode) :
 CQIllustratorModeSizer(mode)
{
  CQIllustrator *illustrator = mode_->getIllustrator();

  c_handle_  = new CQIllustratorControlPointHandle(illustrator);
  ir_handle_ = new CQIllustratorControlPointHandle(illustrator);
  or_handle_ = new CQIllustratorControlPointHandle(illustrator);

  c_handle_->setImage(IMAGE_DATA(center_data), IMAGE_DATA(center_active_data));

  addHandle(c_handle_ );
  addHandle(ir_handle_);
  addHandle(or_handle_);
}

void
CQIllustratorCreateStarSizer::
drawHandles(QPainter *painter, const CQIllustratorShape *shape)
{
  const CQIllustratorStarShape  *star = dynamic_cast<const CQIllustratorStarShape  *>(shape);
  const CQIllustratorNPolyShape *poly = dynamic_cast<const CQIllustratorNPolyShape *>(shape);

  if      (star) {
    CQIllustratorShape::ControlPointList controlPoints;

    star->getControlPoints(controlPoints);

    c_handle_ ->draw(star, controlPoints[0], painter);
    ir_handle_->draw(star, controlPoints[1], painter);
    or_handle_->draw(star, controlPoints[2], painter);
  }
  else if (poly) {
    CQIllustratorShape::ControlPointList controlPoints;

    poly->getControlPoints(controlPoints);

    c_handle_ ->draw(poly, controlPoints[0], painter);
    ir_handle_->draw(poly, controlPoints[1], painter);
  }
}
