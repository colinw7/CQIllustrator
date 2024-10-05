#include <CQIllustratorSetImageMode.h>
#include <CQIllustratorRectShape.h>
#include <CQIllustrator.h>
#include <CQIllustratorHandle.h>
#include <CQIllustratorShapeDrawer.h>
#include <CQIllustratorUtil.h>

#include <QPainter>
#include <QComboBox>
#include <QBoxLayout>
#include <QFileDialog>

#include <COSFile.h>
#include <CLinearGradient.h>
#include <CQMenu.h>
#include <CQUtil.h>
#include <CQUtilGeom.h>
#include <CQImageButton.h>
#include <CQSwatch.h>

#include <svg/image_svg.h>
#include <svg/lalign_svg.h>
#include <svg/hcalign_svg.h>
#include <svg/ralign_svg.h>
#include <svg/balign_svg.h>
#include <svg/vcalign_svg.h>
#include <svg/talign_svg.h>

CQIllustratorSetImageMode::
CQIllustratorSetImageMode(CQIllustrator *illustrator) :
 CQIllustratorMode(illustrator, uint(CQIllustrator::Mode::IMAGE))
{
}

CQIllustratorSetImageToolbar *
CQIllustratorSetImageMode::
createToolbar()
{
  toolbar_ = new CQIllustratorSetImageToolbar(this);

  return toolbar_;
}

CQIllustratorImageSizer *
CQIllustratorSetImageMode::
createSizer()
{
  sizer_ = new CQIllustratorImageSizer(this);

  return sizer_;
}

CQMenuItem *
CQIllustratorSetImageMode::
createMenuItem(CQMenu *menu)
{
  QIcon icon = CQPixmapCacheInst->getIcon("IMAGE");

  menuItem_ = new CQMenuItem(menu, icon, "&Image", CQMenuItem::CHECKABLE);

  menuItem_->setStatusTip("Create/Modify Fill Image");

  connect(menuItem_->getAction(), SIGNAL(toggled(bool)), this, SLOT(menuItemSlot()));

  return menuItem_;
}

void
CQIllustratorSetImageMode::
handleMouseRelease(const MouseEvent &)
{
  if (dragging_ && ! moving_) {
    illustrator_->getSandbox()->clear();

    dragging_ = false;
  }

  // not dragging so add image to selected objects
  if (! dragging_) {
  }
  // dragging finished so commit
  else {
    illustrator_->getSandbox()->commit(CQIllustratorData::ChangeType::FILL);
  }

  dragging_ = false;
}

void
CQIllustratorSetImageMode::
handleMouseDrag(const MouseEvent &)
{
  if (dragging_) {
    CQIllustratorShape *shape = illustrator_->getSandbox()->frontShape();

    CQIllustratorShapeFill &fill = shape->getFill();

    if (fill.hasImage()) {
      CImagePtr image = fill.getImage();

      assert(image.isValid());
    }
  }

  illustrator_->redrawOverlay();
}

void
CQIllustratorSetImageMode::
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
      pen.setWidth(0);

      painter->setPen(pen);
      painter->setBrush(Qt::NoBrush);

      painter->drawLine(press_wpos_, current_wpos);
    }

    //------

    // draw handles for single selected shape
    CQIllustratorSelectedShapes *selection = illustrator_->getSelection();

    if (selection->size() == 1) {
      CQIllustratorShape *shape = selection->front().getShape();

      sizer_->draw(painter, shape);
    }
  }
  else {
    CQIllustratorShape *shape = illustrator_->getSandbox()->frontShape();

    sizer_->draw(painter, shape);
  }
}

QCursor
CQIllustratorSetImageMode::
getCursor() const
{
  return Qt::ArrowCursor;
}

//------------

CQIllustratorSetImageToolbar::
CQIllustratorSetImageToolbar(CQIllustratorSetImageMode *mode) :
 CQIllustratorToolbar(mode), mode_(mode), scale_(CQIllustratorShapeFill::ImageScale::NONE),
 halign_(CHALIGN_TYPE_CENTER), valign_(CVALIGN_TYPE_CENTER)
{
}

QIcon
CQIllustratorSetImageToolbar::
getIcon()
{
  return CQPixmapCacheInst->getIcon("IMAGE");
}

void
CQIllustratorSetImageToolbar::
addWidgets()
{
  QBoxLayout *layout = qobject_cast<QBoxLayout *>(CQIllustratorToolbar::layout());

  //-----

  imageButton_ = new CQImageButton(CQPixmapCacheInst->getIcon("IMAGE"));

  CQSwatch *fileSwatch = new CQSwatch("Image", imageButton_);

  connect(imageButton_, SIGNAL(clicked()), this, SLOT(setImageSlot()));

  layout->addWidget(fileSwatch);

  //-----

  scaleCombo_ = new QComboBox;

  scaleCombo_->addItem("None");
  scaleCombo_->addItem("Fit");
  scaleCombo_->addItem("Equal");

  CQSwatch *scaleSwatch = new CQSwatch("Scale", scaleCombo_);

  connect(scaleCombo_, SIGNAL(currentIndexChanged(int)), this, SLOT(setScaleSlot(int)));

  layout->addWidget(scaleSwatch);

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

  CQSwatch *halignSwatch =
    new CQSwatch("Horizontal Align<3>", lalignButton_, hcalignButton_, ralignButton_);

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

  CQSwatch *valignSwatch =
    new CQSwatch("Vertical Align<3>", talignButton_, vcalignButton_, balignButton_);

  layout->addWidget(valignSwatch);

  //-----

  // tile, zoom, center, scale, stretch, span
}

