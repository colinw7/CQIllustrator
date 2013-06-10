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

  const char *getTitle() const { return "Point Select"; }

  QIcon getIcon();

  void addWidgets();

  void setSelectedShape(const CQIllustratorShape *shape);

 private:
  CQIllustratorPointSelectMode *mode_;
};

class CQIllustratorPointSelectSizer : public CQIllustratorModeSizer {
 public:
  CQIllustratorPointSelectSizer(CQIllustratorPointSelectMode *mode);

  void drawHandles(QPainter *painter, const CQIllustratorShape *shape);

  void updateHandles(uint numHandles);
};

class CQIllustratorPointSelectMode : public CQIllustratorMode {
 public:
  CQIllustratorPointSelectMode(CQIllustrator *illustrator);

  CQIllustratorPointSelectToolbar *createToolbar();

  CQIllustratorPointSelectSizer *createSizer();

  const char *getTitle() const { return "Select Shape Point"; }

  CQMenuItem *createMenuItem(CQMenu *menu);

  bool isSelectMode() const { return true; }

  CQIllustratorPointSelectToolbar *getToolbar() const { return toolbar_; }

  void handleMouseRelease(const MouseEvent &e);
  void handleMouseDrag   (const MouseEvent &e);

  void drawOverlay(CQIllustratorShapeDrawer *drawer);

  QCursor getCursor() const;

 private:
  CQIllustratorPointSelectToolbar *toolbar_;
  CQIllustratorPointSelectSizer   *sizer_;
};

#endif
