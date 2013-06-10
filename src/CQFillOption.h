#include <CQOptionToolSet.h>
#include <CQIllustratorShape.h>

class QPaintEvent;
class QStackedWidget;
class QComboBox;
class QCheckBox;
class QDoubleSpinBox;

class CQFillOptionDialog;
class CQFillOptionSwab;
class CQImageButton;
class CQColorChooser;
class CQRealEdit;
class CQPointEdit;
class CQGradientStopList;
class CQImagePreview;
class CQIllustratorShapeFill;

class CQFillOptionTool : public CQOptionTool {
  Q_OBJECT

 public:
  CQFillOptionTool(CQIllustrator *illustrator);

  CQOptionToolDialog *getDialog();

  CQIllustrator *getIllustrator() const { return illustrator_; }

 private slots:
  void selectionChangedSlot();

 signals:
  void valueChanged(const CQIllustratorShapeFill &fill);
  void clipChanged(bool);

 private:
  CQIllustrator      *illustrator_;
  CQFillOptionDialog *dialog_;
  CQFillOptionSwab   *swab_;
};

class CQFillOptionDialog : public CQOptionToolDialog {
  Q_OBJECT

 public:
  CQFillOptionDialog(CQFillOptionTool *tool);

  void setFill(const CQIllustratorShapeFill &fill);

  const CQIllustratorShapeFill &getFill() const { return fill_; }

 private:
  void initWidgets();
  void setActiveState();
  void updateWidgets();

 private slots:
  void flatSlot();
  void lgradSlot();
  void rgradSlot();
  void imageSlot();

  void colorSlot   (const QColor &color);
  void opacitySlot (double value);
  void fillRuleSlot(const QString &value);
  void clipSlot    ();

  void updateLGrad();
  void updateLGradStops();
  void addLGradStop();
  void removeLGradStop();

  void updateRGrad();
  void updateRGradStops();
  void addRGradStop();
  void removeRGradStop();

  void setImageSlot();
  void imageScaleSlot(const QString &value);

  void setDefaultSlot();
  void setBackgroundSlot();

 signals:
  void valueChanged(const CQIllustratorShapeFill &fill);
  void clipChanged(bool);

 private:
  CQFillOptionTool       *tool_;
  CQIllustratorShapeFill  fill_;

  CQImageButton *flatButton_;
  CQImageButton *lgradButton_;
  CQImageButton *rgradButton_;
  CQImageButton *imageButton_;

  QStackedWidget *stack_;

  // Flat Widgets

  CQColorChooser *colorChooser_;
  QDoubleSpinBox *opacityEdit_;
  QComboBox      *fillRule_;
  QCheckBox      *clipCheck_;

  // Linear Gradient Widgets

  CQPointEdit        *lgrad1_;
  CQPointEdit        *lgrad2_;
  CQGradientStopList *lgradStops_;

  // Radial Gradient Widgets

  CQPointEdit        *rgradC_;
  CQRealEdit         *rgradR_;
  CQPointEdit        *rgradF_;
  CQGradientStopList *rgradStops_;

  // Image Widgets

  CQImagePreview *imagePreview_;
  QComboBox      *imageScale_;
};
