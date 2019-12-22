#include <CQIllustratorAlignMode.h>
#include <CQIllustrator.h>
#include <CQIllustratorShapeDrawer.h>

#include <QGridLayout>
#include <QLabel>
#include <QTabWidget>
#include <QPushButton>
#include <QComboBox>
#include <QStackedWidget>
#include <QPainter>
#include <QMenu>
#include <QAction>
#include <QIcon>
#include <QPixmap>
#include <QEvent>

#include <CQRealEdit.h>
#include <CQPointEdit.h>
#include <CQUtil.h>
#include <CQMenu.h>
#include <CQDockWidget.h>

#include <svg/align_svg.h>

#include <svg/align_to_bottom_svg.h>
#include <svg/align_to_left_svg.h>
#include <svg/align_to_right_svg.h>
#include <svg/align_to_top_svg.h>
#include <svg/align_horizontally_svg.h>
#include <svg/align_vertically_svg.h>

#include <svg/distribute_bottom_svg.h>
#include <svg/distribute_left_svg.h>
#include <svg/distribute_right_svg.h>
#include <svg/distribute_top_svg.h>
#include <svg/spread_horiz_svg.h>
#include <svg/spread_vert_svg.h>

#include <svg/align_select_svg.h>
#include <svg/align_left_bottom_svg.h>
#include <svg/align_right_top_svg.h>
#include <svg/align_middle_middle_svg.h>

CQIllustratorAlignMode::
CQIllustratorAlignMode(CQIllustrator *illustrator) :
 CQIllustratorMode(illustrator, (uint) CQIllustrator::Mode::ALIGN)
{
}

CQIllustratorAlignToolbar *
CQIllustratorAlignMode::
createToolbar()
{
  toolbar_ = new CQIllustratorAlignToolbar(this);

  connect(toolbar_, SIGNAL(alignLeft()), this, SLOT(alignLSlot()));
  connect(toolbar_, SIGNAL(alignLeftPreview()), this, SLOT(alignLPreviewSlot()));

  connect(toolbar_, SIGNAL(alignBottom()), this, SLOT(alignBSlot()));
  connect(toolbar_, SIGNAL(alignBottomPreview()), this, SLOT(alignBPreviewSlot()));

  connect(toolbar_, SIGNAL(alignRight()), this, SLOT(alignRSlot()));
  connect(toolbar_, SIGNAL(alignRightPreview()), this, SLOT(alignRPreviewSlot()));

  connect(toolbar_, SIGNAL(alignTop()), this, SLOT(alignTSlot()));
  connect(toolbar_, SIGNAL(alignTopPreview()), this, SLOT(alignTPreviewSlot()));

  connect(toolbar_, SIGNAL(alignHorizontal()), this, SLOT(alignHSlot()));
  connect(toolbar_, SIGNAL(alignHorizontalPreview()), this, SLOT(alignHPreviewSlot()));
  connect(toolbar_, SIGNAL(alignVertical()), this, SLOT(alignVSlot()));
  connect(toolbar_, SIGNAL(alignVerticalPreview()), this, SLOT(alignVPreviewSlot()));

  connect(toolbar_, SIGNAL(alignPreviewClear()), this, SLOT(alignPreviewClearSlot()));

  connect(toolbar_, SIGNAL(selectAnchorObject()), illustrator_, SLOT(anchorObjectSlot()));
  connect(toolbar_, SIGNAL(selectAnchorPosition()), illustrator_, SLOT(anchorPositionSlot()));
  connect(toolbar_, SIGNAL(cancelSelectAnchor()), illustrator_, SLOT(cancelAnchorSlot()));

  return toolbar_;
}

CQMenuItem *
CQIllustratorAlignMode::
createMenuItem(CQMenu *menu)
{
  QIcon icon = CQPixmapCacheInst->getIcon("ALIGN");

  menuItem_ = new CQMenuItem(menu, icon, "&Align", CQMenuItem::CHECKABLE);

  menuItem_->setStatusTip("Align Objects");

  connect(menuItem_->getAction(), SIGNAL(toggled(bool)), this, SLOT(menuItemSlot()));

  return menuItem_;
}

void
CQIllustratorAlignMode::
handleMouseRelease(const MouseEvent &e)
{
  if (dragging_) {
    // not moving so select control point
    if (! moving_) {
      selectControlPoint(e);
    }
    // dragging finished so commit
    else {
    }
  }
  // not dragging so do a select
  else {
    CPoint2D p1 = CQUtil::fromQPoint(press_wpos_);
    CPoint2D p2 = CQUtil::fromQPoint(release_wpos_);

    // point click - select at point
    if (! moving_) {
      (void) illustrator_->selectAt(p2, e.event->isControlKey(), e.event->isShiftKey());
    }
    // drag then create rectangle using specified bbox
    else {
      (void) illustrator_->selectIn(CBBox2D(p1, p2), e.event->isControlKey(),
                                    e.event->isShiftKey());
    }
  }

  CQIllustratorMode::handleMouseRelease(e);
}

