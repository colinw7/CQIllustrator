#ifndef CQINTEGER_EDIT_H
#define CQINTEGER_EDIT_H

#include <QLineEdit>

class CQIntegerEdit : public QLineEdit {
  Q_OBJECT

 private:
  int value_;

 public:
  CQIntegerEdit(QWidget *parent, int value=0);
  CQIntegerEdit(int value=0);

  virtual ~CQIntegerEdit() { }

  void setValue(int value);

  int getValue() const;

 private:
  void init(const QString &name, int value);

 private slots:
  void editingFinishedI();

 signals:
  void valueChanged(int);

 private:
  void valueToWidget();
  bool widgetToValue();
};

#endif
