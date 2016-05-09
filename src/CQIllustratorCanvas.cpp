#include <CQIllustratorCanvas.h>
#include <CQIllustratorInfo.h>
#include <CQIllustrator.h>
#include <CQUtil.h>

#include <QPainter>
#include <QResizeEvent>
#include <QTimer>
#include <QMenu>
#include <QAction>

enum { TIMEOUT = 500 };

CQIllustratorCanvas::
CQIllustratorCanvas(CQIllustrator *illustrator) :
 illustrator_(illustrator)
{
  setFocusPolicy(Qt::StrongFocus);

  setMouseTracking(true);

  infoLabel_ = new CQIllustratorInfo(this);

  painter_ = new QPainter;

  timer_ = new QTimer;

  connect(timer_, SIGNAL(timeout()), this, SLOT(timeoutSlot()));
}

void
CQIllustratorCanvas::
mousePressEvent(QMouseEvent *event)
{
  if  (event->button() == Qt::RightButton) {
    showMenu(event->pos());

    return;
  }

  pressed_ = true;

  illustrator_->mousePress(CQIllustrator::MouseEvent(event, itransform_));

  timer_->start(TIMEOUT);
}

void
CQIllustratorCanvas::
mouseMoveEvent(QMouseEvent *event)
{
  if (pressed_)
    illustrator_->mouseDrag(CQIllustrator::MouseEvent(event, itransform_));
  else
    illustrator_->mouseMove(CQIllustrator::MouseEvent(event, itransform_));

  updateStatus();

  timer_->start(TIMEOUT);
}

void
CQIllustratorCanvas::
mouseReleaseEvent(QMouseEvent *event)
{
  pressed_ = false;

  illustrator_->mouseRelease(CQIllustrator::MouseEvent(event, itransform_));

  timer_->start(TIMEOUT);
}

void
CQIllustratorCanvas::
keyPressEvent(QKeyEvent *event)
{
  QPoint pos = mapFromGlobal(QCursor::pos());

  illustrator_->keyPress(CQIllustrator::KeyEvent(event, pos, itransform_));

  timer_->start(TIMEOUT);
}

void
CQIllustratorCanvas::
wheelEvent(QWheelEvent *event)
{
  CBBox2D bbox = illustrator_->getBBox();

  int num = abs(event->delta())/15;

  double dx, dy;

  if (event->modifiers() & Qt::ControlModifier)
    dx = num*bbox.getHeight()/64;
  else
    dy = num*bbox.getHeight()/64;

  if (event->delta() > 0)
    bbox.moveBy(CVector2D( dx,  dy));
  else
    bbox.moveBy(CVector2D(-dx, -dy));

  illustrator_->setBBox(bbox);
}

void
CQIllustratorCanvas::
paintEvent(QPaintEvent *)
{
  painter_->begin(this);

  const CBBox2D &bbox = illustrator_->getBBox();

  range_.setEqualScale(true);

  if (illustrator_->getFlipY())
    range_.setPixelRange(0, height() - 1, width() - 1, 0);
  else
    range_.setPixelRange(0, 0, width() - 1, height() - 1);

  range_.setWindowRange(bbox.getXMin(), bbox.getYMin(), bbox.getXMax(), bbox.getYMax());

  transform_  = CQUtil::toQTransform(getMatrix());
  itransform_ = transform_.inverted();

  //painter_->setWorldTransform(transform_);
  painter_->setTransform(transform_);

  illustrator_->draw(painter_);

  painter_->end();
}

void
CQIllustratorCanvas::
resizeEvent(QResizeEvent *e)
{
  illustrator_->resizeCanvas(e->size().width(), e->size().height());
}

void
CQIllustratorCanvas::
showMenu(const QPoint &p)
{
  if (popupMenu_ == 0) {
    popupMenu_ = new QMenu(this);

    QAction *applyAction  = new QAction("Apply" , 0);
    QAction *cancelAction = new QAction("Cancel", 0);

    popupMenu_->addAction(applyAction);
    popupMenu_->addAction(cancelAction);
  }

  popupMenu_->popup(mapToGlobal(p));
}

void
CQIllustratorCanvas::
updateStatus()
{
  QPoint pos = mapFromGlobal(QCursor::pos());

  QPointF wpos = itransform_.map(QPointF(pos));

  CQIllustratorShape *shape = illustrator_->getObjectAt(CQUtil::fromQPoint(wpos));

  if (shape)
    illustrator_->setDeltaLabel("Shape", shape->getName().c_str());
  else
    illustrator_->setDeltaLabel("Shape", "");
}

void
CQIllustratorCanvas::
timeoutSlot()
{
  QPoint pos = mapFromGlobal(QCursor::pos());

  QPointF wpos = itransform_.map(QPointF(pos));

  CQIllustratorShape *shape = illustrator_->getObjectAt(CQUtil::fromQPoint(wpos));

  if (shape != infoShape_) {
    infoShape_ = shape;

    if (infoShape_) {
      infoLabel_->setShape(infoShape_);

      infoLabel_->show();
    }
    else
      infoLabel_->hide();
  }

  timer_->start(TIMEOUT);
}
