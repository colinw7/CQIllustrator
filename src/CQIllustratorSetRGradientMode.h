#ifndef CQIllustratorSetRGradientMode_H
#define CQIllustratorSetRGradientMode_H

#include <CAutoPtr.h>

#include <CQIllustratorMode.h>
#include <CQIllustratorToolbar.h>
#include <CQIllustrator.h>

class CQRealEdit;
class CQIllustrator;
class CQIllustratorControlPointHandle;
class CQIllustratorSetRGradientMode;

class CQIllustratorRGradSizer : public CQIllustratorModeSizer {
 public:
  CQIllustratorRGradSizer(CQIllustratorSetRGradientMode *mode);

  void drawHandles(QPainter *painter, const CQIllustratorShape *shape);

 private:
  CQIllustratorControlPointHandle *center_handle_;
  CQIllustratorControlPointHandle *focus_handle_;
  CQIllustratorControlPointHandle *radius_handle_;
};

class CQIllustratorSetRGradientToolbar : public CQIllustratorToolbar {
  Q_OBJECT

 public:
  CQIllustratorSetRGradientToolbar(CQIllustratorSetRGradientMode *mode);

  const char *getTitle() const { return "Radial Gradient"; }

  QIcon getIcon();

  void addWidgets();

  void setSelectedShape(const CQIllustratorShape *shape);

 private slots:
  void updateShape();

 private:
  CQIllustratorSetRGradientMode *mode_;
  CQRealEdit                    *centerXEdit_;
  CQRealEdit                    *centerYEdit_;
  CQRealEdit                    *focusXEdit_;
  CQRealEdit                    *focusYEdit_;
  CQRealEdit                    *radiusEdit_;
};

class CQIllustratorSetRGradientMode : public CQIllustratorMode {
 public:
  CQIllustratorSetRGradientMode(CQIllustrator *illustrator);

  const char *getTitle() const { return "Set Radial Gradient"; }

  CQIllustratorSetRGradientToolbar *createToolbar();

  CQIllustratorRGradSizer *createSizer();

  CQMenuItem *createMenuItem(CQMenu *menu);

  CQIllustratorSetRGradientToolbar *getToolbar() const { return toolbar_; }

  CQIllustratorData::ChangeType getChangeType() const {
    return CQIllustratorData::CHANGE_FILL;
  }

  CQIllustratorShape::ControlType getControlType() const {
    return CQIllustratorShape::CONTROL_RGRADIENT;
  }

  void handleMouseRelease(const MouseEvent &e);
  void handleMouseDrag   (const MouseEvent &e);

  void drawOverlay(CQIllustratorShapeDrawer *drawer);

  QCursor getCursor() const;

 private:
  CAutoPtr<CQIllustratorRGradSizer>  sizer_;
  CQIllustratorSetRGradientToolbar  *toolbar_;
};

#endif