void
CQIllustratorAlignMode::
handleMouseDrag(const MouseEvent &)
{
  illustrator_->redrawOverlay();
}

void
CQIllustratorAlignMode::
drawOverlay(CQIllustratorShapeDrawer *drawer)
{
  QPainter *painter = drawer->getPainter();

  CQIllustratorAlignToolbar::AnchorMode anchorMode = toolbar_->getAnchorMode();

  if (anchorMode == CQIllustratorAlignToolbar::AnchorMode::POSITION) {
    QPointF pos = toolbar_->getAnchorPosition();

    QPen pen;

    pen.setColor(QColor(255,0,0));
    pen.setStyle(Qt::DashLine);
    pen.setWidth(0);

    QPointF ps1 = illustrator_->getITransform().map(QPointF(0, 0));
    QPointF ps2 = illustrator_->getITransform().map(QPointF(8, 8));

    double px = fabs(ps1.x() - ps2.x());
    double py = fabs(ps1.y() - ps2.y());

    painter->setPen(pen);
    painter->setBrush(Qt::NoBrush);

    painter->drawLine(QPointF(pos.x() - px, pos.y()), QPointF(pos.x() + px, pos.y()));
    painter->drawLine(QPointF(pos.x(), pos.y() - py), QPointF(pos.x(), pos.y() + py));
  }
}

QCursor
CQIllustratorAlignMode::
getCursor() const
{
  return Qt::OpenHandCursor;
}

void
CQIllustratorAlignMode::
alignLSlot()
{
  align(CQIllustrator::ALIGN_LEFT, true);
}

void
CQIllustratorAlignMode::
alignLPreviewSlot()
{
  illustrator_->setDimmed(true);

  align(CQIllustrator::ALIGN_LEFT, false);
}

void
CQIllustratorAlignMode::
alignBSlot()
{
  align(CQIllustrator::ALIGN_BOTTOM, true);
}

void
CQIllustratorAlignMode::
alignBPreviewSlot()
{
  illustrator_->setDimmed(true);

  align(CQIllustrator::ALIGN_BOTTOM, false);
}

void
CQIllustratorAlignMode::
alignRSlot()
{
  align(CQIllustrator::ALIGN_RIGHT, true);
}

void
CQIllustratorAlignMode::
alignRPreviewSlot()
{
  illustrator_->setDimmed(true);

  align(CQIllustrator::ALIGN_RIGHT, false);
}

void
CQIllustratorAlignMode::
alignTSlot()
{
  align(CQIllustrator::ALIGN_TOP, true);
}

void
CQIllustratorAlignMode::
alignTPreviewSlot()
{
  illustrator_->setDimmed(true);

  align(CQIllustrator::ALIGN_TOP, false);
}

void
CQIllustratorAlignMode::
alignHSlot()
{
  align(CQIllustrator::ALIGN_HORIZONTAL, true);
}

void
CQIllustratorAlignMode::
alignHPreviewSlot()
{
  illustrator_->setDimmed(true);

  align(CQIllustrator::ALIGN_HORIZONTAL, false);
}

void
CQIllustratorAlignMode::
alignVSlot()
{
  align(CQIllustrator::ALIGN_VERTICAL, true);
}

void
CQIllustratorAlignMode::
alignVPreviewSlot()
{
  illustrator_->setDimmed(true);

  align(CQIllustrator::ALIGN_VERTICAL, false);
}

void
CQIllustratorAlignMode::
alignPreviewClearSlot()
{
  illustrator_->setDimmed(false);

  illustrator_->clearPreviewObjects();

  illustrator_->redrawOverlay();
}

