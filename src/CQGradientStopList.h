#ifndef CQGRADIENT_STOP_LIST_H
#define CQGRADIENT_STOP_LIST_H

#include <QPointer>

#include <CQTableWidget.h>
#include <CGenGradient.h>

class CQGradientStopList;
class CQGradientStopTable;
class CQColorChooser;
class CQRealEdit;

//------

class CQGradientStopColorItem : public CQTableWidgetItem {
 public:
  enum { TYPE = QTableWidgetItem::UserType + 1 };

  CQGradientStopColorItem(CQGradientStopTable *t, uint ind, const CRGBA &rgba);

  const CRGBA &getColor() const { return rgba_; }

  CQGradientStopColorItem *clone() const;

  QString getString();

  QWidget *createEditor(QWidget *parent) const;

  void setEditorData();

  void getEditorData(QString &str);

  bool sizeHint(const QStyleOptionViewItem &option, QSize &size) const;

  bool paint(QPainter *painter, const QStyleOptionViewItem &option) const;

 private:
  CQGradientStopTable              *table_ { 0 };
  uint                              ind_ { 0 };
  CRGBA                             rgba_;
  mutable QPointer<CQColorChooser>  edit_;
};

//------

class CQGradientStopAlphaItem : public CQTableWidgetItem {
 public:
  enum { TYPE = QTableWidgetItem::UserType + 2 };

  CQGradientStopAlphaItem(CQGradientStopTable *t, uint ind, double offset);

  double getAlpha() const { return alpha_; }

  CQGradientStopAlphaItem *clone() const;

  QString getString();

  QWidget *createEditor(QWidget *parent) const;

  void setEditorData();

  void getEditorData(QString &str);

  QString toString() const;

 private:
  CQGradientStopTable          *table_ { 0 };
  uint                          ind_ { 0 };
  double                        alpha_ { 0 };
  mutable QPointer<CQRealEdit>  edit_;
};

//------

class CQGradientStopOffsetItem : public CQTableWidgetItem {
 public:
  enum { TYPE = QTableWidgetItem::UserType + 2 };

  CQGradientStopOffsetItem(CQGradientStopTable *t, uint ind, double offset);

  double getOffset() const { return offset_; }

  CQGradientStopOffsetItem *clone() const;

  QString getString();

  QWidget *createEditor(QWidget *parent) const;

  void setEditorData();

  void getEditorData(QString &r);

  QString toString() const;

 private:
  CQGradientStopTable          *table_ { 0 };
  uint                          ind_ { 0 };
  double                        offset_ { 0 };
  mutable QPointer<CQRealEdit>  edit_;
};

//------

class CQGradientStopTable : public CQTableWidget {
  Q_OBJECT

 public:
  CQGradientStopTable(CQGradientStopList *list);

  void clear();

 private slots:
  void updateStopsSlot();

 private:
  CQGradientStopList *list_ { 0 };
};

//------

class CQGradientStopList : public QWidget {
  Q_OBJECT

 public:
  CQGradientStopList(QWidget *parent=0);

  void init(const CGenGradient &g);

  const CGenGradient::StopList &getStops() const { return stops_; }

  CGenGradient::StopList &getStops() { return stops_; }

  void emitStopsChanged();

 signals:
  void stopsChanged();

 private:
  CQGradientStopTable    *table_ { 0 };
  CGenGradient::StopList  stops_;
};

#endif
