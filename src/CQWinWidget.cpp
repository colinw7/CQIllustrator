#include <CQWinWidget.h>

#include <QImage>
#include <QMenu>
#include <QPaintEvent>
#include <QMouseEvent>
#include <QPainter>
#include <QBrush>
#include <QBitmap>
#include <QStyleOption>
#include <QFileDialog>

#include "cursors/select.xbm"
#include "cursors/selectmask.xbm"

CQWinWidget::
CQWinWidget(QWidget *parent, const char *name) :
 QWidget        (parent),
 child_         (0),
 active_        (false),
 pressed_       (false),
 decoration_    (HeaderBorderDecoration, SideTop, 10, 1, QColor(80,80,100)),
 closeButton_   (style()->standardIcon(QStyle::SP_TitleBarCloseButton)),
 expandButton_  (style()->standardIcon(QStyle::SP_TitleBarShadeButton)),
 collapseButton_(style()->standardIcon(QStyle::SP_TitleBarUnshadeButton)),
 ops_           (MoveOp | ResizeOp | RaiseOp | LowerOp | CloseOp),
 edit_mode_     (EDIT_MODE_DRAG),
 constraint_    (NO_CONSTRAINT)
{
  setObjectName(name);

  setMouseTracking(true);

  setAutoFillBackground(true);
}

CQWinWidget::
~CQWinWidget()
{
}

void
CQWinWidget::
setChild(QWidget *child)
{
  if (child_)
    child_->setParent(NULL);

  child_ = child;

  child_->setParent(this);

  child_->show();

  if (ops_ & ResizeOp)
    resizeEvent(0);
  else
    fitChild();
}

void
CQWinWidget::
fitChild()
{
  setChildSize(child_->sizeHint());
}

void
CQWinWidget::
setChildSize(const QSize &size)
{
  int w = size.width ();
  int h = size.height();

  if (w <= 0) w = 100;
  if (h <= 0) h = 100;

  int b = getBorder();

  w += 2*b;
  h += 2*b;

  if (decoration_.type & HeaderDecoration) {
    int hh = getHeaderHeight();

    if (decoration_.header_side == SideTop || decoration_.header_side == SideBottom)
      h += hh;
    else
      w += hh;
  }

  resize(w, h);
}

int
CQWinWidget::
getX() const
{
  int x = getBorder();

  if (decoration_.type & HeaderDecoration) {
    if (decoration_.header_side == SideLeft)
      x += getHeaderHeight();
  }

  return x;
}

int
CQWinWidget::
getY() const
{
  int y = getBorder();

  if (decoration_.type & HeaderDecoration) {
    if (decoration_.header_side == SideTop)
      y += getHeaderHeight();
  }

  return y;
}

int
CQWinWidget::
getWidth() const
{
  int b = getBorder();

  int h = 0;

  if (decoration_.type & HeaderDecoration) {
    if (decoration_.header_side == SideLeft || decoration_.header_side == SideRight)
      h = getHeaderHeight();
  }

  return width() - 2*b - h;
}

int
CQWinWidget::
getHeight() const
{
  int b = getBorder();

  int h = 0;

  if (decoration_.type & HeaderDecoration) {
    if (decoration_.header_side == SideTop || decoration_.header_side == SideBottom)
      h = getHeaderHeight();
  }

  return height() - 2*b - h;
}

int
CQWinWidget::
getHeaderHeight() const
{
  QFontMetrics fm(font());

  int h = qMax(decoration_.header_height, fm.height() + 4);

  return h;
}

void
CQWinWidget::
setPos(int x, int y)
{
  move(x - getX(), y - getY());
}

void
CQWinWidget::
setSize(int w, int h)
{
  int b = getBorder();

  int hw = 0, hh = 0;

  if (decoration_.type & HeaderDecoration) {
    if (decoration_.header_side == SideLeft || decoration_.header_side == SideRight)
      hw = getHeaderHeight();

    if (decoration_.header_side == SideTop || decoration_.header_side == SideBottom)
      hh = getHeaderHeight();
  }

  resize(w + hw + 2*b, h + hh + 2*b);
}

bool
CQWinWidget::
checkMove(QPoint &p) const
{
  if (constraint_ == VISIBLE_CONSTRAINT) {
    QRect r(p, this->size());

    int pw = parentWidget()->width();
    int ph = parentWidget()->height();

    if (p.x()            < 0 ) p = QPoint(0, p.y());
    if (p.y()            < 0 ) p = QPoint(p.x(), 0);
    if (p.x() + width () > pw) p = QPoint(pw - width(), p.y());
    if (p.y() + height() > ph) p = QPoint(p.x(), ph - height());
  }

  return true;
}

