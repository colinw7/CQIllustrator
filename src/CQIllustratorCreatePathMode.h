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
  enum CreateMode {
    CREATE_FREE_MODE,
    CREATE_RECT_MODE,
    CREATE_45_MODE
  };

 public:
  CQIllustratorCreatePathToolbar(CQIllustratorCreatePathMode *mode);

  const char *getTitle() const { return "Create Path"; }

  QIcon getIcon();

  void addWidgets();

  CreateMode getCreateMode() const { return createMode_; }

  void setSelectedShape(const CQIllustratorShape *shape);

  void setSelectedShapePoint(const CQIllustratorShape *shape,
                             const CQIllustratorShapeControlPoint *point);

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
  CreateMode                   createMode_ { CREATE_FREE_MODE };
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

  void drawHandles(QPainter *painter, const CQIllustratorShape *shape);

  void updateHandles(uint numHandles);
};

//------

class CQIllustratorCreatePathMode : public CQIllustratorMode {
 public:
  enum SelMode {
    POINT_SEL,
    LINE_SEL
  };

 public:
  CQIllustratorCreatePathMode(CQIllustrator *illustrator);

  const char *getTitle() const { return "Create Path"; }

  CQIllustratorCreatePathToolbar *createToolbar();

  CQIllustratorCreatePathSizer *createSizer();

  void setSelMode(SelMode mode);

  SelMode getSelMode() const { return selMode_; }

  void setPathMode(CPathPartType pathMode);

  CPathPartType getPathMode() const { return pathMode_; }

  CQMenuItem *createMenuItem(CQMenu *menu);

  bool isCreateMode() const { return true; }

  CQIllustratorCreatePathToolbar *getToolbar() const { return toolbar_; }

  void handleMousePress  (const MouseEvent &e);
  void handleMouseRelease(const MouseEvent &e);
  void handleMouseDrag   (const MouseEvent &e);
  void handleMouseMove   (const MouseEvent &e);

  bool handleKeyPress(const KeyEvent &e);

 private:
  void addPoint(QPointF w, QPoint p);

  void snapPoint(CPoint2D &p);

  void addCloseHandle(const CPoint2D &cp, const CPoint2D &p);
  void addSnapHandle (const CPoint2D &cp, const CPoint2D &p);

  void accept();

  bool cancel();

  void drawOverlay(CQIllustratorShapeDrawer *drawer);

  void drawPathPoints(QPainter *painter);

  QCursor getCursor() const;

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
  SelMode                             selMode_ { POINT_SEL };
  CPathPartType                       pathMode_ { CPATH_PART_TYPE_LINE_TO };
  std::vector<CQIllustratorHandle *>  closeHandles_;
  uint                                closeHandleNum_ { 0 };
  std::vector<CQIllustratorHandle *>  snapHandles_;
  uint                                snapHandleNum_ { 0 };
  QPoint                              pathStart_;
  PathPointList                       pathPoints_;
};

#endif
