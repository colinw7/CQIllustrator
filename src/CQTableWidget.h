#ifndef CQTableWidget_H
#define CQTableWidget_H

#include <QTableWidget>

#include <cassert>
#include <vector>
#include <map>

class CQTableWidget;

class CQTableWidgetItem : public QTableWidgetItem {
 public:
  // run-time type ID that is unique for each item
  // e.g. QTableWidgetItem::UserType + N
  static void setType(uint type) { type_ = type; }
  static uint getType() { return type_; }

  static bool isType(uint type) { return type == type_; }

  //------

  CQTableWidgetItem(CQTableWidget *t);

 ~CQTableWidgetItem() { }

  // clone new item
  virtual CQTableWidgetItem *clone() const = 0;

  // create edit widget for item
  virtual QWidget *createEditor(QWidget *parent) const = 0;

  // update edit widget from current item's data
  virtual void setEditorData() = 0;

  // update current item's data from edit widget
  virtual void getEditorData(QString &str) = 0;

  // get hint to item size
  virtual bool sizeHint(const QStyleOptionViewItem &, QSize &) const { return false; }

  // drawing override for static (not editted) item
  virtual bool paint(QPainter *, const QStyleOptionViewItem &) const { return false; }

  // set string value
  virtual QString getString() = 0;

  virtual bool validate() const { return true; }

  // draw background in correct color for item state when editing
  void paintBackground(QPainter *painter, const QStyleOptionViewItem &option) const;

  // set correct pen color to draw text
  void setTextPen(QPainter *painter, const QStyleOptionViewItem &option) const;

 protected:
  static uint type_;

  CQTableWidget *t_;
};

class CQTableWidget : public QTableWidget {
  Q_OBJECT

 public:
  CQTableWidget(QWidget* parent = 0);

  virtual ~CQTableWidget();

  void registerType(uint type, CQTableWidgetItem *item);

  void setColumnLabel(int col, const QString &label);
  void setRowLabel(int row, const QString &label);

  void setColumnLabels(const QStringList &labels);
  void setRowLabels(const QStringList &labels);

  template<typename T>
  T *createItem(int row, int column) {
    uint type = T::getType();

    return dynamic_cast<T *>(createItem(row, column, type));
  }

  template<typename T>
  T *getItem(int row, int column) {
    QTableWidgetItem *i = item(row, column);

    CQTableWidgetItem *i1 = dynamic_cast<CQTableWidgetItem *>(i);
    assert(i1 != NULL);

    assert(i1->getType() == T::getType());

    return dynamic_cast<T *>(i1);
  }

  CQTableWidgetItem *createItem(int row, int column, uint type);

  void setItem(int row, int column, CQTableWidgetItem *item);

  void setItem(int row, int column, const QString &str);

  void setItem(int row, int column, QWidget *w);

  void fixTableColumnWidths();

  void emitValueChanged(int row, int col);

 protected:
  void showEvent(QShowEvent *);

  void fixTableColumnWidths(QTableWidget *table, int max_len, bool init);

 protected:
  typedef std::map<uint,CQTableWidgetItem*> TypeList;

  TypeList types_;

 signals:
  void valueChanged(int row, int col);
};

#endif
