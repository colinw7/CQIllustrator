#ifndef CQIllustratorSetAnchorObjectMode_H
#define CQIllustratorSetAnchorObjectMode_H

#include <CQIllustratorMode.h>

class CQIllustratorSetAnchorObjectMode : public CQIllustratorMode {
 public:
  CQIllustratorSetAnchorObjectMode(CQIllustrator *illustrator);

  const char *getTitle() const { return "Set Anchor Object"  ; }

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
