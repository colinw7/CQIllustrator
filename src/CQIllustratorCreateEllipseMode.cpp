#include <CQIllustratorCreateEllipseMode.h>
#include <CQIllustrator.h>
#include <CQIllustratorCmd.h>
#include <CQIllustratorHandle.h>
#include <CQIllustratorShapeDrawer.h>

#include <QPainter>
#include <QHBoxLayout>

#include <CQMenu.h>
#include <CQUtil.h>
#include <CQPointEdit.h>
#include <CQRealEdit.h>
#include <CQAngleSpinBox.h>
#include <CQSwatch.h>

#include <svg/ellipse_svg.h>

#include <cursors/circle.xbm>
#include <cursors/circlemask.xbm>

class CQIllustratorCreateEllipseCmd : public CQIllustratorCmd {
 public:
  CQIllustratorCreateEllipseCmd();

  const char *getName() const { return "create_ellipse"; }

  bool exec(const std::vector<std::string> &words);
};

CQIllustratorCreateEllipseMode::
CQIllustratorCreateEllipseMode(CQIllustrator *illustrator) :
 CQIllustratorMode(illustrator, CQIllustrator::MODE_ELLIPSE), equalSize_(false)
{
  setCursor(circle_bits, circlemask_bits, 2, 2);

  illustrator_->addCommand(new CQIllustratorCreateEllipseCmd);
}

CQIllustratorCreateEllipseToolbar *
CQIllustratorCreateEllipseMode::
createToolbar()
{
  toolbar_ = new CQIllustratorCreateEllipseToolbar(this);

  return toolbar_;
}

CQIllustratorCreateEllipseSizer *
CQIllustratorCreateEllipseMode::
createSizer()
{
  sizer_ = new CQIllustratorCreateEllipseSizer(this);

  return sizer_;
}

CQMenuItem *
CQIllustratorCreateEllipseMode::
createMenuItem(CQMenu *menu)
{
  QIcon icon = CQPixmapCacheInst->getIcon("ELLIPSE");

  menuItem_ = new CQMenuItem(menu, icon, "&Ellipse", CQMenuItem::CHECKABLE);

  menuItem_->setStatusTip("Create Ellipse");

  connect(menuItem_->getAction(), SIGNAL(toggled(bool)), this, SLOT(menuItemSlot()));

  return menuItem_;
}

void
CQIllustratorCreateEllipseMode::
handleMouseRelease(const MouseEvent &e)
{
  CPoint2D p2 = CQUtil::fromQPoint(release_wpos_);

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
      (void) illustrator_->selectAt(p1, e.event->isControlKey(), e.event->isShiftKey());
    }
    // drag then create rectangle using specified bbox
    else {
      CBBox2D bbox(p1, p2);

      if (e.event->isShiftKey()) {
        double w = bbox.getWidth ();
        double h = bbox.getHeight();

        if (w > h)
          bbox.setHeight(w);
        else
          bbox.setWidth(h);
      }

      CQIllustratorEllipseShape *ellipse =
        illustrator_->createEllipseShape(bbox.getLL(), bbox.getUR());

      illustrator_->addShape(ellipse);

      illustrator_->setSelectShape(ellipse);
    }
  }

  dragging_ = false;

  illustrator_->redraw();
}

void
CQIllustratorCreateEllipseMode::
handleMouseDrag(const MouseEvent &e)
{
  if (dragging_) {
    CQIllustratorShape *shape = illustrator_->getSandbox()->frontShape();

    sizer_->updatePoint(shape, curr_wpos_);
  }
  else {
    CPoint2D p1 = CQUtil::fromQPoint(press_wpos_);
    CPoint2D p2 = CQUtil::fromQPoint(curr_wpos_);

    CBBox2D bbox(p1, p2);

    equalSize_ = e.event->isShiftKey();

    if (equalSize_) {
      double w = bbox.getWidth ();
      double h = bbox.getHeight();

      if (w > h)
        bbox.setHeight(w);
      else
        bbox.setWidth(h);
    }

    toolbar_->setSize(bbox);
  }

  illustrator_->redrawOverlay();
}

