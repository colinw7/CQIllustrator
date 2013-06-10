#ifndef CQILLUSTRATOR_MODE_H
#define CQILLUSTRATOR_MODE_H

#include <QObject>
#include <QPoint>
#include <QCursor>
#include <QTransform>
#include <CEvent.h>
#include <CPoint2D.h>
#include <CBBox2D.h>

#include <CQIllustratorData.h>
#include <CQIllustratorShape.h>

class QMouseEvent;
class QKeyEvent;
class QStackedWidget;
class CQMenu;
class CQMenuItem;

class CQIllustrator;
class CQIllustratorShapeDrawer;
class CQIllustratorToolbar;
class CQIllustratorModeSizer;
class CQIllustratorShape;
class CQIllustratorSelectedShape;
class CQIllustratorHandle;
class CQIllustratorControlPointHandle;

class CQIllustratorMode : public QObject {
  Q_OBJECT

 public:
  enum EditMode {
    CREATE_MODE,
    EDIT_MODE
  };

  struct MouseEvent {
    QPoint       pixel;
    QPointF      window;
    CMouseEvent *event;

    MouseEvent(QMouseEvent *event, const QTransform &transform);
  };

  struct KeyEvent {
    QPoint     pixel;
    QPointF    window;
    CKeyEvent *event;

    KeyEvent(QKeyEvent *event, const QPoint &pos, const QTransform &transform);
  };

 public:
  CQIllustratorMode(CQIllustrator *illustrator, uint id);
  CQIllustratorMode(CQIllustratorMode *parentMode, uint id);

  virtual ~CQIllustratorMode() { }

  void init();

  virtual CQIllustratorToolbar *createToolbar() = 0;

  virtual CQIllustratorModeSizer *createSizer();

  virtual const char *getTitle() const = 0;

  CQIllustrator *getIllustrator() const { return illustrator_; }

  CQIllustratorMode *getParentMode() const { return parentMode_; }

  uint getId() const { return id_; }

  virtual CQIllustratorData::ChangeType getChangeType() const {
    return CQIllustratorData::CHANGE_GEOMETRY;
  }

  virtual CQIllustratorShape::ControlType getControlType() const {
    return CQIllustratorShape::CONTROL_GEOMETRY;
  }

  CQMenuItem *getMenuItem() const { return menuItem_; }

  virtual CQMenuItem *createMenuItem(CQMenu *menu) = 0;

  void setEditMode(EditMode mode);

  EditMode getEditMode() const { return editMode_; }

  virtual bool isCreateMode() const { return false; }
  virtual bool isSelectMode() const { return false; }

  void setStack(QStackedWidget *stack);

  virtual CQIllustratorToolbar *getToolbar() const = 0;

  void mousePress  (const MouseEvent &e);
  void mouseRelease(const MouseEvent &e);
  void mouseDrag   (const MouseEvent &e);
  void mouseMove   (const MouseEvent &e);

  bool keyPress(const KeyEvent &e);

  virtual void showToolbar(bool show);

  virtual void handleMousePress  (const MouseEvent &e);
  virtual void handleMouseRelease(const MouseEvent &e);
  virtual void handleMouseDrag   (const MouseEvent &e) = 0;
  virtual void handleMouseMove   (const MouseEvent &e);

  virtual bool handleKeyPress(const KeyEvent &);

  virtual void selectControlPoint(const MouseEvent &e);

  virtual void addSelectionToSandbox();

  virtual void moveCurrent  (const CPoint2D &d);
  virtual void resizeCurrent(const CBBox2D  &d);

  virtual void deleteCurrent();

  virtual void moveCurrentPoint(const CPoint2D &d);

  virtual void accept() { }
  virtual bool cancel();

  virtual void escape();

  virtual void drawSelection(CQIllustratorShapeDrawer *drawer);

  virtual void drawOverlay(CQIllustratorShapeDrawer *drawer);

  virtual void setSelectedShape(const CQIllustratorShape *shape);

  virtual void setSelectedShapePoint(const CQIllustratorShape *shape,
                                     const CQIllustratorShapeControlPoint *point);

  void setCursor(const uchar *bits, const uchar *mask, int xo, int yo);

  virtual QCursor getCursor() const;

 public slots:
  void menuItemSlot();

  void selectionSlot();

 protected:
  CQIllustrator          *illustrator_;
  CQIllustratorMode      *parentMode_;
  uint                    id_;
  CQIllustratorToolbar   *toolbar_;
  CQIllustratorModeSizer *sizer_;
  CQMenuItem             *menuItem_;
  EditMode                editMode_;
  QStackedWidget         *stack_;
  QCursor                 cursor_;
  bool                    pressed_;
  bool                    moving_;
  bool                    dragging_;
  QPoint                  press_ppos_;
  QPointF                 press_wpos_;
  QPoint                  prev_ppos_;
  QPointF                 prev_wpos_;
  QPoint                  curr_ppos_;
  QPointF                 curr_wpos_;
  QPoint                  release_ppos_;
  QPointF                 release_wpos_;
};

class CQIllustratorModeSizer : public QObject {
  Q_OBJECT

 public:
  typedef CQIllustratorMode::KeyEvent KeyEvent;

 public:
  CQIllustratorModeSizer(CQIllustratorMode *mode);

  virtual ~CQIllustratorModeSizer() { }

  virtual void addHandle(CQIllustratorControlPointHandle *handle);

  virtual void addHandle(CQIllustratorHandle *handle);

  virtual void removeHandle(CQIllustratorControlPointHandle *);

  virtual void removeHandle(CQIllustratorHandle *);

  virtual void draw(QPainter *painter, const CQIllustratorShape *shape);

  virtual void drawHandles(QPainter *painter, const CQIllustratorShape *shape);

  virtual void drawSelectionHandles(QPainter *painter, const CQIllustratorSelectedShape &sshape);

  virtual bool updateActive(const QPointF &p);

  virtual void updatePoint(CQIllustratorShape *shape, const QPointF &p);

  virtual bool isActive() const;

  virtual void updateSelectionHandles(const CQIllustratorSelectedShape &sshape);

  virtual void updatePointSelectionHandles(uint num_handles);
  virtual void updateLineSelectionHandles(uint num_handles);

  virtual bool handleKeyPress(const KeyEvent &e);

 protected:
  typedef std::vector<CQIllustratorControlPointHandle *> ControlPointHandleList;
  typedef std::vector<CQIllustratorHandle *>             HandleList;

  CQIllustratorMode      *mode_;
  QTransform              transform_;
  ControlPointHandleList  controlPointHandles_;
  HandleList              handles_;
  HandleList              lineSelectionHandles_;
  HandleList              pointSelectionHandles_;
};

#endif
