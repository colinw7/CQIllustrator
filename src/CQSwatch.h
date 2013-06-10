#ifndef CQSwatch_H
#define CQSwatch_H

#include <QWidget>

class QGridLayout;

class CQSwatch : public QWidget {
 public:
  CQSwatch();
  CQSwatch(QWidget *widget);
  CQSwatch(const char *title, QWidget *w);
  CQSwatch(const char *title, QWidget *w1, QWidget *w2);
  CQSwatch(const char *title, QWidget *w1, QWidget *w2, QWidget *w3);
  CQSwatch(const char *title, QWidget *w1, QWidget *w2, QWidget *w3, QWidget *w4);

  void setTitle(const char *title);

  void addWidget(QWidget *w);

  void addWidget(QWidget *w, int rows, int cols);

  void addSeparator();

 private:
  void init();

 private:
  QGridLayout *grid_;
  int          pos_;
  bool         has_title_;
};

#endif