void
CQIllustratorCreateEllipseMode::
drawOverlay(CQIllustratorShapeDrawer *drawer)
{
  QPainter *painter = drawer->getPainter();

  if (! dragging_) {
    // draw creation rubber band
    if (pressed_) {
      CPoint2D p1 = CQUtil::fromQPoint(press_wpos_);
      CPoint2D p2 = CQUtil::fromQPoint(curr_wpos_);

      CBBox2D bbox(p1, p2);

      if (equalSize_) {
        double w = bbox.getWidth ();
        double h = bbox.getHeight();

        if (w > h)
          bbox.setHeight(w);
        else
          bbox.setWidth(h);
      }

      QPen pen;

      pen.setColor(QColor(0,0,0));
      pen.setStyle(Qt::DashLine);

      painter->setPen(pen);
      painter->setBrush(Qt::NoBrush);

      painter->drawRect(CQUtil::toQRect(bbox));
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

CQIllustratorCreateEllipseToolbar::
CQIllustratorCreateEllipseToolbar(CQIllustratorCreateEllipseMode *mode) :
 CQIllustratorToolbar(mode), mode_(mode)
{
}

QIcon
CQIllustratorCreateEllipseToolbar::
getIcon()
{
  return CQPixmapCacheInst->getIcon("ELLIPSE");
}

void
CQIllustratorCreateEllipseToolbar::
addWidgets()
{
  QBoxLayout *layout = qobject_cast<QBoxLayout *>(CQIllustratorToolbar::layout());

  //-----

  posEdit_     = new CQPointEdit;
  widthEdit_   = new CQRealEdit(0.0);
  heightEdit_  = new CQRealEdit(0.0);
  angle1Edit_  = new CQAngleSpinBox(CAngle(0.0));
  angle2Edit_  = new CQAngleSpinBox(CAngle(0.0));
  connectEdit_ = new CQEllipseShape2DConnectType();

  posEdit_    ->setFocusPolicy(Qt::ClickFocus);
  widthEdit_  ->setFocusPolicy(Qt::ClickFocus);
  heightEdit_ ->setFocusPolicy(Qt::ClickFocus);
  angle1Edit_ ->setFocusPolicy(Qt::ClickFocus);
  angle2Edit_ ->setFocusPolicy(Qt::ClickFocus);
  connectEdit_->setFocusPolicy(Qt::ClickFocus);

  connect(posEdit_    , SIGNAL(valueChanged()), this, SLOT(updateShape()));
  connect(widthEdit_  , SIGNAL(valueChanged(double)), this, SLOT(updateShape()));
  connect(heightEdit_ , SIGNAL(valueChanged(double)), this, SLOT(updateShape()));
  connect(angle1Edit_ , SIGNAL(angleChanged(const CAngle &)), this, SLOT(updateShape()));
  connect(angle2Edit_ , SIGNAL(angleChanged(const CAngle &)), this, SLOT(updateShape()));
  connect(connectEdit_, SIGNAL(valueChanged()), this, SLOT(updateShape()));

  CQSwatch *posSwatch     = new CQSwatch("Position", posEdit_   );
  CQSwatch *widthSwatch   = new CQSwatch("Width"  , widthEdit_ );
  CQSwatch *heightSwatch  = new CQSwatch("Height" , heightEdit_);
  CQSwatch *angle1Swatch  = new CQSwatch("Start"  , angle1Edit_);
  CQSwatch *angle2Swatch  = new CQSwatch("End"    , angle2Edit_);
  CQSwatch *connectSwatch = new CQSwatch("Connect", connectEdit_);

  layout->addWidget(posSwatch);
  layout->addWidget(widthSwatch);
  layout->addWidget(heightSwatch);
  layout->addWidget(angle1Swatch);
  layout->addWidget(angle2Swatch);
  layout->addWidget(connectSwatch);

  posEdit_    ->setEnabled(false);
  widthEdit_  ->setEnabled(false);
  heightEdit_ ->setEnabled(false);
  angle1Edit_ ->setEnabled(false);
  angle1Edit_ ->setEnabled(false);
  connectEdit_->setEnabled(false);
}

void
CQIllustratorCreateEllipseToolbar::
setSelectedShape(const CQIllustratorShape *shape)
{
  const CQIllustratorEllipseShape *ellipse =
    dynamic_cast<const CQIllustratorEllipseShape *>(shape);

  posEdit_    ->setEnabled(ellipse != 0);
  widthEdit_  ->setEnabled(ellipse != 0);
  heightEdit_ ->setEnabled(ellipse != 0);
  angle1Edit_ ->setEnabled(ellipse != 0);
  angle1Edit_ ->setEnabled(ellipse != 0);
  connectEdit_->setEnabled(ellipse != 0);

  if (ellipse) {
    const CBBox2D &bbox = ellipse->getBBox();

    setSize(bbox);

    angle1Edit_->setValue(ellipse->getAngle1());
    angle2Edit_->setValue(ellipse->getAngle2());

    connectEdit_->setType(ellipse->getConnectType());
  }
  else {
    posEdit_   ->setValue();
    widthEdit_ ->setValue(0.0);
    heightEdit_->setValue(0.0);
    angle1Edit_->setValue(0.0);
    angle2Edit_->setValue(0.0);
  }
}

void
CQIllustratorCreateEllipseToolbar::
setSize(const CBBox2D &bbox)
{
  const CPoint2D &pos = bbox.getLL();

  posEdit_->setValue(pos);

  double w = bbox.getWidth();
  double h = bbox.getHeight();

  widthEdit_ ->setValue(w);
  heightEdit_->setValue(h);
}

void
CQIllustratorCreateEllipseToolbar::
updateShape()
{
  CQIllustrator *illustrator = mode_->getIllustrator();

  CQIllustratorEllipseShape *shape =
    CQIllustratorUtil::getCurrentShape<CQIllustratorEllipseShape>(illustrator);

  if (shape) {
    const CPoint2D &pos = posEdit_->getValue();

    double width  = widthEdit_ ->getValue();
    double height = heightEdit_->getValue();

    double angle1 = angle1Edit_->value();
    double angle2 = angle2Edit_->value();

    CEllipseConnectType connectType = connectEdit_->getType();

    illustrator->checkoutShape(shape, CQIllustratorData::CHANGE_GEOMETRY);

    CQIllustratorEllipseShape *ellipse = dynamic_cast<CQIllustratorEllipseShape *>(shape);

    CBBox2D bbox(pos, CPoint2D(pos.x + width, pos.y + height));

    ellipse->setBBox(bbox);

    ellipse->setAngle1(angle1);
    ellipse->setAngle2(angle2);

    ellipse->setConnectType(connectType);

    illustrator->checkinShape(shape, CQIllustratorData::CHANGE_GEOMETRY);
  }

  illustrator->redraw();
}

//------

CQIllustratorCreateEllipseSizer::
CQIllustratorCreateEllipseSizer(CQIllustratorCreateEllipseMode *mode) :
 CQIllustratorModeSizer(mode)
{
  CQIllustrator *illustrator = mode_->getIllustrator();

  rx_handle_ = new CQIllustratorControlPointHandle(illustrator);
  ry_handle_ = new CQIllustratorControlPointHandle(illustrator);
  a1_handle_ = new CQIllustratorControlPointHandle(illustrator);
  a2_handle_ = new CQIllustratorControlPointHandle(illustrator);

  a1_handle_->setStyle(CQIllustratorHandle::CIRCLE_STYLE);
  a2_handle_->setStyle(CQIllustratorHandle::CIRCLE_STYLE);

  addHandle(rx_handle_);
  addHandle(ry_handle_);
  addHandle(a1_handle_);
  addHandle(a2_handle_);
}

void
CQIllustratorCreateEllipseSizer::
drawHandles(QPainter *painter, const CQIllustratorShape *shape)
{
  const CQIllustratorEllipseShape *ellipse =
    dynamic_cast<const CQIllustratorEllipseShape *>(shape);

  if (ellipse) {
    CQIllustratorShape::ControlPointList controlPoints;

    ellipse->getControlPoints(controlPoints);

    rx_handle_->draw(ellipse, controlPoints[0], painter);
    ry_handle_->draw(ellipse, controlPoints[1], painter);
    a1_handle_->draw(ellipse, controlPoints[2], painter);
    a2_handle_->draw(ellipse, controlPoints[3], painter);
  }
}

//----------

static const char *create_ellipse_opts = "\
-x:r=0 \
-y:r=0 \
-xr:r=100 \
-yr:r=100 \
-a:r=0 \
-name:s";

CQIllustratorCreateEllipseCmd::
CQIllustratorCreateEllipseCmd() :
 CQIllustratorCmd(create_ellipse_opts)
{
}

bool
CQIllustratorCreateEllipseCmd::
exec(const std::vector<std::string> &words)
{
  args_.parse(words);

  double x  = args_.getRealArg("-x");
  double y  = args_.getRealArg("-y");
  double xr = args_.getRealArg("-xr");
  double yr = args_.getRealArg("-yr");
  double a  = args_.getRealArg("-a");

  CBBox2D bbox(CPoint2D(x - xr, y - yr), CSize2D(2*xr, 2*yr));

  CQIllustratorEllipseShape *ellipse =
    illustrator_->createEllipseShape(bbox.getLL(), bbox.getUR());

  std::string name = args_.getStringArg("-name");

  if (name != "")
    ellipse->setName(name);

  ellipse->rotate(M_PI*a/180.0);

  illustrator_->addShape(ellipse);

  illustrator_->setSelectShape(ellipse);

  return true;
}

//---------

CQEllipseShape2DConnectType::
CQEllipseShape2DConnectType(QWidget *parent) :
 QComboBox(parent)
{
  const char *        names [] = { "Line", "Center" };
  CEllipseConnectType values[] = { CELLIPSE_CONNECT_LINE, CELLIPSE_CONNECT_CENTER };

  uint num_names = sizeof(names)/sizeof(names[0]);

  for (uint i = 0; i < num_names; ++i) {
    if (i == 0) value_ = values[0];

    nameValue_[names [i]] = values[i];
    valueName_[values[i]] = names [i];

    addItem(names[i]);
  }

  connect(this, SIGNAL(currentIndexChanged(const QString &)),
          this, SLOT(itemSlot(const QString &)));
}

void
CQEllipseShape2DConnectType::
setType(CEllipseConnectType value)
{
  int num = count();

  for (int i = 0; i < num; ++i) {
    if (nameValue_[itemText(i)] == value) {
      setCurrentIndex(i);
      return;
    }
  }
}

void
CQEllipseShape2DConnectType::
itemSlot(const QString &name)
{
  value_ = nameValue_[name];

  emit valueChanged();
}
