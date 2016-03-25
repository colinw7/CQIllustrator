#include <CQIllustratorZoomMode.h>
#include <CQIllustrator.h>
#include <CQIllustratorShapeDrawer.h>

#include <QPainter>
#include <QBoxLayout>

#include <CQUtil.h>
#include <CQMenu.h>
#include <CQSwatch.h>
#include <CQImageButton.h>

#include <svg/zoom_svg.h>
#include <svg/zoom_fit_svg.h>
#include <svg/zoom_selected_svg.h>
#include <svg/zoom_in_svg.h>
#include <svg/zoom_out_svg.h>

#include <cursors/zoom.xbm>
#include <cursors/zoommask.xbm>

CQIllustratorZoomMode::
CQIllustratorZoomMode(CQIllustrator *illustrator) :
 CQIllustratorMode(illustrator, CQIllustrator::MODE_ZOOM)
{
  setCursor(zoom_bits, zoommask_bits, 2, 2);
}

CQIllustratorZoomToolbar *
CQIllustratorZoomMode::
createToolbar()
{
  toolbar_ = new CQIllustratorZoomToolbar(this);

  return toolbar_;
}

CQMenuItem *
CQIllustratorZoomMode::
createMenuItem(CQMenu *menu)
{
  QIcon icon = CQPixmapCacheInst->getIcon("ZOOM");

  menuItem_ = new CQMenuItem(menu, icon, "&Zoom In/Out", CQMenuItem::CHECKABLE);

  menuItem_->setStatusTip("Zoom In or Out");

  connect(menuItem_->getAction(), SIGNAL(toggled(bool)), this, SLOT(menuItemSlot()));

  return menuItem_;
}

void
CQIllustratorZoomMode::
handleMousePress(const MouseEvent &)
{
}

void
CQIllustratorZoomMode::
handleMouseRelease(const MouseEvent &e)
{
  //QPoint current_ppos = e.pixel;

  if (! moving_) {
    CPoint2D c = CQUtil::fromQPoint(e.window);

    double scale = sqrt(2);

    double size = illustrator_->getBBox().getWidth();

    if      (e.event->getButton() == CBUTTON_LEFT) {
      size /= scale;

      CBBox2D bbox(CPoint2D(c.x - size/2, c.y - size/2), CPoint2D(c.x + size/2, c.y + size/2));

      illustrator_->setBBox(bbox);
    }
    else if (e.event->getButton() == CBUTTON_RIGHT) {
      size *= scale;

      CBBox2D bbox(CPoint2D(c.x - size/2, c.y - size/2), CPoint2D(c.x + size/2, c.y + size/2));

      illustrator_->setBBox(bbox);
    }
  }
  else {
    CBBox2D bbox(CQUtil::fromQPoint(press_wpos_), CQUtil::fromQPoint(e.window));

    if      (e.event->getButton() == CBUTTON_LEFT) {
      illustrator_->setBBox(bbox);
    }
    else if (e.event->getButton() == CBUTTON_RIGHT) {
      double sx = illustrator_->getBBox().getWidth () / bbox.getWidth ();
      double sy = illustrator_->getBBox().getHeight() / bbox.getHeight();

      double s = std::min(sx, sy);

      CPoint2D d(s*illustrator_->getBBox().getWidth ()/2.0,
                 s*illustrator_->getBBox().getHeight()/2.0);

      CBBox2D bbox1(bbox.getCenter() - d, bbox.getCenter() + d);

      illustrator_->setBBox(bbox1);
    }
  }

  illustrator_->redraw();
}

void
CQIllustratorZoomMode::
handleMouseDrag(const MouseEvent &)
{
  illustrator_->redrawOverlay();
}

void
CQIllustratorZoomMode::
handleMouseMove(const MouseEvent &)
{
}

void
CQIllustratorZoomMode::
drawOverlay(CQIllustratorShapeDrawer *drawer)
{
  if (pressed_) {
    QPainter *painter = drawer->getPainter();

    QPointF current_wpos = illustrator_->getITransform().map(QPointF(curr_ppos_));

    QPen pen;

    pen.setColor(QColor(0,0,0));
    pen.setStyle(Qt::DashLine);

    painter->setPen(pen);
    painter->setBrush(Qt::NoBrush);

    painter->drawRect(QRectF(press_wpos_, current_wpos));
  }
}

//------------

CQIllustratorZoomToolbar::
CQIllustratorZoomToolbar(CQIllustratorZoomMode *mode) :
 CQIllustratorToolbar(mode), mode_(mode)
{
}

QIcon
CQIllustratorZoomToolbar::
getIcon()
{
  return CQPixmapCacheInst->getIcon("ZOOM");
}

void
CQIllustratorZoomToolbar::
addWidgets()
{
  QBoxLayout *layout = qobject_cast<QBoxLayout *>(CQIllustratorToolbar::layout());

  zoomFitButton_ = new CQImageButton(CQPixmapCacheInst->getIcon("ZOOM_FIT"     ));
  zoomSelButton_ = new CQImageButton(CQPixmapCacheInst->getIcon("ZOOM_SELECTED"));
  zoomInButton_  = new CQImageButton(CQPixmapCacheInst->getIcon("ZOOM_IN"      ));
  zoomOutButton_ = new CQImageButton(CQPixmapCacheInst->getIcon("ZOOM_OUT"     ));

  zoomFitButton_->setToolTip("Zoom Fit");
  zoomSelButton_->setToolTip("Zoom Selected");
  zoomInButton_ ->setToolTip("Zoom In");
  zoomOutButton_->setToolTip("Zoom Out");

  connect(zoomFitButton_, SIGNAL(clicked()), this, SLOT(zoomFitSlot()));
  connect(zoomSelButton_, SIGNAL(clicked()), this, SLOT(zoomSelSlot()));
  connect(zoomInButton_ , SIGNAL(clicked()), this, SLOT(zoomInSlot()));
  connect(zoomOutButton_, SIGNAL(clicked()), this, SLOT(zoomOutSlot()));

  CQSwatch *swatch =
    new CQSwatch("Zoom<4>", zoomFitButton_, zoomSelButton_, zoomInButton_, zoomOutButton_);

  layout->addWidget(swatch);
}

void
CQIllustratorZoomToolbar::
zoomFitSlot()
{
  CQIllustrator *illustrator = mode_->getIllustrator();

  illustrator->zoomFit();
}

void
CQIllustratorZoomToolbar::
zoomSelSlot()
{
  CQIllustrator *illustrator = mode_->getIllustrator();

  illustrator->zoomSelected();
}

void
CQIllustratorZoomToolbar::
zoomInSlot()
{
  CQIllustrator *illustrator = mode_->getIllustrator();

  illustrator->zoomIn();
}

void
CQIllustratorZoomToolbar::
zoomOutSlot()
{
  CQIllustrator *illustrator = mode_->getIllustrator();

  illustrator->zoomOut();
}
