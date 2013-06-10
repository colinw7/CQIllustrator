#include "CQGradientStopList.h"
#include <CQUtil.h>
#include <CQColorChooser.h>
#include <CQRealEdit.h>

#include <QVBoxLayout>
#include <QPainter>

CQGradientStopList::
CQGradientStopList(QWidget *parent) :
 QWidget(parent)
{
  QVBoxLayout *layout = new QVBoxLayout(this);

  table_ = new CQGradientStopTable(this);

  layout->addWidget(table_);
}

void
CQGradientStopList::
init(const CGenGradient &g)
{
  stops_ = g.getStops();

  table_->clear();

  table_->setRowCount(stops_.size());

  uint ind = 0;

  CGenGradient::StopList::iterator p1, p2;

  for (p1 = stops_.begin(), p2 = stops_.end(); p1 != p2; ++p1, ++ind) {
    const CGradientStop &stop = *p1;

    const CRGBA &rgba = stop.getColor();

    CRGBA  rgba1 = rgba.solid();
    double alpha = rgba.getAlpha();

    CQGradientStopColorItem  *item1 = new CQGradientStopColorItem (table_, ind, rgba1);
    CQGradientStopAlphaItem  *item2 = new CQGradientStopAlphaItem (table_, ind, alpha);
    CQGradientStopOffsetItem *item3 = new CQGradientStopOffsetItem(table_, ind, stop.getOffset());

    table_->setItem(ind, 0, item1);
    table_->setItem(ind, 1, item2);
    table_->setItem(ind, 2, item3);
  }
}

void
CQGradientStopList::
emitStopsChanged()
{
  emit stopsChanged();
}

//------

CQGradientStopTable::
CQGradientStopTable(CQGradientStopList *list) :
 CQTableWidget(), list_(list)
{
  clear();

  connect(this, SIGNAL(valueChanged(int, int)), this, SLOT(updateStopsSlot()));
}

void
CQGradientStopTable::
clear()
{
  CQTableWidget::clear();

  setColumnCount(3);

  setColumnLabel(0, "Color");
  setColumnLabel(1, "Alpha");
  setColumnLabel(2, "Offset");
}

void
CQGradientStopTable::
updateStopsSlot()
{
  int num_rows = rowCount();

  int ind = 0;

  CGenGradient::StopList &stops = list_->getStops();

  CGenGradient::StopList::iterator p1, p2;

  for (p1 = stops.begin(), p2 = stops.end(); p1 != p2 && ind < num_rows; ++p1, ++ind) {
    CGradientStop &stop = *p1;

    CQGradientStopColorItem  *item1 = getItem<CQGradientStopColorItem >(ind, 0);
    CQGradientStopAlphaItem  *item2 = getItem<CQGradientStopAlphaItem >(ind, 1);
    CQGradientStopOffsetItem *item3 = getItem<CQGradientStopOffsetItem>(ind, 2);

    CRGBA rgba = item1->getColor();

    rgba.setAlpha(item2->getAlpha());

    stop.setColor(rgba);

    stop.setOffset(item3->getOffset());
  }

  list_->emitStopsChanged();
}

//------

CQGradientStopColorItem::
CQGradientStopColorItem(CQGradientStopTable *t, uint ind, const CRGBA &rgba) :
 CQTableWidgetItem(t), table_(t), ind_(ind), rgba_(rgba), edit_(NULL)
{
}

CQGradientStopColorItem *
CQGradientStopColorItem::
clone() const
{
  return new CQGradientStopColorItem(table_, ind_, rgba_);
}

QString
CQGradientStopColorItem::
getString()
{
  return "";
}

QWidget *
CQGradientStopColorItem::
createEditor(QWidget *parent) const
{
  uint styles = CQColorChooser::Text | CQColorChooser::ColorButton;

  edit_ = new CQColorChooser(styles, parent);

  return edit_;
}

void
CQGradientStopColorItem::
setEditorData()
{
  if (edit_.isNull()) return;

  edit_->setRGBA(rgba_);
}

void
CQGradientStopColorItem::
getEditorData(QString &str)
{
  if (edit_.isNull()) return;

  rgba_ = edit_->getRGBA();

  str = "";
}

bool
CQGradientStopColorItem::
sizeHint(const QStyleOptionViewItem &option, QSize &size) const
{
  size = QSize(128, option.rect.height());

  return true;
}

bool
CQGradientStopColorItem::
paint(QPainter *painter, const QStyleOptionViewItem &option) const
{
  if (edit_.isNull()) {
    paintBackground(painter, option);

    CRGBA rgba = rgba_.solid();

    QBrush brush;

    QRect rect = option.rect.adjusted(2, 2, -2, -2);

    painter->fillRect(rect, QBrush(CQUtil::rgbaToColor(rgba)));
  }

  return true;
}

//----------

CQGradientStopAlphaItem::
CQGradientStopAlphaItem(CQGradientStopTable *t, uint ind, double alpha) :
 CQTableWidgetItem(t), table_(t), ind_(ind), alpha_(alpha), edit_(NULL)
{
}

CQGradientStopAlphaItem *
CQGradientStopAlphaItem::
clone() const
{
  return new CQGradientStopAlphaItem(table_, ind_, alpha_);
}

QString
CQGradientStopAlphaItem::
getString()
{
  return toString();
}

QWidget *
CQGradientStopAlphaItem::
createEditor(QWidget *parent) const
{
  edit_ = new CQRealEdit(parent);

  return edit_;
}

void
CQGradientStopAlphaItem::
setEditorData()
{
  if (! edit_) return;

  edit_->setValue(alpha_);
}

void
CQGradientStopAlphaItem::
getEditorData(QString &str)
{
  if (! edit_) return;

  alpha_ = edit_->getValue();

  str = toString();
}

QString
CQGradientStopAlphaItem::
toString() const
{
  return QString("%1").arg(alpha_);
}

//----------

CQGradientStopOffsetItem::
CQGradientStopOffsetItem(CQGradientStopTable *t, uint ind, double offset) :
 CQTableWidgetItem(t), table_(t), ind_(ind), offset_(offset), edit_(NULL)
{
}

CQGradientStopOffsetItem *
CQGradientStopOffsetItem::
clone() const
{
  return new CQGradientStopOffsetItem(table_, ind_, offset_);
}

QString
CQGradientStopOffsetItem::
getString()
{
  return toString();
}

QWidget *
CQGradientStopOffsetItem::
createEditor(QWidget *parent) const
{
  edit_ = new CQRealEdit(parent);

  return edit_;
}

void
CQGradientStopOffsetItem::
setEditorData()
{
  if (! edit_) return;

  edit_->setValue(offset_);
}

void
CQGradientStopOffsetItem::
getEditorData(QString &str)
{
  if (! edit_) return;

  offset_ = edit_->getValue();

  str = toString();
}

QString
CQGradientStopOffsetItem::
toString() const
{
  return QString("%1").arg(offset_);
}
