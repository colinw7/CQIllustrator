#include <CQTableWidget.h>

#include <QTableWidget>
#include <QTableWidgetItem>
#include <QItemDelegate>
#include <QHeaderView>
#include <QStringList>
#include <QFont>
#include <QFontMetrics>
#include <QPainter>
#include <QAbstractItemView>

//----------------------------

class CQTableWidgetDelegate : public QItemDelegate {
 public:
  CQTableWidgetDelegate(CQTableWidget *t) :
   QItemDelegate(t), t_(t) {
  }

  // Override to create editor
  QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                        const QModelIndex &index) const;

  // Override to get content from editor
  void setEditorData(QWidget *editor, const QModelIndex &index) const;

  // Override to set editor from content
  void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;

  QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;

  void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option,
                            const QModelIndex &index) const;

  void paint(QPainter *painter, const QStyleOptionViewItem &option,
             const QModelIndex &index) const;

 private:
  CQTableWidget *t_;
};

//----------------------------

uint CQTableWidgetItem::type_ = 0;

//----------------------------

CQTableWidget::
CQTableWidget(QWidget* parent) :
 QTableWidget(parent)
{
  verticalHeader()->hide();

  // Set Item Delegate
  CQTableWidgetDelegate *delegate = new CQTableWidgetDelegate(this);

  setItemDelegate(delegate);
  setEditTriggers(QAbstractItemView::AllEditTriggers);

  setSelectionMode(QAbstractItemView::SingleSelection);
  setSelectionBehavior(QAbstractItemView::SelectRows);

  // Set the value column to stretch to fit the table and other table attributes.
  QHeaderView *header = horizontalHeader();

  header->setStretchLastSection(true) ;

  setEditTriggers(QAbstractItemView::DoubleClicked | QAbstractItemView::EditKeyPressed);

  //setSortingEnabled(false);

  //setRowMovingEnabled(false);
  //setColumnMovingEnabled(false);
}

CQTableWidget::
~CQTableWidget()
{
}

void
CQTableWidget::
registerType(uint type, CQTableWidgetItem *item)
{
  assert(type > QTableWidgetItem::UserType);

  item->setType(type);

  types_[type] = item;
}

void
CQTableWidget::
setColumnLabel(int col, const QString &label)
{
  QTableWidgetItem *item = horizontalHeaderItem(col);

  if (! item) {
    item = new QTableWidgetItem;

    setHorizontalHeaderItem(col, item);
  }

  item->setText(label);
}

void
CQTableWidget::
setRowLabel(int row, const QString &label)
{
  verticalHeader()->show();

  QTableWidgetItem *item = verticalHeaderItem(row);

  if (! item) {
    item = new QTableWidgetItem;

    setVerticalHeaderItem(row, item);
  }

  item->setText(label);
}

void
CQTableWidget::
setColumnLabels(const QStringList &labels)
{
  setHorizontalHeaderLabels(labels);
}

void
CQTableWidget::
setRowLabels(const QStringList &labels)
{
  verticalHeader()->show();

  setVerticalHeaderLabels(labels);
}

CQTableWidgetItem *
CQTableWidget::
createItem(int row, int column, uint type)
{
  assert(types_.find(type) != types_.end());

  CQTableWidgetItem *item = types_[type]->clone();

  QTableWidget::setItem(row, column, item);

  return item;
}

void
CQTableWidget::
setItem(int row, int column, CQTableWidgetItem *item)
{
  QTableWidget::setItem(row, column, item);

  QModelIndex index = model()->index(row, column);

  model()->setData(index, item->getString());
}

void
CQTableWidget::
setItem(int row, int column, const QString &str)
{
  QTableWidgetItem *item = new QTableWidgetItem(str);

  item->setFlags(item->flags() & !Qt::ItemIsEditable);

  QTableWidget::setItem(row, column, item);
}

void
CQTableWidget::
setItem(int row, int column, QWidget *w)
{
  QTableWidget::setCellWidget(row, column, w);
}

void
CQTableWidget::
showEvent(QShowEvent *)
{
  resizeColumnsToContents();

  //fixTableColumnWidths(this, 16, true);
}

void
CQTableWidget::
fixTableColumnWidths()
{
  resizeColumnsToContents();

  //fixTableColumnWidths(this, 16, false);
}

