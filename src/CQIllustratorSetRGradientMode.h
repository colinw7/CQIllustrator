#ifndef CQIllustratorSetRGradientMode_H
#define CQIllustratorSetRGradientMode_H

#include <CQIllustratorMode.h>
#include <CQIllustratorToolbar.h>
#include <CQIllustrator.h>
#include <CAutoPtr.h>

class CQRealEdit;
class CQIllustrator;
class CQIllustratorControlPointHandle;
class CQIllustratorSetRGradientMode;

class CQIllustratorRGradSizer : public CQIllustratorModeSizer {
 public:
  CQIllustratorRGradSizer(CQIllustratorSetRGradientMode *mode);

  void drawHandles(QPainter *painter, const CQIllustratorShape *shape) override;

 private:
  CQIllustratorControlPointHandle *center_handle_ { nullptr };
  CQIllustratorControlPointHandle *focus_handle_ { nullptr };
  CQIllustratorControlPointHandle *radius_handle_ { nullptr };
};

class CQIllustratorSetRGradientToolbar : public CQIllustratorToolbar {
  Q_OBJECT

 public:
  CQIllustratorSetRGradientToolbar(CQIllustratorSetRGradientMode *mode);

  const char *getTitle() const override { return "Radial Gradient"; }

  QIcon getIcon() override;

  void addWidgets() override;

  void setSelectedShape(const CQIllustratorShape *shape) override;

 private slots:
  void updateShape();

 private:
  CQIllustratorSetRGradientMode *mode_ { nullptr };
  CQRealEdit                    *centerXEdit_ { nullptr };
  CQRealEdit                    *centerYEdit_ { nullptr };
  CQRealEdit                    *focusXEdit_ { nullptr };
  CQRealEdit                    *focusYEdit_ { nullptr };
  CQRealEdit                    *radiusEdit_ { nullptr };
};

class CQIllustratorSetRGradientMode : public CQIllustratorMode {
 public:
  CQIllustratorSetRGradientMode(CQIllustrator *illustrator);

  const char *getTitle() const override { return "Set Radial Gradient"; }

  CQIllustratorSetRGradientToolbar *createToolbar() override;

  CQIllustratorRGradSizer *createSizer() override;

  CQMenuItem *createMenuItem(CQMenu *menu) override;

  CQIllustratorSetRGradientToolbar *getToolbar() const override { return toolbar_; }

  CQIllustratorData::ChangeType getChangeType() const override {
    return CQIllustratorData::ChangeType::FILL;
  }

  CQIllustratorShape::ControlType getControlType() const override {
    return CQIllustratorShape::ControlType::RGRADIENT;
  }

  void handleMouseRelease(const MouseEvent &e) override;
  void handleMouseDrag   (const MouseEvent &e) override;

  void drawOverlay(CQIllustratorShapeDrawer *drawer) override;

  QCursor getCursor() const override;

 private:
  CAutoPtr<CQIllustratorRGradSizer>  sizer_;
  CQIllustratorSetRGradientToolbar  *toolbar_ { nullptr };
};

#endif
