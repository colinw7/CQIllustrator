#ifndef CQOptionToolSet_H
#define CQOptionToolSet_H

#include <QFrame>
#include <QScrollArea>
#include <QToolButton>
#include <CQWinWidget.h>

#include <list>

class CQOptionToolSet;
class CQOptionToolArea;
class CQOptionToolDialogArea;

//! Widget for option dialog
class CQOptionToolDialog : public QWidget {
 public:
  CQOptionToolDialog();
};

//! Widget to contain option and popup option panel
class CQOptionTool : public QWidget {
  Q_OBJECT

 public:
  CQOptionTool();

  virtual CQOptionToolDialog *getDialog() = 0;

  CQOptionToolArea *getArea() const { return area_; }

  void setArea(CQOptionToolArea *area);

  void popup  (CQOptionToolDialogArea *dialogArea, QWidget *popupArea);
  void popdown(CQOptionToolDialogArea *area);

 signals:
  void poppingUp();
  void poppedUp();

  void poppingDown();
  void poppedDown();

 private:
  CQOptionToolArea *area_;
};

//! Widget to contain the popup dialog placed in the popup area
class CQOptionToolDialogArea : public CQWinWidget {
  Q_OBJECT

 public:
  CQOptionToolDialogArea(CQOptionToolSet *set);

  void setWidget(QWidget *w);

  void updateSize();

 private slots:
  void closedSlot();

 private:
  CQOptionToolSet *set_;
  QWidget         *w_;
};

class CQOptionToolMenu : public QToolButton {
  Q_OBJECT

 public:
  CQOptionToolMenu();

 private slots:
  void updatePixmap(bool checked);

 private:
  QPixmap on_pixmap_;
  QPixmap off_pixmap_;
};

//! widget to hold the tool and popup menu button
class CQOptionToolArea : public QFrame {
  Q_OBJECT

 public:
  CQOptionToolArea(CQOptionToolSet *toolset, CQOptionTool *tool);

  CQOptionTool *getTool() const { return tool_; }

  CQOptionToolMenu *getMenuButton() const { return menuButton_; }

 private slots:
  void menuPressed(bool show);

 private:
  CQOptionToolSet  *toolset_;
  CQOptionTool     *tool_;
  CQOptionToolMenu *menuButton_;
};

//! Widget to contain list of options
//! each option has a 'optional' popup floating option panel
class CQOptionToolSet : public QWidget {
  Q_OBJECT

 public:
  CQOptionToolSet(QWidget *parent=0);

  void setPopupArea(QWidget *popupArea);

  void addOption(CQOptionTool *tool);

  void popup(CQOptionToolArea *tool);
  void popdown(CQOptionToolArea *tool);

 public slots:
  void popdown();

 private:
  typedef std::list<QWidget *> ToolList;

  QWidget                *popupArea_;
  CQOptionToolDialogArea *dialogArea_;
  ToolList                toolList_;
  CQOptionToolArea       *currentTool_;
};

#endif
