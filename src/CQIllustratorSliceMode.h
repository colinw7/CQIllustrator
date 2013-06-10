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

  const char *getTitle() const { return "Slice"; }

  QIcon getIcon();

  void addWidgets();

 private:
  CQIllustratorSliceMode *mode_;
};

class CQIllustratorSliceMode : public CQIllustratorMode {
 public:
  CQIllustratorSliceMode(CQIllustrator *illustrator);

  const char *getTitle() const { return "Slice"; }

  CQIllustratorSliceToolbar *createToolbar();

  CQMenuItem *createMenuItem(CQMenu *menu);

  CQIllustratorSliceToolbar *getToolbar() const { return toolbar_; }

  void handleMousePress  (const MouseEvent &e);
  void handleMouseRelease(const MouseEvent &e);
  void handleMouseDrag   (const MouseEvent &e);
  void handleMouseMove   (const MouseEvent &e);

  void drawOverlay(CQIllustratorShapeDrawer *drawer);

 private:
  CQIllustratorSliceToolbar *toolbar_;
};

#endif