void
CQIllustratorAlignMode::
align(CQIllustrator::AlignSide side, bool commit)
{
  CQIllustratorSelectedShapes *selection = illustrator_->getSelection();

  if (selection->empty()) return;

  //------

  if (illustrator_->getFlipY()) {
    if      (side == CQIllustrator::ALIGN_BOTTOM) side = CQIllustrator::ALIGN_TOP;
    else if (side == CQIllustrator::ALIGN_TOP   ) side = CQIllustrator::ALIGN_BOTTOM;
  }

  //------

  CQIllustratorShape *ashape = 0;

  CBBox2D abbox;
  CBBox2D bbbox;
  bool    afixed = false;

  CQIllustratorAlignToolbar::AnchorMode anchorMode = toolbar_->getAnchorMode();

  if      (anchorMode == CQIllustratorAlignToolbar::AnchorMode::SELECTION) {
    // get anchor shape for side

    CQIllustratorSelectedShapes::iterator ps1, ps2;

    ps1 = selection->begin();
    ps2 = selection->end();

    for ( ; ps1 != ps2; ++ps1) {
      CQIllustratorShape *shape = (*ps1).getShape();

      bbbox += shape->getFlatBBox();
    }

    ps1 = selection->begin();

    for ( ; ps1 != ps2; ++ps1) {
      CQIllustratorShape *shape = (*ps1).getShape();

      const CBBox2D &bbox = shape->getFlatBBox();

      if (! abbox.isSet() || (! afixed && shape->getFixed())) {
        ashape = shape;
        abbox  = bbox;
        afixed = ashape->getFixed();

        continue;
      }

      if (afixed && ! shape->getFixed())
        continue;

      if ((side == CQIllustrator::ALIGN_LEFT       && bbox.getXMin() < abbox.getXMin()) ||
          (side == CQIllustrator::ALIGN_BOTTOM     && bbox.getYMin() < abbox.getYMin()) ||
          (side == CQIllustrator::ALIGN_RIGHT      && bbox.getXMax() > abbox.getXMax()) ||
          (side == CQIllustrator::ALIGN_TOP        && bbox.getYMax() > abbox.getYMax()) ||
          (side == CQIllustrator::ALIGN_HORIZONTAL && bbox.getYMid() < abbox.getYMid()) ||
          (side == CQIllustrator::ALIGN_VERTICAL   && bbox.getXMid() < abbox.getXMid())) {
        ashape = shape;
        abbox  = bbox;
        afixed = ashape->getFixed();
      }
    }
  }
  else if (anchorMode == CQIllustratorAlignToolbar::AnchorMode::OBJECT) {
    CQIllustratorShape *shape = illustrator_->getShape(toolbar_->getAnchorObject().toStdString());

    if (shape == 0) return;

    CQIllustratorAlignToolbar::ObjectEdgeType edgeType =
      toolbar_->getAnchorObjectEdgeType();

    abbox = shape->getFlatBBox();

    if      (edgeType == CQIllustratorAlignToolbar::ObjectEdgeType::LEFT_BOTTOM)
      abbox = CBBox2D(abbox.getLL(), abbox.getLR());
    else if (edgeType == CQIllustratorAlignToolbar::ObjectEdgeType::RIGHT_TOP)
      abbox = CBBox2D(abbox.getUR(), abbox.getUR());
    else if (edgeType == CQIllustratorAlignToolbar::ObjectEdgeType::MIDDLE)
      abbox = CBBox2D(abbox.getCenter(), abbox.getCenter());

    bbbox = abbox;
  }
  else if (anchorMode == CQIllustratorAlignToolbar::AnchorMode::POSITION) {
    QPointF pos = toolbar_->getAnchorPosition();

    abbox = CBBox2D(CQUtil::fromQPoint(pos), CQUtil::fromQPoint(pos));

    CQIllustratorSelectedShapes::iterator ps1 = selection->begin(), ps2 = selection->end();

    for ( ; ps1 != ps2; ++ps1) {
      CQIllustratorShape *shape = (*ps1).getShape();

      const CBBox2D &bbox = shape->getFlatBBox();

      bbbox += bbox;
    }
  }
  else
    return;

  // align

  double offset = toolbar_->getOffset();

  if (commit) {
    illustrator_->startUndoGroup("Align");

    CQIllustratorSelectedShapes::iterator ps1 = selection->begin(), ps2 = selection->end();

    for ( ; ps1 != ps2; ++ps1) {
      CQIllustratorShape *shape = (*ps1).getShape();

      if (shape->getFixed()) continue;

      const CBBox2D &bbox = shape->getFlatBBox();

      double dx = 0.0, dy = 0.0;

      if (shape != ashape) {
        if      (side == CQIllustrator::ALIGN_LEFT      ) dx = abbox.getXMin() - bbox.getXMin();
        else if (side == CQIllustrator::ALIGN_BOTTOM    ) dy = abbox.getYMin() - bbox.getYMin();
        else if (side == CQIllustrator::ALIGN_RIGHT     ) dx = abbox.getXMax() - bbox.getXMax();
        else if (side == CQIllustrator::ALIGN_TOP       ) dy = abbox.getYMax() - bbox.getYMax();
        else if (side == CQIllustrator::ALIGN_HORIZONTAL) dy = abbox.getYMid() - bbox.getYMid();
        else if (side == CQIllustrator::ALIGN_VERTICAL  ) dx = abbox.getXMid() - bbox.getXMid();
      }

      if      (side == CQIllustrator::ALIGN_LEFT      ) dx += offset;
      else if (side == CQIllustrator::ALIGN_BOTTOM    ) dy += offset;
      else if (side == CQIllustrator::ALIGN_RIGHT     ) dx -= offset;
      else if (side == CQIllustrator::ALIGN_TOP       ) dy -= offset;
      else if (side == CQIllustrator::ALIGN_HORIZONTAL) dy += offset;
      else if (side == CQIllustrator::ALIGN_VERTICAL  ) dx += offset;

      shape->moveBy(CPoint2D(dx, dy));
    }

    illustrator_->endUndoGroup();

    illustrator_->setDimmed(false);

    illustrator_->clearPreviewObjects();

    illustrator_->redraw();
  }
  else {
    illustrator_->clearPreviewObjects();

    CQIllustratorSelectedShapes::iterator ps1 = selection->begin(), ps2 = selection->end();

    for ( ; ps1 != ps2; ++ps1) {
      CQIllustratorShape *shape = (*ps1).getShape();

      if (shape->getFixed()) continue;

      const CBBox2D &bbox = shape->getFlatBBox();

      double dx = 0.0, dy = 0.0;

      if (shape != ashape) {
        if      (side == CQIllustrator::ALIGN_LEFT      ) dx = abbox.getXMin() - bbox.getXMin();
        else if (side == CQIllustrator::ALIGN_BOTTOM    ) dy = abbox.getYMin() - bbox.getYMin();
        else if (side == CQIllustrator::ALIGN_RIGHT     ) dx = abbox.getXMax() - bbox.getXMax();
        else if (side == CQIllustrator::ALIGN_TOP       ) dy = abbox.getYMax() - bbox.getYMax();
        else if (side == CQIllustrator::ALIGN_HORIZONTAL) dy = abbox.getYMid() - bbox.getYMid();
        else if (side == CQIllustrator::ALIGN_VERTICAL  ) dx = abbox.getXMid() - bbox.getXMid();
      }

      if      (side == CQIllustrator::ALIGN_LEFT      ) dx += offset;
      else if (side == CQIllustrator::ALIGN_BOTTOM    ) dy += offset;
      else if (side == CQIllustrator::ALIGN_RIGHT     ) dx -= offset;
      else if (side == CQIllustrator::ALIGN_TOP       ) dy -= offset;
      else if (side == CQIllustrator::ALIGN_HORIZONTAL) dy += offset;
      else if (side == CQIllustrator::ALIGN_VERTICAL  ) dx += offset;

      illustrator_->addPreviewObject(new CQIllustrator::PreviewShape(shape, CPoint2D(dx, dy)));
    }

    double x1 = 0.0, y1 = 0.0, x2 = 0.0, y2 = 0.0;

    if      (side == CQIllustrator::ALIGN_LEFT || side == CQIllustrator::ALIGN_RIGHT) {
      x1 = abbox.getXMin();
      x2 = abbox.getXMax();

      y1 = bbbox.getYMin();
      y2 = bbbox.getYMax();
    }
    else if (side == CQIllustrator::ALIGN_BOTTOM || side == CQIllustrator::ALIGN_TOP) {
      y1 = abbox.getYMin();
      y2 = abbox.getYMax();

      x1 = bbbox.getXMin();
      x2 = bbbox.getXMax();
    }
    else if (side == CQIllustrator::ALIGN_HORIZONTAL) {
      x1 = bbbox.getXMin();
      x2 = bbbox.getXMax();

      y1 = abbox.getYMid();
      y2 = y1;
    }
    else if (side == CQIllustrator::ALIGN_VERTICAL) {
      y1 = bbbox.getYMin();
      y2 = bbbox.getYMax();

      x1 = abbox.getXMid();
      x2 = x1;
    }

    double dx = (x2 - x1)/8;
    double dy = (y2 - y1)/8;

    if      (side == CQIllustrator::ALIGN_LEFT      )
      illustrator_->addPreviewObject(
        new CQIllustrator::PreviewLine(CPoint2D(x1, y1 - dy), CPoint2D(x1, y2 + dy)));
    else if (side == CQIllustrator::ALIGN_BOTTOM    )
      illustrator_->addPreviewObject(
        new CQIllustrator::PreviewLine(CPoint2D(x1 - dx, y1), CPoint2D(x2 + dx, y1)));
    else if (side == CQIllustrator::ALIGN_RIGHT     )
      illustrator_->addPreviewObject(
        new CQIllustrator::PreviewLine(CPoint2D(x2, y1 - dy), CPoint2D(x2, y2 + dy)));
    else if (side == CQIllustrator::ALIGN_TOP       )
      illustrator_->addPreviewObject(
        new CQIllustrator::PreviewLine(CPoint2D(x1 - dx, y2), CPoint2D(x2 + dx, y2)));
    else if (side == CQIllustrator::ALIGN_HORIZONTAL)
      illustrator_->addPreviewObject(
        new CQIllustrator::PreviewLine(CPoint2D(x1 - dx, y1), CPoint2D(x2 + dx, y1)));
    else if (side == CQIllustrator::ALIGN_VERTICAL  )
      illustrator_->addPreviewObject(
        new CQIllustrator::PreviewLine(CPoint2D(x1, y1 - dy), CPoint2D(x1, y2 + dy)));

    illustrator_->redrawOverlay();
  }
}

