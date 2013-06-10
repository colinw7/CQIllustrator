#ifndef CQScrollArea_H
#define CQScrollArea_H

#include <QWidget>

class QScrollBar;
class QPainter;

class CQScrollArea : public QWidget {
  Q_OBJECT

 public:
  CQScrollArea(QWidget *widget);

  QWidget *getWidget() const { return widget_; }

  void showHBar(bool flag);
  void showVBar(bool flag);

  void setCornerWidget(QWidget *w);

  void setXSize(int x_size);
  void setYSize(int y_size);

  int getXSize() const { return x_size_; }
  int getYSize() const { return y_size_; }

  void setXOffset(int x_offset);
  void setYOffset(int y_offset);

  int getXOffset() const { return x_offset_; }
  int getYOffset() const { return y_offset_; }

  void setXPageStep(int x);
  void setYPageStep(int y);

  void scrollUp(bool page=false);
  void scrollDown(bool page=false);
  void scrollLeft(bool page=false);
  void scrollRight(bool page=false);

  void ensureVisible(int x, int y, int xmargin=50, int ymargin=50);

  void updateScrollbars();

 private slots:
  void hscrollSlot(int value);
  void vscrollSlot(int value);

 signals:
  void updateArea();

 private:
  QWidget    *widget_;
  QScrollBar *hbar_;
  QScrollBar *vbar_;
  int         x_size_;
  int         y_size_;
  int         x_offset_;
  int         y_offset_;
};

#endif