void
CQTableWidget::
fixTableColumnWidths(QTableWidget *table, int max_len, bool init)
{
  const QFont &font = table->font();

  QFontMetrics fm(font);

  int num_cols = table->columnCount();

  for (int c = 0; c < num_cols - 1; ++c) {
    if (table->isColumnHidden(c)) continue;

    int width = (init ? 0 : table->columnWidth(c));

    if (table->horizontalHeader()->isVisible()) {
      QTableWidgetItem *item = table->horizontalHeaderItem(c);

      if (item) {
        QVariant var = item->data(Qt::DisplayRole);

        QString str = var.toString();

        if (str.length() <= max_len)
          width = qMax(width, fm.width(str) + 10);
      }
    }

    int num_rows = table->rowCount();

    for (int r = 0; r < num_rows; ++r) {
      QTableWidgetItem *item = table->item(r, c);

      if (item) {
        QVariant var = item->data(Qt::DisplayRole);

        QString str = var.toString();

        if (str.length() <= max_len)
          width = qMax(width, fm.width(str) + 28);

        QWidget *w = table->cellWidget(r, c);

        if (w)
          width = qMax(width, w->width());
      }
    }

    table->setColumnWidth(c, qMax(24, width));
  }
}

void
CQTableWidget::
emitValueChanged(int row, int col)
{
  emit valueChanged(row, col);
}

//----------------------------

QWidget *
CQTableWidgetDelegate::
createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
  QTableWidgetItem *item = t_->item(index.row(), index.column());

  CQTableWidgetItem *item1 = dynamic_cast<CQTableWidgetItem *>(item);

  if (item1) {
    QWidget *w = item1->createEditor(parent);

    if (w)
      w->installEventFilter(const_cast<CQTableWidgetDelegate*>(this));

    return w;
  }
  else
    return QItemDelegate::createEditor(parent, option, index);
}

void
CQTableWidgetDelegate::
setEditorData(QWidget *, const QModelIndex &index) const
{
  QTableWidgetItem *item = t_->item(index.row(), index.column());

  CQTableWidgetItem *item1 = dynamic_cast<CQTableWidgetItem *>(item);

  if (item1)
    item1->setEditorData();
}

void
CQTableWidgetDelegate::
setModelData(QWidget *, QAbstractItemModel *model, const QModelIndex &index) const
{
  QTableWidgetItem *item = t_->item(index.row(), index.column());

  CQTableWidgetItem *item1 = dynamic_cast<CQTableWidgetItem *>(item);

  if (item1) {
    QString str;

    item1->getEditorData(str);

    model->setData(index, str);

    t_->emitValueChanged(index.row(), index.column());
  }
}

QSize
CQTableWidgetDelegate::
sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
  QTableWidgetItem *item = t_->item(index.row(), index.column());

  CQTableWidgetItem *item1 = dynamic_cast<CQTableWidgetItem *>(item);

  if (item1) {
    QSize size;

    if (item1->sizeHint(option, size))
      return size;
  }

  return QItemDelegate::sizeHint(option, index);
}

void
CQTableWidgetDelegate::
updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option,
                     const QModelIndex &/* index */) const
{
  editor->setGeometry(option.rect);
}

void
CQTableWidgetDelegate::
paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
  QTableWidgetItem *item = t_->item(index.row(), index.column());

  CQTableWidgetItem *item1 = dynamic_cast<CQTableWidgetItem *>(item);

  if (! item1 || ! item1->paint(painter, option))
    QItemDelegate::paint(painter, option, index);
}

//------------------------------

CQTableWidgetItem::
CQTableWidgetItem(CQTableWidget *t) :
 t_(t)
{
}

void
CQTableWidgetItem::
paintBackground(QPainter *painter, const QStyleOptionViewItem &option) const
{
  if (option.state & QStyle::State_Selected) {
    QPalette::ColorGroup cg =
     (option.state & QStyle::State_Enabled ? QPalette::Normal : QPalette::Disabled);

    painter->fillRect(option.rect, option.palette.brush(cg, QPalette::Highlight));
  }
}

void
CQTableWidgetItem::
setTextPen(QPainter *painter, const QStyleOptionViewItem &option) const
{
  QPalette::ColorGroup cg =
   (option.state & QStyle::State_Enabled ? QPalette::Normal : QPalette::Disabled);

  if (option.state & QStyle::State_Selected)
    painter->setPen(option.palette.color(cg, QPalette::HighlightedText));
  else
    painter->setPen(option.palette.color(cg, QPalette::Text));
}

//------------------------------

#if 0
CQObjEditStdItem::
CQObjEditStdItem(const QString &label) :
 CQTableWidgetItem(CQOBJ_EDIT_STD_ITEM_ID)
{
  setText(label);

  setFlags(flags() | Qt::ItemIsEditable);
}

CQObjEditStdItem::
CQObjEditStdItem(const CQObjEditStdItem &item) :
 QTreeWidgetItem(item)
{
  for (int i = 0; i < item.childCount(); ++i)
    setText(i, item.text(i));

  setFlags(flags() | Qt::ItemIsEditable);
}

bool
CQObjEditStdItem::
paint(QPainter *, const QStyleOptionViewItem &, const QModelIndex &) const
{
  return false;
}
#endif
