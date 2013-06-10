#ifndef CQPOINT_EDIT_H
#define CQPOINT_EDIT_H

#include <QLineEdit>
#include <QValidator>
#include <CPoint2D.h>

class CQPointEdit : public QLineEdit {
  Q_OBJECT

 private:
  CPoint2D point_;

 public:
  CQPointEdit(QWidget *parent, const CPoint2D &value=CPoint2D(0,0));
  CQPointEdit(const CPoint2D &value=CPoint2D(0,0));

  virtual ~CQPointEdit() { }

  void setValue(const CPoint2D &point=CPoint2D(0,0));

  const CPoint2D &getValue() const;

 private:
  void init(const QString &name, const CPoint2D &value);

 private slots:
  void editingFinishedI();
  void returnPressedI();

 signals:
  void valueChanged();
  void valueAccepted();

 private:
  void pointToWidget();

  bool widgetToPoint();

 public:
  static std::string pointToString(const CPoint2D &point);

  static QValidator::State stringToPoint(const std::string &text, CPoint2D &point);
};

#endif
