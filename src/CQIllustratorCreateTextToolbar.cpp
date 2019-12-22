#include <CQIllustratorCreateTextToolbar.h>
#include <CQIllustratorCreateTextMode.h>
#include <CQIllustratorTextShape.h>
#include <CQIllustratorUtil.h>
#include <CQPixmapCache.h>
#include <CQImageButton.h>
#include <CQRealEdit.h>
#include <CQSwatch.h>
#include <CQFontUtil.h>
#include <CQUtil.h>
#include <QHBoxLayout>
#include <QFontComboBox>
#include <QComboBox>
#include <QLineEdit>

#include <svg/bold_svg.h>
#include <svg/italic_svg.h>
#include <svg/lalign_svg.h>
#include <svg/hcalign_svg.h>
#include <svg/ralign_svg.h>
#include <svg/balign_svg.h>
#include <svg/vcalign_svg.h>
#include <svg/talign_svg.h>

namespace {

int fontPixelSize(const QFont &font) {
  int font_size = font.pixelSize();

  if (font_size < 0) {
    QFontInfo finfo(font);

    font_size = finfo.pixelSize();
  }

  return font_size;
}

}

//------

CQIllustratorCreateTextToolbar::
CQIllustratorCreateTextToolbar(CQIllustratorCreateTextMode *mode) :
 CQIllustratorToolbar(mode), mode_(mode)
{
}

QIcon
CQIllustratorCreateTextToolbar::
getIcon()
{
  return CQPixmapCacheInst->getIcon("TEXT");
}

void
CQIllustratorCreateTextToolbar::
addWidgets()
{
  QBoxLayout *layout = qobject_cast<QBoxLayout *>(CQIllustratorToolbar::layout());

  //-----

  fontCombo_    = new QFontComboBox;
  sizeCombo_    = new QComboBox;
  boldButton_   = new CQImageButton(CQPixmapCacheInst->getIcon("BOLD"));
  italicButton_ = new CQImageButton(CQPixmapCacheInst->getIcon("ITALIC"));

  sizeCombo_   ->setEditable(true);
  boldButton_  ->setCheckable(true);
  italicButton_->setCheckable(true);

  fontCombo_->setWritingSystem(QFontDatabase::Latin);

  fontCombo_   ->setToolTip("Set Font Family");
  sizeCombo_   ->setToolTip("Set Size");
  boldButton_  ->setToolTip("Set Bold");
  italicButton_->setToolTip("Set Italic");

  connect(fontCombo_   , SIGNAL(activated(int)), this, SLOT(fontComboSlot()));
  connect(sizeCombo_   , SIGNAL(activated(int)), this, SLOT(sizeComboSlot()));
  connect(boldButton_  , SIGNAL(clicked(bool)),  this, SLOT(boldSlot()));
  connect(italicButton_, SIGNAL(clicked(bool)),  this, SLOT(italicSlot()));

  CQSwatch *fontSwatch = new CQSwatch("Font", fontCombo_, sizeCombo_, boldButton_, italicButton_);

  layout->addWidget(fontSwatch);

  //-----

  textEdit_   = new QLineEdit(mode_->getText());
  widthEdit_  = new CQRealEdit(0.0);
  heightEdit_ = new CQRealEdit(0.0);

  textEdit_  ->setFocusPolicy(Qt::ClickFocus);
  widthEdit_ ->setFocusPolicy(Qt::ClickFocus);
  heightEdit_->setFocusPolicy(Qt::ClickFocus);

  textEdit_  ->setToolTip("Set Text");
  widthEdit_ ->setToolTip("Set Width");
  heightEdit_->setToolTip("Set Height");

  connect(textEdit_  , SIGNAL(returnPressed()), this, SLOT(updateShape()));
  connect(widthEdit_ , SIGNAL(valueChanged(double)),  this, SLOT(updateShape()));
  connect(heightEdit_, SIGNAL(valueChanged(double)),  this, SLOT(updateShape()));

  CQSwatch *textSwatch   = new CQSwatch("Text"  , textEdit_);
  CQSwatch *widthSwatch  = new CQSwatch("Width" , widthEdit_ );
  CQSwatch *heightSwatch = new CQSwatch("Height", heightEdit_);

  layout->addWidget(textSwatch);
  layout->addWidget(widthSwatch);
  layout->addWidget(heightSwatch);

  //-----

  lalignButton_  = new CQImageButton(CQPixmapCacheInst->getIcon("LALIGN" ));
  hcalignButton_ = new CQImageButton(CQPixmapCacheInst->getIcon("HCALIGN"));
  ralignButton_  = new CQImageButton(CQPixmapCacheInst->getIcon("RALIGN" ));

  lalignButton_ ->setCheckable(true);
  hcalignButton_->setCheckable(true);
  ralignButton_ ->setCheckable(true);

  lalignButton_ ->setToolTip("Left Align");
  hcalignButton_->setToolTip("Horizontal Center Align");
  ralignButton_ ->setToolTip("Right Align");

  connect(lalignButton_ , SIGNAL(clicked()), this, SLOT(lalignSlot ()));
  connect(hcalignButton_, SIGNAL(clicked()), this, SLOT(hcalignSlot()));
  connect(ralignButton_ , SIGNAL(clicked()), this, SLOT(ralignSlot ()));

  CQSwatch *halignSwatch = new CQSwatch("HAlign", lalignButton_, hcalignButton_, ralignButton_);

  layout->addWidget(halignSwatch);

  //-----

  talignButton_  = new CQImageButton(CQPixmapCacheInst->getIcon("TALIGN" ));
  vcalignButton_ = new CQImageButton(CQPixmapCacheInst->getIcon("VCALIGN"));
  balignButton_  = new CQImageButton(CQPixmapCacheInst->getIcon("BALIGN" ));

  talignButton_ ->setCheckable(true);
  vcalignButton_->setCheckable(true);
  balignButton_ ->setCheckable(true);

  talignButton_ ->setToolTip("Top Align");
  vcalignButton_->setToolTip("Vertical Center Align");
  balignButton_ ->setToolTip("Bottom Align");

  connect(talignButton_ , SIGNAL(clicked()), this, SLOT(talignSlot ()));
  connect(vcalignButton_, SIGNAL(clicked()), this, SLOT(vcalignSlot()));
  connect(balignButton_ , SIGNAL(clicked()), this, SLOT(balignSlot ()));

  CQSwatch *valignSwatch = new CQSwatch("VAlign", talignButton_, vcalignButton_, balignButton_);

  layout->addWidget(valignSwatch);

  //-----

  updateWidgets();
}

