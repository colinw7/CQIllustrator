#include <CQIllustratorCreatePolygonMode.h>
#include <CQIllustratorPolygonShape.h>
#include <CQIllustrator.h>
#include <CQIllustratorHandle.h>
#include <CQIllustratorShapeDrawer.h>
#include <CQIllustratorUtil.h>

#include <QPainter>
#include <QPainterPath>
#include <QHBoxLayout>
#include <QRadioButton>

#include <CQMenu.h>
#include <CQUtil.h>
#include <CQUtilGeom.h>
#include <CQPointEdit.h>
#include <CQImageButton.h>
#include <CQSwatch.h>

#include <svg/polygon_svg.h>
#include <svg/path_add_svg.h>
#include <svg/path_remove_svg.h>

#include <svg/poly_free_svg.h>
#include <svg/poly_rect_svg.h>
#include <svg/poly_45_svg.h>

#include <svg/control_point_svg.h>
#include <svg/control_point_active_svg.h>

#include <svg/snap_point_svg.h>
#include <svg/snap_point_active_svg.h>

#include <cursors/select.xbm>
#include <cursors/selectmask.xbm>

CQIllustratorCreatePolygonMode::
CQIllustratorCreatePolygonMode(CQIllustrator *illustrator) :
 CQIllustratorMode(illustrator, uint(CQIllustrator::Mode::POLYGON))
{
  setCursor(select_bits, selectmask_bits, 2, 2);
}

CQIllustratorCreatePolygonToolbar *
CQIllustratorCreatePolygonMode::
createToolbar()
{
  toolbar_ = new CQIllustratorCreatePolygonToolbar(this);

  return toolbar_;
}

CQIllustratorCreatePolygonSizer *
CQIllustratorCreatePolygonMode::
createSizer()
{
  sizer_ = new CQIllustratorCreatePolygonSizer(this);

  return sizer_;
}

CQMenuItem *
CQIllustratorCreatePolygonMode::
createMenuItem(CQMenu *menu)
{
  QIcon icon = CQPixmapCacheInst->getIcon("POLYGON");

  menuItem_ = new CQMenuItem(menu, icon, "&Polygon", CQMenuItem::CHECKABLE);

  menuItem_->setStatusTip("Create Polygon");

  connect(menuItem_->getAction(), SIGNAL(toggled(bool)), this, SLOT(menuItemSlot()));

  return menuItem_;
}

void
CQIllustratorCreatePolygonMode::
handleMousePress(const MouseEvent &e)
{
  CQIllustratorMode::handleMousePress(e);

  if (! dragging_) {
    if (editMode_ == EditMode::CREATE) {
      addPoint(e.window, e.pixel);
    }
  }
}

void
CQIllustratorCreatePolygonMode::
handleMouseRelease(const MouseEvent &e)
{
  CPoint2D p1 = CQUtil::fromQPoint(press_wpos_);
  CPoint2D p2 = CQUtil::fromQPoint(e.window);

  if (dragging_) {
    if (! moving_) {
      selectControlPoint(e);
    }
    // dragging finished so commit
    else {
      illustrator_->getSandbox()->commit(CQIllustratorData::ChangeType::GEOMETRY);
    }
  }
  // not dragging so do a select
  else {
    if (editMode_ == EditMode::CREATE) {
    }
    else {
      // point click - select at point
      if (! moving_) {
        (void) illustrator_->selectAt(p2, e.event->isControlKey(), e.event->isShiftKey());
      }
      // drag - select points in rectangle
      else {
        CBBox2D bbox(p1, p2);

        illustrator_->selectPointsIn(bbox, CQIllustratorShape::ControlType::GEOMETRY,
                                     e.event->isControlKey(), e.event->isShiftKey());
      }
    }
  }

  dragging_ = false;

  illustrator_->redraw();
}

void
CQIllustratorCreatePolygonMode::
handleMouseDrag(const MouseEvent &e)
{
  if (dragging_) {
    CQIllustratorShape *shape = illustrator_->getSandbox()->frontShape();

    if (shape)
      sizer_->updatePoint(shape, e.window);

    toolbar_->setSelectionPoint(CQUtil::fromQPoint(e.window));
  }
  else {
    if (editMode_ == EditMode::CREATE) {
    }
    else {
    }
  }

  illustrator_->redrawOverlay();

  prev_wpos_ = e.window;
}

