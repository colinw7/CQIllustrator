#include <CQIllustratorShapeStroke.h>
#include <CQIllustratorShapeDrawer.h>

void
CQIllustratorShapeStroke::
draw(const CQIllustratorShape *shape, CQIllustratorShapeDrawer *drawer) const
{
  if (! isStroked())
    return;

  drawer->setStroke(shape, *this);

  drawer->pathStroke();
}
