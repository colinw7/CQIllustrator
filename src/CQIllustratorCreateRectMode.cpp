#include <CQIllustratorCreateRectMode.h>
#include <CQIllustratorRectShape.h>
#include <CQIllustrator.h>
#include <CQIllustratorCmd.h>
#include <CQIllustratorHandle.h>
#include <CQIllustratorShapeDrawer.h>
#include <CQIllustratorUtil.h>

#include <QPainter>
#include <QHBoxLayout>

#include <CQMenu.h>
#include <CQUtil.h>
#include <CQUtilGeom.h>
#include <CQPointEdit.h>
#include <CQRealEdit.h>
#include <CQSwatch.h>

#include <svg/rect_svg.h>

#include <cursors/rect.xbm>
#include <cursors/rectmask.xbm>

class CQIllustratorCreateRectCmd : public CQIllustratorCmd {
 public:
  CQIllustratorCreateRectCmd();

  const char *getName() const override { return "create_rect"; }

  bool exec(const std::vector<std::string> &words) override;
};

//------

CQIllustratorCreateRectMode::
CQIllustratorCreateRectMode(CQIllustrator *illustrator) :
 CQIllustratorMode(illustrator, uint(CQIllustrator::Mode::RECT))
{
  setCursor(rect_bits, rectmask_bits, 2, 2);

  illustrator_->addCommand(new CQIllustratorCreateRectCmd);
}

CQIllustratorCreateRectToolbar *
CQIllustratorCreateRectMode::
createToolbar()
{
  toolbar_ = new CQIllustratorCreateRectToolbar(this);

  return toolbar_;
}

CQIllustratorCreateRectSizer *
CQIllustratorCreateRectMode::
createSizer()
{
  sizer_ = new CQIllustratorCreateRectSizer(this);

  return sizer_;
}

CQMenuItem *
CQIllustratorCreateRectMode::
createMenuItem(CQMenu *menu)
{
  QIcon icon = CQPixmapCacheInst->getIcon("RECT");

  menuItem_ = new CQMenuItem(menu, icon, "&Rectangle", CQMenuItem::CHECKABLE);

  menuItem_->setStatusTip("Create Rectangle");

  connect(menuItem_->getAction(), SIGNAL(toggled(bool)), this, SLOT(menuItemSlot()));

  return menuItem_;
}

void
CQIllustratorCreateRectMode::
handleMouseRelease(const MouseEvent &e)
{
  CPoint2D p1 = CQUtil::fromQPoint(press_wpos_);
  CPoint2D p2 = CQUtil::fromQPoint(release_wpos_);

  if (dragging_) {
    if (! moving_) {
      selectControlPoint(e);
    }
    // dragging finished so commit
    else {
      illustrator_->getSandbox()->commit(CQIllustratorData::ChangeType::GEOMETRY);
    }
  }
  // not dragging so do a select
  else {
    if (editMode_ == EditMode::CREATE) {
      // drag then create rectangle using specified bbox
      if (moving_) {
        CBBox2D bbox(p1, p2);

        if (e.event->isShiftKey()) {
          double w = bbox.getWidth ();
          double h = bbox.getHeight();

          if (w > h)
            bbox.setHeight(w);
          else
            bbox.setWidth(h);
        }

        CQIllustratorRectShape *rect =
          illustrator_->createRectShape(bbox.getLL(), bbox.getUR());

        illustrator_->addShape(rect);

        illustrator_->setSelectShape(rect);
      }
    }
    else {
      // point click - select at point
      if (! moving_) {
        (void) illustrator_->selectAt(p2, e.event->isControlKey(), e.event->isShiftKey());
      }
      // drag - select points in rectangle
      else {
        CBBox2D bbox(p1, p2);

        illustrator_->selectPointsIn(bbox, CQIllustratorShape::ControlType::GEOMETRY,
                                     e.event->isControlKey(), e.event->isShiftKey());
      }
    }
  }

  dragging_ = false;

  illustrator_->redraw();
}

