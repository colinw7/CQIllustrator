#ifndef CQIllustratorOffsetPathMode_H
#define CQIllustratorOffsetPathMode_H

#include <CQIllustratorMode.h>
#include <CQIllustratorToolbar.h>

class CQRealEdit;
class CQIllustrator;
class CQIllustratorHandle;
class CQIllustratorOffsetPathMode;
class CQIllustratorOffsetPathSizer;

class CQIllustratorOffsetPathToolbar : public CQIllustratorToolbar {
  Q_OBJECT

 public:
  CQIllustratorOffsetPathToolbar(CQIllustratorOffsetPathMode *mode);

  const char *getTitle() const override { return "Offset Path"; }

  QIcon getIcon() override;

  void addWidgets() override;

  void setSelectedShape(const CQIllustratorShape *shape) override;

  void setWidth(double w);

 private slots:
  void updateShape();

 private:
  CQIllustratorOffsetPathMode *mode_;
  CQRealEdit                  *widthEdit_;
};

class CQIllustratorOffsetPathSizer : public CQIllustratorModeSizer {
 public:
  CQIllustratorOffsetPathSizer(CQIllustratorOffsetPathMode *mode);

  void setSelectedShape(CQIllustratorShape *shape);

  void updatePoint(CQIllustratorShape *shape, const QPointF &p) override;

  void drawHandles(QPainter *painter, const CQIllustratorShape *shape) override;

 private:
  CQIllustratorHandle *width_handle_;
  CQIllustratorShape  *shape_;
};

class CQIllustratorOffsetPathMode : public CQIllustratorMode {
  Q_OBJECT

 public:
  CQIllustratorOffsetPathMode(CQIllustrator *illustrator);

  const char *getTitle() const override { return "Offset Path"; }

  CQIllustratorOffsetPathToolbar *getToolbar() { return toolbar_; }

  CQIllustratorOffsetPathToolbar *createToolbar() override;

  CQIllustratorOffsetPathSizer *createSizer() override;

  CQMenuItem *createMenuItem(CQMenu *menu) override;

  CQIllustratorOffsetPathToolbar *getToolbar() const override { return toolbar_; }

  void handleMouseRelease(const MouseEvent &e) override;
  void handleMouseDrag   (const MouseEvent &e) override;

  void addSelectionToSandbox() override;

  void drawOverlay(CQIllustratorShapeDrawer *drawer) override;

  void setSelectedShape(CQIllustratorShape *shape);

  void setWidth(double w);

 private:
  using CQIllustratorMode::setSelectedShape;

 private:
  CQIllustratorOffsetPathToolbar *toolbar_;
  CQIllustratorOffsetPathSizer   *sizer_;
  double                          w_;
};

#endif
