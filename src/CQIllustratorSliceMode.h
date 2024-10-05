#ifndef CQIllustratorSliceMode_H
#define CQIllustratorSliceMode_H

#include <CQIllustratorMode.h>
#include <CQIllustratorToolbar.h>

class CQIllustrator;
class CQIllustratorSliceMode;

class CQIllustratorSliceToolbar : public CQIllustratorToolbar {
  Q_OBJECT

 public:
  CQIllustratorSliceToolbar(CQIllustratorSliceMode *mode);

  const char *getTitle() const override { return "Slice"; }

  QIcon getIcon() override;

  void addWidgets() override;

 private:
  CQIllustratorSliceMode *mode_ { nullptr };
};

class CQIllustratorSliceMode : public CQIllustratorMode {
 public:
  CQIllustratorSliceMode(CQIllustrator *illustrator);

  const char *getTitle() const override { return "Slice"; }

  CQIllustratorSliceToolbar *createToolbar() override;

  CQMenuItem *createMenuItem(CQMenu *menu) override;

  CQIllustratorSliceToolbar *getToolbar() const override { return toolbar_; }

  void handleMousePress  (const MouseEvent &e) override;
  void handleMouseRelease(const MouseEvent &e) override;
  void handleMouseDrag   (const MouseEvent &e) override;
  void handleMouseMove   (const MouseEvent &e) override;

  void drawOverlay(CQIllustratorShapeDrawer *drawer) override;

 private:
  CQIllustratorSliceToolbar *toolbar_ { nullptr };
};

#endif