void
CQIllustratorCreateTextToolbar::
setSelectedShape(const CQIllustratorShape *shape)
{
  const CQIllustratorTextShape *text = dynamic_cast<const CQIllustratorTextShape *>(shape);

  if (text) {
    qfont_  = CQFontUtil::toQFont(text->getFont());
    halign_ = text->getHAlign();
    valign_ = text->getVAlign();

    textEdit_->setText(text->getText().c_str());

    updateWidgets();

    const CBBox2D &bbox = text->getBBox();

    setSize(bbox);
  }
  else
    textEdit_->setText("");
}

void
CQIllustratorCreateTextToolbar::
setSize(const CBBox2D &bbox)
{
  double w = bbox.getWidth();
  double h = bbox.getHeight();

  widthEdit_ ->setValue(w);
  heightEdit_->setValue(h);
}

void
CQIllustratorCreateTextToolbar::
updateShape()
{
  CQIllustrator *illustrator = mode_->getIllustrator();

  CQIllustratorTextShape *shape =
    CQIllustratorUtil::getCurrentShape<CQIllustratorTextShape>(illustrator);

  QString str = textEdit_->text();

  if (shape) {
    illustrator->checkoutShape(shape, CQIllustratorData::ChangeType::GEOMETRY);

    CQIllustratorTextShape *text = dynamic_cast<CQIllustratorTextShape *>(shape);

    text->setText(str.toStdString());

    text->setFont(CQFontUtil::fromQFont(qfont_));

    text->setHAlign(halign_);
    text->setVAlign(valign_);

    const CBBox2D &bbox = text->getBBox();

    double width1  = bbox.getWidth ();
    double height1 = bbox.getHeight();

    double width  = widthEdit_ ->getValue();
    double height = heightEdit_->getValue();

    text->scale(bbox.getCenter(), width/width1, height/height1);

    illustrator->checkinShape(shape, CQIllustratorData::ChangeType::GEOMETRY);
  }

  mode_->setText(str);

  illustrator->redraw();
}

