#include <CQImagePreview.h>
#include <CQUtil.h>
#include <CQScrollArea.h>

#include <control.xpm>

#include <QPainter>
#include <QHBoxLayout>
#include <QToolButton>

CQImagePreview::
CQImagePreview(QWidget *parent) :
 QWidget(parent)
{
  QHBoxLayout *layout = new QHBoxLayout(this);
  layout->setMargin(0); layout->setSpacing(0);

  canvas_ = new CQImagePreviewCanvas(this);

  sarea_ = new CQScrollArea(canvas_);

  connect(sarea_, SIGNAL(updateArea()), this, SLOT(canvasUpdateSlot()));

  QToolButton *button = new QToolButton;

  button->setAutoRaise(true);

  button->setIcon(QIcon(QPixmap(control_data)));

  sarea_->setCornerWidget(button);

  layout->addWidget(sarea_);
}

CQImagePreview::
~CQImagePreview()
{
}

void
CQImagePreview::
canvasUpdateSlot()
{
  canvas_->update();
}

void
CQImagePreview::
sendCancelSignal()
{
  emit cancelSignal();
}

CImagePtr
CQImagePreview::
getImage() const
{
  return canvas_->getImage();
}

void
CQImagePreview::
setImage(CImagePtr image)
{
  canvas_->setImage(image);
}

void
CQImagePreview::
setBackground(const CRGBA &bg)
{
  canvas_->setBackground(bg);
}

void
CQImagePreview::
scrollUp(bool page)
{
  sarea_->scrollUp(page);
}

void
CQImagePreview::
scrollDown(bool page)
{
  sarea_->scrollDown(page);
}

void
CQImagePreview::
scrollLeft(bool page)
{
  sarea_->scrollLeft(page);
}

void
CQImagePreview::
scrollRight(bool page)
{
  sarea_->scrollRight(page);
}

//----------

CQImagePreviewCanvas::
CQImagePreviewCanvas(CQImagePreview *view) :
 QWidget     (0),
 view_       (view),
 zoom_factor_(1.0),
 fill_screen_(false),
 keep_aspect_(true)
{
  setFocusPolicy(Qt::StrongFocus);
}

CQImagePreviewCanvas::
~CQImagePreviewCanvas()
{
}

void
CQImagePreviewCanvas::
setImage(CImagePtr image)
{
  image_ = image;

  update();
}

void
CQImagePreviewCanvas::
setBackground(const CRGBA &bg)
{
  bg_ = bg;

  update();
}

void
CQImagePreviewCanvas::
setZoomFactor(double factor)
{
  zoom_factor_ = factor;

  update();
}

void
CQImagePreviewCanvas::
zoomIncrease()
{
  setZoomFactor(2.0*getZoomFactor());
}

void
CQImagePreviewCanvas::
zoomDecrease()
{
  setZoomFactor(0.5*getZoomFactor());
}

void
CQImagePreviewCanvas::
resizeEvent(QResizeEvent *)
{
  view_->getScrollArea()->updateScrollbars();
}

void
CQImagePreviewCanvas::
paintEvent(QPaintEvent *)
{
  QPainter painter(this);

  painter.fillRect(rect(), CQUtil::rgbaToColor(bg_));

  if (! image_.isValid()) return;

  int dx = 0, dy = 0;

  int iw = image_->getWidth ();
  int ih = image_->getHeight();

  if (iw > width())
    dx = view_->getScrollArea()->getXOffset();
  else
    dx = (width() - iw)/2;

  if (ih > height())
    dy = view_->getScrollArea()->getYOffset();
  else
    dy = (height() - ih)/2;

  painter.drawImage(QPoint(dx, dy), CQUtil::toQImage(image_));

  view_->getScrollArea()->setXSize(iw);
  view_->getScrollArea()->setYSize(ih);
}

void
CQImagePreviewCanvas::
keyPressEvent(QKeyEvent *e)
{
  CKeyEvent *ke = CQUtil::convertEvent(e);

  if      (ke->getType() == CKEY_TYPE_Escape)
    view_->sendCancelSignal();
  else if (ke->getType() == CKEY_TYPE_Up)
    view_->scrollUp(ke->isShiftKey());
  else if (ke->getType() == CKEY_TYPE_Down)
    view_->scrollDown(ke->isShiftKey());
  else if (ke->getType() == CKEY_TYPE_Left)
    view_->scrollLeft(ke->isShiftKey());
  else if (ke->getType() == CKEY_TYPE_Right)
    view_->scrollRight(ke->isShiftKey());
}