void
CQIllustratorCreatePolygonMode::
handleMouseMove(const MouseEvent &e)
{
  uint numCloseHandles = closeHandles_.size();

  for (uint i = 0; i < numCloseHandles; ++i)
    closeHandles_[i]->updateActive(e.pixel);

  uint numSnapHandles = snapHandles_.size();

  for (uint i = 0; i < numSnapHandles; ++i)
    snapHandles_[i]->updateActive(e.pixel);

  CQIllustratorMode::handleMouseMove(e);
}

bool
CQIllustratorCreatePolygonMode::
handleKeyPress(const KeyEvent &e)
{
  CKeyType key = e.event->getType();

  if      (key == CKEY_TYPE_BackSpace) {
    if (! polygonPoints_.empty()) {
      polygonPoints_.pop_back();

      illustrator_->redrawOverlay();

      return true;
    }
  }
  else if (key == CKEY_TYPE_Space) {
    addPoint(e.window, e.pixel);

    return true;
  }

  return CQIllustratorMode::handleKeyPress(e);
}

void
CQIllustratorCreatePolygonMode::
addPoint(QPointF w, QPoint p)
{
  bool closed = false;

  uint numCloseHandles = closeHandles_.size();

  for (uint i = 0; i < numCloseHandles; ++i) {
    if (closeHandles_[i]->getVisible() && closeHandles_[i]->getActive())
      closed = true;
  }

  //----

  bool snapped = false;

  uint numSnapHandles = snapHandles_.size();

  for (uint i = 0; i < numSnapHandles; ++i) {
    if (snapHandles_[i]->getVisible() && snapHandles_[i]->getActive())
      snapped = true;
  }

  //----

  uint num_points = polygonPoints_.size();

  if (num_points == 0)
    polygonStart_ = p;

  CPoint2D p1 = CQUtil::fromQPoint(w);

  snapPoint(p1);

  if      (closed)
    commitPolygonPoints();
  else if (snapped) {
    for (uint i = 0; i < numSnapHandles; ++i) {
      if (snapHandles_[i]->getVisible() && snapHandles_[i]->getActive())
        polygonPoints_.push_back(snapHandles_[i]->getPosition());
    }
  }
  else
    polygonPoints_.push_back(p1);
}

