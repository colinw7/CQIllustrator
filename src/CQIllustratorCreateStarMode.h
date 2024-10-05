#ifndef CQIllustratorCreateStarMode_H
#define CQIllustratorCreateStarMode_H

#include <CQIllustratorMode.h>
#include <CQIllustratorToolbar.h>

#include <QComboBox>
#include <QSpinBox>

class CQImageButton;
class CQPointEdit;
class CQRealEdit;
class CQAngleSpinBox;
class CQIllustrator;
class CQIllustratorControlPointHandle;
class CQIllustratorCreateStarMode;
class CQIllustratorCreateStarSizer;

class CQIllustratorCreateStarToolbar : public CQIllustratorToolbar {
  Q_OBJECT

 public:
  enum class CreateMode {
    STAR,
    POLY
  };

 public:
  CQIllustratorCreateStarToolbar(CQIllustratorCreateStarMode *mode);

  const char *getTitle() const override { return "Create Star"; }

  QIcon getIcon() override;

  void addWidgets() override;

  CreateMode getCreateMode() const { return createMode_; }

  void setSelectedShape(const CQIllustratorShape *shape) override;

  void setSize(const CBBox2D &bbox);

 private slots:
  void starModeSlot(bool);
  void polyModeSlot(bool);

  void updateShape();

 private:
  CQIllustratorCreateStarMode *mode_        { nullptr };
  CreateMode                   createMode_  { CreateMode::STAR };
  CQImageButton               *starButton_  { nullptr };
  CQImageButton               *polyButton_  { nullptr };
  CQPointEdit                 *centerEdit_  { nullptr };
  QSpinBox                    *numEdit_     { nullptr };
  CQRealEdit                  *radius1Edit_ { nullptr };
  CQRealEdit                  *radius2Edit_ { nullptr };
  CQAngleSpinBox              *angle1Edit_  { nullptr };
  CQAngleSpinBox              *angle2Edit_  { nullptr };
};

//------

class CQIllustratorCreateStarSizer : public CQIllustratorModeSizer {
 public:
  CQIllustratorCreateStarSizer(CQIllustratorCreateStarMode *mode);

  void drawHandles(QPainter *painter, const CQIllustratorShape *shape) override;

 private:
  CQIllustratorControlPointHandle *c_handle_ { 0 };
  CQIllustratorControlPointHandle *ir_handle_ { 0 };
  CQIllustratorControlPointHandle *or_handle_ { 0 };
};

//------

class CQIllustratorCreateStarMode : public CQIllustratorMode {
  Q_OBJECT

 public:
  CQIllustratorCreateStarMode(CQIllustrator *illustrator);

  const char *getTitle() const override { return "Create Star"; }

  CQIllustratorCreateStarToolbar *getToolbar() { return toolbar_; }

  CQIllustratorCreateStarToolbar *createToolbar() override;

  CQIllustratorCreateStarSizer *createSizer() override;

  CQMenuItem *createMenuItem(CQMenu *menu) override;

  bool isCreateMode() const override { return true; }

  CQIllustratorCreateStarToolbar *getToolbar() const override { return toolbar_; }

  void handleMouseRelease(const MouseEvent &e) override;
  void handleMouseDrag   (const MouseEvent &e) override;

  void drawOverlay(CQIllustratorShapeDrawer *drawer) override;

 private:
  CQIllustratorCreateStarToolbar *toolbar_ { 0 };
  CQIllustratorCreateStarSizer   *sizer_ { 0 };
};

#endif
