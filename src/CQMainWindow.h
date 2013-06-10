#ifndef CQMAIN_WINDOW_H
#define CQMAIN_WINDOW_H

#include <QMainWindow>

class CQMainWindow : public QMainWindow {
  Q_OBJECT

 private:
  QString title_;

 public:
  CQMainWindow(const QString &title="");

  virtual ~CQMainWindow() { }

  void init();

 protected:
  virtual QWidget *createCentralWidget() { return NULL; }

  virtual void createWorkspace() { }
  virtual void createMenus() { }
  virtual void createToolBars() { }
  virtual void createStatusBar() { }
  virtual void createDockWindows() { }

  virtual void initTerm() { }
};

#endif
