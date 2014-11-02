#include <CQIllustratorCreatePathMode.h>
#include <CQIllustrator.h>
#include <CQIllustratorHandle.h>
#include <CQIllustratorShapeDrawer.h>

#include <QPainter>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QRadioButton>

#include <CQMenu.h>
#include <CQUtil.h>
#include <CQPointEdit.h>
#include <CQImageButton.h>
#include <CQSwatch.h>

#include <xpm/path.xpm>
#include <xpm/path_line.xpm>
#include <xpm/path_curve2.xpm>
#include <xpm/path_curve3.xpm>

#include <xpm/path_free.xpm>
#include <xpm/path_rect.xpm>
#include <xpm/path_45.xpm>

#include <xpm/path_corner.xpm>
#include <xpm/path_curve.xpm>
#include <xpm/path_add.xpm>
#include <xpm/path_remove.xpm>

#include <xpm/control_point.xpm>
#include <xpm/control_point_active.xpm>

#include <xpm/snap_point.xpm>
#include <xpm/snap_point_active.xpm>

#include <cursors/select.xbm>
#include <cursors/selectmask.xbm>

#define IMAGE_DATA(I) I, sizeof(I)/sizeof(char *)

CQIllustratorCreatePathMode::
CQIllustratorCreatePathMode(CQIllustrator *illustrator) :
 CQIllustratorMode(illustrator, CQIllustrator::MODE_PATH), selMode_(POINT_SEL),
 pathMode_(CPATH_PART_TYPE_LINE_TO)
{
  setCursor(select_bits, selectmask_bits, 2, 2);
}

void
CQIllustratorCreatePathMode::
setSelMode(SelMode selMode)
{
  selMode_ = selMode;
}

void
CQIllustratorCreatePathMode::
setPathMode(CPathPartType pathMode)
{
  pathMode_ = pathMode;
}

CQIllustratorCreatePathToolbar *
CQIllustratorCreatePathMode::
createToolbar()
{
  toolbar_ = new CQIllustratorCreatePathToolbar(this);

  return toolbar_;
}

CQIllustratorCreatePathSizer *
CQIllustratorCreatePathMode::
createSizer()
{
  sizer_ = new CQIllustratorCreatePathSizer(this);

  return sizer_;
}

CQMenuItem *
CQIllustratorCreatePathMode::
createMenuItem(CQMenu *menu)
{
  menuItem_ = new CQMenuItem(menu, "&Path", CQMenuItem::CHECKABLE);

  menuItem_->setStatusTip("Create Path");
  menuItem_->setXPMIcon(path_data);

  connect(menuItem_->getAction(), SIGNAL(toggled(bool)), this, SLOT(menuItemSlot()));

  return menuItem_;
}

void
CQIllustratorCreatePathMode::
handleMousePress(const MouseEvent &e)
{
  CQIllustratorMode::handleMousePress(e);

  if (! dragging_) {
    if (editMode_ == CREATE_MODE) {
      addPoint(e.window, e.pixel);
    }
  }
}

void
CQIllustratorCreatePathMode::
handleMouseRelease(const MouseEvent &e)
{
  CPoint2D p1 = CQUtil::fromQPoint(press_wpos_);
  CPoint2D p2 = CQUtil::fromQPoint(e.window);

  if (dragging_) {
    if (! moving_) {
      selectControlPoint(e);

      // TODO: use point selection signal for this
      CQIllustratorSelectedShapes *selection = illustrator_->getSelection();

      if (selection->size() == 1) {
        const CQIllustratorSelectedShape &sshape = selection->front();

        const CQIllustratorShape *shape = sshape.getShape();

        uint num_points = sshape.numPoints();

        for (uint i = 0; i < num_points; ++i) {
          const CQIllustratorShapeControlPoint *spoint = sshape.getPoint(i);

          CPoint2D p = spoint->getPoint(shape);

          toolbar_->setSelectionPoint(p);
        }
      }

      illustrator_->getSandbox()->clear();
    }
    // dragging finished so commit
    else {
      illustrator_->getSandbox()->commit(CQIllustratorData::CHANGE_GEOMETRY);
    }
  }
  // not dragging so do a select
  else {
    if (editMode_ == CREATE_MODE) {
    }
    else {
      // point click - select at point
      if (! moving_) {
        (void) illustrator_->selectAt(p2, e.event->isControlKey(), e.event->isShiftKey());
      }
      // drag - select points in rectangle
      else {
        CBBox2D bbox(p1, p2);

        if (selMode_ == POINT_SEL)
          illustrator_->selectPointsIn(bbox, CQIllustratorShape::CONTROL_GEOMETRY,
                                       e.event->isControlKey(), e.event->isShiftKey());
        else
          illustrator_->selectLinesIn(bbox, e.event->isControlKey(), e.event->isShiftKey());
      }
    }
  }

  dragging_ = false;

  illustrator_->redraw();
}

