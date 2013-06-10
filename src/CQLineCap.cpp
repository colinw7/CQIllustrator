#include <CQLineCap.h>
#include <CQUtil.h>
#include <QAction>

#include "pixmaps/cap_square.xpm"
#include "pixmaps/cap_butt.xpm"
#include "pixmaps/cap_round.xpm"

CQLineCap::
CQLineCap(QWidget *parent) :
 QComboBox(parent)
{
  CImagePtr cap_image1 = CImage::create(cap_butt_data  , sizeof(cap_butt_data  )/sizeof(char *),
                                        CFILE_TYPE_IMAGE_XPM);
  CImagePtr cap_image2 = CImage::create(cap_round_data , sizeof(cap_round_data )/sizeof(char *),
                                        CFILE_TYPE_IMAGE_XPM);
  CImagePtr cap_image3 = CImage::create(cap_square_data, sizeof(cap_square_data)/sizeof(char *),
                                        CFILE_TYPE_IMAGE_XPM);

  addItem(CQUtil::imageToIcon(cap_image1), "Butt"  );
  addItem(CQUtil::imageToIcon(cap_image2), "Round" );
  addItem(CQUtil::imageToIcon(cap_image3), "Square");

  setToolTip("Line Cap");

  connect(this, SIGNAL(currentIndexChanged(int)), this, SLOT(capChangedSlot(int)));
}

void
CQLineCap::
setLineCap(CLineCapType cap)
{
  setCurrentIndex(int(cap) - 1);
}

CLineCapType
CQLineCap::
getLineCap() const
{
  return CLineCapType(currentIndex() + 1);
}

void
CQLineCap::
capChangedSlot(int value)
{
  emit valueChanged(CLineCapType(value + 1));
}
