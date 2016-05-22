#include <CLineDash.h>
#include <CQOptionToolSet.h>
#include <CQIllustratorShape.h>

class CQIllustratorShapeStroke;
class CQStrokeOptionTool;
class CQStrokeOptionSwab;
class CQColorChooser;
class QPaintEvent;
class CQLineDash;
class CQLineCap;
class CQLineJoin;
class QDoubleSpinBox;
class QCheckBox;

class CQStrokeOptionDialog : public CQOptionToolDialog {
  Q_OBJECT

 public:
  CQStrokeOptionDialog(CQStrokeOptionTool *tool);

  void setStroke(const CQIllustratorShapeStroke &stroke);

  const CQIllustratorShapeStroke &getStroke() const { return stroke_; }

 private:
  void initWidgets();
  void updateWidgets();

 private slots:
  void shownSlot  (int state);
  void colorSlot  (const QColor &color);
  void widthSlot  (double);
  void opacitySlot(double);
  void dashSlot   (const CLineDash &dash);
  void capSlot    (CLineCapType cap);
  void joinSlot   (CLineJoinType join);
  void mitreSlot  (double);

  void setDefaultSlot();

 signals:
  void valueChanged(const CQIllustratorShapeStroke &stroke);

 private:
  CQStrokeOptionTool*      tool_ { 0 };
  CQIllustratorShapeStroke stroke_;
  QCheckBox*               shownCheck_ { 0 };
  CQColorChooser*          colorChooser_ { 0 };
  QDoubleSpinBox*          widthEdit_ { 0 };
  QDoubleSpinBox*          opacityEdit_ { 0 };
  CQLineDash*              dashEdit_ { 0 };
  CQLineCap*               capEdit_ { 0 };
  CQLineJoin*              joinEdit_ { 0 };
  QDoubleSpinBox*          mitreEdit_ { 0 };
};
