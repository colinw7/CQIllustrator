#ifndef CQANGLE_SPINBOX_H
#define CQANGLE_SPINBOX_H

#include <QDoubleSpinBox>

class CQAngleSpinBox : public QDoubleSpinBox {
  Q_OBJECT

 public:
  CQAngleSpinBox(QWidget *parent, double value=0.0);
  CQAngleSpinBox(double value=0.0);

  virtual ~CQAngleSpinBox() { }

  double getValue() const;

 private:
  void init(const QString &name, double value);

 signals:
  void angleChanged(double);
};

#endif
