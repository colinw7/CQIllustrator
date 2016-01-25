#include <CQIllustratorUndoDock.h>
#include <CQIllustrator.h>
#include <CQIllustratorUndo.h>

#include <QHBoxLayout>
#include <QLabel>
#include <QListWidget>
#include <QListWidgetItem>

#include <xpm/undo_redo_png.h>

class CQIllustratorUndoListGroupItem : public QListWidgetItem {
 public:
  CQIllustratorUndoListGroupItem(const CUndoGroup *group, int ind) :
   ind_(ind) {
    if (group)
      QListWidgetItem::setText(group->getDesc().c_str());
    else
      QListWidgetItem::setText("<Current>");
  }

  int getInd() const { return ind_; }

 private:
  int ind_;
};

class CQIllustratorUndoListDataItem : public QListWidgetItem {
 public:
  CQIllustratorUndoListDataItem(const CUndoData *data, int ind) :
   ind_(ind) {
    if (data)
      QListWidgetItem::setText(data->getDesc().c_str());
    else
      QListWidgetItem::setText("<Current>");
  }

  int getInd() const { return ind_; }

 private:
  int ind_;
};

CQIllustratorUndoDock::
CQIllustratorUndoDock(CQIllustrator *illustrator) :
 QWidget(0), illustrator_(illustrator)
{
  QHBoxLayout *layout = new QHBoxLayout(this);
  layout->setMargin(0); layout->setSpacing(0);

  QLabel *label = new QLabel;

  QPixmap pixmap;

  pixmap.loadFromData(undo_redo_data, UNDO_REDO_DATA_LEN);

  label->setPixmap(pixmap);

  list_ = new QListWidget;

  connect(list_, SIGNAL(itemClicked(QListWidgetItem *)),
          this, SLOT(itemSelectedSlot(QListWidgetItem *)));

  layout->addWidget(label);
  layout->addWidget(list_);

  CQIllustratorUndo *undo = illustrator_->getUndo();

  connect(undo, SIGNAL(undoChanged()), this, SLOT(undoChangedSlot()));
}

void
CQIllustratorUndoDock::
undoChangedSlot()
{
  reload();
}

void
CQIllustratorUndoDock::
itemSelectedSlot(QListWidgetItem *item)
{
  CQIllustratorUndoListGroupItem *item1 = dynamic_cast<CQIllustratorUndoListGroupItem *>(item);

  int ind = item1->getInd();

  CQIllustratorUndo *undo = illustrator_->getUndo();

  if      (ind < 0)
    undo->undo(-ind);
  else if (ind > 0)
    undo->redo( ind);
}

void
CQIllustratorUndoDock::
reload()
{
  CQIllustratorUndo *undo = illustrator_->getUndo();

  list_->clear();

  const CUndo::GroupList &undoList = undo->getUndoList();

  int ind = -undoList.size();

  CUndo::GroupList::const_iterator p1, p2;

  for (p1 = undoList.begin(), p2 = undoList.end(); p1 != p2; ++p1) {
    const CUndoGroup *group = *p1;

    const CUndoGroup::DataList &dataList = group->getDataList();

    if (dataList.empty()) continue;

    CQIllustratorUndoListGroupItem *item = new CQIllustratorUndoListGroupItem(group, ind);

   list_->addItem(item);

#if 0
    CUndoGroup::DataList::const_iterator pg1, pg2;

    for (pg1 = dataList.begin(), pg2 = dataList.end(); pg1 != pg2; ++pg1) {
      const CUndoData *data = *pg1;

      CQIllustratorUndoListDataItem *item = new CQIllustratorUndoListDataItem(data, ind);

      list_->addItem(item);

      ++ind;
    }
#endif

    ++ind;
  }

  CQIllustratorUndoListGroupItem *item = new CQIllustratorUndoListGroupItem(0, 0);

  list_->addItem(item);

  list_->setCurrentItem(item);

  ind = 1;

  const CUndo::GroupList &redoList = undo->getRedoList();

  for (p1 = redoList.begin(), p2 = redoList.end(); p1 != p2; ++p1) {
    const CUndoGroup *group = *p1;

    const CUndoGroup::DataList &dataList = group->getDataList();

    if (dataList.empty()) continue;

   CQIllustratorUndoListGroupItem *item = new CQIllustratorUndoListGroupItem(group, ind);

   list_->addItem(item);

#if 0
    CUndoGroup::DataList::const_iterator pg1, pg2;

    for (pg1 = dataList.begin(), pg2 = dataList.end(); pg1 != pg2; ++pg1) {
      const CUndoData *data = *pg1;

      CQIllustratorUndoListDataItem *item = new CQIllustratorUndoListDataItem(data, ind);

      list_->addItem(item);

      ++ind;
    }
#endif

   ++ind;
  }
}
