#ifndef CQIMAGE_BUTTON_H
#define CQIMAGE_BUTTON_H

#include <QToolButton>
#include <CImageLib.h>

class CQImageButton : public QToolButton {
  Q_OBJECT

 private:
  QSize size_;

 public:
  CQImageButton(CImagePtr image);
  CQImageButton(const QString &fileName);
  CQImageButton(uchar *data, uint len);
  CQImageButton(const QPixmap &pixmap);
  CQImageButton(const QIcon &icon);

  virtual ~CQImageButton() { }

  QSize sizeHint() const;
  QSize minimumSizeHint() const;

 private:
  void init(CImagePtr image);
  void init(const QPixmap &pixmap);
  void init(const QIcon &icon);
};

#endif