void
CQIllustratorCreatePolygonMode::
snapPoint(CPoint2D &p)
{
  uint numCloseHandles = closeHandles_.size();

  for (uint i = 0; i < numCloseHandles; ++i)
    closeHandles_[i]->setVisible(false);

  uint numSnapHandles = snapHandles_.size();

  for (uint i = 0; i < numSnapHandles; ++i)
    snapHandles_[i]->setVisible(false);

  uint num_points = polygonPoints_.size();

  if (num_points == 0) return;

  if (num_points > 0) {
    const CPoint2D &p1 = polygonPoints_[0];

    addCloseHandle(p, p1);
  }

  closeHandleNum_ = 0;
  snapHandleNum_  = 0;

  CQIllustratorCreatePolygonToolbar::CreateMode createMode = toolbar_->getCreateMode();

  if      (createMode == CQIllustratorCreatePolygonToolbar::CreateMode::RECT) {
    if      (num_points > 1) {
      const CPoint2D &p1 = polygonPoints_[num_points - 2];
      const CPoint2D &p2 = polygonPoints_[num_points - 1];

      double dx = fabs(p2.x - p1.x);
      double dy = fabs(p2.y - p1.y);

      if      (dx > dy) { // last horizontal, next vertical
        p.x = p2.x;
      }
      else if (dy > dx) { // last vertical, next horizontal
        p.y = p2.y;
      }
    }
    else if (num_points > 0) {
      const CPoint2D &p1 = polygonPoints_[num_points - 1];

      double dx = fabs(p.x - p1.x);
      double dy = fabs(p.y - p1.y);

      if      (dx > dy) { // snap horizontal
        p.y = p1.y;
      }
      else if (dy > dx) { // snap vertical
        p.x = p1.x;
      }
    }

    if (num_points > 1) {
      const CPoint2D &p1 = polygonPoints_[num_points - 1];

      CLine2D l(p1, p);

      for (uint i = 0; i < num_points; ++i) {
        const CPoint2D &pp = polygonPoints_[i];

        CLine2D l1(pp, pp + CPoint2D(1, 0)); // H
        CLine2D l2(pp, pp + CPoint2D(0, 1)); // V

        CPoint2D pi;

        if (CMathGeom2D::IntersectLine(l, l1, &pi, nullptr, nullptr)) addSnapHandle(p, pi);
        if (CMathGeom2D::IntersectLine(l, l2, &pi, nullptr, nullptr)) addSnapHandle(p, pi);
      }
    }
  }
  else if (createMode == CQIllustratorCreatePolygonToolbar::CreateMode::ANGLE_45) {
    if (num_points > 0) {
      const CPoint2D &p1 = polygonPoints_[num_points - 1];

      double dx = fabs(p.x - p1.x);
      double dy = fabs(p.y - p1.y);

      if      (fabs(dy - dx) < dx && fabs(dy - dx) < dy) { // 45
        double d = p.x - p1.x;

        p.x = p1.x + d;

        if (p.y > p1.y)
          p.y = p1.y + fabs(d);
        else
          p.y = p1.y - fabs(d);
      }
      else if (dx > dy) { // snap horizontal
        p.y = p1.y;
      }
      else if (dy > dx) { // snap vertical
        p.x = p1.x;
      }
    }

    if (num_points > 1) {
      const CPoint2D &p1 = polygonPoints_[num_points - 1];

      CLine2D l(p1, p);

      for (uint i = 0; i < num_points; ++i) {
        const CPoint2D &pp = polygonPoints_[i];

        CLine2D l1(pp, pp + CPoint2D(1,  0)); // H
        CLine2D l2(pp, pp + CPoint2D(0,  1)); // V
        CLine2D l3(pp, pp + CPoint2D(1,  1)); // 45 (up)
        CLine2D l4(pp, pp + CPoint2D(1, -1)); // 45 (down)

        CPoint2D pi;

        if (CMathGeom2D::IntersectLine(l, l1, &pi, nullptr, nullptr)) addSnapHandle(p, pi);
        if (CMathGeom2D::IntersectLine(l, l2, &pi, nullptr, nullptr)) addSnapHandle(p, pi);
        if (CMathGeom2D::IntersectLine(l, l3, &pi, nullptr, nullptr)) addSnapHandle(p, pi);
        if (CMathGeom2D::IntersectLine(l, l4, &pi, nullptr, nullptr)) addSnapHandle(p, pi);
      }
    }
  }
}

void
CQIllustratorCreatePolygonMode::
addCloseHandle(const CPoint2D &cp, const CPoint2D &p)
{
  while (closeHandleNum_ >= closeHandles_.size()) {
    CQIllustratorHandle *closeHandle = new CQIllustratorHandle(illustrator_);

    closeHandle->setImage(CQPixmapCacheInst->getIcon("CONTROL_POINT"),
                          CQPixmapCacheInst->getIcon("CONTROL_POINT_ACTIVE"));

    closeHandles_.push_back(closeHandle);

    closeHandle->setVisible(false);
  }

  QPointF pp1 = illustrator_->getTransform().map(CQUtil::toQPoint(cp));
  QPointF pp2 = illustrator_->getTransform().map(CQUtil::toQPoint(p ));

  double dx = fabs(pp1.x() - pp2.x());
  double dy = fabs(pp1.y() - pp2.y());

  if (dx*dx + dy*dy > 100) return;

  for (uint i = 0; i < closeHandles_.size(); ++i) {
    const CPoint2D &p1 = closeHandles_[i]->getPosition();

    QPointF pp3 = illustrator_->getTransform().map(CQUtil::toQPoint(p1));

    if (fabs(pp3.x() - p.x) < 1E-3 && fabs(pp3.y() - p.y) < 1E-3)
      return;
  }

  closeHandles_[closeHandleNum_]->setPosition(p);

  closeHandles_[closeHandleNum_]->setVisible(true);

  ++closeHandleNum_;
}

