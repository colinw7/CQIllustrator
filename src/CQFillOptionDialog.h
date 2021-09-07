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
class CQRealSpin;

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
  CQFillOptionTool       *tool_ { nullptr };
  CQIllustratorShapeFill  fill_;

  // Mode Buttons
  CQImageButton *flatButton_  { nullptr };
  CQImageButton *lgradButton_ { nullptr };
  CQImageButton *rgradButton_ { nullptr };
  CQImageButton *imageButton_ { nullptr };

  QStackedWidget *stack_ { nullptr };

  // Flat Widgets
  QCheckBox*      shownCheck_   { nullptr };
  CQColorChooser* colorChooser_ { nullptr };
  CQRealSpin*     opacityEdit_  { nullptr };
  QComboBox*      fillRule_     { nullptr };
  QCheckBox*      clipCheck_    { nullptr };

  // Linear Gradient Widgets
  CQPointEdit        *lgrad1_     { nullptr };
  CQPointEdit        *lgrad2_     { nullptr };
  CQGradientStopList *lgradStops_ { nullptr };

  // Radial Gradient Widgets
  CQPointEdit        *rgradC_     { nullptr };
  CQRealEdit         *rgradR_     { nullptr };
  CQPointEdit        *rgradF_     { nullptr };
  CQGradientStopList *rgradStops_ { nullptr };

  // Image Widgets
  CQImagePreview *imagePreview_ { nullptr };
  QComboBox      *imageScale_   { nullptr };
};

#endif
