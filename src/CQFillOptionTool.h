#ifndef CQFillOptionTool_H
#define CQFillOptionTool_H

#include <CQOptionToolSet.h>
#include <CQIllustratorShape.h>

class CQFillOptionDialog;
class CQFillOptionSwab;
class CQIllustratorShapeFill;

class CQFillOptionTool : public CQOptionTool {
  Q_OBJECT

 public:
  CQFillOptionTool(CQIllustrator *illustrator);

  CQOptionToolDialog *getDialog() override;

  CQIllustrator *getIllustrator() const { return illustrator_; }

 private slots:
  void selectionChangedSlot();

 signals:
  void valueChanged(const CQIllustratorShapeFill &fill);
  void clipChanged(bool);

 private:
  CQIllustrator      *illustrator_ { 0 };
  CQFillOptionDialog *dialog_      { 0 };
  CQFillOptionSwab   *swab_        { 0 };
};

#endif
