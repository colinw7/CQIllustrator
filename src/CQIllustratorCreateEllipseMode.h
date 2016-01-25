#ifndef CQIllustratorCreateEllipseMode_H
#define CQIllustratorCreateEllipseMode_H

#include <CQIllustratorMode.h>
#include <CQIllustratorToolbar.h>

#include <QComboBox>

class CQPointEdit;
class CQRealEdit;
class CQAngleSpinBox;
class CQIllustrator;
class CQIllustratorControlPointHandle;
class CQIllustratorCreateEllipseMode;
class CQIllustratorCreateEllipseSizer;

class CQEllipseShape2DConnectType : public QComboBox {
  Q_OBJECT

 public:
  CQEllipseShape2DConnectType(QWidget *parent=0);

  void setType(CEllipseConnectType value);

  CEllipseConnectType getType() const { return value_; }

 private slots:
  void itemSlot(const QString &name);

 signals:
  void valueChanged();

 private:
  std::map<QString,CEllipseConnectType> nameValue_;
  std::map<CEllipseConnectType,QString> valueName_;
  CEllipseConnectType                   value_;
};

class CQIllustratorCreateEllipseToolbar : public CQIllustratorToolbar {
  Q_OBJECT

 public:
  CQIllustratorCreateEllipseToolbar(CQIllustratorCreateEllipseMode *mode);

  const char *getTitle() const { return "Create Ellipse"; }

  QIcon getIcon();

  void addWidgets();

  void setSelectedShape(const CQIllustratorShape *shape);

  void setSize(const CBBox2D &bbox);

 private slots:
  void updateShape();

 private:
  CQIllustratorCreateEllipseMode *mode_;
  CQPointEdit                    *posEdit_;
  CQRealEdit                     *widthEdit_;
  CQRealEdit                     *heightEdit_;
  CQAngleSpinBox                 *angle1Edit_;
  CQAngleSpinBox                 *angle2Edit_;
  CQEllipseShape2DConnectType    *connectEdit_;
};

class CQIllustratorCreateEllipseSizer : public CQIllustratorModeSizer {
 public:
  CQIllustratorCreateEllipseSizer(CQIllustratorCreateEllipseMode *mode);

  void drawHandles(QPainter *painter, const CQIllustratorShape *shape);

 private:
  CQIllustratorControlPointHandle *rx_handle_;
  CQIllustratorControlPointHandle *ry_handle_;
  CQIllustratorControlPointHandle *a1_handle_;
  CQIllustratorControlPointHandle *a2_handle_;
};

class CQIllustratorCreateEllipseMode : public CQIllustratorMode {
  Q_OBJECT

 public:
  CQIllustratorCreateEllipseMode(CQIllustrator *illustrator);

  const char *getTitle() const { return "Create Ellipse"; }

  CQIllustratorCreateEllipseToolbar *getToolbar() { return toolbar_; }

  CQIllustratorCreateEllipseToolbar *createToolbar();

  CQIllustratorCreateEllipseSizer *createSizer();

  CQMenuItem *createMenuItem(CQMenu *menu);

  bool isCreateMode() const { return true; }

  CQIllustratorCreateEllipseToolbar *getToolbar() const { return toolbar_; }

  void handleMouseRelease(const MouseEvent &e);
  void handleMouseDrag   (const MouseEvent &e);

  void drawOverlay(CQIllustratorShapeDrawer *drawer);

 private:
  CQIllustratorCreateEllipseToolbar *toolbar_;
  CQIllustratorCreateEllipseSizer   *sizer_;
  bool                               equalSize_;
};

#endif