bool
CQWinWidget::
checkGeometry(const QRect &rect) const
{
  if (constraint_ == VISIBLE_CONSTRAINT) {
    if (rect.x()                 < 0 ||
        rect.y()                 < 0 ||
        rect.x() + rect.width () > parentWidget()->width() ||
        rect.y() + rect.height() > parentWidget()->height())
     return false;
  }

  return true;
}

void
CQWinWidget::
paintEvent(QPaintEvent *)
{
  QPainter painter(this);

  // draw border
  int b = getBorder();

  qDrawPlainRect(&painter, 0, 0, width(), height(), decoration_.border_color, b);

  // draw header
  if (decoration_.type & HeaderDecoration) {
    QPalette pal = palette();

    int hh = getHeaderHeight();

    // draw background
    QBrush bgBrush = QBrush(pal.color(QPalette::Window));

    if      (decoration_.header_side == SideTop)
      decoration_.header_rect = QRect(b, b, width() - 2*b, hh);
    else if (decoration_.header_side == SideBottom)
      decoration_.header_rect = QRect(b, height() - b - hh, width() - 2*b, hh);
    else if (decoration_.header_side == SideLeft)
      decoration_.header_rect = QRect(b, b, hh, height() - 2*b);
    else if (decoration_.header_side == SideRight)
      decoration_.header_rect = QRect(width() - b - hh, b, hh, height() - 2*b);

    painter.fillRect(decoration_.header_rect, bgBrush);

    int b1 = hh - 4;

    closeButton_   .displayed = (ops_ & CloseOp );
    expandButton_  .displayed = (ops_ & ExpandOp);
    collapseButton_.displayed = (ops_ & CollapseOp);

    // draw title bar handle
    int panelSize = 3;
    int margin    = (decoration_.header_height - 2*panelSize - 1)/2;

    if (decoration_.header_side == SideTop || decoration_.header_side == SideBottom) {
      int x1 = width() - b + 2;

      int panelWidth = qMax(1, x1 - 2*b - 2*margin);

      if (closeButton_   .displayed) panelWidth -= b1 + 2;
      if (expandButton_  .displayed) panelWidth -= b1 + 2;
      if (collapseButton_.displayed) panelWidth -= b1 + 2;

      qDrawShadePanel(&painter, b + margin, b + margin                , panelWidth, panelSize, pal);
      qDrawShadePanel(&painter, b + margin, b + margin + 1 + panelSize, panelWidth, panelSize, pal);
    }
    else {
       int y1 = b + 2;

      int panelWidth = qMax(1, height() - y1 - 2*b - 2*margin);

      if (closeButton_   .displayed) panelWidth -= b1 + 2;
      if (expandButton_  .displayed) panelWidth -= b1 + 2;
      if (collapseButton_.displayed) panelWidth -= b1 + 2;

      qDrawShadePanel(&painter, b + margin                , b + margin, panelSize, panelWidth, pal);
      qDrawShadePanel(&painter, b + margin + 1 + panelSize, b + margin, panelSize, panelWidth, pal);
    }

    // draw title
    QString text = windowTitle();

    if (text.length()) {
      painter.setPen(pal.color(QPalette::WindowText));

      QFontMetrics fm(font());

      int tw = fm.width(text);

      painter.save();

      if (decoration_.header_side == SideTop || decoration_.header_side == SideBottom)
        painter.fillRect(QRect(b, b, tw + 4, hh), bgBrush);
      else {
        painter.fillRect(QRect(b, height() - tw - b - 4, hh, tw + 4), bgBrush);

        painter.translate(0, height());
        painter.rotate(-90);
      }

      int ym = b + hh/2;

      painter.drawText(b + 2, b + ym - fm.height()/2 + fm.ascent(), text);

      painter.restore();
    }

    // draw header buttons
    painter.setPen(decoration_.border_color);

    if (decoration_.header_side == SideTop || decoration_.header_side == SideBottom) {
      int x1 = width() - b + 2;
      int y1 = b + 2;

      drawHeaderButtonH(&painter, closeButton_   , x1, y1, b1);
      drawHeaderButtonH(&painter, expandButton_  , x1, y1, b1);
      drawHeaderButtonH(&painter, collapseButton_, x1, y1, b1);
    }
    else {
      int x1 = b - 2;
      int y1 = b + 2;

      drawHeaderButtonV(&painter, closeButton_   , x1, y1, b1);
      drawHeaderButtonV(&painter, expandButton_  , x1, y1, b1);
      drawHeaderButtonV(&painter, collapseButton_, x1, y1, b1);
    }
  }
}