void
CQIllustratorCreatePathMode::
handleMouseDrag(const MouseEvent &e)
{
  if (dragging_) {
    CQIllustratorShape *shape = illustrator_->getSandbox()->frontShape();

    sizer_->updatePoint(shape, e.window);

    toolbar_->setSelectionPoint(CQUtil::fromQPoint(e.window));
  }
  else {
    if (editMode_ == CREATE_MODE) {
      //CPoint2D p1 = CQUtil::fromQPoint(press_wpos_);
      CPoint2D p2 = CQUtil::fromQPoint(e.window);

      uint num_points = pathPoints_.size();

      if (num_points <= 0) return;

      double dx = p2.x - pathPoints_[num_points - 1].p.x;
      double dy = p2.y - pathPoints_[num_points - 1].p.y;

      if (num_points > 0)
        pathPoints_[num_points - 1].c1 = CPoint2D(pathPoints_[num_points - 1].p.x + dx,
                                                  pathPoints_[num_points - 1].p.y + dy);

      if (num_points > 1)
        pathPoints_[num_points - 2].c2 = CPoint2D(pathPoints_[num_points - 1].p.x - dx,
                                                  pathPoints_[num_points - 1].p.y - dy);
    }
  }

  illustrator_->redrawOverlay();

  prev_wpos_ = e.window;
}

