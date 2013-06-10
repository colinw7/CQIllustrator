#include <CQIllustratorSetAnchorPositionMode.h>
#include <CQIllustratorAlignMode.h>

#include <CQMenu.h>
#include <CQIllustrator.h>

#include "xpm/select.xpm"

CQIllustratorSetAnchorPositionMode::
CQIllustratorSetAnchorPositionMode(CQIllustrator *illustrator) :
 CQIllustratorMode(illustrator->getMode(CQIllustrator::MODE_ALIGN),
                   CQIllustrator::MODE_ANCHOR_POSITION)
{
}

CQMenuItem *
CQIllustratorSetAnchorPositionMode::
createMenuItem(CQMenu *menu)
{
  menuItem_ = new CQMenuItem(menu, "&Set Anchor Position", CQMenuItem::CHECKABLE);

  menuItem_->setStatusTip("Set Anchor Position");
  menuItem_->setXPMIcon(select_data);

  connect(menuItem_->getAction(), SIGNAL(toggled(bool)), this, SLOT(menuItemSlot()));

  return menuItem_;
}

void
CQIllustratorSetAnchorPositionMode::
handleMousePress(const MouseEvent &)
{
  illustrator_->getAlignToolbar()->setAnchorPosition(press_wpos_);
}

void
CQIllustratorSetAnchorPositionMode::
handleMouseRelease(const MouseEvent &e)
{
  illustrator_->getAlignToolbar()->setAnchorPosition(e.window);

  illustrator_->cancelAnchorSlot();

  illustrator_->redrawOverlay();
}

void
CQIllustratorSetAnchorPositionMode::
handleMouseDrag(const MouseEvent &e)
{
  illustrator_->getAlignToolbar()->setAnchorPosition(e.window);

  illustrator_->redrawOverlay();
}

void
CQIllustratorSetAnchorPositionMode::
handleMouseMove(const MouseEvent &)
{
}

QCursor
CQIllustratorSetAnchorPositionMode::
getCursor() const
{
  return Qt::CrossCursor;
}
