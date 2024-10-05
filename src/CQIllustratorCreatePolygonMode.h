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
  enum class CreateMode {
    FREE,
    RECT,
    ANGLE_45
  };

 public:
  CQIllustratorCreatePolygonToolbar(CQIllustratorCreatePolygonMode *mode);

  const char *getTitle() const override { return "Create Polygon"; }

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

  void drawHandles(QPainter *painter, const CQIllustratorShape *shape) override;

  void updateHandles(uint numHandles);
};

class CQIllustratorCreatePolygonMode : public CQIllustratorMode {
 public:
  CQIllustratorCreatePolygonMode(CQIllustrator *illustrator);

  const char *getTitle() const override { return "Create Polygon"; }

  CQIllustratorCreatePolygonToolbar *createToolbar() override;

  CQIllustratorCreatePolygonSizer *createSizer() override;

  CQMenuItem *createMenuItem(CQMenu *menu) override;

  bool isCreateMode() const override { return true; }

  CQIllustratorCreatePolygonToolbar *getToolbar() const override { return toolbar_; }

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

  void drawPolygonPoints(QPainter *painter);

  QCursor getCursor() const override;

  void commitPolygonPoints();

 public:
  void addPointToCurrent();
  void removePointFromCurrent();

 private:
  typedef std::vector<CPoint2D> PointList;

  CQIllustratorCreatePolygonToolbar  *toolbar_ { 0 };
  CQIllustratorCreatePolygonSizer    *sizer_ { 0 };
  std::vector<CQIllustratorHandle *>  closeHandles_;
  uint                                closeHandleNum_ { 0 };
  std::vector<CQIllustratorHandle *>  snapHandles_;
  uint                                snapHandleNum_ { 0 };
  QPoint                              polygonStart_;
  PointList                           polygonPoints_;
};

#endif