void
CQIllustratorCreatePathMode::
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
CQIllustratorCreatePathMode::
handleKeyPress(const KeyEvent &e)
{
  CKeyType key = e.event->getType();

  if (key == CKEY_TYPE_BackSpace) {
    if (! pathPoints_.empty()) {
      pathPoints_.pop_back();

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
CQIllustratorCreatePathMode::
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

  uint num_points = pathPoints_.size();

  if (num_points == 0)
    pathStart_ = p;

  CPoint2D p1 = CQUtil::fromQPoint(w);

  snapPoint(p1);

  if      (closed) {
    commitPathPoints(closed);
  }
  else if (snapped) {
    for (uint i = 0; i < numSnapHandles; ++i) {
      if (snapHandles_[i]->getVisible() && snapHandles_[i]->getActive()) {
        CPathPartType pathMode = getPathMode();

        pathPoints_.push_back(PathPoint(pathMode, snapHandles_[i]->getPosition()));
      }
    }
  }
  else {
    CPathPartType pathMode = getPathMode();

    pathPoints_.push_back(PathPoint(pathMode, p1));
  }
}

void
CQIllustratorCreatePathMode::
snapPoint(CPoint2D &p)
{
  uint numCloseHandles = closeHandles_.size();

  for (uint i = 0; i < numCloseHandles; ++i)
    closeHandles_[i]->setVisible(false);

  uint numSnapHandles = snapHandles_.size();

  for (uint i = 0; i < numSnapHandles; ++i)
    snapHandles_[i]->setVisible(false);

  uint num_points = pathPoints_.size();

  if (num_points == 0) return;

  if (num_points > 0) {
    const CPoint2D &p1 = pathPoints_[0].p;

    addCloseHandle(p, p1);
  }

  closeHandleNum_ = 0;
  snapHandleNum_  = 0;

  CQIllustratorCreatePathToolbar::CreateMode createMode = toolbar_->getCreateMode();

  if      (createMode == CQIllustratorCreatePathToolbar::CREATE_RECT_MODE) {
    if      (num_points > 1) {
      const CPoint2D &p1 = pathPoints_[num_points - 2].p;
      const CPoint2D &p2 = pathPoints_[num_points - 1].p;

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
      const CPoint2D &p1 = pathPoints_[num_points - 1].p;

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
      const CPoint2D &p1 = pathPoints_[num_points - 1].p;

      CLine2D l(p1, p);

      for (uint i = 0; i < num_points; ++i) {
        const CPoint2D &pp = pathPoints_[i].p;

        CLine2D l1(pp, pp + CPoint2D(1, 0)); // H
        CLine2D l2(pp, pp + CPoint2D(0, 1)); // V

        CPoint2D pi;

        if (CMathGeom2D::IntersectLine(l, l1, &pi, NULL, NULL)) addSnapHandle(p, pi);
        if (CMathGeom2D::IntersectLine(l, l2, &pi, NULL, NULL)) addSnapHandle(p, pi);
      }
    }
  }
  else if (createMode == CQIllustratorCreatePathToolbar::CREATE_45_MODE) {
    if (num_points > 0) {
      const CPoint2D &p1 = pathPoints_[num_points - 1].p;

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
      const CPoint2D &p1 = pathPoints_[num_points - 1].p;

      CLine2D l(p1, p);

      for (uint i = 0; i < num_points; ++i) {
        const CPoint2D &pp = pathPoints_[i].p;

        CLine2D l1(pp, pp + CPoint2D(1,  0)); // H
        CLine2D l2(pp, pp + CPoint2D(0,  1)); // V
        CLine2D l3(pp, pp + CPoint2D(1,  1)); // 45 (up)
        CLine2D l4(pp, pp + CPoint2D(1, -1)); // 45 (down)

        CPoint2D pi;

        if (CMathGeom2D::IntersectLine(l, l1, &pi, NULL, NULL)) addSnapHandle(p, pi);
        if (CMathGeom2D::IntersectLine(l, l2, &pi, NULL, NULL)) addSnapHandle(p, pi);
        if (CMathGeom2D::IntersectLine(l, l3, &pi, NULL, NULL)) addSnapHandle(p, pi);
        if (CMathGeom2D::IntersectLine(l, l4, &pi, NULL, NULL)) addSnapHandle(p, pi);
      }
    }
  }
}

void
CQIllustratorCreatePathMode::
addCloseHandle(const CPoint2D &cp, const CPoint2D &p)
{
  while (closeHandleNum_ >= closeHandles_.size()) {
    CQIllustratorHandle *closeHandle = new CQIllustratorHandle(illustrator_);

    closeHandle->setImage(IMAGE_DATA(control_point_data), IMAGE_DATA(control_point_active_data));

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
CQIllustratorCreatePathMode::
addSnapHandle(const CPoint2D &cp, const CPoint2D &p)
{
  while (snapHandleNum_ >= snapHandles_.size()) {
    CQIllustratorHandle *snapHandle = new CQIllustratorHandle(illustrator_);

    snapHandle->setImage(IMAGE_DATA(snap_point_data), IMAGE_DATA(snap_point_active_data));

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
CQIllustratorCreatePathMode::
accept()
{
  commitPathPoints(false);
}

bool
CQIllustratorCreatePathMode::
cancel()
{
  pathPoints_.clear();

  illustrator_->redrawOverlay();

  return false;
}

void
CQIllustratorCreatePathMode::
drawOverlay(CQIllustratorShapeDrawer *drawer)
{
  QPainter *painter = drawer->getPainter();

  drawPathPoints(painter);

  //------

  if (! dragging_) {
    if (pressed_) {
      if (editMode_ == CREATE_MODE) {
      }
      else {
        // draw rubber band
        QPen pen;

        pen.setColor(QColor(0,0,0));
        pen.setStyle(Qt::DashLine);

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

    sizer_->draw(painter, shape);
  }
}

void
CQIllustratorCreatePathMode::
drawPathPoints(QPainter *painter)
{
  uint num_points = pathPoints_.size();

  if (num_points == 0) return;

  // draw current path points
  QPen pen;

  pen.setColor(QColor(0,0,0));
  pen.setStyle(Qt::DashLine);

  painter->setPen(pen);
  painter->setBrush(Qt::NoBrush);

  QPainterPath path;

  const CPoint2D &ps = pathPoints_[0].p;

  path.moveTo(ps.x, ps.y);

  for (uint i = 1; i < num_points; ++i) {
    const PathPoint &p1 = pathPoints_[i - 1];
    const PathPoint &p2 = pathPoints_[i];

    if      (p1.type == CPATH_PART_TYPE_LINE_TO)
      path.lineTo(p2.p.x, p2.p.y);
    else if (p1.type == CPATH_PART_TYPE_CURVE2_TO) {
      path.quadTo(CQUtil::toQPoint(p1.c1), CQUtil::toQPoint(p2.p));

      painter->drawLine(CQUtil::toQPoint(p1.p ), CQUtil::toQPoint(p1.c1));
      painter->drawLine(CQUtil::toQPoint(p1.c1), CQUtil::toQPoint(p2.p ));
    }
    else if (p1.type == CPATH_PART_TYPE_CURVE3_TO) {
      path.cubicTo(CQUtil::toQPoint(p1.c1), CQUtil::toQPoint(p1.c2), CQUtil::toQPoint(p2.p));

      painter->drawLine(CQUtil::toQPoint(p1.p ), CQUtil::toQPoint(p1.c1));
      painter->drawLine(CQUtil::toQPoint(p1.c1), CQUtil::toQPoint(p1.c2));
      painter->drawLine(CQUtil::toQPoint(p1.c2), CQUtil::toQPoint(p2.p ));
    }
  }

  const PathPoint &p1 = pathPoints_[num_points - 1];

  CPoint2D pc = CQUtil::fromQPoint(curr_wpos_);

  snapPoint(pc);

  PathPoint p2(getPathMode(), pc);

  if (! pressed_) {
    if      (p1.type == CPATH_PART_TYPE_LINE_TO)
      path.lineTo(p2.p.x, p2.p.y);
    else if (p1.type == CPATH_PART_TYPE_CURVE2_TO) {
      path.quadTo(CQUtil::toQPoint(p1.c1), CQUtil::toQPoint(p2.p));

      painter->drawLine(CQUtil::toQPoint(p1.p ), CQUtil::toQPoint(p1.c1));
      painter->drawLine(CQUtil::toQPoint(p1.c1), CQUtil::toQPoint(p2.p ));
    }
    else if (p1.type == CPATH_PART_TYPE_CURVE3_TO) {
      path.cubicTo(CQUtil::toQPoint(p1.c1), CQUtil::toQPoint(p1.c2), CQUtil::toQPoint(p2.p));

      painter->drawLine(CQUtil::toQPoint(p1.p ), CQUtil::toQPoint(p1.c1));
      painter->drawLine(CQUtil::toQPoint(p1.c1), CQUtil::toQPoint(p1.c2));
      painter->drawLine(CQUtil::toQPoint(p1.c2), CQUtil::toQPoint(p2.p ));
    }
  }

  painter->strokePath(path, QPen(QColor(255,0,0)));

  if (pressed_) {
    double dx = p2.p.x - p1.p.x;
    double dy = p2.p.y - p1.p.y;

    if      (getPathMode() == CPATH_PART_TYPE_CURVE2_TO) {
      painter->drawLine(CQUtil::toQPoint(p1.p - CPoint2D(dx, dy)),
                        CQUtil::toQPoint(p1.p + CPoint2D(dx, dy)));
    }
    else if (getPathMode() == CPATH_PART_TYPE_CURVE3_TO) {
      painter->drawLine(CQUtil::toQPoint(p1.p - CPoint2D(dx, dy)),
                        CQUtil::toQPoint(p1.p + CPoint2D(dx, dy)));
    }
  }

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
CQIllustratorCreatePathMode::
getCursor() const
{
  return Qt::ArrowCursor;
}

void
CQIllustratorCreatePathMode::
commitPathPoints(bool closed)
{
  uint num_points = pathPoints_.size();

  if (num_points == 0) return;

  CPathShape *path = illustrator_->createPathShape();

  CQIllustratorShapeFill fill;

  fill.setOpacity(0.0);

  path->setFill(fill);

  const PathPoint &ps = pathPoints_[0];

  path->addMoveTo(CPoint2D(ps.p.x, ps.p.y));

  for (uint i = 1; i < num_points; ++i) {
    const PathPoint &p1 = pathPoints_[i - 1];
    const PathPoint &p2 = pathPoints_[i];

    if      (pathPoints_[i].type == CPATH_PART_TYPE_LINE_TO)
      path->addLineTo (CPoint2D(p2.p .x, p2.p .y));
    else if (pathPoints_[i].type == CPATH_PART_TYPE_CURVE2_TO)
      path->addCurveTo(CPoint2D(p1.c1.x, p1.c1.y), CPoint2D(p2.p .x, p2.p .y));
    else if (pathPoints_[i].type == CPATH_PART_TYPE_CURVE3_TO)
      path->addCurveTo(CPoint2D(p1.c1.x, p1.c1.y), CPoint2D(p1.c2.x, p1.c2.y),
                       CPoint2D(p2.p .x, p2.p .y));
  }

  if (closed)
    path->addClose();

  illustrator_->addShape(path);

  illustrator_->setSelectShape(path);

  pathPoints_.clear();

  illustrator_->redraw();
}

void
CQIllustratorCreatePathMode::
setCurrentToCornerNode()
{
  CQIllustratorSelectedShapes *selection = illustrator_->getSelection();

  if (selection->size() != 1) return;

  const CQIllustratorSelectedShape &sshape = selection->front();

  uint num = sshape.numPoints();

  if (num == 0) return;

  illustrator_->getSandbox()->addSelection();

  CQIllustratorShape *shape = illustrator_->getSandbox()->frontShape();

  for (uint i = 0; i < num; ++i) {
    const CPathShapeControlPoint *controlPoint =
      dynamic_cast<const CPathShapeControlPoint *>(sshape.getPoint(i));

    controlPoint->setCornerNode(shape);
  }

  illustrator_->getSandbox()->commit(CQIllustratorData::CHANGE_GEOMETRY);
}

void
CQIllustratorCreatePathMode::
setCurrentToCurveNode()
{
  CQIllustratorSelectedShapes *selection = illustrator_->getSelection();

  if (selection->size() != 1) return;

  const CQIllustratorSelectedShape &sshape = selection->front();

  uint num = sshape.numPoints();

  if (num == 0) return;

  illustrator_->getSandbox()->addSelection();

  CQIllustratorShape *shape = illustrator_->getSandbox()->frontShape();

  for (uint i = 0; i < num; ++i) {
    const CPathShapeControlPoint *controlPoint =
      dynamic_cast<const CPathShapeControlPoint *>(sshape.getPoint(i));

    controlPoint->setCurveNode(shape);
  }

  illustrator_->getSandbox()->commit(CQIllustratorData::CHANGE_GEOMETRY);
}

void
CQIllustratorCreatePathMode::
addNodeToCurrent()
{
  CQIllustratorSelectedShapes *selection = illustrator_->getSelection();

  if (selection->size() != 1) return;

  const CQIllustratorSelectedShape &sshape = selection->front();

  uint num = sshape.numPoints();

  if (num != 2) return;

  illustrator_->getSandbox()->addSelection();

  CQIllustratorShape *shape = illustrator_->getSandbox()->frontShape();

  const CPathShapeControlPoint *controlPoint1 =
    dynamic_cast<const CPathShapeControlPoint *>(sshape.getPoint(0));
  const CPathShapeControlPoint *controlPoint2 =
    dynamic_cast<const CPathShapeControlPoint *>(sshape.getPoint(1));

  CPoint2D p1 = controlPoint1->getPoint(shape);
  CPoint2D p2 = controlPoint2->getPoint(shape);

  CPathShape *path = dynamic_cast<CPathShape *>(shape);

  int ind = std::min(controlPoint1->getInd(), controlPoint2->getInd());

  path->addLineTo(ind + 1, (p1 + p2)/2);

  illustrator_->getSandbox()->commit(CQIllustratorData::CHANGE_GEOMETRY);
}

void
CQIllustratorCreatePathMode::
removeNodeFromCurrent()
{
}

//------------

CQIllustratorCreatePathToolbar::
CQIllustratorCreatePathToolbar(CQIllustratorCreatePathMode *mode) :
 CQIllustratorToolbar(mode), mode_(mode), createMode_(CREATE_FREE_MODE)
{
}

QIcon
CQIllustratorCreatePathToolbar::
getIcon()
{
  return QIcon(QPixmap(path_data));
}

void
CQIllustratorCreatePathToolbar::
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

  pathFreeButton_ = new CQImageButton(QPixmap(path_free_data));
  pathRectButton_ = new CQImageButton(QPixmap(path_rect_data));
  path45Button_   = new CQImageButton(QPixmap(path_45_data));

  connect(pathFreeButton_, SIGNAL(toggled(bool)), this, SLOT(pathFreeSlot(bool)));
  connect(pathRectButton_, SIGNAL(toggled(bool)), this, SLOT(pathRectSlot(bool)));
  connect(path45Button_  , SIGNAL(toggled(bool)), this, SLOT(path45Slot(bool)));

  pathFreeButton_->setCheckable(true);
  pathRectButton_->setCheckable(true);
  path45Button_  ->setCheckable(true);

  pathFreeButton_->setChecked(true);

  CQSwatch *createModeSwatch =
    new CQSwatch("Mode", pathFreeButton_, pathRectButton_, path45Button_);

  layout->addWidget(createModeSwatch);

  //-----

  QWidget *selFrame = new QWidget;

  QVBoxLayout *selLayout = new QVBoxLayout(selFrame);

  pointRadio_ = new QRadioButton("Point");
  lineRadio_  = new QRadioButton("Line");

  pointRadio_->setChecked(true);

  pointRadio_->setFocusPolicy(Qt::NoFocus);
  lineRadio_ ->setFocusPolicy(Qt::NoFocus);

  selLayout->addWidget(pointRadio_);
  selLayout->addWidget(lineRadio_);

  connect(pointRadio_, SIGNAL(toggled(bool)), this, SLOT(selChangedSlot()));

  CQSwatch *selSwatch = new CQSwatch(selFrame);

  layout->addWidget(selSwatch);

  //-----

  pathLineButton_   = new CQImageButton(QPixmap(path_line_data  ));
  pathCurve2Button_ = new CQImageButton(QPixmap(path_curve2_data));
  pathCurve3Button_ = new CQImageButton(QPixmap(path_curve3_data));

  pathLineButton_  ->setCheckable(true);
  pathCurve2Button_->setCheckable(true);
  pathCurve3Button_->setCheckable(true);

  pathLineButton_  ->setToolTip("Straight Line");
  pathCurve2Button_->setToolTip("Quadratic Curve");
  pathCurve3Button_->setToolTip("Bezier Curve");

  connect(pathLineButton_  , SIGNAL(toggled(bool)), this, SLOT(lineModeSlot  (bool)));
  connect(pathCurve2Button_, SIGNAL(toggled(bool)), this, SLOT(curve2ModeSlot(bool)));
  connect(pathCurve3Button_, SIGNAL(toggled(bool)), this, SLOT(curve3ModeSlot(bool)));

  CPathPartType pathMode = mode_->getPathMode();

  pathLineButton_  ->setChecked(pathMode == CPATH_PART_TYPE_LINE_TO);
  pathCurve2Button_->setChecked(pathMode == CPATH_PART_TYPE_CURVE2_TO);
  pathCurve3Button_->setChecked(pathMode == CPATH_PART_TYPE_CURVE3_TO);

  CQSwatch *createSwatch =
    new CQSwatch("Line Type<2>", pathLineButton_, pathCurve2Button_, pathCurve3Button_);

  layout->addWidget(createSwatch);

  //-----

  nodeCornerButton_ = new CQImageButton(QPixmap(path_corner_data));
  nodeCurveButton_  = new CQImageButton(QPixmap(path_curve_data ));

  nodeCornerButton_->setToolTip("Point Node");
  nodeCurveButton_ ->setToolTip("Curve Node");

  connect(nodeCornerButton_, SIGNAL(clicked()), this, SLOT(cornerNodeSlot()));
  connect(nodeCurveButton_ , SIGNAL(clicked()), this, SLOT(curveNodeSlot ()));

  CQSwatch *nodeSwatch = new CQSwatch("Point Type<2>", nodeCornerButton_, nodeCurveButton_);

  layout->addWidget(nodeSwatch);

  //-----

  addNodeButton_    = new CQImageButton(QPixmap(path_add_data   ));
  removeNodeButton_ = new CQImageButton(QPixmap(path_remove_data));

  addNodeButton_   ->setToolTip("Add Node");
  removeNodeButton_->setToolTip("Remove Node");

  connect(addNodeButton_   , SIGNAL(clicked()), this, SLOT(addNodeSlot   ()));
  connect(removeNodeButton_, SIGNAL(clicked()), this, SLOT(removeNodeSlot()));

  CQSwatch *modifySwatch = new CQSwatch("Modify<2>", addNodeButton_, removeNodeButton_);

  layout->addWidget(modifySwatch);

  //-----

  pointEdit_ = new CQPointEdit;

  pointEdit_->setEnabled(false);

  connect(pointEdit_, SIGNAL(valueChanged()), this, SLOT(updateNodeSlot()));

  CQSwatch *pointSwatch = new CQSwatch("Position", pointEdit_);

  layout->addWidget(pointSwatch);
}

void
CQIllustratorCreatePathToolbar::
modeChangedSlot()
{
  if (createRadio_->isChecked())
    mode_->setEditMode(CQIllustratorMode::CREATE_MODE);
  else
    mode_->setEditMode(CQIllustratorMode::EDIT_MODE);

  updateMode();
}

void
CQIllustratorCreatePathToolbar::
pathFreeSlot(bool state)
{
  if (state) {
    createMode_ = CREATE_FREE_MODE;

    pathRectButton_->setChecked(false);
    path45Button_  ->setChecked(false);
  }
}

void
CQIllustratorCreatePathToolbar::
pathRectSlot(bool state)
{
  if (state) {
    createMode_ = CREATE_RECT_MODE;

    pathFreeButton_->setChecked(false);
    path45Button_  ->setChecked(false);
  }
}

void
CQIllustratorCreatePathToolbar::
path45Slot(bool state)
{
  if (state) {
    createMode_ = CREATE_45_MODE;

    pathRectButton_->setChecked(false);
    pathFreeButton_->setChecked(false);
  }
}

void
CQIllustratorCreatePathToolbar::
selChangedSlot()
{
  if (pointRadio_->isChecked())
    mode_->setSelMode(CQIllustratorCreatePathMode::POINT_SEL);
  else
    mode_->setSelMode(CQIllustratorCreatePathMode::LINE_SEL);

  updateMode();
}

void
CQIllustratorCreatePathToolbar::
lineModeSlot(bool state)
{
  setPathMode(CPATH_PART_TYPE_LINE_TO, state);
}

void
CQIllustratorCreatePathToolbar::
curve2ModeSlot(bool state)
{
  setPathMode(CPATH_PART_TYPE_CURVE2_TO, state);
}

void
CQIllustratorCreatePathToolbar::
curve3ModeSlot(bool state)
{
  setPathMode(CPATH_PART_TYPE_CURVE3_TO, state);
}

void
CQIllustratorCreatePathToolbar::
cornerNodeSlot()
{
  mode_->setCurrentToCornerNode();
}

void
CQIllustratorCreatePathToolbar::
curveNodeSlot()
{
  mode_->setCurrentToCurveNode();
}

void
CQIllustratorCreatePathToolbar::
addNodeSlot()
{
  mode_->addNodeToCurrent();
}

void
CQIllustratorCreatePathToolbar::
removeNodeSlot()
{
  mode_->removeNodeFromCurrent();
}

void
CQIllustratorCreatePathToolbar::
updateNodeSlot()
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
CQIllustratorCreatePathToolbar::
setPathMode(CPathPartType pathMode, bool state)
{
  if (mode_->getEditMode() == CQIllustratorMode::CREATE_MODE) {
    if (state)
      mode_->setPathMode(pathMode);
  }
  else {
    if (state) {
      CQIllustratorCreatePathMode::SelMode selMode = mode_->getSelMode();

      if (selMode == CQIllustratorCreatePathMode::LINE_SEL) {
        CQIllustrator *illustrator = mode_->getIllustrator();

        CQIllustratorSelectedShapes *selection = illustrator->getSelection();

        if (selection->size() == 1) {
          const CQIllustratorSelectedShape &sshape = selection->front();

          uint num_lines = sshape.numLines();

          if (num_lines == 1) {
            CQIllustratorSelectedShape &sshape1 =
              selection->checkoutShape(sshape, mode_->getChangeType());

            CQIllustratorShapeControlLine *line = sshape1.getLine(0);

            line->setType(sshape1.getShape(), pathMode);

            selection->checkinShape(sshape, mode_->getChangeType());
          }
        }
      }
    }
  }

  updateMode();
}

void
CQIllustratorCreatePathToolbar::
updateMode()
{
  if (mode_->getEditMode() == CQIllustratorMode::CREATE_MODE) {
    CPathPartType pathMode = mode_->getPathMode();

    pathLineButton_  ->setChecked(pathMode == CPATH_PART_TYPE_LINE_TO);
    pathCurve2Button_->setChecked(pathMode == CPATH_PART_TYPE_CURVE2_TO);
    pathCurve3Button_->setChecked(pathMode == CPATH_PART_TYPE_CURVE3_TO);
  }
  else {
    pathLineButton_  ->setChecked(false);
    pathCurve2Button_->setChecked(false);
    pathCurve3Button_->setChecked(false);
  }
}

void
CQIllustratorCreatePathToolbar::
setSelectedShape(const CQIllustratorShape *shape)
{
  const CPathShape *path =
    dynamic_cast<const CPathShape *>(shape);

  if (! path) return;
}

void
CQIllustratorCreatePathToolbar::
setSelectedShapePoint(const CQIllustratorShape *shape,
                      const CQIllustratorShapeControlPoint *point)
{
  const CPathShape *path =
    dynamic_cast<const CPathShape *>(shape);

  if (! path) return;

  if (point)
    pointEdit_->setValue(point->getPoint(shape));
  else
    pointEdit_->setValue(CPoint2D(0,0));

  pointEdit_->setEnabled(point != NULL);
}

void
CQIllustratorCreatePathToolbar::
updateShape()
{
  CQIllustrator *illustrator = mode_->getIllustrator();

  CPathShape *shape =
    CQIllustratorUtil::getCurrentShape<CPathShape>(illustrator);

  CPathShape *path = NULL;

  if (shape)
    path = dynamic_cast<CPathShape *>(shape);

  if (path) {
    //illustrator->checkoutShape(shape);

    //illustrator->checkinShape(shape);
  }

  illustrator->redraw();
}

void
CQIllustratorCreatePathToolbar::
setSelectionPoint(const CPoint2D &p)
{
  pointEdit_->setValue(p);
}

//------

CQIllustratorCreatePathSizer::
CQIllustratorCreatePathSizer(CQIllustratorCreatePathMode *mode) :
 CQIllustratorModeSizer(mode)
{
}

void
CQIllustratorCreatePathSizer::
drawHandles(QPainter *painter, const CQIllustratorShape *shape)
{
  const CPathShape *path =
    dynamic_cast<const CPathShape *>(shape);

  if (path) {
    CQIllustratorShape::ControlPointList controlPoints;

    path->getControlPoints(controlPoints);

    uint numControlPoints = controlPoints.size();

    updateHandles(numControlPoints);

    for (uint i = 0; i < numControlPoints; ++i) {
      CPathShapeControlPoint *controlPoint =
        dynamic_cast<CPathShapeControlPoint *>(controlPoints[i]);

      if (controlPoint->isMaster())
        controlPointHandles_[i]->setStyle(CQIllustratorHandle::CIRCLE_STYLE);
      else
        controlPointHandles_[i]->setStyle(CQIllustratorHandle::RECT_STYLE);

      controlPointHandles_[i]->draw(path, controlPoint, painter);
    }
  }
}

void
CQIllustratorCreatePathSizer::
updateHandles(uint numHandles)
{
  CQIllustrator *illustrator = mode_->getIllustrator();

  while (controlPointHandles_.size() < numHandles) {
    CQIllustratorControlPointHandle *handle = new CQIllustratorControlPointHandle(illustrator);

    addHandle(handle);
  }

  while (controlPointHandles_.size() > numHandles) {
    CQIllustratorControlPointHandle *handle = NULL;

    removeHandle(handle);
  }
}
