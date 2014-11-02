#include <CQLineJoin.h>
#include <CQUtil.h>
#include <QAction>

#include <pixmaps/join_mitre.xpm>
#include <pixmaps/join_round.xpm>
#include <pixmaps/join_bevel.xpm>

CQLineJoin::
CQLineJoin(QWidget *parent) :
 QComboBox(parent)
{
  CImagePtr join_image1 = CImage::create(join_mitre_data, sizeof(join_mitre_data)/sizeof(char *),
                                         CFILE_TYPE_IMAGE_XPM);
  CImagePtr join_image2 = CImage::create(join_round_data, sizeof(join_round_data)/sizeof(char *),
                                         CFILE_TYPE_IMAGE_XPM);
  CImagePtr join_image3 = CImage::create(join_bevel_data, sizeof(join_bevel_data)/sizeof(char *),
                                         CFILE_TYPE_IMAGE_XPM);

  addItem(CQUtil::imageToIcon(join_image1), "Mitre");
  addItem(CQUtil::imageToIcon(join_image2), "Round");
  addItem(CQUtil::imageToIcon(join_image3), "Bevel");

  setToolTip("Line Join");

  connect(this, SIGNAL(currentIndexChanged(int)), this, SLOT(joinChangedSlot(int)));
}

void
CQLineJoin::
setLineJoin(CLineJoinType join)
{
  setCurrentIndex(int(join) - 1);
}

CLineJoinType
CQLineJoin::
getLineJoin() const
{
  return CLineJoinType(currentIndex() + 1);
}

void
CQLineJoin::
joinChangedSlot(int ind)
{
  emit valueChanged(CLineJoinType(ind + 1));
}
