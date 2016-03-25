#include <CQIllustratorSetAnchorObjectMode.h>
#include <CQIllustratorAlignMode.h>

#include <CQUtil.h>
#include <CQMenu.h>
#include <CQIllustrator.h>

#include <svg/select_svg.h>

CQIllustratorSetAnchorObjectMode::
CQIllustratorSetAnchorObjectMode(CQIllustrator *illustrator) :
 CQIllustratorMode(illustrator->getMode(CQIllustrator::MODE_ALIGN),
                   CQIllustrator::MODE_ANCHOR_OBJECT)
{
}

CQMenuItem *
CQIllustratorSetAnchorObjectMode::
createMenuItem(CQMenu *menu)
{
  menuItem_ = new CQMenuItem(menu,
    CQPixmapCacheInst->getIcon("SELECT"), "&Set Anchor Object", CQMenuItem::CHECKABLE);

  menuItem_->setStatusTip("Set Anchor Object");

  connect(menuItem_->getAction(), SIGNAL(toggled(bool)), this, SLOT(menuItemSlot()));

  return menuItem_;
}

void
CQIllustratorSetAnchorObjectMode::
handleMousePress(const MouseEvent &)
{
  CQIllustratorShape *shape = illustrator_->getObjectAt(CQUtil::fromQPoint(press_wpos_));

  illustrator_->getAlignToolbar()->setAnchorObject(shape->getName().c_str());
}

void
CQIllustratorSetAnchorObjectMode::
handleMouseRelease(const MouseEvent &)
{
  illustrator_->cancelAnchorSlot();

  illustrator_->redrawOverlay();
}

void
CQIllustratorSetAnchorObjectMode::
handleMouseDrag(const MouseEvent &)
{
}

void
CQIllustratorSetAnchorObjectMode::
handleMouseMove(const MouseEvent &)
{
}

QCursor
CQIllustratorSetAnchorObjectMode::
getCursor() const
{
  return Qt::ArrowCursor;
}