void
CQWinWidget::
drawHeaderButtonH(QPainter *painter, HeaderButton &button, int &x1, int y1, int b1)
{
  if (! button.displayed) return;

  x1 -= b1 + 2;

  button.rect = QRect(x1, y1, b1, b1);

  button.draw(painter, this);
}

void
CQWinWidget::
drawHeaderButtonV(QPainter *painter, HeaderButton &button, int x1, int &y1, int b1)
{
  if (! button.displayed) return;

  y1 += b1 + 2;

  button.rect = QRect(x1, y1, b1, b1);

  button.draw(painter, this);
}

void
CQWinWidget::
moveEvent(QMoveEvent *)
{
  resizeEvent(0);
}

void
CQWinWidget::
resizeEvent(QResizeEvent *)
{
  int b = getBorder();

  int x = b;
  int y = b;
  int w = width () - 2*b;
  int h = height() - 2*b;

  if (decoration_.type & HeaderDecoration) {
    int hh = getHeaderHeight();

    if      (decoration_.header_side == SideTop)
      y += hh;
    else if (decoration_.header_side == SideLeft)
      x += hh;

    if (decoration_.header_side == SideTop || decoration_.header_side == SideBottom)
      h -= hh;
    else
      w -= hh;
  }

  if (child_) {
    child_->move  (x, y);
    child_->resize(w, h);

    emit geometryChanging();
  }
}

void
CQWinWidget::
mousePressEvent(QMouseEvent *event)
{
  pressed_ = true;

  if (edit_mode_ == EDIT_MODE_CLICK) {
    if (state_.moving || state_.resizing) {
      edit_mode_ = EDIT_MODE_DRAG;

      mouseReleaseEvent(event);

      releaseMouse();

      edit_mode_ = EDIT_MODE_CLICK;

      return;
    }
  }

  //-----

  state_.init_pos  = pos();
  state_.init_size = size();
  state_.press_pos = event->globalPos();

  if      (event->button() == Qt::LeftButton) {
    state_.moving   = false;
    state_.resizing = false;

    QPoint p = event->pos();

    if (decoration_.type & HeaderDecoration) {
      closeButton_   .updateActive(p, pressed_);
      expandButton_  .updateActive(p, pressed_);
      collapseButton_.updateActive(p, pressed_);

      update();
    }

    if (ops_ & MoveOp) {
      if (decoration_.type & HeaderDecoration) {
        if (decoration_.header_rect.contains(p)) {
          if (! closeButton_.active && ! expandButton_.active && ! collapseButton_.active)
            state_.moving = true;
        }
      }
      else
        state_.moving = true;
    }

    if ((ops_ & ResizeOp) && ! state_.moving) {
      int b = getBorder() + 2;

      state_.resize_l = (event->x() <= b);
      state_.resize_t = (event->y() <= b);
      state_.resize_r = (event->x() >= width () - b - 1);
      state_.resize_b = (event->y() >= height() - b - 1);

      if (state_.resize_l || state_.resize_t || state_.resize_r || state_.resize_b)
        state_.resizing = true;
    }

    if (ops_ & RaiseOp)
      raise();

    if (edit_mode_ == EDIT_MODE_CLICK)
      grabMouse();
  }
  else if (event->button() == Qt::MidButton) {
    state_.moving   = false;
    state_.resizing = false;

    if (ops_ & MoveOp)
      state_.moving = true;

    if (ops_ & RaiseOp)
      raise();

    if (edit_mode_ == EDIT_MODE_CLICK)
      grabMouse();
  }
}

