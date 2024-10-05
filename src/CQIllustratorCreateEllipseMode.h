#ifndef CQIllustratorCreateEllipseMode_H
#define CQIllustratorCreateEllipseMode_H

#include <CQIllustratorMode.h>
#include <CQIllustratorToolbar.h>
#include <CQIllustratorCreateEllipseMode.h>
#include <CQIllustratorEllipseShape.h>

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
  CQEllipseShape2DConnectType(QWidget *parent=nullptr);

  void setType(CQIllustratorEllipseShape::ConnectType value);

  CQIllustratorEllipseShape::ConnectType getType() const { return value_; }

 private slots:
  void itemSlot(const QString &name);

 signals:
  void valueChanged();

 private:
  typedef CQIllustratorEllipseShape::ConnectType ConnectType;

  typedef std::map<QString,CQIllustratorEllipseShape::ConnectType> NameValue;
  typedef std::map<CQIllustratorEllipseShape::ConnectType,QString> ValueName;

  NameValue   nameValue_;
  ValueName   valueName_;
  ConnectType value_ { ConnectType::LINE };
};

//------

class CQIllustratorCreateEllipseToolbar : public CQIllustratorToolbar {
  Q_OBJECT

 public:
  CQIllustratorCreateEllipseToolbar(CQIllustratorCreateEllipseMode *mode);

  const char *getTitle() const override { return "Create Ellipse"; }

  QIcon getIcon() override;

  void addWidgets() override;

  void setSelectedShape(const CQIllustratorShape *shape) override;

  void setSize(const CBBox2D &bbox);

 private slots:
  void updateShape();

 private:
  CQIllustratorCreateEllipseMode *mode_ { nullptr };
  CQPointEdit                    *posEdit_ { nullptr };
  CQRealEdit                     *widthEdit_ { nullptr };
  CQRealEdit                     *heightEdit_ { nullptr };
  CQAngleSpinBox                 *angle1Edit_ { nullptr };
  CQAngleSpinBox                 *angle2Edit_ { nullptr };
  CQEllipseShape2DConnectType    *connectEdit_ { nullptr };
};

//------

class CQIllustratorCreateEllipseSizer : public CQIllustratorModeSizer {
 public:
  CQIllustratorCreateEllipseSizer(CQIllustratorCreateEllipseMode *mode);

  void drawHandles(QPainter *painter, const CQIllustratorShape *shape) override;

 private:
  CQIllustratorControlPointHandle *rx_handle_ { nullptr };
  CQIllustratorControlPointHandle *ry_handle_ { nullptr };
  CQIllustratorControlPointHandle *a1_handle_ { nullptr };
  CQIllustratorControlPointHandle *a2_handle_ { nullptr };
};

//------

class CQIllustratorCreateEllipseMode : public CQIllustratorMode {
  Q_OBJECT

 public:
  CQIllustratorCreateEllipseMode(CQIllustrator *illustrator);

  const char *getTitle() const override { return "Create Ellipse"; }

  CQIllustratorCreateEllipseToolbar *getToolbar() { return toolbar_; }

  CQIllustratorCreateEllipseToolbar *createToolbar() override;

  CQIllustratorCreateEllipseSizer *createSizer() override;

  CQMenuItem *createMenuItem(CQMenu *menu) override;

  bool isCreateMode() const override { return true; }

  CQIllustratorCreateEllipseToolbar *getToolbar() const override { return toolbar_; }

  void handleMouseRelease(const MouseEvent &e) override;
  void handleMouseDrag   (const MouseEvent &e) override;

  void drawOverlay(CQIllustratorShapeDrawer *drawer) override;

 private:
  CQIllustratorCreateEllipseToolbar *toolbar_ { nullptr };
  CQIllustratorCreateEllipseSizer   *sizer_ { nullptr };
  bool                               equalSize_ { false };
};

#endif
