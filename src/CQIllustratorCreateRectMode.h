#ifndef CQIllustratorCreateRectMode_H
#define CQIllustratorCreateRectMode_H

#include <CQIllustratorMode.h>
#include <CQIllustratorToolbar.h>

class CQPointEdit;
class CQRealEdit;
class CQIllustrator;
class CQIllustratorControlPointHandle;
class CQIllustratorCreateRectMode;
class CQIllustratorCreateRectSizer;

class CQIllustratorCreateRectToolbar : public CQIllustratorToolbar {
  Q_OBJECT

 public:
  CQIllustratorCreateRectToolbar(CQIllustratorCreateRectMode *mode);

  const char *getTitle() const { return "Create Rect"; }

  QIcon getIcon();

  void addWidgets();

  void setSelectedShape(const CQIllustratorShape *shape);

  void setSize(const CBBox2D &bbox);

 private slots:
  void updateShape();

 private:
  CQIllustratorCreateRectMode *mode_;
  CQPointEdit                 *posEdit_;
  CQRealEdit                  *widthEdit_;
  CQRealEdit                  *heightEdit_;
  CQRealEdit                  *xRadEdit_;
  CQRealEdit                  *yRadEdit_;
};

class CQIllustratorCreateRectSizer : public CQIllustratorModeSizer {
 public:
  CQIllustratorCreateRectSizer(CQIllustratorCreateRectMode *mode);

  void drawHandles(QPainter *painter, const CQIllustratorShape *shape);

 private:
  CQIllustratorControlPointHandle *ll_handle_;
  CQIllustratorControlPointHandle *ur_handle_;
  CQIllustratorControlPointHandle *rx_handle_;
  CQIllustratorControlPointHandle *ry_handle_;
};

class CQIllustratorCreateRectMode : public CQIllustratorMode {
  Q_OBJECT

 public:
  CQIllustratorCreateRectMode(CQIllustrator *illustrator);

  const char *getTitle() const { return "Create Rectangle"; }

  CQIllustratorCreateRectToolbar *getToolbar() { return toolbar_; }

  CQIllustratorCreateRectToolbar *createToolbar();

  CQIllustratorCreateRectSizer *createSizer();

  CQMenuItem *createMenuItem(CQMenu *menu);

  bool isCreateMode() const { return true; }

  CQIllustratorCreateRectToolbar *getToolbar() const { return toolbar_; }

  void handleMouseRelease(const MouseEvent &e);
  void handleMouseDrag   (const MouseEvent &e);

  void drawOverlay(CQIllustratorShapeDrawer *drawer);

 private:
  CQIllustratorCreateRectToolbar *toolbar_;
  CQIllustratorCreateRectSizer   *sizer_;
  bool                            equalSize_;
};

#endif
