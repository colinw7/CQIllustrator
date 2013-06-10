#ifndef CQLINE_DASH_H
#define CQLINE_DASH_H

#include <QLineEdit>
#include <CLineDash.h>

class QMenu;

class CQLineDash : public QLineEdit {
  Q_OBJECT

 public:
  CQLineDash(QWidget *parent=NULL);

  void setLineDash(const CLineDash &dash);

  CLineDash getLineDash() const;

 private:
  void contextMenuEvent(QContextMenuEvent *);

 private slots:
  void dashChangedSlot();
  void menuItemActivated(QAction *);

 signals:
  void valueChanged(const CLineDash &dash);

 private:
  CLineDash  dash_;
  QMenu     *menu_;
};

#endif
