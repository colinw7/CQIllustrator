#include <CQIllustratorCreateTextMode.h>
#include <CQIllustrator.h>
#include <CQIllustratorHandle.h>
#include <CQIllustratorShapeDrawer.h>

#include <QPainter>
#include <QHBoxLayout>
#include <QFontComboBox>

#include <CQMenu.h>
#include <CQUtil.h>
#include <CQRealEdit.h>
#include <CQImageButton.h>
#include <CQSwatch.h>

#include "xpm/text.xpm"
#include "xpm/lalign.xpm"
#include "xpm/hcalign.xpm"
#include "xpm/ralign.xpm"
#include "xpm/balign.xpm"
#include "xpm/vcalign.xpm"
#include "xpm/talign.xpm"
#include "xpm/bold.xpm"
#include "xpm/italic.xpm"

#include "cursors/text.xbm"
#include "cursors/textmask.xbm"

CQIllustratorCreateTextMode::
CQIllustratorCreateTextMode(CQIllustrator *illustrator) :
 CQIllustratorMode(illustrator, CQIllustrator::MODE_TEXT), equalSize_(false),
 text_("Hello"), cursor_pos_(0)
{
  setCursor(text_bits, textmask_bits, 2, 2);
}

CQIllustratorCreateTextToolbar *
CQIllustratorCreateTextMode::
createToolbar()
{
  toolbar_ = new CQIllustratorCreateTextToolbar(this);

  return toolbar_;
}

CQIllustratorCreateTextSizer *
CQIllustratorCreateTextMode::
createSizer()
{
  sizer_ = new CQIllustratorCreateTextSizer(this);

  return sizer_;
}

CQMenuItem *
CQIllustratorCreateTextMode::
createMenuItem(CQMenu *menu)
{
  menuItem_ = new CQMenuItem(menu, "&Text", CQMenuItem::CHECKABLE);

  menuItem_->setStatusTip("Create Text");
  menuItem_->setXPMIcon(text_data);

  connect(menuItem_->getAction(), SIGNAL(toggled(bool)), this, SLOT(menuItemSlot()));

  return menuItem_;
}

void
CQIllustratorCreateTextMode::
handleMouseRelease(const MouseEvent &e)
{
  CPoint2D p2 = CQUtil::fromQPoint(e.window);

  if (dragging_) {
    if (! moving_) {
      selectControlPoint(e);
    }
    // dragging finished so commit
    else {
      illustrator_->getSandbox()->commit(CQIllustratorData::CHANGE_GEOMETRY);
    }
  }
  // not dragging so do a select
  else {
    CPoint2D p1 = CQUtil::fromQPoint(press_wpos_);

    // point click - select at point
    if (! moving_) {
      (void) illustrator_->selectAt(p2, e.event->isControlKey(), e.event->isShiftKey());
    }
    // drag then create text using specified bbox
    else {
      CBBox2D bbox(p1, p2);

      if (e.event->isShiftKey()) {
        double w = bbox.getWidth ();
        double h = bbox.getHeight();

        if (w > h)
          bbox.setHeight(w);
        else
          bbox.setWidth(h);
      }

      CQIllustratorTextShape *text =
        illustrator_->createTextShape(bbox.getLL(), bbox.getUR(), text_.toStdString());

      CQIllustratorShapeStroke stroke;
      CQIllustratorShapeFill   fill;

      stroke.setOpacity(0.0);
      fill  .setColor  (CRGBA(0,0,0));

      text->setStroke(stroke);
      text->setFill  (fill);

      text->setFont(CQUtil::fromQFont(toolbar_->getFont()));

      illustrator_->addShape(text);

      illustrator_->setSelectShape(text);
    }
  }

  dragging_ = false;

  illustrator_->redraw();
}

void
CQIllustratorCreateTextMode::
handleMouseDrag(const MouseEvent &e)
{
  if (dragging_) {
    CQIllustratorShape *shape = illustrator_->getSandbox()->frontShape();

    sizer_->updatePoint(shape, e.window);
  }
  else {
    CPoint2D p1 = CQUtil::fromQPoint(press_wpos_);
    CPoint2D p2 = CQUtil::fromQPoint(curr_wpos_);

    CBBox2D bbox(p1, p2);

    equalSize_ = e.event->isShiftKey();

    if (equalSize_) {
      double w = bbox.getWidth ();
      double h = bbox.getHeight();

      if (w > h)
        bbox.setHeight(w);
      else
        bbox.setWidth(h);
    }

    toolbar_->setSize(bbox);
  }

  illustrator_->redrawOverlay();
}

