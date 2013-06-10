#ifndef CQMATRIX_2D_H
#define CQMATRIX_2D_H

#include <QWidget>
#include <CMatrix2D.h>

class CQTableWidget;

class CQMatrix2D : public QWidget {
  Q_OBJECT

 private:
  CQTableWidget *table_;
  CMatrix2D      m_;

 public:
  CQMatrix2D(QWidget *parent, const CMatrix2D &m=CMatrix2D(CMATRIX_TYPE_IDENTITY));
  CQMatrix2D(const CMatrix2D &m=CMatrix2D(CMATRIX_TYPE_IDENTITY));

  QSize minimumSizeHint() const;
  QSize sizeHint() const;

  CQTableWidget *getTableWidget() const { return table_; }

  void setValue(const CMatrix2D &m);

  const CMatrix2D &getValue() const { return m_; }

  void setValue(int r, int c, double v);

  double getValue(int r, int c) const;

 private:
  void init();
  void updateValues();

 signals:
  void valueChanged();
};

#endif
