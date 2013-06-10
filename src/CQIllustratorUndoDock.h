#ifndef CQIllustratorUndoDock_H
#define CQIllustratorUndoDock_H

#include <QWidget>

class CQIllustrator;
class QListWidget;
class QListWidgetItem;

class CQIllustratorUndoDock : public QWidget {
  Q_OBJECT

 public:
  CQIllustratorUndoDock(CQIllustrator *illustrator);

  void reload();

 private slots:
  void itemSelectedSlot(QListWidgetItem *item);

  void undoChangedSlot();

 private:
  CQIllustrator *illustrator_;
  QListWidget   *list_;
};

#endif
