#ifndef CQIllustratorAlignMode_H
#define CQIllustratorAlignMode_H

#include <CQIllustrator.h>
#include <CQIllustratorMode.h>
#include <CQIllustratorToolbar.h>
#include <QToolButton>

class QDockWidget;
class QComboBox;
class QPushButton;
class QToolButton;
class QStackedWidget;
class QLineEdit;
class QLabel;

class CQAlignButtons;
class CQDistButtons;
class CQSpreadButtons;
class CQRealEdit;
class CQAlignAnchor;
class CQPointEdit;
class CQIllustratorAlignMode;

class CQIllustratorAlignToolbar : public CQIllustratorToolbar {
  Q_OBJECT

 public:
  enum AnchorMode {
    SELECTION_MODE,
    OBJECT_MODE,
    POSITION_MODE
  };

  enum ObjectEdgeType {
    EDGE_LEFT_BOTTOM,
    EDGE_RIGHT_TOP,
    EDGE_MIDDLE
  };

 public:
  CQIllustratorAlignToolbar(CQIllustratorAlignMode *mode);

  const char *getTitle() const { return "Align"; }

  QIcon getIcon();

  void addWidgets();

  double getOffset() const;

  AnchorMode getAnchorMode() const;

  QString getAnchorObject() const;

  void setAnchorObject(const QString &name);

  ObjectEdgeType getAnchorObjectEdgeType() const;

  QPointF getAnchorPosition() const;

  void setAnchorPosition(const QPointF &pos);

  void resetSelectMode();

 signals:
  void alignLeft();
  void alignBottom();
  void alignRight();
  void alignTop();
  void alignHorizontal();
  void alignVertical();

  void alignLeftPreview();
  void alignBottomPreview();
  void alignRightPreview();
  void alignTopPreview();
  void alignHorizontalPreview();
  void alignVerticalPreview();

  void alignPreviewClear();

  void selectAnchorObject();
  void selectAnchorPosition();
  void cancelSelectAnchor();

 private:
  CQIllustratorAlignMode *mode_;
  CQAlignButtons         *alignw_;
  CQDistButtons          *distw_;
  CQSpreadButtons        *spreadw_;
  CQRealEdit             *offset_;
  CQAlignAnchor          *anchor_;
};

class CQIllustratorAlignMode : public CQIllustratorMode {
  Q_OBJECT

 public:
  CQIllustratorAlignMode(CQIllustrator *illustrator);

  const char *getTitle() const { return "Align"; }

  CQIllustratorAlignToolbar *createToolbar();

  CQMenuItem *createMenuItem(CQMenu *menu);

  CQIllustratorAlignToolbar *getToolbar() const { return toolbar_; }

  void handleMouseRelease(const MouseEvent &e);
  void handleMouseDrag   (const MouseEvent &e);

  void drawOverlay(CQIllustratorShapeDrawer *drawer);

  QCursor getCursor() const;

  void align(CQIllustrator::AlignSide side, bool commit);

 private:
  CQIllustratorAlignToolbar *toolbar_;

 private slots:
  void alignLSlot();
  void alignLPreviewSlot();
  void alignBSlot();
  void alignBPreviewSlot();
  void alignRSlot();
  void alignRPreviewSlot();
  void alignTSlot();
  void alignTPreviewSlot();
  void alignHSlot();
  void alignHPreviewSlot();
  void alignVSlot();
  void alignVPreviewSlot();
  void alignPreviewClearSlot();
};

class CQAlignButtons : public QWidget {
  Q_OBJECT

 public:
  CQAlignButtons();

 signals:
  void alignLeft();
  void alignBottom();
  void alignRight();
  void alignTop();
  void alignHorizontal();
  void alignVertical();

  void alignLeftPreview();
  void alignBottomPreview();
  void alignRightPreview();
  void alignTopPreview();
  void alignHorizontalPreview();
  void alignVerticalPreview();

  void alignPreviewClear();
};

class CQDistButtons : public QWidget {
 public:
  CQDistButtons();
};

class CQSpreadButtons : public QWidget {
 public:
  CQSpreadButtons();
};

class CQAlignAnchorObject;
class CQAlignAnchorPoint;

class CQAlignAnchor : public QWidget {
  Q_OBJECT

 public:
  typedef CQIllustratorAlignToolbar::AnchorMode     AnchorMode;
  typedef CQIllustratorAlignToolbar::ObjectEdgeType ObjectEdgeType;

 public:
  CQAlignAnchor(QWidget *parent = 0);

  AnchorMode getMode() const;

  QString getObject() const;

  void setObject(const QString &name);

  ObjectEdgeType getObjectEdgeType() const;

  QPointF getPosition() const;

  void setPosition(const QPointF &pos);

  void resetSelectMode();

 private:
  void updateState();

 private:
  AnchorMode           mode_;
  QComboBox           *objectCombo_;
  QStackedWidget      *anchorStack_;
  QWidget             *anchorLabel_;
  CQAlignAnchorObject *anchorObject_;
  CQAlignAnchorPoint  *anchorPoint_;

 signals:
  void selectObject();
  void selectPosition();
  void cancelSelect();

 private slots:
  void objectSlot(const QString &obj);
};

class CQAlignAnchorObject : public QWidget {
  Q_OBJECT

 public:
  typedef CQIllustratorAlignToolbar::ObjectEdgeType ObjectEdgeType;

 public:
  CQAlignAnchorObject(QWidget *parent=NULL);

  QString getName() const;

  void setName(const QString &name);

  ObjectEdgeType getEdgeType() const { return edgeType_; }

  void resetSelectMode();

 private:
  ObjectEdgeType  edgeType_;
  QLineEdit      *nameEdit_;
  QToolButton    *edgeButton_;
  QToolButton    *selButton_;

 signals:
  void selectObject();
  void cancelSelect();

 private slots:
  void edgeSlot(QAction *);
  void selectSlot(bool);
};

class CQAlignAnchorPoint : public QWidget {
  Q_OBJECT

 public:
  CQAlignAnchorPoint(QWidget *parent=NULL);

  QPointF getValue() const;

  void setValue(const QPointF &point);

  void resetSelectMode();

 private:
  CQPointEdit *pointEdit_;
  QToolButton *selButton_;

 signals:
  void selectPoint();
  void cancelSelect();

 private slots:
  void selectSlot(bool);
};

class CQToolButton : public QToolButton {
  Q_OBJECT

 public:
  CQToolButton(const char **xpmData);

 private:
  bool event(QEvent*);

 signals:
  void previewStart();
  void previewStop();

 private:
  bool preview_;
};

#endif