//-------------------

CQIllustratorAlignToolbar::
CQIllustratorAlignToolbar(CQIllustratorAlignMode *mode) :
 CQIllustratorToolbar(mode), mode_(mode)
{
}

QIcon
CQIllustratorAlignToolbar::
getIcon()
{
  return CQPixmapCacheInst->getIcon("ALIGN");
}

void
CQIllustratorAlignToolbar::
addWidgets()
{
  QBoxLayout *layout = qobject_cast<QBoxLayout *>(CQIllustratorToolbar::layout());

  QGridLayout *grid = new QGridLayout;
  grid->setMargin(0); grid->setSpacing(2);

  layout->addLayout(grid);

  //-----

  QTabWidget *tab = new QTabWidget;

  QFont font = tab->font();

  font.setPointSizeF(font.pointSizeF()*0.7);

  tab->setFont(font);

  //------

  alignw_ = new CQAlignButtons;

  tab->addTab(alignw_, "Align");

  connect(alignw_, SIGNAL(alignLeft()), this, SIGNAL(alignLeft()));
  connect(alignw_, SIGNAL(alignBottom()), this, SIGNAL(alignBottom()));
  connect(alignw_, SIGNAL(alignRight()), this, SIGNAL(alignRight()));
  connect(alignw_, SIGNAL(alignTop()), this, SIGNAL(alignTop()));
  connect(alignw_, SIGNAL(alignHorizontal()), this, SIGNAL(alignHorizontal()));
  connect(alignw_, SIGNAL(alignVertical()), this, SIGNAL(alignVertical()));

  connect(alignw_, SIGNAL(alignLeftPreview()), this, SIGNAL(alignLeftPreview()));
  connect(alignw_, SIGNAL(alignBottomPreview()), this, SIGNAL(alignBottomPreview()));
  connect(alignw_, SIGNAL(alignRightPreview()), this, SIGNAL(alignRightPreview()));
  connect(alignw_, SIGNAL(alignTopPreview()), this, SIGNAL(alignTopPreview()));
  connect(alignw_, SIGNAL(alignHorizontalPreview()), this, SIGNAL(alignHorizontalPreview()));
  connect(alignw_, SIGNAL(alignVerticalPreview()), this, SIGNAL(alignVerticalPreview()));

  connect(alignw_, SIGNAL(alignPreviewClear()), this, SIGNAL(alignPreviewClear()));

  //------

  distw_ = new CQDistButtons;

  tab->addTab(distw_, "Distribute");

  //------

  spreadw_ = new CQSpreadButtons;

  tab->addTab(spreadw_, "Spread");

  //------

  grid->addWidget(tab, 0, 0, 2, 1);

  //------

  QLabel *olabel = new QLabel("<small><b>Offset</b></small>");

  offset_ = new CQRealEdit(0.0);

  offset_->setFocusPolicy(Qt::ClickFocus);

  grid->addWidget(olabel , 0, 1);
  grid->addWidget(offset_, 1, 1);

  //------

  anchor_ = new CQAlignAnchor;

  connect(anchor_, SIGNAL(selectObject()), this, SIGNAL(selectAnchorObject()));
  connect(anchor_, SIGNAL(selectPosition()), this, SIGNAL(selectAnchorPosition()));
  connect(anchor_, SIGNAL(cancelSelect()), this, SIGNAL(cancelSelectAnchor()));

  grid->addWidget(anchor_, 0, 2, 2, 1);

  //------

  grid->setColumnStretch(3, 1);
}