void
CQIllustratorCreatePolygonMode::
addSnapHandle(const CPoint2D &cp, const CPoint2D &p)
{
  while (snapHandleNum_ >= snapHandles_.size()) {
    CQIllustratorHandle *snapHandle = new CQIllustratorHandle(illustrator_);

    snapHandle->setImage(CQPixmapCacheInst->getIcon("SNAP_POINT"),
                         CQPixmapCacheInst->getIcon("SNAP_POINT_ACTIVE"));

    snapHandles_.push_back(snapHandle);

    snapHandle->setVisible(false);
  }

  QPointF pp1 = illustrator_->getTransform().map(CQUtil::toQPoint(cp));
  QPointF pp2 = illustrator_->getTransform().map(CQUtil::toQPoint(p ));

  double dx = fabs(pp1.x() - pp2.x());
  double dy = fabs(pp1.y() - pp2.y());

  if (dx*dx + dy*dy > 100) return;

  for (uint i = 0; i < closeHandles_.size(); ++i) {
    const CPoint2D &p1 = closeHandles_[i]->getPosition();

    QPointF pp3 = illustrator_->getTransform().map(CQUtil::toQPoint(p1));

    if (fabs(pp3.x() - p.x) < 1E-3 && fabs(pp3.y() - p.y) < 1E-3)
      return;
  }

  for (uint i = 0; i < snapHandles_.size(); ++i) {
    const CPoint2D &p1 = snapHandles_[i]->getPosition();

    QPointF pp3 = illustrator_->getTransform().map(CQUtil::toQPoint(p1));

    if (fabs(pp3.x() - p.x) < 1E-3 && fabs(pp3.y() - p.y) < 1E-3)
      return;
  }

  snapHandles_[snapHandleNum_]->setPosition(p);

  snapHandles_[snapHandleNum_]->setVisible(true);

  ++snapHandleNum_;
}

void
CQIllustratorCreatePolygonMode::
accept()
{
  commitPolygonPoints();
}

bool
CQIllustratorCreatePolygonMode::
cancel()
{
  polygonPoints_.clear();

  illustrator_->redrawOverlay();

  return false;
}

