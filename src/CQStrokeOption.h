#include <CLineDash.h>
#include <CQOptionToolSet.h>
#include <CQIllustratorShape.h>

class CQIllustratorShapeStroke;
class CQStrokeOptionDialog;
class CQStrokeOptionSwab;
class CQColorChooser;
class QPaintEvent;
class CQLineDash;
class CQLineCap;
class CQLineJoin;
class QDoubleSpinBox;

class CQStrokeOptionTool : public CQOptionTool {
  Q_OBJECT

 public:
  CQStrokeOptionTool(CQIllustrator *illustrator);

  CQOptionToolDialog *getDialog();

  CQIllustrator *getIllustrator() const { return illustrator_; }

 signals:
  void valueChanged(const CQIllustratorShapeStroke &stroke);

 private slots:
  void selectionChangedSlot();

 private:
  CQIllustrator        *illustrator_;
  CQStrokeOptionDialog *dialog_;
  CQStrokeOptionSwab   *swab_;
};

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
  CQStrokeOptionTool       *tool_;
  CQIllustratorShapeStroke  stroke_;
  CQColorChooser           *colorChooser_;
  QDoubleSpinBox           *widthEdit_;
  QDoubleSpinBox           *opacityEdit_;
  CQLineDash               *dashEdit_;
  CQLineCap                *capEdit_;
  CQLineJoin               *joinEdit_;
  QDoubleSpinBox           *mitreEdit_;
};