void
CQIllustratorCreateRectMode::
handleMouseDrag(const MouseEvent &e)
{
  if (dragging_) {
    CQIllustratorShape *shape = illustrator_->getSandbox()->frontShape();

    sizer_->updatePoint(shape, e.window);
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
CQIllustratorCreateRectMode::
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
      pen.setWidth(0);

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

CQIllustratorCreateRectToolbar::
CQIllustratorCreateRectToolbar(CQIllustratorCreateRectMode *mode) :
 CQIllustratorToolbar(mode), mode_(mode)
{
}

QIcon
CQIllustratorCreateRectToolbar::
getIcon()
{
  return CQPixmapCacheInst->getIcon("RECT");
}

void
CQIllustratorCreateRectToolbar::
addWidgets()
{
  QBoxLayout *layout = qobject_cast<QBoxLayout *>(CQIllustratorToolbar::layout());

  //-----

  posEdit_    = new CQPointEdit;
  widthEdit_  = new CQRealEdit(0.0);
  heightEdit_ = new CQRealEdit(0.0);
  xRadEdit_   = new CQRealEdit(0.0);
  yRadEdit_   = new CQRealEdit(0.0);

  posEdit_   ->setFocusPolicy(Qt::ClickFocus);
  widthEdit_ ->setFocusPolicy(Qt::ClickFocus);
  heightEdit_->setFocusPolicy(Qt::ClickFocus);
  xRadEdit_  ->setFocusPolicy(Qt::ClickFocus);
  yRadEdit_  ->setFocusPolicy(Qt::ClickFocus);

  connect(posEdit_   , SIGNAL(valueChanged()), this, SLOT(updateShape()));
  connect(widthEdit_ , SIGNAL(valueChanged(double)), this, SLOT(updateShape()));
  connect(heightEdit_, SIGNAL(valueChanged(double)), this, SLOT(updateShape()));
  connect(xRadEdit_  , SIGNAL(valueChanged(double)), this, SLOT(updateShape()));
  connect(yRadEdit_  , SIGNAL(valueChanged(double)), this, SLOT(updateShape()));

  CQSwatch *posSwatch    = new CQSwatch("Position", posEdit_   );
  CQSwatch *widthSwatch  = new CQSwatch("Width"   , widthEdit_ );
  CQSwatch *heightSwatch = new CQSwatch("Height"  , heightEdit_);
  CQSwatch *xRadSwatch   = new CQSwatch("X Radius", xRadEdit_  );
  CQSwatch *yRadSwatch   = new CQSwatch("Y Radius", yRadEdit_  );

  layout->addWidget(posSwatch);
  layout->addWidget(widthSwatch);
  layout->addWidget(heightSwatch);
  layout->addWidget(xRadSwatch);
  layout->addWidget(yRadSwatch);

  posEdit_   ->setEnabled(false);
  widthEdit_ ->setEnabled(false);
  heightEdit_->setEnabled(false);
  xRadEdit_  ->setEnabled(false);
  yRadEdit_  ->setEnabled(false);
}

void
CQIllustratorCreateRectToolbar::
setSelectedShape(const CQIllustratorShape *shape)
{
  const CQIllustratorRectShape *rect = dynamic_cast<const CQIllustratorRectShape *>(shape);

  posEdit_   ->setEnabled(rect != nullptr);
  widthEdit_ ->setEnabled(rect != nullptr);
  heightEdit_->setEnabled(rect != nullptr);
  xRadEdit_  ->setEnabled(rect != nullptr);
  yRadEdit_  ->setEnabled(rect != nullptr);

  if (rect) {
    const CBBox2D &bbox = rect->getBBox();

    setSize(bbox);

    xRadEdit_->setValue(rect->getRadiusX());
    yRadEdit_->setValue(rect->getRadiusY());
  }
  else {
    posEdit_   ->setValue();
    widthEdit_ ->setValue(0.0);
    heightEdit_->setValue(0.0);
    xRadEdit_  ->setValue(0.0);
    yRadEdit_  ->setValue(0.0);
  }
}

void
CQIllustratorCreateRectToolbar::
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
CQIllustratorCreateRectToolbar::
updateShape()
{
  CQIllustrator *illustrator = mode_->getIllustrator();

  CQIllustratorRectShape *shape =
    CQIllustratorUtil::getCurrentShape<CQIllustratorRectShape>(illustrator);

  if (shape) {
    const CPoint2D &pos = posEdit_->getValue();

    double width  = widthEdit_ ->getValue();
    double height = heightEdit_->getValue();

    double xRad = xRadEdit_->getValue();
    double yRad = yRadEdit_->getValue();

    illustrator->checkoutShape(shape, CQIllustratorData::ChangeType::GEOMETRY);

    CQIllustratorRectShape *rect = dynamic_cast<CQIllustratorRectShape *>(shape);

    CBBox2D bbox(pos, CPoint2D(pos.x + width, pos.y + height));

    rect->setBBox(bbox);

    rect->setRadiusX(xRad);
    rect->setRadiusY(yRad);

    illustrator->checkinShape(shape, CQIllustratorData::ChangeType::GEOMETRY);
  }

  illustrator->redraw();
}

//------

CQIllustratorCreateRectSizer::
CQIllustratorCreateRectSizer(CQIllustratorCreateRectMode *mode) :
 CQIllustratorModeSizer(mode)
{
  CQIllustrator *illustrator = mode_->getIllustrator();

  ll_handle_ = new CQIllustratorControlPointHandle(illustrator);
  ur_handle_ = new CQIllustratorControlPointHandle(illustrator);
  rx_handle_ = new CQIllustratorControlPointHandle(illustrator);
  ry_handle_ = new CQIllustratorControlPointHandle(illustrator);

  rx_handle_->setStyle(CQIllustratorHandle::CIRCLE_STYLE);
  ry_handle_->setStyle(CQIllustratorHandle::CIRCLE_STYLE);

  addHandle(ll_handle_);
  addHandle(ur_handle_);
  addHandle(rx_handle_);
  addHandle(ry_handle_);
}

void
CQIllustratorCreateRectSizer::
drawHandles(QPainter *painter, const CQIllustratorShape *shape)
{
  const CQIllustratorRectShape *rect = dynamic_cast<const CQIllustratorRectShape *>(shape);

  if (rect) {
    CQIllustratorShape::ControlPointList controlPoints;

    rect->getControlPoints(controlPoints);

    ll_handle_->draw(rect, controlPoints[0], painter);
    ur_handle_->draw(rect, controlPoints[1], painter);
    rx_handle_->draw(rect, controlPoints[2], painter);
    ry_handle_->draw(rect, controlPoints[3], painter);
  }
}

//----------

static const char *create_rect_opts = "\
-x:r=0 \
-y:r=0 \
-w:r=100 \
-h:r=100 \
-rx:r=0 \
-ry:r=0 \
-a:r=0 \
-name:s";

CQIllustratorCreateRectCmd::
CQIllustratorCreateRectCmd() :
 CQIllustratorCmd(create_rect_opts)
{
}

bool
CQIllustratorCreateRectCmd::
exec(const std::vector<std::string> &words)
{
  args_.parse(words);

  double x  = args_.getRealArg("-x");
  double y  = args_.getRealArg("-y");
  double w  = args_.getRealArg("-w");
  double h  = args_.getRealArg("-h");
  double rx = args_.getRealArg("-rx");
  double ry = args_.getRealArg("-ry");
  double a  = args_.getRealArg("-a");

  CBBox2D bbox(CPoint2D(x, y), CSize2D(w, h));

  CQIllustratorRectShape *rect =
    illustrator_->createRectShape(bbox.getLL(), bbox.getUR(), rx, ry);

  std::string name = args_.getStringArg("-name");

  if (name != "")
    rect->setName(name);

  rect->rotate(M_PI*a/180.0);

  illustrator_->addShape(rect);

  illustrator_->setSelectShape(rect);

  return true;
}
