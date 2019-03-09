#ifndef CQIllustratorSetLGradientMode_H
#define CQIllustratorSetLGradientMode_H

#include <CQIllustratorMode.h>
#include <CQIllustratorToolbar.h>
#include <CQIllustrator.h>
#include <CAutoPtr.h>

class QRadioButton;
class CQRealEdit;
class CQIllustrator;
class CQIllustratorControlPointHandle;
class CQIllustratorSetLGradientMode;

class CQIllustratorLGradSizer : public CQIllustratorModeSizer {
 public:
  CQIllustratorLGradSizer(CQIllustratorSetLGradientMode *mode);

  void drawHandles(QPainter *painter, const CQIllustratorShape *shape);

 private:
  CQIllustratorControlPointHandle *start_handle_;
  CQIllustratorControlPointHandle *end_handle_;
};

class CQIllustratorSetLGradientToolbar : public CQIllustratorToolbar {
  Q_OBJECT

 public:
  CQIllustratorSetLGradientToolbar(CQIllustratorSetLGradientMode *mode);

  const char *getTitle() const { return "Linear Gradient"; }

  QIcon getIcon();

  void addWidgets();

  void setSelectedShape(const CQIllustratorShape *shape);

 private slots:
  void modeChangedSlot();

  void updateShape();

 private:
  CQIllustratorSetLGradientMode *mode_;
  QRadioButton                  *createRadio_;
  QRadioButton                  *editRadio_;
  CQRealEdit                    *startXEdit_;
  CQRealEdit                    *startYEdit_;
  CQRealEdit                    *endXEdit_;
  CQRealEdit                    *endYEdit_;
};

class CQIllustratorSetLGradientMode : public CQIllustratorMode {
 public:
  CQIllustratorSetLGradientMode(CQIllustrator *illustrator);

  const char *getTitle() const { return "Set Linear Gradient"; }

  CQIllustratorSetLGradientToolbar *createToolbar();

  CQIllustratorLGradSizer *createSizer();

  CQMenuItem *createMenuItem(CQMenu *menu);

  CQIllustratorSetLGradientToolbar *getToolbar() const { return toolbar_; }

  CQIllustratorData::ChangeType getChangeType() const {
    return CQIllustratorData::ChangeType::FILL;
  }

  CQIllustratorShape::ControlType getControlType() const {
    return CQIllustratorShape::ControlType::LGRADIENT;
  }

  void handleMouseRelease(const MouseEvent &e);
  void handleMouseDrag   (const MouseEvent &e);

  void drawOverlay(CQIllustratorShapeDrawer *drawer);

  QCursor getCursor() const;

 private:
  CQIllustratorSetLGradientToolbar  *toolbar_;
  CAutoPtr<CQIllustratorLGradSizer>  sizer_;
};

#endif
