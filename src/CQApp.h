#ifndef CQAPP_H
#define CQAPP_H

//#define USE_OBJEDIT 1

#include <QApplication>

#define CQAppInst CQApp::getApp()

class CQApp : public QApplication {
  Q_OBJECT

 public:
  static CQApp *getApp() { return app_; }

  CQApp(int &argc, char **argv);

 ~CQApp() { }

#ifdef USE_OBJEDIT
  void addObjEditFilter(QWidget *widget);
#endif

 private:
  static CQApp *app_;
};

#ifdef USE_OBJEDIT
class CQAppObjEditFilter : public QObject {
  Q_OBJECT

 protected:
  bool eventFilter(QObject *obj, QEvent *event);
};
#endif

#endif
