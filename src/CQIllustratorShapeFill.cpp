#include <CQIllustratorShapeFill.h>
#include <CQIllustratorShapeDrawer.h>

void
CQIllustratorShapeFill::
draw(const CQIllustratorShape *shape, CQIllustratorShapeDrawer *drawer) const
{
  if (! isFilled())
    return;

  drawer->setFill(shape, *this);

  drawer->pathFill(rule_);
}