double
CQIllustratorAlignToolbar::
getOffset() const
{
  return offset_->getValue();
}

CQIllustratorAlignToolbar::AnchorMode
CQIllustratorAlignToolbar::
getAnchorMode() const
{
  return anchor_->getMode();
}

QString
CQIllustratorAlignToolbar::
getAnchorObject() const
{
  return anchor_->getObject();
}

void
CQIllustratorAlignToolbar::
setAnchorObject(const QString &name)
{
  anchor_->setObject(name);
}

CQIllustratorAlignToolbar::ObjectEdgeType
CQIllustratorAlignToolbar::
getAnchorObjectEdgeType() const
{
  return anchor_->getObjectEdgeType();
}

QPointF
CQIllustratorAlignToolbar::
getAnchorPosition() const
{
  return anchor_->getPosition();
}

void
CQIllustratorAlignToolbar::
setAnchorPosition(const QPointF &pos)
{
  anchor_->setPosition(pos);
}

void
CQIllustratorAlignToolbar::
resetSelectMode()
{
  anchor_->resetSelectMode();
}

//-------

CQAlignButtons::
CQAlignButtons() :
 QWidget(0)
{
  QGridLayout *agrid = new QGridLayout(this);
  agrid->setMargin(0); agrid->setSpacing(8);

  CQToolButton *alignl  = new CQToolButton(CQPixmapCacheInst->getIcon("ALIGN_TO_LEFT"));
  CQToolButton *alignb  = new CQToolButton(CQPixmapCacheInst->getIcon("ALIGN_TO_BOTTOM"));
  CQToolButton *alignr  = new CQToolButton(CQPixmapCacheInst->getIcon("ALIGN_TO_RIGHT"));
  CQToolButton *alignt  = new CQToolButton(CQPixmapCacheInst->getIcon("ALIGN_TO_TOP"));
  CQToolButton *alignhc = new CQToolButton(CQPixmapCacheInst->getIcon("ALIGN_HORIZONTALLY"));
  CQToolButton *alignvc = new CQToolButton(CQPixmapCacheInst->getIcon("ALIGN_VERTICALLY"));

  alignl ->setToolTip("Align Left");
  alignb ->setToolTip("Align Bottom");
  alignr ->setToolTip("Align Right");
  alignt ->setToolTip("Align Top");
  alignhc->setToolTip("Align Horizontal Center");
  alignvc->setToolTip("Align Vertical Center");

  agrid->addWidget(alignl , 0, 0);
  agrid->addWidget(alignb , 0, 1);
  agrid->addWidget(alignr , 0, 2);
  agrid->addWidget(alignt , 0, 3);
  agrid->addWidget(alignhc, 0, 4);
  agrid->addWidget(alignvc, 0, 5);

  agrid->setColumnStretch(6, 1);

  connect(alignl , SIGNAL(clicked()), this, SIGNAL(alignLeft()));
  connect(alignb , SIGNAL(clicked()), this, SIGNAL(alignBottom()));
  connect(alignr , SIGNAL(clicked()), this, SIGNAL(alignRight()));
  connect(alignt , SIGNAL(clicked()), this, SIGNAL(alignTop()));
  connect(alignhc, SIGNAL(clicked()), this, SIGNAL(alignHorizontal()));
  connect(alignvc, SIGNAL(clicked()), this, SIGNAL(alignVertical()));

  connect(alignl , SIGNAL(previewStart()), this, SIGNAL(alignLeftPreview()));
  connect(alignb , SIGNAL(previewStart()), this, SIGNAL(alignBottomPreview()));
  connect(alignr , SIGNAL(previewStart()), this, SIGNAL(alignRightPreview()));
  connect(alignt , SIGNAL(previewStart()), this, SIGNAL(alignTopPreview()));
  connect(alignhc, SIGNAL(previewStart()), this, SIGNAL(alignHorizontalPreview()));
  connect(alignvc, SIGNAL(previewStart()), this, SIGNAL(alignVerticalPreview()));

  connect(alignl , SIGNAL(previewStop()), this, SIGNAL(alignPreviewClear()));
  connect(alignb , SIGNAL(previewStop()), this, SIGNAL(alignPreviewClear()));
  connect(alignr , SIGNAL(previewStop()), this, SIGNAL(alignPreviewClear()));
  connect(alignt , SIGNAL(previewStop()), this, SIGNAL(alignPreviewClear()));
  connect(alignhc, SIGNAL(previewStop()), this, SIGNAL(alignPreviewClear()));
  connect(alignvc, SIGNAL(previewStop()), this, SIGNAL(alignPreviewClear()));
}

