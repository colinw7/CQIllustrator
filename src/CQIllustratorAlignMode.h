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
  enum class AnchorMode {
    SELECTION,
    OBJECT,
    POSITION
  };

  enum class ObjectEdgeType {
    LEFT_BOTTOM,
    RIGHT_TOP,
    MIDDLE
  };

 public:
  CQIllustratorAlignToolbar(CQIllustratorAlignMode *mode);

  const char *getTitle() const override { return "Align"; }

  QIcon getIcon() override;

  void addWidgets() override;

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
  CQAlignButtons         *alignw_ { 0 };
  CQDistButtons          *distw_ { 0 };
  CQSpreadButtons        *spreadw_ { 0 };
  CQRealEdit             *offset_ { 0 };
  CQAlignAnchor          *anchor_ { 0 };
};

//------

class CQIllustratorAlignMode : public CQIllustratorMode {
  Q_OBJECT

 public:
  CQIllustratorAlignMode(CQIllustrator *illustrator);

  const char *getTitle() const override { return "Align"; }

  CQIllustratorAlignToolbar *createToolbar() override;

  CQMenuItem *createMenuItem(CQMenu *menu) override;

  CQIllustratorAlignToolbar *getToolbar() const override { return toolbar_; }

  void handleMouseRelease(const MouseEvent &e) override;
  void handleMouseDrag   (const MouseEvent &e) override;

  void drawOverlay(CQIllustratorShapeDrawer *drawer) override;

  QCursor getCursor() const override;

  void align(CQIllustrator::AlignSide side, bool commit);

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

 private:
  CQIllustratorAlignToolbar *toolbar_ { 0 };
};

//------

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

//------

class CQDistButtons : public QWidget {
 public:
  CQDistButtons();
};

//------

class CQSpreadButtons : public QWidget {
 public:
  CQSpreadButtons();
};

//------

class CQAlignAnchorObject;
class CQAlignAnchorPoint;

class CQAlignAnchor : public QWidget {
  Q_OBJECT

 public:
  typedef CQIllustratorAlignToolbar::AnchorMode     AnchorMode;
  typedef CQIllustratorAlignToolbar::ObjectEdgeType ObjectEdgeType;

 public:
  CQAlignAnchor(QWidget *parent = nullptr);

  AnchorMode getMode() const;

  QString getObject() const;

  void setObject(const QString &name);

  ObjectEdgeType getObjectEdgeType() const;

  QPointF getPosition() const;

  void setPosition(const QPointF &pos);

  void resetSelectMode();

 private:
  void updateState();

 signals:
  void selectObject();
  void selectPosition();
  void cancelSelect();

 private slots:
  void objectSlot(const QString &obj);

 private:
  AnchorMode           mode_ { AnchorMode::SELECTION};
  QComboBox           *objectCombo_ { 0 };
  QStackedWidget      *anchorStack_ { 0 };
  QWidget             *anchorLabel_ { 0 };
  CQAlignAnchorObject *anchorObject_ { 0 };
  CQAlignAnchorPoint  *anchorPoint_ { 0 };
};

//------

class CQAlignAnchorObject : public QWidget {
  Q_OBJECT

 public:
  typedef CQIllustratorAlignToolbar::ObjectEdgeType ObjectEdgeType;

 public:
  CQAlignAnchorObject(QWidget *parent=nullptr);

  QString getName() const;

  void setName(const QString &name);

  ObjectEdgeType getEdgeType() const { return edgeType_; }

  void resetSelectMode();

 signals:
  void selectObject();
  void cancelSelect();

 private slots:
  void edgeSlot(QAction *);
  void selectSlot(bool);

 private:
  ObjectEdgeType  edgeType_ { CQIllustratorAlignToolbar::ObjectEdgeType::LEFT_BOTTOM };
  QLineEdit      *nameEdit_ { 0 };
  QToolButton    *edgeButton_ { 0 };
  QToolButton    *selButton_ { 0 };
};

//------

class CQAlignAnchorPoint : public QWidget {
  Q_OBJECT

 public:
  CQAlignAnchorPoint(QWidget *parent=nullptr);

  QPointF getValue() const;

  void setValue(const QPointF &point);

  void resetSelectMode();

 signals:
  void selectPoint();
  void cancelSelect();

 private slots:
  void selectSlot(bool);

 private:
  CQPointEdit *pointEdit_ { 0 };
  QToolButton *selButton_ { 0 };
};

//------

class CQToolButton : public QToolButton {
  Q_OBJECT

 public:
  CQToolButton(const QIcon &icon);

 private:
  bool event(QEvent*) override;

 signals:
  void previewStart();
  void previewStop();

 private:
  bool preview_ { false };
};

#endif