void
CQWinWidget::
mouseMoveEvent(QMouseEvent *event)
{
  if      (state_.moving) {
    QPoint d = event->globalPos() - state_.press_pos;

    QPoint p = state_.init_pos + d;

    if (checkMove(p))
      move(p);
  }
  else if (state_.resizing) {
    int dx = 0; int dy = 0;
    int dw = 0; int dh = 0;

    if (state_.resize_l) {
      dx = event->globalPos().x() - state_.press_pos.x();
      dw = -dx;
    }

    if (state_.resize_r)
      dw = event->globalPos().x() - state_.press_pos.x();

    if (state_.resize_t) {
      dy = event->globalPos().y() - state_.press_pos.y();
      dh = -dy;
    }

    if (state_.resize_b)
      dh = event->globalPos().y() - state_.press_pos.y();

    move  (pos () + QPoint(dx, dy));
    resize(size() + QSize (dw, dh));

    state_.press_pos = event->globalPos();
  }

  //------

  setCursor(select_bits, selectmask_bits, 2, 2);

  QPoint p = event->pos();

  if (decoration_.type & HeaderDecoration) {
    bool insideClose    = closeButton_   .rect.contains(p);
    bool insideExpand   = expandButton_  .rect.contains(p);
    bool insideCollapse = collapseButton_.rect.contains(p);

    bool changed = (insideClose    != closeButton_   .active ||
                    insideExpand   != expandButton_  .active ||
                    insideCollapse != collapseButton_.active);

    closeButton_   .updateActive(p, pressed_);
    expandButton_  .updateActive(p, pressed_);
    collapseButton_.updateActive(p, pressed_);

    if (changed)
      update();
  }

  if (ops_ & MoveOp) {
    if (decoration_.type & HeaderDecoration) {
      if (decoration_.header_rect.contains(p)) {
        if (closeButton_.active || expandButton_.active || collapseButton_.active)
          setCursor(select_bits, selectmask_bits, 2, 2);
        else
          QWidget::setCursor(Qt::SizeAllCursor);
      }
    }
    else
      QWidget::setCursor(Qt::SizeAllCursor);
  }

  if (ops_ & ResizeOp) {
    int b = getBorder() + 2;

    bool resize_l = (event->x() <= b);
    bool resize_t = (event->y() <= b);
    bool resize_r = (event->x() >= width () - b - 1);
    bool resize_b = (event->y() >= height() - b - 1);

    if      (resize_l && resize_t) QWidget::setCursor(Qt::SizeFDiagCursor);
    else if (resize_l && resize_b) QWidget::setCursor(Qt::SizeBDiagCursor);
    else if (resize_r && resize_t) QWidget::setCursor(Qt::SizeBDiagCursor);
    else if (resize_r && resize_b) QWidget::setCursor(Qt::SizeFDiagCursor);
    else if (resize_l            ) QWidget::setCursor(Qt::SizeHorCursor);
    else if (resize_r            ) QWidget::setCursor(Qt::SizeHorCursor);
    else if (resize_t            ) QWidget::setCursor(Qt::SizeVerCursor);
    else if (resize_b            ) QWidget::setCursor(Qt::SizeVerCursor);
  }
}

void
CQWinWidget::
mouseReleaseEvent(QMouseEvent *event)
{
  pressed_ = false;

  if (edit_mode_ == EDIT_MODE_DRAG) {
    if (state_.moving || state_.resizing) {
      mouseMoveEvent(event);

      if (! checkGeometry(geometry())) {
        move  (state_.init_pos );
        resize(state_.init_size);
      }
      else {
        emit geometryChanged();
      }

      state_.moving   = false;
      state_.resizing = false;
    }
  }

  QPoint p = event->pos();

  if (decoration_.type & HeaderDecoration) {
    closeButton_   .updateActive(p, pressed_);
    expandButton_  .updateActive(p, pressed_);
    collapseButton_.updateActive(p, pressed_);

    update();

    if (closeButton_.active)
      closeSlot();

    if (expandButton_.active)
      expandSlot();

    if (collapseButton_.active)
      collapseSlot();
  }
}

void
CQWinWidget::
contextMenuEvent(QContextMenuEvent *event)
{
  emit showContextMenu(event->globalPos());
}

void
CQWinWidget::
enterEvent(QEvent *)
{
  active_ = true;

  update();
}

void
CQWinWidget::
leaveEvent(QEvent *)
{
  active_ = false;

  update();
}

int
CQWinWidget::
getBorder() const
{
  int b = (decoration_.type & BorderDecoration ? decoration_.border : 0);

  return b;
}

void
CQWinWidget::
closeSlot()
{
  emit closing();

  hide();

  emit closed();
}

void
CQWinWidget::
expandSlot()
{
  emit expand();
}

void
CQWinWidget::
collapseSlot()
{
  emit collapse();
}

void
CQWinWidget::
setCursor(const uchar *bits, const uchar *mask, int xo, int yo)
{
  QCursor cursor(QBitmap::fromData(QSize(16,16), bits),
                 QBitmap::fromData(QSize(16,16), mask),
                 xo, yo);

  QWidget::setCursor(cursor);
}

//------

