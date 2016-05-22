#include <CLineDash.h>
#include <CQOptionToolSet.h>
#include <CQIllustratorShape.h>

class CQIllustratorShapeStroke;
class CQStrokeOptionDialog;
class CQStrokeOptionSwab;

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
  CQIllustrator        *illustrator_ { 0 };
  CQStrokeOptionDialog *dialog_ { 0 };
  CQStrokeOptionSwab   *swab_ { 0 };
};