bool
CQIllustratorCreateTextMode::
handleKeyPress(const KeyEvent &e)
{
  CQIllustratorSelectedShapes *selection = illustrator_->getSelection();

  if (selection->size() != 1) return false;

  const CQIllustratorSelectedShape &sshape = selection->front();

  const CQIllustratorShape *shape = sshape.getShape();

  const CQIllustratorTextShape *text = dynamic_cast<const CQIllustratorTextShape *>(shape);

  if (! text) return false;

  std::string str  = text->getText();
  std::string str1 = str;

  CKeyType key = e.event->getType();

  if      (key >= CKEY_TYPE_Space && key <= CKEY_TYPE_AsciiTilde) {
    str1 += (char) key;
  }
  else if (key == CKEY_TYPE_Return) {
    str1 += '\n';
  }
  else if (key == CKEY_TYPE_BackSpace) {
    if (str1 != "")
      str1 = str1.substr(0, str1.size() - 1);
  }
  else
    return CQIllustratorMode::handleKeyPress(e);

  if (str != str1) {
    CQIllustratorTextShape *text1 = const_cast<CQIllustratorTextShape *>(text);

    illustrator_->checkoutShape(text1, CQIllustratorData::CHANGE_GEOMETRY);

    text1->setText(str1);

    illustrator_->checkinShape(text1, CQIllustratorData::CHANGE_GEOMETRY);
  }

  return true;
}

void
CQIllustratorCreateTextMode::
drawOverlay(CQIllustratorShapeDrawer *drawer)
{
  QPainter *painter = drawer->getPainter();

  if (! dragging_) {
    // draw creation rubber band
    if (pressed_) {
      QPointF current_wpos = illustrator_->getITransform().map(QPointF(curr_ppos_));

      QPen pen;

      pen.setColor(QColor(0,0,0));
      pen.setStyle(Qt::DashLine);

      painter->setPen(pen);
      painter->setBrush(Qt::NoBrush);

      painter->drawRect(QRectF(press_wpos_, current_wpos));
    }

    //------

    // draw handles for single selected shape
    CQIllustratorSelectedShapes *selection = illustrator_->getSelection();

    if (selection->size() == 1) {
      const CQIllustratorSelectedShape &sshape = selection->front();

      const CQIllustratorShape *shape = sshape.getShape();

      sizer_->draw(painter, shape);

      sizer_->drawSelectionHandles(painter, sshape);
    }
  }
  else {
    CQIllustratorShape *shape = illustrator_->getSandbox()->frontShape();

    sizer_->draw(painter, shape);
  }
}

void
CQIllustratorCreateTextMode::
setText(const QString &text)
{
  text_ = text;
}

//------------

CQIllustratorCreateTextToolbar::
CQIllustratorCreateTextToolbar(CQIllustratorCreateTextMode *mode) :
 CQIllustratorToolbar(mode), mode_(mode), font_ind_(-1), font_size_(-1),
 halign_(CHALIGN_TYPE_CENTER), valign_(CVALIGN_TYPE_CENTER)
{
}

QIcon
CQIllustratorCreateTextToolbar::
getIcon()
{
  return QIcon(QPixmap(text_data));
}

