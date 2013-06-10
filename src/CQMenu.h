#ifndef CQMENU_H
#define CQMENU_H

#include <QMenu>
#include <QAction>
#include <QMainWindow>
#include <QMenuBar>

#include <map>

class CQMenuItem;

class CQMenu {
 private:
  typedef std::map<QString, CQMenuItem *> MenuItemMap;
  typedef std::map<QString, QAction *>    ActionMap;

  QMainWindow  *main_window_;
  QMenu        *menu_;
  QActionGroup *action_group_;
  MenuItemMap   menu_item_map_;
  ActionMap     action_map_;

 public:
  CQMenu(QMainWindow *main_window, const QString &name) :
   main_window_(main_window), menu_(NULL), action_group_(NULL) {
    if (name == "|")
      main_window->menuBar()->addSeparator();
    else
      menu_ = main_window->menuBar()->addMenu(name);
  }

  CQMenu(QWidget *widget, const QString &name="") :
   main_window_(NULL), menu_(NULL), action_group_(NULL) {
    menu_ = new QMenu(widget);

    if (name.length())
      menu_->setObjectName(name);
  }

  QMainWindow *getMainWindow() const { return main_window_; }

  QWidget *getParent() const {
    return (main_window_ ? main_window_ : menu_->parentWidget());
  }

  QMenu *getMenu() const { return menu_; }

  void startGroup() {
    action_group_ = new QActionGroup(main_window_);
  }

  void endGroup() {
    action_group_ = NULL;
  }

  void addMenuItem(CQMenuItem *menu_item);

  void addAction(QAction *action) {
    if (action_group_)
      action_group_->addAction(action);

    menu_->addAction(action);

    action_map_[action->text()] = action;
  }

  CQMenuItem *getMenuItem(const QString &name) {
    MenuItemMap::const_iterator p = menu_item_map_.find(name);

    if (p != menu_item_map_.end())
      return (*p).second;
    else
      return NULL;
  }

  QAction *getAction(const QString &name) {
    ActionMap::const_iterator p = action_map_.find(name);

    if (p != action_map_.end())
      return (*p).second;
    else
      return NULL;
  }

  void addSeparator() {
    menu_->addSeparator();
  }

  void exec(const QPoint &pos) {
    (void) menu_->exec(pos);
  }
};

class CQMenuItem {
 private:
  CQMenu  *menu_;
  QAction *action_;

 public:
  enum Type {
    NORMAL    = 0,
    CHECKABLE = (1<<0),
    CHECKED   = (1<<1)
  };

  CQMenuItem(CQMenu *menu, const QString &name, Type type = NORMAL) :
   menu_(menu) {
    action_ = new QAction(name, menu->getParent());

    menu_->addMenuItem(this);

    if ((type & CHECKABLE) || (type & CHECKED))
      setCheckable(true);

    if (type & CHECKED)
      setChecked(true);
  }

  CQMenuItem(CQMenu *menu, const QIcon &icon, const QString &name, Type type = NORMAL) :
   menu_(menu) {
    action_ = new QAction(icon, name, menu->getParent());

    menu_->addMenuItem(this);

    if ((type & CHECKABLE) || (type & CHECKED))
      setCheckable(true);

    if (type & CHECKED)
      setChecked(true);
  }

  CQMenuItem(CQMenu *menu, const QString &name, const QIcon &icon, Type type = NORMAL) :
   menu_(menu) {
    action_ = new QAction(icon, name, menu->getParent());

    menu_->addMenuItem(this);

    if ((type & CHECKABLE) || (type & CHECKED))
      setCheckable(true);

    if (type & CHECKED)
      setChecked(true);
  }

  QString getName() const { return action_->text(); }

  void setName(const std::string &name) { action_->setText(name.c_str()); }

  QAction *getAction() const { return action_; }

  void setShortcut(const QString &shortcut) {
    action_->setShortcut(shortcut);
  }

  void setStatusTip(const QString &tip) {
    action_->setStatusTip(tip);
  }

  void setIcon(uchar *data, uint len) {
    QPixmap pixmap;

    pixmap.loadFromData(data, len);

    QIcon icon(pixmap);

    action_->setIcon(icon);
  }

  void setXPMIcon(const char **xpm_data) {
    action_->setIcon(QIcon(QPixmap(xpm_data)));
  }

  bool isChecked() const {
    return action_->isChecked();
  }

  void setCheckable(bool checkable) {
    action_->setCheckable(checkable);
  }

  void setChecked(bool checked) {
    action_->setChecked(checked);
  }

  bool isEnabled() const {
    return action_->isEnabled();
  }

  void setEnabled(bool enabled) {
    action_->setEnabled(enabled);
  }

  bool connect(const char *signal, const QObject *recv, const char *method) {
    return QObject::connect(getAction(), signal, recv, method);
  }

  bool connect(const QObject *recv, const char *method) {
    return QObject::connect(getAction(), SIGNAL(triggered()), recv, method);
  }
};

inline void
CQMenu::
addMenuItem(CQMenuItem *menu_item)
{
  addAction(menu_item->getAction());

  menu_item_map_[menu_item->getName()] = menu_item;
}

#endif
