#include <CQIllustratorCreateTextMode.h>
#include <CQIllustratorTextShape.h>
#include <CQIllustrator.h>
#include <CQIllustratorHandle.h>
#include <CQIllustratorShapeDrawer.h>

#include <QPainter>
#include <QHBoxLayout>
#include <QFontComboBox>

#include <CQMenu.h>
#include <CQFontUtil.h>
#include <CQUtil.h>
#include <CQUtilGeom.h>
#include <CQRealEdit.h>
#include <CQImageButton.h>
#include <CQSwatch.h>

#include <svg/text_svg.h>

#include <cursors/text.xbm>
#include <cursors/textmask.xbm>

CQIllustratorCreateTextMode::
CQIllustratorCreateTextMode(CQIllustrator *illustrator) :
 CQIllustratorMode(illustrator, uint(CQIllustrator::Mode::TEXT)), text_("Hello")
{
  setCursor(text_bits, textmask_bits, 2, 2);
}

CQIllustratorCreateTextToolbar *
CQIllustratorCreateTextMode::
createToolbar()
{
  toolbar_ = new CQIllustratorCreateTextToolbar(this);

  return toolbar_;
}

CQIllustratorCreateTextSizer *
CQIllustratorCreateTextMode::
createSizer()
{
  sizer_ = new CQIllustratorCreateTextSizer(this);

  return sizer_;
}

CQMenuItem *
CQIllustratorCreateTextMode::
createMenuItem(CQMenu *menu)
{
  QIcon icon = CQPixmapCacheInst->getIcon("TEXT");

  menuItem_ = new CQMenuItem(menu, icon, "&Text", CQMenuItem::CHECKABLE);

  menuItem_->setStatusTip("Create Text");

  connect(menuItem_->getAction(), SIGNAL(toggled(bool)), this, SLOT(menuItemSlot()));

  return menuItem_;
}

void
CQIllustratorCreateTextMode::
handleMouseRelease(const MouseEvent &e)
{
  CPoint2D p2 = CQUtil::fromQPoint(e.window);

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
    CPoint2D p1 = CQUtil::fromQPoint(press_wpos_);

    // point click - select at point
    if (! moving_) {
      (void) illustrator_->selectAt(p2, e.event->isControlKey(), e.event->isShiftKey());
    }
    // drag then create text using specified bbox
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

      CQIllustratorTextShape *text =
        illustrator_->createTextShape(bbox.getLL(), bbox.getUR(), text_.toStdString());

      CQIllustratorShapeStroke stroke;
      CQIllustratorShapeFill   fill;

      stroke.setOpacity(0.0);
      fill  .setColor  (CRGBA(0,0,0));

      text->setStroke(stroke);
      text->setFill  (fill);

      text->setFont(CQFontUtil::fromQFont(toolbar_->getFont()));

      illustrator_->addShape(text);

      illustrator_->setSelectShape(text);
    }
  }

  dragging_ = false;

  illustrator_->redraw();
}

void
CQIllustratorCreateTextMode::
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

bool
CQIllustratorCreateTextMode::
handleKeyPress(const KeyEvent &e)
{
  CQIllustratorSelectedShapes *selection = illustrator_->getSelection();

  if (selection->size() != 1) return false;

  const CQIllustratorSelectedShape &sshape = selection->front();

  const CQIllustratorShape *shape = sshape.getShape();

  const auto *text = dynamic_cast<const CQIllustratorTextShape *>(shape);

  if (! text) return false;

  std::string str  = text->getText();
  std::string str1 = str;

  CKeyType key = e.event->getType();

  if      (key >= CKEY_TYPE_Space && key <= CKEY_TYPE_AsciiTilde) {
    str1 += char(key);
  }
  else if (key == CKEY_TYPE_Return) {
    str1 += '\n';
  }
  else if (key == CKEY_TYPE_BackSpace) {
    if (str1 != "")
      str1 = str1.substr(0, str1.size() - 1);
  }
  else
    return CQIllustratorMode::handleKeyPress(e);

  if (str != str1) {
    auto *text1 = const_cast<CQIllustratorTextShape *>(text);

    illustrator_->checkoutShape(text1, CQIllustratorData::ChangeType::GEOMETRY);

    text1->setText(str1);

    illustrator_->checkinShape(text1, CQIllustratorData::ChangeType::GEOMETRY);
  }

  return true;
}

void
CQIllustratorCreateTextMode::
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

void
CQIllustratorCreateTextMode::
setText(const QString &text)
{
  text_ = text;
}

//------

CQIllustratorCreateTextSizer::
CQIllustratorCreateTextSizer(CQIllustratorCreateTextMode *mode) :
 CQIllustratorModeSizer(mode)
{
  CQIllustrator *illustrator = mode_->getIllustrator();

  ll_handle_ = new CQIllustratorControlPointHandle(illustrator);
  ur_handle_ = new CQIllustratorControlPointHandle(illustrator);

  addHandle(ll_handle_);
  addHandle(ur_handle_);
}

void
CQIllustratorCreateTextSizer::
drawHandles(QPainter *painter, const CQIllustratorShape *shape)
{
  const auto *text = dynamic_cast<const CQIllustratorTextShape *>(shape);

  if (text) {
    CQIllustratorShape::ControlPointList controlPoints;

    text->getControlPoints(controlPoints);

    ll_handle_->draw(text, controlPoints[0], painter);
    ur_handle_->draw(text, controlPoints[1], painter);
  }
}