void
CQIllustratorCreateTextToolbar::
addWidgets()
{
  QBoxLayout *layout = qobject_cast<QBoxLayout *>(CQIllustratorToolbar::layout());

  //-----

  fontCombo_    = new QFontComboBox;
  sizeCombo_    = new QComboBox;
  boldButton_   = new CQImageButton(QPixmap(bold_data  ));
  italicButton_ = new CQImageButton(QPixmap(italic_data));

  boldButton_  ->setCheckable(true);
  italicButton_->setCheckable(true);

  fontCombo_->setWritingSystem(QFontDatabase::Latin);

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

  lalignButton_  = new CQImageButton(QPixmap(lalign_data ));
  hcalignButton_ = new CQImageButton(QPixmap(hcalign_data));
  ralignButton_  = new CQImageButton(QPixmap(ralign_data ));

  lalignButton_ ->setCheckable(true);
  hcalignButton_->setCheckable(true);
  ralignButton_ ->setCheckable(true);

  lalignButton_ ->setToolTip("Left Align");
  hcalignButton_->setToolTip("Horizontal Center Align");
  ralignButton_ ->setToolTip("Right Align");

  connect(lalignButton_ , SIGNAL(clicked()), this, SLOT(lalignSlot ()));
  connect(hcalignButton_, SIGNAL(clicked()), this, SLOT(hcalignSlot()));
  connect(ralignButton_ , SIGNAL(clicked()), this, SLOT(ralignSlot ()));

  CQSwatch *halignSwatch = new CQSwatch("Align<3>", lalignButton_, hcalignButton_, ralignButton_);

  layout->addWidget(halignSwatch);

  //-----

  talignButton_  = new CQImageButton(QPixmap(talign_data ));
  vcalignButton_ = new CQImageButton(QPixmap(vcalign_data));
  balignButton_  = new CQImageButton(QPixmap(balign_data ));

  talignButton_ ->setCheckable(true);
  vcalignButton_->setCheckable(true);
  balignButton_ ->setCheckable(true);

  talignButton_ ->setToolTip("Top Align");
  vcalignButton_->setToolTip("Vertical Center Align");
  balignButton_ ->setToolTip("Bottom Align");

  connect(talignButton_ , SIGNAL(clicked()), this, SLOT(talignSlot ()));
  connect(vcalignButton_, SIGNAL(clicked()), this, SLOT(vcalignSlot()));
  connect(balignButton_ , SIGNAL(clicked()), this, SLOT(balignSlot ()));

  CQSwatch *valignSwatch = new CQSwatch("Align<3>", talignButton_, vcalignButton_, balignButton_);

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
    qfont_  = CQUtil::toQFont(text->getFont());
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
    illustrator->checkoutShape(shape, CQIllustratorData::CHANGE_GEOMETRY);

    CQIllustratorTextShape *text = dynamic_cast<CQIllustratorTextShape *>(shape);

    text->setText(str.toStdString());

    text->setFont(CQUtil::fromQFont(qfont_));

    text->setHAlign(halign_);
    text->setVAlign(valign_);

    const CBBox2D &bbox = text->getBBox();

    double width1  = bbox.getWidth ();
    double height1 = bbox.getHeight();

    double width  = widthEdit_ ->getValue();
    double height = heightEdit_->getValue();

    text->scale(bbox.getCenter(), width/width1, height/height1);

    illustrator->checkinShape(shape, CQIllustratorData::CHANGE_GEOMETRY);
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

    int font_diff     = INT_MAX;
    int font_size     = -1;
    int font_size_ind = -1;

    sizeCombo_->clear();

    for (int i = 0; i < sizes.size(); ++i) {
      int size = sizes[i];

      if (font_size_ == -1)
        font_size_ = size;

      if (abs(font_size_ - size) < font_diff) {
        font_diff     = abs(font_size_ - size);
        font_size     = size;
        font_size_ind = i;
      }

      sizeCombo_->addItem(QString("%1").arg(size));
    }

    if (font_size_ > 0) {
      font_size_ = font_size;

      sizeCombo_->setCurrentIndex(font_size_ind);
    }
  }

  int font_size = qfont_.pointSize();

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

//------

CQIllustratorCreateTextSizer::
CQIllustratorCreateTextSizer(CQIllustratorCreateTextMode *mode) :
 CQIllustratorModeSizer(mode)
{
  CQIllustrator *illustrator = mode_->getIllustrator();

  ll_handle_ = new CQIllustratorControlPointHandle(illustrator);
  ur_handle_ = new CQIllustratorControlPointHandle(illustrator);

  addHandle(ll_handle_);
  addHandle(ur_handle_);
}

void
CQIllustratorCreateTextSizer::
drawHandles(QPainter *painter, const CQIllustratorShape *shape)
{
  const CQIllustratorTextShape *text = dynamic_cast<const CQIllustratorTextShape *>(shape);

  if (text) {
    CQIllustratorShape::ControlPointList controlPoints;

    text->getControlPoints(controlPoints);

    ll_handle_->draw(text, controlPoints[0], painter);
    ur_handle_->draw(text, controlPoints[1], painter);
  }
}
