#ifndef CQIllustratorSetAnchorPositionMode_H
#define CQIllustratorSetAnchorPositionMode_H

#include <CQIllustratorMode.h>

class CQIllustratorSetAnchorPositionMode : public CQIllustratorMode {
 public:
  CQIllustratorSetAnchorPositionMode(CQIllustrator *illustrator);

  const char *getTitle() const override { return "Set Anchor Position"; }

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
