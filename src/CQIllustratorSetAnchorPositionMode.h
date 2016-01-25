#ifndef CQIllustratorSetAnchorPositionMode_H
#define CQIllustratorSetAnchorPositionMode_H

#include <CQIllustratorMode.h>

class CQIllustratorSetAnchorPositionMode : public CQIllustratorMode {
 public:
  CQIllustratorSetAnchorPositionMode(CQIllustrator *illustrator);

  const char *getTitle() const { return "Set Anchor Position"; }

  CQIllustratorToolbar *createToolbar() { return 0; }

  CQMenuItem *createMenuItem(CQMenu *menu);

  CQIllustratorToolbar *getToolbar() const { return 0; }

  void handleMousePress  (const MouseEvent &e);
  void handleMouseRelease(const MouseEvent &e);
  void handleMouseDrag   (const MouseEvent &e);
  void handleMouseMove   (const MouseEvent &e);

  QCursor getCursor() const;

 private:
};

#endif
