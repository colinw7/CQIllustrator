#ifndef CQANGLE_SPINBOX_H
#define CQANGLE_SPINBOX_H

#include <CAngle.h>
#include <QDoubleSpinBox>

class CQAngleSpinBox : public QDoubleSpinBox {
  Q_OBJECT

 public:
  CQAngleSpinBox(QWidget *parent, const CAngle &angle=CAngle());
  CQAngleSpinBox(const CAngle &angle=CAngle());

  virtual ~CQAngleSpinBox() { }

  const CAngle &getAngle() const { return angle_; }
  void setAngle(const CAngle &angle);

 private:
  void init();

 signals:
  void angleChanged(const CAngle &);

 private slots:
  void valueChangedSlot(double a);

 private:
  CAngle angle_;
};

#endif