void
CQIllustratorCreatePolygonMode::
drawOverlay(CQIllustratorShapeDrawer *drawer)
{
  QPainter *painter = drawer->getPainter();

  drawPolygonPoints(painter);

  //------

  if (! dragging_) {
    if (pressed_) {
      if (editMode_ == EditMode::CREATE) {
      }
      else {
        // draw rubber band
        QPen pen;

        pen.setColor(QColor(0,0,0));
        pen.setStyle(Qt::DashLine);
        pen.setWidth(0);

        painter->setPen(pen);
        painter->setBrush(Qt::NoBrush);

        painter->drawRect(QRectF(press_wpos_, curr_wpos_));
      }
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

    if (shape)
      sizer_->draw(painter, shape);
  }
}

void
CQIllustratorCreatePolygonMode::
drawPolygonPoints(QPainter *painter)
{
  uint num_points = polygonPoints_.size();

  if (num_points == 0) return;

  // draw current polygon points
  QPen pen;

  pen.setColor(QColor(0,0,0));
  pen.setStyle(Qt::DashLine);
  pen.setWidth(0);

  painter->setPen(pen);
  painter->setBrush(Qt::NoBrush);

  QPainterPath path;

  const CPoint2D &ps = polygonPoints_[0];

  path.moveTo(ps.x, ps.y);

  for (uint i = 1; i < num_points; ++i) {
    const CPoint2D &p = polygonPoints_[i];

    path.lineTo(p.x, p.y);
  }

  CPoint2D pc = CQUtil::fromQPoint(curr_wpos_);

  snapPoint(pc);

  if (! pressed_)
    path.lineTo(pc.x, pc.y);

  path.lineTo(ps.x, ps.y);

  painter->strokePath(path, QPen(QColor(255,0,0)));

  //-----

  // draw close points
  uint numCloseHandles = closeHandles_.size();

  for (uint i = 0; i < numCloseHandles; ++i) {
    if (closeHandles_[i]->getVisible())
      closeHandles_[i]->draw(painter);
  }

  //-----

  // draw snap points
  uint numSnapHandles = snapHandles_.size();

  for (uint i = 0; i < numSnapHandles; ++i) {
    if (snapHandles_[i]->getVisible())
      snapHandles_[i]->draw(painter);
  }
}

QCursor
CQIllustratorCreatePolygonMode::
getCursor() const
{
  return Qt::ArrowCursor;
}

void
CQIllustratorCreatePolygonMode::
commitPolygonPoints()
{
  uint num_points = polygonPoints_.size();

  if (num_points == 0) return;

  CQIllustratorPolygonShape *polygon = illustrator_->createPolygonShape();

  polygon->setPoints(polygonPoints_);

  illustrator_->addShape(polygon);

  illustrator_->setSelectShape(polygon);

  polygonPoints_.clear();

  illustrator_->redraw();
}

void
CQIllustratorCreatePolygonMode::
addPointToCurrent()
{
  CQIllustratorSelectedShapes *selection = illustrator_->getSelection();

  if (selection->size() != 1) return;

  const CQIllustratorSelectedShape &sshape = selection->front();

  uint num = sshape.numPoints();

  if (num != 2) return;

  illustrator_->getSandbox()->addSelection();

  CQIllustratorShape *shape = illustrator_->getSandbox()->frontShape();

  CQIllustratorPolygonShape *polygon = dynamic_cast<CQIllustratorPolygonShape *>(shape);

  const CQIllustratorPolygonShapeControlPoint *controlPoint1 =
    dynamic_cast<const CQIllustratorPolygonShapeControlPoint *>(sshape.getPoint(0));
  const CQIllustratorPolygonShapeControlPoint *controlPoint2 =
    dynamic_cast<const CQIllustratorPolygonShapeControlPoint *>(sshape.getPoint(1));

  CPoint2D p1 = controlPoint1->getPoint(shape);
  CPoint2D p2 = controlPoint2->getPoint(shape);

  if (controlPoint1->getInd() < controlPoint2->getInd())
    polygon->addPoint(controlPoint2, (p1 + p2)/2);
  else
    polygon->addPoint(controlPoint1, (p1 + p2)/2);

  illustrator_->getSandbox()->commit(CQIllustratorData::ChangeType::GEOMETRY);
}

void
CQIllustratorCreatePolygonMode::
removePointFromCurrent()
{
  CQIllustratorSelectedShapes *selection = illustrator_->getSelection();

  if (selection->size() < 1) return;

  const CQIllustratorSelectedShape &sshape = selection->front();

  uint num = sshape.numPoints();

  illustrator_->getSandbox()->addSelection();

  CQIllustratorShape *shape = illustrator_->getSandbox()->frontShape();

  CQIllustratorPolygonShape *polygon = dynamic_cast<CQIllustratorPolygonShape *>(shape);

  for (uint i = 0; i < num; ++i) {
    int i1 = num - i - 1;

    const CQIllustratorPolygonShapeControlPoint *controlPoint =
      dynamic_cast<const CQIllustratorPolygonShapeControlPoint *>(sshape.getPoint(i1));

    polygon->removePoint(controlPoint);
  }

  illustrator_->getSandbox()->commit(CQIllustratorData::ChangeType::GEOMETRY);
}

//------------

CQIllustratorCreatePolygonToolbar::
CQIllustratorCreatePolygonToolbar(CQIllustratorCreatePolygonMode *mode) :
 CQIllustratorToolbar(mode), mode_(mode), createMode_(CreateMode::FREE)
{
}

QIcon
CQIllustratorCreatePolygonToolbar::
getIcon()
{
  return CQPixmapCacheInst->getIcon("POLYGON");
}

void
CQIllustratorCreatePolygonToolbar::
addWidgets()
{
  QBoxLayout *layout = qobject_cast<QBoxLayout *>(CQIllustratorToolbar::layout());

  //-----

#if 0
  QWidget *modeFrame = new QWidget;

  QVBoxLayout *modeLayout = new QVBoxLayout(modeFrame);

  createRadio_ = new QRadioButton("Create");
  editRadio_   = new QRadioButton("Edit");

  createRadio_->setChecked(true);

  createRadio_->setFocusPolicy(Qt::NoFocus);
  editRadio_  ->setFocusPolicy(Qt::NoFocus);

  modeLayout->addWidget(createRadio_);
  modeLayout->addWidget(editRadio_);

  connect(createRadio_, SIGNAL(toggled(bool)), this, SLOT(modeChangedSlot()));

  CQSwatch *modeSwatch = new CQSwatch(modeFrame);

  layout->addWidget(modeSwatch);
#endif

  //-----

  polyFreeButton_ = new CQImageButton(CQPixmapCacheInst->getIcon("POLY_FREE"));
  polyRectButton_ = new CQImageButton(CQPixmapCacheInst->getIcon("POLY_RECT"));
  poly45Button_   = new CQImageButton(CQPixmapCacheInst->getIcon("POLY_45"));

  polyFreeButton_->setToolTip("Polygon (Free)");
  polyRectButton_->setToolTip("Polygon (Rectilinear)");
  poly45Button_  ->setToolTip("Polygon (45 degree)");

  connect(polyFreeButton_, SIGNAL(toggled(bool)), this, SLOT(polyFreeSlot(bool)));
  connect(polyRectButton_, SIGNAL(toggled(bool)), this, SLOT(polyRectSlot(bool)));
  connect(poly45Button_  , SIGNAL(toggled(bool)), this, SLOT(poly45Slot(bool)));

  polyFreeButton_->setCheckable(true);
  polyRectButton_->setCheckable(true);
  poly45Button_  ->setCheckable(true);

  polyFreeButton_->setChecked(true);

  CQSwatch *createModeSwatch =
    new CQSwatch("Mode", polyFreeButton_, polyRectButton_, poly45Button_);

  layout->addWidget(createModeSwatch);

  //-----

  addPointButton_    = new CQImageButton(CQPixmapCacheInst->getIcon("PATH_ADD"));
  removePointButton_ = new CQImageButton(CQPixmapCacheInst->getIcon("PATH_REMOVE"));

  addPointButton_   ->setToolTip("Add Point");
  removePointButton_->setToolTip("Remove Point");

  connect(addPointButton_   , SIGNAL(clicked()), this, SLOT(addPointSlot   ()));
  connect(removePointButton_, SIGNAL(clicked()), this, SLOT(removePointSlot()));

  CQSwatch *modifySwatch = new CQSwatch("Modify", addPointButton_, removePointButton_);

  layout->addWidget(modifySwatch);

  //-----

  pointEdit_ = new CQPointEdit;

  connect(pointEdit_, SIGNAL(valueChanged()), this, SLOT(updatePointSlot()));

  CQSwatch *pointSwatch = new CQSwatch("Position", pointEdit_);

  layout->addWidget(pointSwatch);
}

void
CQIllustratorCreatePolygonToolbar::
modeChangedSlot()
{
  if (createRadio_->isChecked())
    mode_->setEditMode(CQIllustratorMode::EditMode::CREATE);
  else
    mode_->setEditMode(CQIllustratorMode::EditMode::EDIT);
}

void
CQIllustratorCreatePolygonToolbar::
polyFreeSlot(bool state)
{
  if (state) {
    createMode_ = CreateMode::FREE;

    polyRectButton_->setChecked(false);
    poly45Button_  ->setChecked(false);
  }
}

void
CQIllustratorCreatePolygonToolbar::
polyRectSlot(bool state)
{
  if (state) {
    createMode_ = CreateMode::RECT;

    polyFreeButton_->setChecked(false);
    poly45Button_  ->setChecked(false);
  }
}

void
CQIllustratorCreatePolygonToolbar::
poly45Slot(bool state)
{
  if (state) {
    createMode_ = CreateMode::ANGLE_45;

    polyRectButton_->setChecked(false);
    polyFreeButton_->setChecked(false);
  }
}

void
CQIllustratorCreatePolygonToolbar::
addPointSlot()
{
  mode_->addPointToCurrent();
}

void
CQIllustratorCreatePolygonToolbar::
removePointSlot()
{
  mode_->removePointFromCurrent();
}

void
CQIllustratorCreatePolygonToolbar::
updatePointSlot()
{
  CPoint2D p = pointEdit_->getValue();

  CQIllustrator *illustrator = mode_->getIllustrator();

  CQIllustratorSelectedShapes *selection = illustrator->getSelection();

  if (selection->size() != 1) return;

  const CQIllustratorSelectedShape &sshape = selection->front();

  uint num = sshape.numPoints();

  if (num != 1) return;

  CQIllustratorSelectedShape &sshape1 =
    selection->checkoutShape(sshape, mode_->getChangeType());

  CQIllustratorShapeControlPoint *point = sshape1.getPoint(0);

  point->setPoint(sshape1.getShape(), p);

  selection->checkinShape(sshape, mode_->getChangeType());
}

void
CQIllustratorCreatePolygonToolbar::
setSelectedShape(const CQIllustratorShape *shape)
{
  const CQIllustratorPolygonShape *polygon =
    dynamic_cast<const CQIllustratorPolygonShape *>(shape);

  if (! polygon) return;
}

void
CQIllustratorCreatePolygonToolbar::
setSelectedShapePoint(const CQIllustratorShape *shape,
                      const CQIllustratorShapeControlPoint *point)
{
  const CQIllustratorPolygonShape *polygon =
    dynamic_cast<const CQIllustratorPolygonShape *>(shape);

  if (! polygon) return;

  if (point)
    pointEdit_->setValue(point->getPoint(shape));
  else
    pointEdit_->setValue(CPoint2D(0,0));
}

void
CQIllustratorCreatePolygonToolbar::
updateShape()
{
  CQIllustrator *illustrator = mode_->getIllustrator();

  CQIllustratorPolygonShape *shape =
    CQIllustratorUtil::getCurrentShape<CQIllustratorPolygonShape>(illustrator);

  CQIllustratorPolygonShape *polygon = nullptr;

  if (shape)
    polygon = dynamic_cast<CQIllustratorPolygonShape *>(shape);

  if (polygon) {
    //illustrator->checkoutShape(shape);

    //illustrator->checkinShape(shape);
  }

  illustrator->redraw();
}

void
CQIllustratorCreatePolygonToolbar::
setSelectionPoint(const CPoint2D &p)
{
  pointEdit_->setValue(p);
}

//------

CQIllustratorCreatePolygonSizer::
CQIllustratorCreatePolygonSizer(CQIllustratorCreatePolygonMode *mode) :
 CQIllustratorModeSizer(mode)
{
}

void
CQIllustratorCreatePolygonSizer::
drawHandles(QPainter *painter, const CQIllustratorShape *shape)
{
  const CQIllustratorPolygonShape *polygon =
    dynamic_cast<const CQIllustratorPolygonShape *>(shape);

  if (polygon) {
    CQIllustratorShape::ControlPointList controlPoints;

    polygon->getControlPoints(controlPoints);

    uint numControlPoints = controlPoints.size();

    updateHandles(numControlPoints);

    for (uint i = 0; i < numControlPoints; ++i) {
      CQIllustratorPolygonShapeControlPoint *controlPoint =
        dynamic_cast<CQIllustratorPolygonShapeControlPoint *>(controlPoints[i]);

      controlPointHandles_[i]->draw(polygon, controlPoint, painter);
    }
  }
}

void
CQIllustratorCreatePolygonSizer::
updateHandles(uint numHandles)
{
  CQIllustrator *illustrator = mode_->getIllustrator();

  while (controlPointHandles_.size() < numHandles) {
    CQIllustratorControlPointHandle *handle = new CQIllustratorControlPointHandle(illustrator);

    addHandle(handle);
  }

  while (controlPointHandles_.size() > numHandles) {
    CQIllustratorControlPointHandle *handle = nullptr;

    removeHandle(handle);
  }
}
