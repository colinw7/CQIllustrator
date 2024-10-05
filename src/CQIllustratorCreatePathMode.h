#ifndef CQIllustratorCreatePathMode_H
#define CQIllustratorCreatePathMode_H

#include <CQIllustratorMode.h>
#include <CQIllustratorToolbar.h>

class QRadioButton;
class CQImageButton;
class CQPointEdit;
class CQIllustrator;
class CQIllustratorHandle;
class CQIllustratorCreatePathMode;

class CQIllustratorCreatePathToolbar : public CQIllustratorToolbar {
  Q_OBJECT

 public:
  enum class CreateMode {
    FREE,
    RECT,
    ANGLE_45
  };

 public:
  CQIllustratorCreatePathToolbar(CQIllustratorCreatePathMode *mode);

  const char *getTitle() const override { return "Create Path"; }

  QIcon getIcon() override;

  void addWidgets() override;

  CreateMode getCreateMode() const { return createMode_; }

  void setSelectedShape(const CQIllustratorShape *shape) override;

  void setSelectedShapePoint(const CQIllustratorShape *shape,
                             const CQIllustratorShapeControlPoint *point) override;

  void updateShape();

  void setSize(const CBBox2D &bbox);

  void setSelectionPoint(const CPoint2D &p);

 private slots:
  void modeChangedSlot();

  void pathFreeSlot(bool);
  void pathRectSlot(bool);
  void path45Slot(bool);

  void selChangedSlot();

  void lineModeSlot(bool);
  void curve2ModeSlot(bool);
  void curve3ModeSlot(bool);

  void cornerNodeSlot();
  void curveNodeSlot();

  void addNodeSlot();
  void removeNodeSlot();

  void updateNodeSlot();

 private:
  void setPathMode(CPathPartType pathMode, bool state);

  void updateMode();

 private:
  CQIllustratorCreatePathMode *mode_ { 0 };
  CreateMode                   createMode_ { CreateMode::FREE };
  QRadioButton                *createRadio_ { 0 };
  QRadioButton                *editRadio_ { 0 };
  CQImageButton               *pathFreeButton_ { 0 };
  CQImageButton               *pathRectButton_ { 0 };
  CQImageButton               *path45Button_ { 0 };
  QRadioButton                *pointRadio_ { 0 };
  QRadioButton                *lineRadio_ { 0 };
  CQImageButton               *pathLineButton_ { 0 };
  CQImageButton               *pathCurve2Button_ { 0 };
  CQImageButton               *pathCurve3Button_ { 0 };
  CQImageButton               *nodeCornerButton_ { 0 };
  CQImageButton               *nodeCurveButton_ { 0 };
  CQImageButton               *addNodeButton_ { 0 };
  CQImageButton               *removeNodeButton_ { 0 };
  CQPointEdit                 *pointEdit_ { 0 };
};

//------

class CQIllustratorCreatePathSizer : public CQIllustratorModeSizer {
 public:
  CQIllustratorCreatePathSizer(CQIllustratorCreatePathMode *mode);

  void drawHandles(QPainter *painter, const CQIllustratorShape *shape) override;

  void updateHandles(uint numHandles);
};

//------

class CQIllustratorCreatePathMode : public CQIllustratorMode {
 public:
  enum class SelMode {
    POINT,
    LINE
  };

 public:
  CQIllustratorCreatePathMode(CQIllustrator *illustrator);

  const char *getTitle() const override { return "Create Path"; }

  CQIllustratorCreatePathToolbar *createToolbar() override;

  CQIllustratorCreatePathSizer *createSizer() override;

  void setSelMode(SelMode mode);

  SelMode getSelMode() const { return selMode_; }

  void setPathMode(CPathPartType pathMode);

  CPathPartType getPathMode() const { return pathMode_; }

  CQMenuItem *createMenuItem(CQMenu *menu) override;

  bool isCreateMode() const override { return true; }

  CQIllustratorCreatePathToolbar *getToolbar() const override { return toolbar_; }

  void handleMousePress  (const MouseEvent &e) override;
  void handleMouseRelease(const MouseEvent &e) override;
  void handleMouseDrag   (const MouseEvent &e) override;
  void handleMouseMove   (const MouseEvent &e) override;

  bool handleKeyPress(const KeyEvent &e) override;

 private:
  void addPoint(QPointF w, QPoint p);

  void snapPoint(CPoint2D &p);

  void addCloseHandle(const CPoint2D &cp, const CPoint2D &p);
  void addSnapHandle (const CPoint2D &cp, const CPoint2D &p);

  void accept() override;

  bool cancel() override;

  void drawOverlay(CQIllustratorShapeDrawer *drawer) override;

  void drawPathPoints(QPainter *painter);

  QCursor getCursor() const override;

  void commitPathPoints(bool closed);

 public:
  void setCurrentToCornerNode();
  void setCurrentToCurveNode();

  void addNodeToCurrent();
  void removeNodeFromCurrent();

 private:
  struct PathPoint {
    CPathPartType type;
    CPoint2D      p;
    CPoint2D      c1;
    CPoint2D      c2;

    PathPoint(CPathPartType type1, const CPoint2D &p1=CPoint2D()) :
     type(type1), p(p1), c1(p1), c2(p1) {
    }
  };

  typedef std::vector<PathPoint> PathPointList;

  CQIllustratorCreatePathToolbar     *toolbar_ { 0 };
  CQIllustratorCreatePathSizer       *sizer_ { 0 };
  SelMode                             selMode_ { SelMode::POINT };
  CPathPartType                       pathMode_ { CPATH_PART_TYPE_LINE_TO };
  std::vector<CQIllustratorHandle *>  closeHandles_;
  uint                                closeHandleNum_ { 0 };
  std::vector<CQIllustratorHandle *>  snapHandles_;
  uint                                snapHandleNum_ { 0 };
  QPoint                              pathStart_;
  PathPointList                       pathPoints_;
};

#endif