//-------

CQDistButtons::
CQDistButtons() :
 QWidget(0)
{
  QGridLayout *dgrid = new QGridLayout(this);
  dgrid->setMargin(0); dgrid->setSpacing(8);

  CQToolButton *distl = new CQToolButton(CQPixmapCacheInst->getIcon("DISTRIBUTE_LEFT"));
  CQToolButton *distb = new CQToolButton(CQPixmapCacheInst->getIcon("DISTRIBUTE_BOTTOM"));
  CQToolButton *distt = new CQToolButton(CQPixmapCacheInst->getIcon("DISTRIBUTE_RIGHT"));
  CQToolButton *distr = new CQToolButton(CQPixmapCacheInst->getIcon("DISTRIBUTE_TOP"));

  dgrid->addWidget(distl, 0, 0);
  dgrid->addWidget(distb, 0, 1);
  dgrid->addWidget(distt, 0, 2);
  dgrid->addWidget(distr, 0, 3);

  dgrid->setColumnStretch(4, 1);
}

//-------

CQSpreadButtons::
CQSpreadButtons() :
 QWidget(0)
{
  QGridLayout *sgrid = new QGridLayout(this);
  sgrid->setMargin(0); sgrid->setSpacing(8);

  CQToolButton *spreadh = new CQToolButton(CQPixmapCacheInst->getIcon("SPREAD_HORIZ"));
  CQToolButton *spreadv = new CQToolButton(CQPixmapCacheInst->getIcon("SPREAD_VERT"));

  sgrid->addWidget(spreadh, 0, 0);
  sgrid->addWidget(spreadv, 0, 1);

  sgrid->setColumnStretch(2, 1);
}

