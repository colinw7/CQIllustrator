#ifndef CQIllustratorCreatePolygonMode_H
#define CQIllustratorCreatePolygonMode_H

#include <CQIllustratorMode.h>
#include <CQIllustratorToolbar.h>

class QRadioButton;
class CQImageButton;
class CQPointEdit;
class CQIllustrator;
class CQIllustratorHandle;
class CQIllustratorCreatePolygonMode;

class CQIllustratorCreatePolygonToolbar : public CQIllustratorToolbar {
  Q_OBJECT

 public:
  enum CreateMode {
    CREATE_FREE_MODE,
    CREATE_RECT_MODE,
    CREATE_45_MODE
  };

 public:
  CQIllustratorCreatePolygonToolbar(CQIllustratorCreatePolygonMode *mode);

  const char *getTitle() const { return "Create Polygon"; }

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

  void polyFreeSlot(bool);
  void polyRectSlot(bool);
  void poly45Slot(bool);

  void addPointSlot();
  void removePointSlot();

  void updatePointSlot();

 private:
  CQIllustratorCreatePolygonMode *mode_;
  CreateMode                      createMode_;
  QRadioButton                   *createRadio_;
  QRadioButton                   *editRadio_;
  CQImageButton                  *polyFreeButton_;
  CQImageButton                  *polyRectButton_;
  CQImageButton                  *poly45Button_;
  CQImageButton                  *addPointButton_;
  CQImageButton                  *removePointButton_;
  CQPointEdit                    *pointEdit_;
};

class CQIllustratorCreatePolygonSizer : public CQIllustratorModeSizer {
 public:
  CQIllustratorCreatePolygonSizer(CQIllustratorCreatePolygonMode *mode);

  void drawHandles(QPainter *painter, const CQIllustratorShape *shape);

  void updateHandles(uint numHandles);
};

class CQIllustratorCreatePolygonMode : public CQIllustratorMode {
 public:
  CQIllustratorCreatePolygonMode(CQIllustrator *illustrator);

  const char *getTitle() const { return "Create Polygon"; }

  CQIllustratorCreatePolygonToolbar *createToolbar();

  CQIllustratorCreatePolygonSizer *createSizer();

  CQMenuItem *createMenuItem(CQMenu *menu);

  bool isCreateMode() const { return true; }

  CQIllustratorCreatePolygonToolbar *getToolbar() const { return toolbar_; }

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

  void drawPolygonPoints(QPainter *painter);

  QCursor getCursor() const;

  void commitPolygonPoints();

 public:
  void addPointToCurrent();
  void removePointFromCurrent();

 private:
  typedef std::vector<CPoint2D> PointList;

  CQIllustratorCreatePolygonToolbar  *toolbar_;
  CQIllustratorCreatePolygonSizer    *sizer_;
  std::vector<CQIllustratorHandle *>  closeHandles_;
  uint                                closeHandleNum_;
  std::vector<CQIllustratorHandle *>  snapHandles_;
  uint                                snapHandleNum_;
  QPoint                              polygonStart_;
  PointList                           polygonPoints_;
};

#endif
