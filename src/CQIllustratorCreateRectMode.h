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

  const char *getTitle() const override { return "Create Rect"; }

  QIcon getIcon() override;

  void addWidgets() override;

  void setSelectedShape(const CQIllustratorShape *shape) override;

  void setSize(const CBBox2D &bbox);

 private slots:
  void updateShape();

 private:
  CQIllustratorCreateRectMode *mode_ { nullptr };
  CQPointEdit                 *posEdit_ { nullptr };
  CQRealEdit                  *widthEdit_ { nullptr };
  CQRealEdit                  *heightEdit_ { nullptr };
  CQRealEdit                  *xRadEdit_ { nullptr };
  CQRealEdit                  *yRadEdit_ { nullptr };
};

//------

class CQIllustratorCreateRectSizer : public CQIllustratorModeSizer {
 public:
  CQIllustratorCreateRectSizer(CQIllustratorCreateRectMode *mode);

  void drawHandles(QPainter *painter, const CQIllustratorShape *shape) override;

 private:
  CQIllustratorControlPointHandle *ll_handle_ { nullptr };
  CQIllustratorControlPointHandle *ur_handle_ { nullptr };
  CQIllustratorControlPointHandle *rx_handle_ { nullptr };
  CQIllustratorControlPointHandle *ry_handle_ { nullptr };
};

//------

class CQIllustratorCreateRectMode : public CQIllustratorMode {
  Q_OBJECT

 public:
  CQIllustratorCreateRectMode(CQIllustrator *illustrator);

  const char *getTitle() const override { return "Create Rectangle"; }

  CQIllustratorCreateRectToolbar *getToolbar() { return toolbar_; }

  CQIllustratorCreateRectToolbar *createToolbar() override;

  CQIllustratorCreateRectSizer *createSizer() override;

  CQMenuItem *createMenuItem(CQMenu *menu) override;

  bool isCreateMode() const override { return true; }

  CQIllustratorCreateRectToolbar *getToolbar() const override { return toolbar_; }

  void handleMouseRelease(const MouseEvent &e) override;
  void handleMouseDrag   (const MouseEvent &e) override;

  void drawOverlay(CQIllustratorShapeDrawer *drawer) override;

 private:
  CQIllustratorCreateRectToolbar *toolbar_ { 0 };
  CQIllustratorCreateRectSizer   *sizer_ { 0 };
  bool                            equalSize_ { false };
};

#endif
