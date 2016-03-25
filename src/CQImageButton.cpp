#include <CQImageButton.h>
#include <CImageMgr.h>
#include <CQImage.h>

CQImageButton::
CQImageButton(const QString &fileName)
{
  CImageFileSrc src(fileName.toStdString());

  CImagePtr image = CImageMgrInst->lookupImage(src);

  init(image);
}

CQImageButton::
CQImageButton(CImagePtr image)
{
  init(image);
}

CQImageButton::
CQImageButton(uchar *data, uint len)
{
  QPixmap pixmap;

  pixmap.loadFromData(data, len);

  init(pixmap);
}

CQImageButton::
CQImageButton(const QPixmap &pixmap)
{
  init(pixmap);
}

CQImageButton::
CQImageButton(const QIcon &icon)
{
  init(icon);
}

void
CQImageButton::
init(CImagePtr image)
{
  CQImage *qimage = image.cast<CQImage>();

  if (qimage) {
    QPixmap pixmap = QPixmap::fromImage(qimage->getQImage());

    init(pixmap);
  }
}

void
CQImageButton::
init(const QPixmap &pixmap)
{
  size_ = pixmap.size();

  QIcon icon(pixmap);

  setIcon(icon);

#ifndef OS_OSX
  setFixedSize(size_ + QSize(8, 8));

  setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
#endif
}

void
CQImageButton::
init(const QIcon &icon)
{
  size_ = iconSize();

  setIcon(icon);

#ifndef OS_OSX
  setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
#endif
}

QSize
CQImageButton::
sizeHint() const
{
  return size_ + QSize(8, 8);
}

QSize
CQImageButton::
minimumSizeHint () const
{
  return size_ + QSize(8, 8);
}
