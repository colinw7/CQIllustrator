#include <CQIllustratorPanMode.h>
#include <CQIllustrator.h>

#include <CQMenu.h>

#include <xpm/pan.xpm>

CQIllustratorPanMode::
CQIllustratorPanMode(CQIllustrator *illustrator) :
 CQIllustratorMode(illustrator, CQIllustrator::MODE_PAN)
{
}

CQIllustratorPanToolbar *
CQIllustratorPanMode::
createToolbar()
{
  toolbar_ = new CQIllustratorPanToolbar(this);

  return toolbar_;
}

CQMenuItem *
CQIllustratorPanMode::
createMenuItem(CQMenu *menu)
{
  menuItem_ = new CQMenuItem(menu, "&Pan", CQMenuItem::CHECKABLE);

  menuItem_->setStatusTip("Pan Display");
  menuItem_->setXPMIcon(pan_data);

  connect(menuItem_->getAction(), SIGNAL(toggled(bool)), this, SLOT(menuItemSlot()));

  return menuItem_;
}

void
CQIllustratorPanMode::
handleMousePress(const MouseEvent &)
{
}

void
CQIllustratorPanMode::
handleMouseRelease(const MouseEvent &)
{
  illustrator_->redraw();
}

void
CQIllustratorPanMode::
handleMouseDrag(const MouseEvent &)
{
  QPointF p1 = illustrator_->getITransform().map(curr_ppos_);
  QPointF p2 = illustrator_->getITransform().map(prev_ppos_);

  double dx = p1.x() - p2.x();
  double dy = p1.y() - p2.y();

  const CBBox2D &bbox = illustrator_->getBBox();

  double x    = bbox.getLL().x - dx;
  double y    = bbox.getLL().y - dy;
  double size = bbox.getWidth ();

  CBBox2D bbox1(CPoint2D(x, y), CPoint2D(x + size, y + size));

  illustrator_->setBBox(bbox1);
}

void
CQIllustratorPanMode::
handleMouseMove(const MouseEvent &)
{
}

QCursor
CQIllustratorPanMode::
getCursor() const
{
  return Qt::OpenHandCursor;
}

//------------

CQIllustratorPanToolbar::
CQIllustratorPanToolbar(CQIllustratorPanMode *mode) :
 CQIllustratorToolbar(mode), panMode_(mode)
{
}

QIcon
CQIllustratorPanToolbar::
getIcon()
{
  return QIcon(QPixmap(pan_data));
}
