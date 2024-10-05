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

  void drawHandles(QPainter *painter, const CQIllustratorShape *shape) override;

 private:
  CQIllustratorControlPointHandle *start_handle_ { nullptr };
  CQIllustratorControlPointHandle *end_handle_ { nullptr };
};

class CQIllustratorSetLGradientToolbar : public CQIllustratorToolbar {
  Q_OBJECT

 public:
  CQIllustratorSetLGradientToolbar(CQIllustratorSetLGradientMode *mode);

  const char *getTitle() const override { return "Linear Gradient"; }

  QIcon getIcon() override;

  void addWidgets() override;

  void setSelectedShape(const CQIllustratorShape *shape) override;

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

  const char *getTitle() const override { return "Set Linear Gradient"; }

  CQIllustratorSetLGradientToolbar *createToolbar() override;

  CQIllustratorLGradSizer *createSizer() override;

  CQMenuItem *createMenuItem(CQMenu *menu) override;

  CQIllustratorSetLGradientToolbar *getToolbar() const override { return toolbar_; }

  CQIllustratorData::ChangeType getChangeType() const override {
    return CQIllustratorData::ChangeType::FILL;
  }

  CQIllustratorShape::ControlType getControlType() const override {
    return CQIllustratorShape::ControlType::LGRADIENT;
  }

  void handleMouseRelease(const MouseEvent &e) override;
  void handleMouseDrag   (const MouseEvent &e) override;

  void drawOverlay(CQIllustratorShapeDrawer *drawer) override;

  QCursor getCursor() const override;

 private:
  CQIllustratorSetLGradientToolbar  *toolbar_ { nullptr };
  CAutoPtr<CQIllustratorLGradSizer>  sizer_;
};

#endif
