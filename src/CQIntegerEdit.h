#ifndef CQINTEGER_EDIT_H
#define CQINTEGER_EDIT_H

#include <QLineEdit>

class CQIntegerEdit : public QLineEdit {
  Q_OBJECT

 public:
  CQIntegerEdit(QWidget *parent, int value=0);
  CQIntegerEdit(int value=0);

  virtual ~CQIntegerEdit() { }

  int getValue() const;
  void setValue(int value);

 private:
  void init(const QString &name, int value);

 private slots:
  void editingFinishedI();

 signals:
  void valueChanged(int);

 private:
  void valueToWidget();
  bool widgetToValue();

 private:
  int value_ { 0 };
};

#endif
