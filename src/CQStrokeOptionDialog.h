#include <CLineDash.h>
#include <CQOptionToolSet.h>
#include <CQIllustratorShape.h>

class CQIllustratorShapeStroke;
class CQStrokeOptionTool;
class CQStrokeOptionSwab;
class CQColorChooser;
class CQLineDash;
class CQLineCap;
class CQLineJoin;
class CQRealSpin;

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
  CQStrokeOptionTool*      tool_        { nullptr };
  CQIllustratorShapeStroke stroke_;
  QCheckBox*               shownCheck_   { nullptr };
  CQColorChooser*          colorChooser_ { nullptr };
  CQRealSpin*              widthEdit_    { nullptr };
  CQRealSpin*              opacityEdit_  { nullptr };
  CQLineDash*              dashEdit_     { nullptr };
  CQLineCap*               capEdit_      { nullptr };
  CQLineJoin*              joinEdit_     { nullptr };
  CQRealSpin*              mitreEdit_    { nullptr };
};
