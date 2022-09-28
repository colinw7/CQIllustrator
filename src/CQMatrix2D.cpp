#include <CQMatrix2D.h>
#include <CQTableWidget.h>
#include <CQRealEdit.h>
#include <CStrUtil.h>

#include <QVBoxLayout>
#include <QPointer>

class MatrixCell : public CQTableWidgetItem {
 public:
  enum { TYPE = QTableWidgetItem::UserType + 1 };

  MatrixCell(CQMatrix2D *m, int row=-1, int col=-1) :
   CQTableWidgetItem(m->getTableWidget()), m_(m), row_(row), col_(col) {
  }

  MatrixCell *clone() const override { return new MatrixCell(m_); }

  QString getString() const override {
    double v = getValue();

    return CStrUtil::toString(v).c_str();
  }

  QWidget *createEditor(QWidget *parent) const override {
    edit_ = new CQRealEdit(parent);

    return edit_;
  }

  void setEditorData() override {
    if (edit_.isNull()) return;

    double v = getValue();

    edit_->setValue(v);
  }

  void getEditorData(QString &str) override {
    if (edit_.isNull()) return;

    double v = edit_->getValue();

    m_->setValue(row_, col_, v);

    str = getString();
  }

  bool sizeHint(const QStyleOptionViewItem &, QSize &size) const override {
    QFontMetrics fm(font());

    size = QSize(fm.horizontalAdvance("XXX.XXXX") + 8, fm.ascent() + fm.descent() + 8);

    return true;
  }

  double getValue() const {
    double v = 0.0;

    if (row_ >= 0 && row_ < 3 && col_ >= 0 && col_ < 3)
      v = m_->getValue(row_, col_);

    return v;
  }

  void setValue(double value) {
    setText(CStrUtil::toString(value).c_str());
  }

 private:
  static uint type_;

  mutable QPointer<CQRealEdit>  edit_;
  CQMatrix2D                   *m_;
  int                           row_, col_;
};

CQMatrix2D::
CQMatrix2D(const CMatrix2D &m) :
 QWidget(0), m_(m)
{
  init();
}

CQMatrix2D::
CQMatrix2D(QWidget *parent, const CMatrix2D &m) :
 QWidget(parent), m_(m)
{
  init();
}

QSize
CQMatrix2D::
minimumSizeHint() const
{
  return sizeHint();
}

QSize
CQMatrix2D::
sizeHint() const
{
  QFontMetrics fm(font());

  QSize size(fm.horizontalAdvance("XXX.XXXX") + 8, fm.ascent() + fm.descent() + 8);

  return QSize(3*size.width() + 42, 4*size.height() + 42);
}

void
CQMatrix2D::
init()
{
  auto *layout = new QVBoxLayout(this);

  table_ = new CQTableWidget;

  table_->setColumnCount(3);

  table_->setColumnLabels(QStringList() << "1" << "2" << "3");
  table_->setRowLabels   (QStringList() << "1" << "2" << "3");

  layout->addWidget(table_);

  table_->registerType(MatrixCell::TYPE, new MatrixCell(this));

  table_->setRowCount(3);

  table_->setItem(0, 0, new MatrixCell(this, 0, 0));
  table_->setItem(0, 1, new MatrixCell(this, 0, 1));
  table_->setItem(0, 2, new MatrixCell(this, 0, 2));
  table_->setItem(1, 0, new MatrixCell(this, 1, 0));
  table_->setItem(1, 1, new MatrixCell(this, 1, 1));
  table_->setItem(1, 2, new MatrixCell(this, 1, 2));
  table_->setItem(2, 0, new MatrixCell(this, 2, 0));
  table_->setItem(2, 1, new MatrixCell(this, 2, 1));
  table_->setItem(2, 2, new MatrixCell(this, 2, 2));
}

void
CQMatrix2D::
setValue(const CMatrix2D &m)
{
  m_ = m;

  updateValues();

  update();
}

void
CQMatrix2D::
updateValues()
{
  for (uint row = 0; row < 3; ++row)
    for (uint col = 0; col < 3; ++col)
      table_->getItem<MatrixCell>(int(row), int(col))->setValue(m_.getValue(row, col));
}

void
CQMatrix2D::
setValue(int row, int col, double v)
{
  m_.setValue(uint(col), uint(row), v);

  emit valueChanged();
}

double
CQMatrix2D::
getValue(int row, int col) const
{
  return m_.getValue(uint(col), uint(row));
}