void
CQWinWidget::HeaderButton::
draw(QPainter *painter, CQWinWidget *preview)
{
  QStyle *style = preview->style();

  QStyleOptionButton opt;

  opt.init(preview);

  opt.rect = rect;

  opt.state    |= QStyle::State_AutoRaise;
  opt.features |= QStyleOptionButton::Flat;

  if (! active || ! pressed)
    opt.state |= QStyle::State_Raised;

  if (active && pressed)
    opt.state |= QStyle::State_Sunken;

  style->drawPrimitive(QStyle::PE_PanelButtonTool, &opt, painter, preview);

  int shiftHorizontal = (opt.state & QStyle::State_Sunken ?
                         style->pixelMetric(QStyle::PM_ButtonShiftHorizontal, &opt, preview) : 0);
  int shiftVertical   = (opt.state & QStyle::State_Sunken ?
                         style->pixelMetric(QStyle::PM_ButtonShiftVertical  , &opt, preview) : 0);

  QRect r = rect;

  r.adjust(2, 2, -2, -2);

  r.translate(shiftHorizontal, shiftVertical);

  QPixmap pm = icon.pixmap(r.size(),
                           active  ? QIcon::Active : QIcon::Normal,
                           pressed ? QIcon::On     : QIcon::Off);

  style->drawItemPixmap(painter, r, Qt::AlignCenter, pm);
}

//---------------

CQWinWidgetBase::
CQWinWidgetBase(QWidget *parent)
{
  widget_ = new CQWinWidget(parent);
}

CQWinWidgetBase::
~CQWinWidgetBase()
{
}

void
CQWinWidgetBase::
setEditMode(CQWinWidget::EditMode edit_mode)
{
  widget_->setEditMode(edit_mode);
}

//---------------

CQWinTextEdit::
CQWinTextEdit(QWidget *parent, const char *name) :
 CQWinWidgetBase(parent)
{
  QTextEdit::setObjectName(name);

  widget_->setChild(this);

  connect(this, SIGNAL(textChanged()), this, SIGNAL(valueChanged()));

  connect(widget_, SIGNAL(geometryChanging()), this, SIGNAL(geometryChanging()));
  connect(widget_, SIGNAL(geometryChanged()), this, SIGNAL(geometryChanged()));
}

//---------------

CQWinImage::
CQWinImage(QWidget *parent, const char *name) :
 CQWinWidgetBase(parent)
{
  QLabel::setObjectName(name);

  widget_->setChild(this);

  widget_->setDecorationType(CQWinWidget::BorderDecoration);

  widget_->setResizable(false);

  connect(widget_, SIGNAL(geometryChanging()), this, SIGNAL(geometryChanging()));
  connect(widget_, SIGNAL(geometryChanged()), this, SIGNAL(geometryChanged()));

  //------

  menu_ = new QMenu;

  QAction *loadAction = menu_->addAction("Load Image");

  connect(loadAction, SIGNAL(triggered()), this, SLOT(loadImage()));
}

void
CQWinImage::
setImageName(const QString &fileName)
{
  fileName_ = fileName;

  QImage image(fileName);

  setImage(image);
}

void
CQWinImage::
setImage(const QImage &image)
{
  image_ = image;

  QPixmap pixmap = QPixmap::fromImage(image_);

  if (! pixmap.isNull()) {
    setPixmap(pixmap);

    widget_->setResizable(true);

    widget_->setChildSize(pixmap.size());

    widget_->setResizable(false);
  }
}

void
CQWinImage::
contextMenuEvent(QContextMenuEvent *event)
{
  menu_->exec(event->globalPos());
}

void
CQWinImage::
loadImage()
{
  QString fileName = QFileDialog::getOpenFileName(this,
    "Open Image", "", "Image Files (*.png *.gif *.jpg)");

  if (fileName.length())
    setImageName(fileName);

  emit valueChanged();
}

//---------------

CQWinTable::
CQWinTable(QWidget *parent, const char *name) :
 CQWinWidgetBase(parent)
{
  QTableWidget::setObjectName(name);

  widget_->setChild(this);

  connect(widget_, SIGNAL(geometryChanging()), this, SIGNAL(geometryChanging()));
  connect(widget_, SIGNAL(geometryChanged()), this, SIGNAL(geometryChanged()));
}

//---------------

CQWinTree::
CQWinTree(QWidget *parent, const char *name) :
 CQWinWidgetBase(parent)
{
  QTreeWidget::setObjectName(name);

  widget_->setChild(this);

  connect(widget_, SIGNAL(geometryChanging()), this, SIGNAL(geometryChanging()));
  connect(widget_, SIGNAL(geometryChanged()), this, SIGNAL(geometryChanged()));
}
