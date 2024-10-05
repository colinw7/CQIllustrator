#ifndef CQIllustratorSetAnchorObjectMode_H
#define CQIllustratorSetAnchorObjectMode_H

#include <CQIllustratorMode.h>

class CQIllustratorSetAnchorObjectMode : public CQIllustratorMode {
 public:
  CQIllustratorSetAnchorObjectMode(CQIllustrator *illustrator);

  const char *getTitle() const override { return "Set Anchor Object"  ; }

  CQIllustratorToolbar *createToolbar() override { return nullptr; }

  CQMenuItem *createMenuItem(CQMenu *menu) override;

  CQIllustratorToolbar *getToolbar() const override { return nullptr; }

  void handleMousePress  (const MouseEvent &e) override;
  void handleMouseRelease(const MouseEvent &e) override;
  void handleMouseDrag   (const MouseEvent &e) override;
  void handleMouseMove   (const MouseEvent &e) override;

  QCursor getCursor() const override;

 private:
};

#endif
