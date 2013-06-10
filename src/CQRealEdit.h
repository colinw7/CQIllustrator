#ifndef CQREAL_EDIT_H
#define CQREAL_EDIT_H

#include <QLineEdit>
#include <QAction>

class QDoubleValidator;
class QContextMenuEvent;
class QMenu;

class CQRealEdit : public QLineEdit {
  Q_OBJECT

 public:
  CQRealEdit(QWidget *parent, double value=0.0);
  CQRealEdit(double value=0.0);

  virtual ~CQRealEdit() { }

  void setValue(double value);

  double getValue() const;

  void setRange(double lower, double upper, int decimals=0);

  void addMenuValue(double value);

 private:
  void init(const QString &name, double value);

  void contextMenuEvent(QContextMenuEvent *);

 private slots:
  void editingFinishedI();
  void menuItemActivated(QAction *);

 signals:
  void valueChanged(double);

 private:
  void valueToWidget();
  bool widgetToValue();

 private:
  double            value_;
  QDoubleValidator *validator_;
  QMenu            *menu_;
};

class CQRealEditAction : public QAction {
 public:
  CQRealEditAction(double value);

  double getValue() const { return value_; }

 private:
  double value_;
};

#endif