void
CQIllustratorSetImageToolbar::
setSelectedShape(const CQIllustratorShape *shape)
{
  if (shape) {
    const CQIllustratorShapeFill &fill = shape->getFill();

    if (fill.hasImage()) {
      scale_  = fill.getImageScale();
      halign_ = fill.getImageHAlign();
      valign_ = fill.getImageVAlign();

      updateWidgets();
    }
  }
}

void
CQIllustratorSetImageToolbar::
updateShape()
{
  CQIllustrator *illustrator = mode_->getIllustrator();

  CQIllustratorShape *shape =
    CQIllustratorUtil::getCurrentShape<CQIllustratorShape>(illustrator);

  if (shape) {
    illustrator->checkoutShape(shape, CQIllustratorData::ChangeType::GEOMETRY);

    shape = dynamic_cast<CQIllustratorShape *>(shape);

    CQIllustratorShapeFill &fill = shape->getFill();

    if (fill.hasImage()) {
      fill.setImageScale (scale_ );
      fill.setImageHAlign(halign_);
      fill.setImageVAlign(valign_);
    }

    illustrator->checkinShape(shape, CQIllustratorData::ChangeType::GEOMETRY);
  }

  illustrator->redraw();
}

void
CQIllustratorSetImageToolbar::
updateWidgets()
{
  if      (scale_ == CQIllustratorShapeFill::ImageScale::NONE)
    scaleCombo_->setCurrentIndex(0);
  else if (scale_ == CQIllustratorShapeFill::ImageScale::FIT)
    scaleCombo_->setCurrentIndex(1);
  else
    scaleCombo_->setCurrentIndex(2);

  lalignButton_ ->setChecked(halign_ == CHALIGN_TYPE_LEFT);
  hcalignButton_->setChecked(halign_ == CHALIGN_TYPE_CENTER);
  ralignButton_ ->setChecked(halign_ == CHALIGN_TYPE_RIGHT);

  talignButton_ ->setChecked(valign_ == CVALIGN_TYPE_TOP);
  vcalignButton_->setChecked(valign_ == CVALIGN_TYPE_CENTER);
  balignButton_ ->setChecked(valign_ == CVALIGN_TYPE_BOTTOM);
}

void
CQIllustratorSetImageToolbar::
setImageSlot()
{
  CQIllustrator *illustrator = mode_->getIllustrator();

  CQIllustratorSelectedShapes *selection = illustrator->getSelection();

  CQIllustratorShape *shape = nullptr;

  if (selection->size() == 1)
    shape = selection->front().getShape();

  //-------

  QString title  = "Load Image";
  QString cwd    = QString(COSFile::getCurrentDir().c_str());
  QString filter = "Image Files (*)";

  QStringList filenames = QFileDialog::getOpenFileNames(this, title, cwd, filter);

  if (filenames.size() == 0)
    return;

  QStringListIterator fi(filenames);

  while (fi.hasNext()) {
    CImageFileSrc src(fi.next().toStdString());

    CImagePtr image = CImageMgrInst->lookupImage(src);

    if (image.isValid()) {
      setShapeImage(shape, image);
      return;
    }
  }
}

void
CQIllustratorSetImageToolbar::
setShapeImage(CQIllustratorShape *shape, CImagePtr image)
{
  CQIllustrator *illustrator = mode_->getIllustrator();

  if (shape == nullptr) {
    CPoint2D p1(0, 0);
    CPoint2D p2(image->getWidth(), image->getHeight());

    CQIllustratorRectShape *rect = illustrator->createRectShape(p1, p2);

    illustrator->addShape(rect);

    illustrator->setSelectShape(rect);

    shape = rect;
  }

  CQIllustratorShapeFill fill = shape->getFill();

  fill.setImage(image);

  fill.setImageScale(CQIllustratorShapeFill::ImageScale::FIT);

  shape->setFill(fill);

  illustrator->emitFillChanged();

  illustrator->redraw();
}

void
CQIllustratorSetImageToolbar::
setScaleSlot(int ind)
{
  if      (ind == 0) scale_ = CQIllustratorShapeFill::ImageScale::NONE;
  else if (ind == 1) scale_ = CQIllustratorShapeFill::ImageScale::FIT;
  else               scale_ = CQIllustratorShapeFill::ImageScale::EQUAL;

  updateShape();
}

void
CQIllustratorSetImageToolbar::
lalignSlot()
{
  halign_ = CHALIGN_TYPE_LEFT;

  updateShape();
}

void
CQIllustratorSetImageToolbar::
hcalignSlot()
{
  halign_ = CHALIGN_TYPE_CENTER;

  updateShape();
}

void
CQIllustratorSetImageToolbar::
ralignSlot()
{
  halign_ = CHALIGN_TYPE_RIGHT;

  updateShape();
}

void
CQIllustratorSetImageToolbar::
talignSlot()
{
  valign_ = CVALIGN_TYPE_TOP;

  updateShape();
}

void
CQIllustratorSetImageToolbar::
vcalignSlot()
{
  valign_ = CVALIGN_TYPE_CENTER;

  updateShape();
}

void
CQIllustratorSetImageToolbar::
balignSlot()
{
  valign_ = CVALIGN_TYPE_BOTTOM;

  updateShape();
}

//----------

CQIllustratorImageSizer::
CQIllustratorImageSizer(CQIllustratorSetImageMode *mode) :
 CQIllustratorModeSizer(mode)
{
}

void
CQIllustratorImageSizer::
drawHandles(QPainter *, const CQIllustratorShape *)
{
}
