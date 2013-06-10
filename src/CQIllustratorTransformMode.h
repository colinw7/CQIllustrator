#ifndef CQIllustratorTransformMode_H
#define CQIllustratorTransformMode_H

#include <CQIllustrator.h>
#include <CQIllustratorMode.h>
#include <CQIllustratorToolbar.h>

class QCheckBox;

class CQTransformMoveTool;
class CQTransformScaleTool;
class CQTransformRotateTool;
class CQTransformSkewTool;
class CQAngleSpinBox;
class CQRealEdit;
class CQPointEdit;
class CQIllustratorTransformMode;

class CQIllustratorTransformToolbar : public CQIllustratorToolbar {
  Q_OBJECT

 public:
  CQIllustratorTransformToolbar(CQIllustratorTransformMode *mode);

  const char *getTitle() const { return "Transform"; }

  CQIllustrator *getIllustrator() const;

  QIcon getIcon();

  void addWidgets();

  void resetSelectMode();

 private slots:
  void resetSlot();
  void applySlot();

 private:
  CQIllustratorTransformMode *mode_;
  QTabWidget                 *tab_;
  CQTransformMoveTool        *moveTool_;
  CQTransformScaleTool       *scaleTool_;
  CQTransformRotateTool      *rotateTool_;
  CQTransformSkewTool        *skewTool_;
};

class CQIllustratorTransformMode : public CQIllustratorMode {
  Q_OBJECT

 public:
  CQIllustratorTransformMode(CQIllustrator *illustrator);

  const char *getTitle() const { return "Transform"; }

  CQIllustratorTransformToolbar *createToolbar();

  CQMenuItem *createMenuItem(CQMenu *menu);

  CQIllustratorTransformToolbar *getToolbar() const { return toolbar_; }

  void handleMouseRelease(const MouseEvent &e);
  void handleMouseDrag   (const MouseEvent &e);

  void drawOverlay(CQIllustratorShapeDrawer *drawer);

  QCursor getCursor() const;

 private:
  CQIllustratorTransformToolbar *toolbar_;
};

class CQTransformMoveTool : public QWidget {
  Q_OBJECT

 public:
  CQTransformMoveTool(CQIllustratorTransformToolbar *toolbar);

  void apply();

 private:
  CQIllustratorTransformToolbar *toolbar_;
  CQRealEdit                    *hedit_;
  CQRealEdit                    *vedit_;
  QCheckBox                     *relativeCheck_;
  QCheckBox                     *groupCheck_;
};

class CQTransformScaleTool : public QWidget {
 public:
  CQTransformScaleTool(CQIllustratorTransformToolbar *toolbar);

  void apply();

 private:
  CQIllustratorTransformToolbar *toolbar_;
  CQRealEdit                    *wedit_;
  CQRealEdit                    *hedit_;
  QCheckBox                     *equalCheck_;
  QCheckBox                     *groupCheck_;
};

class CQTransformRotateTool : public QWidget {
 public:
  CQTransformRotateTool(CQIllustratorTransformToolbar *toolbar);

  void apply();

 private:
  CQIllustratorTransformToolbar *toolbar_;
  CQAngleSpinBox                *angleEdit_;
  QCheckBox                     *groupCheck_;
};

class CQTransformSkewTool : public QWidget {
 public:
  CQTransformSkewTool(CQIllustratorTransformToolbar *toolbar);

  void apply();

 private:
  CQIllustratorTransformToolbar *toolbar_;
  CQRealEdit                    *hedit_;
  CQRealEdit                    *vedit_;
  QCheckBox                     *groupCheck_;
};

#endif
