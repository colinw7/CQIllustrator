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
  CQIllustratorCreateRectMode *mode_ { 0 };
  CQPointEdit                 *posEdit_ { 0 };
  CQRealEdit                  *widthEdit_ { 0 };
  CQRealEdit                  *heightEdit_ { 0 };
  CQRealEdit                  *xRadEdit_ { 0 };
  CQRealEdit                  *yRadEdit_ { 0 };
};

//------

class CQIllustratorCreateRectSizer : public CQIllustratorModeSizer {
 public:
  CQIllustratorCreateRectSizer(CQIllustratorCreateRectMode *mode);

  void drawHandles(QPainter *painter, const CQIllustratorShape *shape);

 private:
  CQIllustratorControlPointHandle *ll_handle_ { 0 };
  CQIllustratorControlPointHandle *ur_handle_ { 0 };
  CQIllustratorControlPointHandle *rx_handle_ { 0 };
  CQIllustratorControlPointHandle *ry_handle_ { 0 };
};

//------

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
  CQIllustratorCreateRectToolbar *toolbar_ { 0 };
  CQIllustratorCreateRectSizer   *sizer_ { 0 };
  bool                            equalSize_ { false };
};

#endif