void
CQIllustratorCreateTextToolbar::
updateWidgets()
{
  updateFontWidgets();

  lalignButton_ ->setChecked(halign_ == CHALIGN_TYPE_LEFT);
  hcalignButton_->setChecked(halign_ == CHALIGN_TYPE_CENTER);
  ralignButton_ ->setChecked(halign_ == CHALIGN_TYPE_RIGHT);

  talignButton_ ->setChecked(valign_ == CVALIGN_TYPE_TOP);
  vcalignButton_->setChecked(valign_ == CVALIGN_TYPE_CENTER);
  balignButton_ ->setChecked(valign_ == CVALIGN_TYPE_BOTTOM);
}

void
CQIllustratorCreateTextToolbar::
updateFontWidgets()
{
  int nind = fontCombo_->findText(qfont_.family());

  if (nind >= 0 && nind != font_ind_) {
    font_ind_ = nind;

    fontCombo_->setCurrentIndex(nind);

    QFontDatabase database;

    const QString &family = fontCombo_->itemText(fontCombo_->currentIndex());

    QList<int> sizes = database.pointSizes(family);

    sizeCombo_->clear();

    int font_size = fontPixelSize(qfont_);

    int size1 = -1, size2 = -1;

    for (int i = 0; i < sizes.size(); ++i) {
      size1 = size2;
      size2 = sizes[i];

      if (font_size > size1 && font_size < size2)
        sizeCombo_->addItem(QString("%1").arg(font_size));

      sizeCombo_->addItem(QString("%1").arg(size2));
    }

    if (font_size > size2)
      sizeCombo_->addItem(QString("%1").arg(font_size));
  }

  int font_size = fontPixelSize(qfont_);

  if (font_size != font_size_) {
    int ind = sizeCombo_->findText(QString("%1").arg(font_size));

    if (ind >= 0 && ind != sizeCombo_->currentIndex()) {
      sizeCombo_->setCurrentIndex(ind);

      font_size_ = font_size;
    }
  }

  boldButton_  ->setChecked(qfont_.bold());
  italicButton_->setChecked(qfont_.italic());
}

void
CQIllustratorCreateTextToolbar::
fontComboSlot()
{
  updateFontFromWidgets();
}

void
CQIllustratorCreateTextToolbar::
sizeComboSlot()
{
  updateFontFromWidgets();
}

void
CQIllustratorCreateTextToolbar::
boldSlot()
{
  updateFontFromWidgets();
}

void
CQIllustratorCreateTextToolbar::
italicSlot()
{
  updateFontFromWidgets();
}

void
CQIllustratorCreateTextToolbar::
updateFontFromWidgets()
{
  QString name = fontCombo_->itemText(fontCombo_->currentIndex());

  font_size_ = sizeCombo_->itemText(sizeCombo_->currentIndex()).toInt();

  int weight = QFont::Normal;

  if (boldButton_->isChecked())
    weight = QFont::Bold;

  bool italic = italicButton_->isChecked();

  QFont qfont(name, font_size_, weight, italic);

  qfont_ = qfont;

  updateShape();

  updateFontWidgets();
}

void
CQIllustratorCreateTextToolbar::
lalignSlot()
{
  halign_ = CHALIGN_TYPE_LEFT;

  updateShape();
}

void
CQIllustratorCreateTextToolbar::
hcalignSlot()
{
  halign_ = CHALIGN_TYPE_CENTER;

  updateShape();
}

void
CQIllustratorCreateTextToolbar::
ralignSlot()
{
  halign_ = CHALIGN_TYPE_RIGHT;

  updateShape();
}

void
CQIllustratorCreateTextToolbar::
talignSlot()
{
  valign_ = CVALIGN_TYPE_TOP;

  updateShape();
}

void
CQIllustratorCreateTextToolbar::
vcalignSlot()
{
  valign_ = CVALIGN_TYPE_CENTER;

  updateShape();
}

void
CQIllustratorCreateTextToolbar::
balignSlot()
{
  valign_ = CVALIGN_TYPE_BOTTOM;

  updateShape();
}
