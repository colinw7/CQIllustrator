#ifndef CQFillOptionDialog_H
#define CQFillOptionDialog_H

#include <CQOptionToolSet.h>
#include <CQIllustratorShapeFill.h>

class CQFillOptionTool;
class CQIllustratorShapeFill;
class CQGradientStopList;
class CQImageButton;
class CQImagePreview;
class CQPointEdit;
class CQRealEdit;
class CQColorChooser;
class QDoubleSpinBox;
class QStackedWidget;
class QCheckBox;
class QComboBox;

class CQFillOptionDialog : public CQOptionToolDialog {
  Q_OBJECT

 public:
  CQFillOptionDialog(CQFillOptionTool *tool);

 ~CQFillOptionDialog();

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

  void shownSlot   (int state);
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
  CQFillOptionTool       *tool_ { 0 };
  CQIllustratorShapeFill  fill_;

  // Mode Buttons
  CQImageButton *flatButton_  { 0 };
  CQImageButton *lgradButton_ { 0 };
  CQImageButton *rgradButton_ { 0 };
  CQImageButton *imageButton_ { 0 };

  QStackedWidget *stack_ { 0 };

  // Flat Widgets
  QCheckBox*      shownCheck_   { 0 };
  CQColorChooser* colorChooser_ { 0 };
  QDoubleSpinBox* opacityEdit_  { 0 };
  QComboBox*      fillRule_     { 0 };
  QCheckBox*      clipCheck_    { 0 };

  // Linear Gradient Widgets
  CQPointEdit        *lgrad1_     { 0 };
  CQPointEdit        *lgrad2_     { 0 };
  CQGradientStopList *lgradStops_ { 0 };

  // Radial Gradient Widgets
  CQPointEdit        *rgradC_     { 0 };
  CQRealEdit         *rgradR_     { 0 };
  CQPointEdit        *rgradF_     { 0 };
  CQGradientStopList *rgradStops_ { 0 };

  // Image Widgets
  CQImagePreview *imagePreview_ { 0 };
  QComboBox      *imageScale_   { 0 };
};

#endif