//--------

CQAlignAnchor::
CQAlignAnchor(QWidget *parent) :
 QWidget(parent)
{
  QGridLayout *layout = new QGridLayout(this);
  layout->setMargin(0); layout->setSpacing(2);

  QLabel *anchorLabel = new QLabel("<small><b>Anchor</b></small>");

  layout->addWidget(anchorLabel, 0, 0, 1, 2);

  objectCombo_ = new QComboBox;

  objectCombo_->addItem("Selection");
  objectCombo_->addItem("Object");
  objectCombo_->addItem("Position");

  connect(objectCombo_, SIGNAL(activated(const QString &)),
          this, SLOT(objectSlot(const QString &)));

  layout->addWidget(objectCombo_, 1, 0);

  anchorStack_ = new QStackedWidget;

  anchorLabel_  = new QWidget;
  anchorObject_ = new CQAlignAnchorObject;
  anchorPoint_  = new CQAlignAnchorPoint;

  anchorStack_->addWidget(anchorLabel_);
  anchorStack_->addWidget(anchorObject_);
  anchorStack_->addWidget(anchorPoint_);

  layout->addWidget(anchorStack_, 1, 1);

  connect(anchorObject_, SIGNAL(selectObject()), this, SIGNAL(selectObject()));
  connect(anchorObject_, SIGNAL(cancelSelect()), this, SIGNAL(cancelSelect()));

  connect(anchorPoint_, SIGNAL(selectPoint()), this, SIGNAL(selectPosition()));
  connect(anchorPoint_, SIGNAL(cancelSelect()), this, SIGNAL(cancelSelect()));

  //-----

  updateState();
}

void
CQAlignAnchor::
updateState()
{
  if      (mode_ == CQIllustratorAlignToolbar::AnchorMode::SELECTION)
    anchorStack_->setCurrentIndex(0);
  else if (mode_ == CQIllustratorAlignToolbar::AnchorMode::OBJECT)
    anchorStack_->setCurrentIndex(1);
  else if (mode_ == CQIllustratorAlignToolbar::AnchorMode::POSITION)
    anchorStack_->setCurrentIndex(2);
}

CQIllustratorAlignToolbar::AnchorMode
CQAlignAnchor::
getMode() const
{
  return mode_;
}

QString
CQAlignAnchor::
getObject() const
{
  return anchorObject_->getName();
}

void
CQAlignAnchor::
setObject(const QString &name)
{
  anchorObject_->setName(name);
}

CQIllustratorAlignToolbar::ObjectEdgeType
CQAlignAnchor::
getObjectEdgeType() const
{
  return anchorObject_->getEdgeType();
}

QPointF
CQAlignAnchor::
getPosition() const
{
  return anchorPoint_->getValue();
}

void
CQAlignAnchor::
setPosition(const QPointF &pos)
{
  anchorPoint_->setValue(pos);
}

void
CQAlignAnchor::
resetSelectMode()
{
  anchorObject_->resetSelectMode();
  anchorPoint_ ->resetSelectMode();
}

void
CQAlignAnchor::
objectSlot(const QString &obj)
{
  if      (obj == "Selection")
    mode_ = CQIllustratorAlignToolbar::AnchorMode::SELECTION;
  else if (obj == "Object")
    mode_ = CQIllustratorAlignToolbar::AnchorMode::OBJECT;
  else if (obj == "Position")
    mode_ = CQIllustratorAlignToolbar::AnchorMode::POSITION;

  updateState();
}

//--------

