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
  enum CreateMode {
    CREATE_STAR_MODE,
    CREATE_POLY_MODE
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
  CQIllustratorCreateStarMode *mode_;
  CreateMode                   createMode_;
  CQImageButton               *starButton_;
  CQImageButton               *polyButton_;
  CQPointEdit                 *centerEdit_;
  QSpinBox                    *numEdit_;
  CQRealEdit                  *radius1Edit_;
  CQRealEdit                  *radius2Edit_;
  CQAngleSpinBox              *angle1Edit_;
  CQAngleSpinBox              *angle2Edit_;
};

class CQIllustratorCreateStarSizer : public CQIllustratorModeSizer {
 public:
  CQIllustratorCreateStarSizer(CQIllustratorCreateStarMode *mode);

  void drawHandles(QPainter *painter, const CQIllustratorShape *shape);

 private:
  CQIllustratorControlPointHandle *c_handle_;
  CQIllustratorControlPointHandle *ir_handle_;
  CQIllustratorControlPointHandle *or_handle_;
};

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
  CQIllustratorCreateStarToolbar *toolbar_;
  CQIllustratorCreateStarSizer   *sizer_;
};

#endif
