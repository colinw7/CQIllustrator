#ifndef CQDOCKWIDGET_H
#define CQDOCKWIDGET_H

#include <QDockWidget>
#include <CQMenu.h>

#define CQDockMgrInst CQDockMgr::getInstance()

class CQDockMgr {
 private:
  CQMenu *toolsMenu_;

 public:
  static CQDockMgr *getInstance() {
    static CQDockMgr *instance;

    if (! instance)
      instance = new CQDockMgr;

    return instance;
  }

  CQMenu *getToolsMenu() const { return toolsMenu_; }

  CQMenu *addToolsMenu(QMainWindow *main_window) {
    toolsMenu_ = new CQMenu(main_window, "&Tools");

    return toolsMenu_;
  }

 private:
  CQDockMgr() :
   toolsMenu_(NULL) {
  }
};

class CQDockWidget {
 private:
  QMainWindow *main_window_;
  QDockWidget *dock_;
  QWidget     *widget_;

 public:
  CQDockWidget(QMainWindow *main_window, const QString &name, QWidget *w,
               Qt::DockWidgetArea area=Qt::RightDockWidgetArea) :
   main_window_(main_window), widget_(w) {
    dock_ = new QDockWidget(name, main_window);

    dock_->setWidget(widget_);

    main_window->addDockWidget(area, dock_);

    addToToolsMenu(dock_);
  }

  void addToToolsMenu(QDockWidget *dock) {
    CQMenu *toolsMenu = CQDockMgrInst->getToolsMenu();

    if (toolsMenu)
      toolsMenu->addAction(dock->toggleViewAction());
  }

  QMainWindow *getMainWindow() const { return main_window_; }

  QDockWidget *getDockWidget() const { return dock_; }

  QWidget *getWidget() const { return widget_; }

  void setAllowedAreas(Qt::DockWidgetAreas areas) {
    dock_->setAllowedAreas(areas);
  }
};

#endif
