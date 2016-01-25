#include <CQScrollArea.h>

#include <QGridLayout>
#include <QScrollBar>

CQScrollArea::
CQScrollArea(QWidget *widget) :
 QWidget  (0),
 widget_  (widget),
 x_size_  (-1),
 y_size_  (-1),
 x_offset_(0),
 y_offset_(0)
{
  QGridLayout *grid = new QGridLayout(this);
  grid->setMargin(0); grid->setSpacing(0);

  hbar_ = new QScrollBar(Qt::Horizontal);
  vbar_ = new QScrollBar(Qt::Vertical  );

  grid->addWidget(widget_, 0, 0);
  grid->addWidget(hbar_  , 1, 0);
  grid->addWidget(vbar_  , 0, 1);

  QObject::connect(hbar_, SIGNAL(valueChanged(int)), this, SLOT(hscrollSlot(int)));
  QObject::connect(vbar_, SIGNAL(valueChanged(int)), this, SLOT(vscrollSlot(int)));
}

void
CQScrollArea::
showHBar(bool flag)
{
  flag ? hbar_->show() : hbar_->hide();
}

void
CQScrollArea::
showVBar(bool flag)
{
  flag ? vbar_->show() : vbar_->hide();
}

void
CQScrollArea::
setCornerWidget(QWidget *w)
{
  QGridLayout *grid = qobject_cast<QGridLayout *>(layout());

  grid->addWidget(w, 1, 1);
}

void
CQScrollArea::
setXSize(int x_size)
{
  x_size_ = x_size;

  updateScrollbars();
}

void
CQScrollArea::
setYSize(int y_size)
{
  y_size_ = y_size;

  updateScrollbars();
}

void
CQScrollArea::
setXOffset(int x_offset)
{
  x_offset_ = x_offset;

  updateScrollbars();
}

void
CQScrollArea::
setYOffset(int y_offset)
{
  y_offset_ = y_offset;

  updateScrollbars();
}

void
CQScrollArea::
setXPageStep(int x)
{
  hbar_->setPageStep(x);
}

void
CQScrollArea::
setYPageStep(int y)
{
  vbar_->setPageStep(y);
}

void
CQScrollArea::
scrollUp(bool page)
{
  vbar_->triggerAction(page ? QAbstractSlider::SliderPageStepSub :
                              QAbstractSlider::SliderSingleStepSub);
}

void
CQScrollArea::
scrollDown(bool page)
{
  vbar_->triggerAction(page ? QAbstractSlider::SliderPageStepAdd :
                              QAbstractSlider::SliderSingleStepAdd);
}

void
CQScrollArea::
scrollLeft(bool page)
{
  hbar_->triggerAction(page ? QAbstractSlider::SliderPageStepSub :
                              QAbstractSlider::SliderSingleStepSub);
}

void
CQScrollArea::
scrollRight(bool page)
{
  hbar_->triggerAction(page ? QAbstractSlider::SliderPageStepAdd :
                              QAbstractSlider::SliderSingleStepAdd);
}

void
CQScrollArea::
ensureVisible(int x, int y, int xmargin, int ymargin)
{
  int xmin = -getXOffset();
  int xmax = xmin + width();
  int ymin = -getYOffset();
  int ymax = ymin + height();

  if      (y < ymin)
    setYOffset(-y + ymargin);
  else if (y > ymax)
    setYOffset(-y + height() - ymargin);

  if      (x < xmin)
    setXOffset(-x + xmargin);
  else if (x > xmax)
    setXOffset(-x + width() - xmargin);
}

void
CQScrollArea::
updateScrollbars()
{
  int w = widget_->width ();
  int h = widget_->height();

  int dx = std::max(0, getXSize() - w);
  int dy = std::max(0, getYSize() - h);

  hbar_->setPageStep(w);

  hbar_->setMinimum(0);
  hbar_->setMaximum(dx);

  hbar_->setValue(-getXOffset());

  vbar_->setPageStep(h);

  vbar_->setMinimum(0);
  vbar_->setMaximum(dy);

  vbar_->setValue(-getYOffset());
}

void
CQScrollArea::
hscrollSlot(int value)
{
  setXOffset(-value);

  emit updateArea();
}

void
CQScrollArea::
vscrollSlot(int value)
{
  setYOffset(-value);

  emit updateArea();
}