CQAlignAnchorObject::
CQAlignAnchorObject(QWidget *parent) :
 QWidget(parent)
{
  QHBoxLayout *layout = new QHBoxLayout(this);
  layout->setMargin(0); layout->setSpacing(2);

  nameEdit_ = new QLineEdit;

  nameEdit_->setFocusPolicy(Qt::ClickFocus);

  layout->addWidget(nameEdit_);

  edgeButton_ = new QToolButton;

  edgeButton_->setPopupMode(QToolButton::MenuButtonPopup);

  edgeButton_->setIcon(CQPixmapCacheInst->getIcon("ALIGN_LEFT_BOTTOM"));

  QMenu *edgeMenu = new QMenu;

  edgeMenu->addAction(CQPixmapCacheInst->getIcon("ALIGN_LEFT_BOTTOM"  ), "Left/Bottom");
  edgeMenu->addAction(CQPixmapCacheInst->getIcon("ALIGN_RIGHT_TOP"    ), "Right/Top");
  edgeMenu->addAction(CQPixmapCacheInst->getIcon("ALIGN_MIDDLE_MIDDLE"), "Middle");

  edgeButton_->setMenu(edgeMenu);

  connect(edgeButton_, SIGNAL(triggered(QAction *)), this, SLOT(edgeSlot(QAction *)));

  layout->addWidget(edgeButton_);

  selButton_ = new QToolButton;

  selButton_->setCheckable(true);
  selButton_->setAutoRaise(true);

  selButton_->setIcon(CQPixmapCacheInst->getIcon("ALIGN_SELECT"));

  connect(selButton_, SIGNAL(clicked(bool)), this, SLOT(selectSlot(bool)));

  layout->addWidget(selButton_);

}

QString
CQAlignAnchorObject::
getName() const
{
  return nameEdit_->text();
}

void
CQAlignAnchorObject::
setName(const QString &name)
{
  nameEdit_->setText(name);
}

void
CQAlignAnchorObject::
resetSelectMode()
{
  selButton_->setChecked(false);
}

void
CQAlignAnchorObject::
edgeSlot(QAction *action)
{
  QString str = action->text();

  if      (str == "Left/Bottom")
    edgeType_ = CQIllustratorAlignToolbar::ObjectEdgeType::LEFT_BOTTOM;
  else if (str == "Right/Top")
    edgeType_ = CQIllustratorAlignToolbar::ObjectEdgeType::RIGHT_TOP;
  else if (str == "Middle")
    edgeType_ = CQIllustratorAlignToolbar::ObjectEdgeType::MIDDLE;

  edgeButton_->setIcon(action->icon());
}

void
CQAlignAnchorObject::
selectSlot(bool enabled)
{
  if (enabled)
    emit selectObject();
  else
    emit cancelSelect();
}

//--------

CQAlignAnchorPoint::
CQAlignAnchorPoint(QWidget *parent) :
 QWidget(parent)
{
  QHBoxLayout *layout = new QHBoxLayout(this);
  layout->setMargin(0); layout->setSpacing(2);

  pointEdit_ = new CQPointEdit(CPoint2D(0,0));

  pointEdit_->setFocusPolicy(Qt::ClickFocus);

  layout->addWidget(pointEdit_);

  selButton_ = new QToolButton;

  selButton_->setCheckable(true);
  selButton_->setAutoRaise(true);

  selButton_->setIcon(CQPixmapCacheInst->getIcon("ALIGN_SELECT"));

  connect(selButton_, SIGNAL(clicked(bool)), this, SLOT(selectSlot(bool)));

  layout->addWidget(selButton_);
}

QPointF
CQAlignAnchorPoint::
getValue() const
{
  return CQUtil::toQPoint(pointEdit_->getValue());
}

void
CQAlignAnchorPoint::
setValue(const QPointF &pos)
{
  pointEdit_->setValue(CQUtil::fromQPoint(pos));
}

void
CQAlignAnchorPoint::
resetSelectMode()
{
  selButton_->setChecked(false);
}

void
CQAlignAnchorPoint::
selectSlot(bool enabled)
{
  if (enabled)
    emit selectPoint();
  else
    emit cancelSelect();
}

//--------

CQToolButton::
CQToolButton(const QIcon &icon) :
 QToolButton(0)
{
  setAutoRaise(true);

  setIcon(icon);
}

bool
CQToolButton::
event(QEvent *e)
{
  QEvent::Type type = e->type();

  switch (type) {
    case QEvent::ToolTip: {
      //QHelpEvent *helpEvent = static_cast<QHelpEvent *>(event);

      preview_ = true;

      emit previewStart();

      break;
    }
    case QEvent::MouseMove:
    case QEvent::Leave: {
      if (preview_) {
        preview_ = false;

        emit previewStop();
      }

      break;
    }
    default:
      break;
  }

  return QToolButton::event(e);
}
