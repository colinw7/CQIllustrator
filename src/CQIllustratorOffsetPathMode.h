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

  const char *getTitle() const { return "Offset Path"; }

  QIcon getIcon();

  void addWidgets();

  void setSelectedShape(const CQIllustratorShape *shape);

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

  void updatePoint(CQIllustratorShape *shape, const QPointF &p);

  void drawHandles(QPainter *painter, const CQIllustratorShape *shape);

 private:
  CQIllustratorHandle *width_handle_;
  CQIllustratorShape  *shape_;
};

class CQIllustratorOffsetPathMode : public CQIllustratorMode {
  Q_OBJECT

 public:
  CQIllustratorOffsetPathMode(CQIllustrator *illustrator);

  const char *getTitle() const { return "Offset Path"; }

  CQIllustratorOffsetPathToolbar *getToolbar() { return toolbar_; }

  CQIllustratorOffsetPathToolbar *createToolbar();

  CQIllustratorOffsetPathSizer *createSizer();

  CQMenuItem *createMenuItem(CQMenu *menu);

  CQIllustratorOffsetPathToolbar *getToolbar() const { return toolbar_; }

  void handleMouseRelease(const MouseEvent &e);
  void handleMouseDrag   (const MouseEvent &e);

  void addSelectionToSandbox();

  void drawOverlay(CQIllustratorShapeDrawer *drawer);

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
