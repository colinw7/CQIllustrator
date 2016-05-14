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

  const char *getTitle() const { return "Create Star"; }

  QIcon getIcon();

  void addWidgets();

  CreateMode getCreateMode() const { return createMode_; }

  void setSelectedShape(const CQIllustratorShape *shape);

  void setSize(const CBBox2D &bbox);

 private slots:
  void starModeSlot(bool);
  void polyModeSlot(bool);

  void updateShape();

 private:
  CQIllustratorCreateStarMode *mode_ { 0 };
  CreateMode                   createMode_ { CreateMode::STAR };
  CQImageButton               *starButton_ { 0 };
  CQImageButton               *polyButton_ { 0 };
  CQPointEdit                 *centerEdit_ { 0 };
  QSpinBox                    *numEdit_ { 0 };
  CQRealEdit                  *radius1Edit_ { 0 };
  CQRealEdit                  *radius2Edit_ { 0 };
  CQAngleSpinBox              *angle1Edit_ { 0 };
  CQAngleSpinBox              *angle2Edit_ { 0 };
};

//------

class CQIllustratorCreateStarSizer : public CQIllustratorModeSizer {
 public:
  CQIllustratorCreateStarSizer(CQIllustratorCreateStarMode *mode);

  void drawHandles(QPainter *painter, const CQIllustratorShape *shape);

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

  const char *getTitle() const { return "Create Star"; }

  CQIllustratorCreateStarToolbar *getToolbar() { return toolbar_; }

  CQIllustratorCreateStarToolbar *createToolbar();

  CQIllustratorCreateStarSizer *createSizer();

  CQMenuItem *createMenuItem(CQMenu *menu);

  bool isCreateMode() const { return true; }

  CQIllustratorCreateStarToolbar *getToolbar() const { return toolbar_; }

  void handleMouseRelease(const MouseEvent &e);
  void handleMouseDrag   (const MouseEvent &e);

  void drawOverlay(CQIllustratorShapeDrawer *drawer);

 private:
  CQIllustratorCreateStarToolbar *toolbar_ { 0 };
  CQIllustratorCreateStarSizer   *sizer_ { 0 };
};

#endif
