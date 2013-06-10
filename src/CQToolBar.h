#ifndef CQTOOLBAR_H
#define CQTOOLBAR_H

#include <QToolBar>
#include <QAbstractButton>

#include <CQMenu.h>

class CQToolBar {
 private:
  QMainWindow  *main_window_;
  QToolBar     *toolbar_;

 public:
  CQToolBar(QMainWindow *main_window, const QString &name,
            Qt::ToolBarArea area=Qt::TopToolBarArea) :
   main_window_(main_window) {
    toolbar_ = new QToolBar(name);

    main_window->addToolBar(area, toolbar_);

    setIconSize(QSize(16,16));
  }

  QMainWindow *getMainWindow() const { return main_window_; }

  QToolBar *getToolBar() const { return toolbar_; }

  void setIconSize(const QSize &iconSize) {
    toolbar_->setIconSize(iconSize);
  }

  QAction *addItem(CQMenuItem *item) {
    QAction *action = item->getAction();

    return addItem(action);
  }

  QAction *addItem(QAction *action) {
    toolbar_->addAction(action);

    return action;
  }

  QAction *addWidget(QWidget *widget) {
    return toolbar_->addWidget(widget);
  }

  QAction *addWidget(QWidget *widget, bool checked) {
    QAction *action = toolbar_->addWidget(widget);

    QAbstractButton *button = qobject_cast<QAbstractButton *>(widget);

    if (button) {
      button->setCheckable(true);
      button->setChecked(checked);
    }

    return action;
  }

  QAction *addSeparator() {
    return toolbar_->addSeparator();
  }

  void setAllowedAreas(Qt::ToolBarAreas areas) {
    toolbar_->setAllowedAreas(areas);
  }

  void setVisible(bool visible) {
    toolbar_->setVisible(visible);
  }
};

#endif
