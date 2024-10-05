#ifndef CQIllustratorPointSelectMode_H
#define CQIllustratorPointSelectMode_H

#include <CQIllustratorMode.h>
#include <CQIllustratorToolbar.h>

class CQIllustratorShape;
class CQIllustratorControlPointHandle;
class CQIllustratorPointSelectMode;
class CQIllustratorPointSelectSizer;

class CQIllustratorPointSelectToolbar : public CQIllustratorToolbar {
  Q_OBJECT

 public:
  CQIllustratorPointSelectToolbar(CQIllustratorPointSelectMode *selectMode);

  const char *getTitle() const override { return "Point Select"; }

  QIcon getIcon() override;

  void addWidgets() override;

  void setSelectedShape(const CQIllustratorShape *shape) override;

 private:
  CQIllustratorPointSelectMode *mode_ { nullptr };
};

class CQIllustratorPointSelectSizer : public CQIllustratorModeSizer {
 public:
  CQIllustratorPointSelectSizer(CQIllustratorPointSelectMode *mode);

  void drawHandles(QPainter *painter, const CQIllustratorShape *shape) override;

  void updateHandles(uint numHandles);
};

class CQIllustratorPointSelectMode : public CQIllustratorMode {
 public:
  CQIllustratorPointSelectMode(CQIllustrator *illustrator);

  CQIllustratorPointSelectToolbar *createToolbar() override;

  CQIllustratorPointSelectSizer *createSizer() override;

  const char *getTitle() const override { return "Select Shape Point"; }

  CQMenuItem *createMenuItem(CQMenu *menu) override;

  bool isSelectMode() const override { return true; }

  CQIllustratorPointSelectToolbar *getToolbar() const override { return toolbar_; }

  void handleMouseRelease(const MouseEvent &e) override;
  void handleMouseDrag   (const MouseEvent &e) override;

  void drawOverlay(CQIllustratorShapeDrawer *drawer) override;

  QCursor getCursor() const override;

 private:
  CQIllustratorPointSelectToolbar *toolbar_ { nullptr };
  CQIllustratorPointSelectSizer   *sizer_ { nullptr };
};

#endif
