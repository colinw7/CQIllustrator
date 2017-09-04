#include <CQIllustratorMode.h>
#include <CQIllustrator.h>
#include <CQIllustratorHandle.h>
#include <CQIllustratorToolbar.h>
#include <CQIllustratorShapeControlLine.h>
#include <CQIllustratorShapeDrawer.h>
#include <CQUtil.h>
#include <CMathRound.h>

#include <QBitmap>
#include <QPainter>
#include <QMouseEvent>
#include <QStackedWidget>

#include <svg/selected_point_svg.h>
#include <svg/selected_hline_svg.h>
#include <svg/selected_vline_svg.h>

CQIllustratorMode::
CQIllustratorMode(CQIllustrator *illustrator, uint id) :
 illustrator_(illustrator), id_(id)
{
}

CQIllustratorMode::
CQIllustratorMode(CQIllustratorMode *parentMode, uint id) :
 illustrator_(parentMode->getIllustrator()), parentMode_(parentMode), id_(id)
{
}

void
CQIllustratorMode::
init()
{
  toolbar_ = createToolbar();

  if (toolbar_)
    illustrator_->addModeToolbar(this, toolbar_);

  sizer_ = createSizer();

  connect(illustrator_, SIGNAL(selectionChanged()), this, SLOT(selectionSlot()));
}

CQIllustratorModeSizer *
CQIllustratorMode::
createSizer()
{
  return 0;
}

void
CQIllustratorMode::
setCursor(const uchar *bits, const uchar *mask, int xo, int yo)
{
  cursor_ = QCursor(QBitmap::fromData(QSize(16,16), bits),
                    QBitmap::fromData(QSize(16,16), mask),
                    xo, yo);
}

QCursor
CQIllustratorMode::
getCursor() const
{
  return cursor_;
}

void
CQIllustratorMode::
setStack(QStackedWidget *stack)
{
  stack_ = stack;
}

void
CQIllustratorMode::
setEditMode(EditMode editMode)
{
  editMode_ = editMode;
}

//----------

void
CQIllustratorMode::
mousePress(const MouseEvent &e)
{
  if (e.event->getButton() == CBUTTON_RIGHT) return;

  pressed_ = true;
  moving_  = false;

  press_ppos_ = e.pixel;
  press_wpos_ = e.window;

  prev_ppos_  = press_ppos_;
  prev_wpos_  = press_wpos_;

  curr_ppos_  = press_ppos_;
  curr_wpos_  = press_wpos_;

  press_wpos_ = CQUtil::toQPoint(illustrator_->snapPoint(CQUtil::fromQPoint(press_wpos_)));

  handleMousePress(e);
}

void
CQIllustratorMode::
mouseRelease(const MouseEvent &e)
{
  if (e.event->getButton() == CBUTTON_RIGHT) return;

  pressed_ = false;

  release_ppos_ = e.pixel;
  release_wpos_ = e.window;

  release_wpos_ = CQUtil::toQPoint(illustrator_->snapPoint(CQUtil::fromQPoint(release_wpos_)));

  handleMouseRelease(e);
}

void
CQIllustratorMode::
mouseDrag(const MouseEvent &e)
{
  if (e.event->getButton() == CBUTTON_RIGHT) return;

  prev_ppos_ = curr_ppos_;

  curr_ppos_ = e.pixel;
  curr_wpos_ = e.window;

  curr_wpos_ = CQUtil::toQPoint(illustrator_->snapPoint(CQUtil::fromQPoint(curr_wpos_)));

  if (! moving_) {
    int dx = abs(curr_ppos_.x() - press_ppos_.x());
    int dy = abs(curr_ppos_.y() - press_ppos_.y());

    if (dx >= 4 || dy >= 4)
      moving_ = true;
  }

  handleMouseDrag(e);

  prev_wpos_ = curr_wpos_;
}

void
CQIllustratorMode::
mouseMove(const MouseEvent &e)
{
  prev_ppos_ = curr_ppos_;
  prev_wpos_ = curr_wpos_;

  curr_ppos_ = e.pixel;
  curr_wpos_ = e.window;

  handleMouseMove(e);
}

bool
CQIllustratorMode::
keyPress(const KeyEvent &e)
{
  return handleKeyPress(e);
}

void
CQIllustratorMode::
handleMousePress(const MouseEvent &)
{
  if (sizer_) {
    dragging_ = sizer_->isActive();

    if (dragging_) {
      addSelectionToSandbox();

      illustrator_->redraw();
    }
  }

  prev_wpos_ = press_wpos_;
}

void
CQIllustratorMode::
handleMouseMove(const MouseEvent &e)
{
  if (sizer_) {
    sizer_->updateActive(e.window);

    illustrator_->redrawOverlay();
  }
}

void
CQIllustratorMode::
handleMouseRelease(const MouseEvent &)
{
  dragging_ = false;

  illustrator_->redraw();
}

bool
CQIllustratorMode::
handleKeyPress(const KeyEvent &e)
{
  if (sizer_) {
    if (sizer_->handleKeyPress(e))
      return true;
  }

  CKeyType key = e.event->getType();

  if (key == CKEY_TYPE_Colon)
    illustrator_->showConsole();

  return false;
}

void
CQIllustratorMode::
selectControlPoint(const MouseEvent &e)
{
  CPoint2D p2 = CQUtil::fromQPoint(release_wpos_);

  CQIllustratorSelectedShapes *selection = illustrator_->getSelection();

  if (selection->size() == 1) {
    CQIllustratorSelectedShape &sshape = selection->front();

    CQIllustratorShape *shape = sshape.getShape();

    (void) illustrator_->selectPointAt(shape, p2, getControlType(), e.event->isControlKey(),
                                       e.event->isShiftKey());

    CQIllustratorSelectedShape &sshape1 = selection->front();

    sizer_->updateSelectionHandles(sshape1);
  }

  illustrator_->getSandbox()->clear();
}

//----------

void
CQIllustratorMode::
addSelectionToSandbox()
{
  illustrator_->getSandbox()->addSelection();
}

//----------

void
CQIllustratorMode::
menuItemSlot()
{
  illustrator_->setMode((CQIllustrator::Mode) id_);
}

void
CQIllustratorMode::
selectionSlot()
{
  CQIllustratorSelectedShapes *selection = illustrator_->getSelection();

  uint num_shapes = selection->size();

  if      (num_shapes == 0) {
    setSelectedShape(0);

    setSelectedShapePoint(0, 0);
  }
  else if (num_shapes == 1) {
    const CQIllustratorSelectedShape &sshape = selection->front();

    const CQIllustratorShape *shape = sshape.getShape();

    setSelectedShape(shape);

    uint num_points = sshape.numPoints();

    if      (num_points == 0)
      setSelectedShapePoint(shape, 0);
    else if (num_points == 1) {
      const CQIllustratorShapeControlPoint *point = sshape.getPoint(0);

      setSelectedShapePoint(shape, point);
    }
    else
      setSelectedShapePoint(shape, 0);
  }
  else {
    setSelectedShape(0);

    setSelectedShapePoint(0, 0);
  }
}

void
CQIllustratorMode::
showToolbar(bool show)
{
  if (parentMode_)
    parentMode_->showToolbar(show);
  else if (stack_) {
    if (show)
      stack_->setCurrentWidget(getToolbar());
  }
}

void
CQIllustratorMode::
moveCurrent(const CPoint2D &d)
{
  bool snap_x = (fabs(d.x) > 0);
  bool snap_y = (fabs(d.y) > 0);

  illustrator_->startUndoGroup("Move Current");

  CQIllustratorSelectedShapes *selection = illustrator_->getSelection();

  CQIllustratorSelectedShapes::iterator ps1, ps2;

  for (ps1 = selection->begin(), ps2 = selection->end(); ps1 != ps2; ++ps1) {
    CQIllustratorShape *shape = (*ps1).getShape();

    if (shape->getFixed()) continue;

    const CBBox2D &bbox = shape->getFlatBBox();

    CPoint2D ll = bbox.getLL() + d;

    if (snap_x) ll.x = CMathRound::Round(ll.x);
    if (snap_y) ll.y = CMathRound::Round(ll.y);

    shape->moveTo(ll);
  }

  illustrator_->endUndoGroup();

  illustrator_->redraw();
}

void
CQIllustratorMode::
moveCurrentPoint(const CPoint2D &d)
{
  bool snap_x = (fabs(d.x) > 0);
  bool snap_y = (fabs(d.y) > 0);

  illustrator_->startUndoGroup("Move Current Point");

  CQIllustratorSelectedShapes *selection = illustrator_->getSelection();

  CQIllustratorSelectedShapes::iterator ps1, ps2;

  for (ps1 = selection->begin(), ps2 = selection->end(); ps1 != ps2; ++ps1) {
    const CQIllustratorSelectedShape &sshape = *ps1;

    const CQIllustratorShape *shape = sshape.getShape();

    if (shape->getFixed()) continue;

    uint num_points = sshape.numPoints();

    if (num_points == 0) continue;

    CQIllustratorSelectedShape &sshape1 =
      selection->checkoutShape(sshape, getChangeType());

    for (uint i = 0; i < num_points; ++i) {
      CQIllustratorShapeControlPoint *point = sshape1.getPoint(i);

      CPoint2D p = point->getPoint(sshape.getShape());

      p += d;

      if (snap_x) p.x = CMathRound::Round(p.x);
      if (snap_y) p.y = CMathRound::Round(p.y);

      point->setPoint(sshape1.getShape(), p);
    }

    selection->checkinShape(sshape, getChangeType());
  }

  illustrator_->endUndoGroup();

  illustrator_->redraw();
}

void
CQIllustratorMode::
resizeCurrent(const CBBox2D &d)
{
  bool snap_x1 = (fabs(d.getLL().x) > 0);
  bool snap_y1 = (fabs(d.getLL().y) > 0);
  bool snap_x2 = (fabs(d.getUR().x) > 0);
  bool snap_y2 = (fabs(d.getUR().y) > 0);

  illustrator_->startUndoGroup("Resize Current");

  CQIllustratorSelectedShapes *selection = illustrator_->getSelection();

  CQIllustratorSelectedShapes::iterator ps1, ps2;

  for (ps1 = selection->begin(), ps2 = selection->end(); ps1 != ps2; ++ps1) {
    CQIllustratorShape *shape = (*ps1).getShape();

    if (shape->getFixed()) continue;

    const CBBox2D &bbox = shape->getFlatBBox();

    CPoint2D ll = bbox.getLL() + d.getLL();
    CPoint2D ur = bbox.getUR() + d.getUR();

    if (snap_x1) ll.x = CMathRound::Round(ll.x);
    if (snap_y1) ll.y = CMathRound::Round(ll.y);
    if (snap_x2) ur.x = CMathRound::Round(ur.x);
    if (snap_y2) ur.y = CMathRound::Round(ur.y);

    shape->setBBox(CBBox2D(ll, ur));
  }

  illustrator_->endUndoGroup();

  illustrator_->redraw();
}

void
CQIllustratorMode::
deleteCurrent()
{
  illustrator_->deleteSelectedShapes();
}

bool
CQIllustratorMode::
cancel()
{
  pressed_  = false;
  moving_   = false;
  dragging_ = false;

  return true;
}

void
CQIllustratorMode::
escape()
{
  pressed_  = false;
  moving_   = false;
  dragging_ = false;
}

void
CQIllustratorMode::
drawSelection(CQIllustratorShapeDrawer *drawer)
{
  CQIllustratorSelectedShapes *selection = illustrator_->getSelection();

  CQIllustratorSelectedShapes::iterator ps1, ps2;

  for (ps1 = selection->begin(), ps2 = selection->end(); ps1 != ps2; ++ps1) {
    CQIllustratorShape *shape = (*ps1).getShape();

    if (! shape->getVisible()) continue;

    shape->drawSelect(drawer);
  }
}

void
CQIllustratorMode::
drawOverlay(CQIllustratorShapeDrawer *)
{
}

void
CQIllustratorMode::
setSelectedShape(const CQIllustratorShape *shape)
{
  if (toolbar_)
    toolbar_->setSelectedShape(shape);
}

void
CQIllustratorMode::
setSelectedShapePoint(const CQIllustratorShape *shape,
                      const CQIllustratorShapeControlPoint *point)
{
  if (toolbar_)
    toolbar_->setSelectedShapePoint(shape, point);
}

//------

CQIllustratorMode::MouseEvent::
MouseEvent(QMouseEvent *e, const QTransform &transform)
{
  pixel  = e->pos();
  window = transform.map(QPointF(pixel));
  event  = CQUtil::convertEvent(e);
}

CQIllustratorMode::KeyEvent::
KeyEvent(QKeyEvent *e, const QPoint &pos, const QTransform &transform)
{
  pixel  = pos;
  window = transform.map(QPointF(pixel));
  event  = CQUtil::convertEvent(e);
}

//------

CQIllustratorModeSizer::
CQIllustratorModeSizer(CQIllustratorMode *mode) :
 mode_(mode)
{
}

void
CQIllustratorModeSizer::
addHandle(CQIllustratorControlPointHandle *handle)
{
  controlPointHandles_.push_back(handle);
}

void
CQIllustratorModeSizer::
addHandle(CQIllustratorHandle *handle)
{
  handles_.push_back(handle);
}

void
CQIllustratorModeSizer::
removeHandle(CQIllustratorControlPointHandle *)
{
  CQIllustratorControlPointHandle *handle = controlPointHandles_.back();

  controlPointHandles_.pop_back();

  delete handle;
}

void
CQIllustratorModeSizer::
removeHandle(CQIllustratorHandle *)
{
  CQIllustratorHandle *handle = handles_.back();

  handles_.pop_back();

  delete handle;
}

void
CQIllustratorModeSizer::
draw(QPainter *painter, const CQIllustratorShape *shape)
{
  //transform_ = painter->worldTransform();
  transform_ = painter->transform();

  drawHandles(painter, shape);
}

bool
CQIllustratorModeSizer::
updateActive(const QPointF &p)
{
  QPointF p1 = transform_.map(p);

  uint numControlPointHandles = controlPointHandles_.size();

  for (uint i = 0; i < numControlPointHandles; ++i)
    if (controlPointHandles_[i]->updateActive(p1))
      return true;

  uint numHandles = handles_.size();

  for (uint i = 0; i < numHandles; ++i)
    if (handles_[i]->updateActive(p1))
      return true;

  return false;
}

void
CQIllustratorModeSizer::
updatePoint(CQIllustratorShape *shape, const QPointF &p)
{
  CPoint2D p1 = CQUtil::fromQPoint(p);

  uint numControlPointHandles = controlPointHandles_.size();

  for (uint i = 0; i < numControlPointHandles; ++i) {
    if (controlPointHandles_[i]->getActive()) {
      controlPointHandles_[i]->updatePoint(shape, p1);
      break;
    }
  }

  mode_->getToolbar()->setSelectedShape(shape);
}

bool
CQIllustratorModeSizer::
isActive() const
{
  uint numControlPointHandles = controlPointHandles_.size();

  for (uint i = 0; i < numControlPointHandles; ++i)
    if (controlPointHandles_[i]->getActive())
      return true;

  uint numHandles = handles_.size();

  for (uint i = 0; i < numHandles; ++i)
    if (handles_[i]->getActive())
      return true;

  return false;
}

void
CQIllustratorModeSizer::
drawHandles(QPainter *, const CQIllustratorShape *)
{
}

void
CQIllustratorModeSizer::
drawSelectionHandles(QPainter *painter, const CQIllustratorSelectedShape &sshape)
{
  const CQIllustratorShape *shape = sshape.getShape();

  uint num_points = sshape.numPoints();

  updatePointSelectionHandles(num_points);

  for (uint i = 0; i < num_points; ++i) {
    const CQIllustratorShapeControlPoint *point = sshape.getPoint(i);

    CPoint2D p = point->getPoint(shape);

    pointSelectionHandles_[i]->setPosition(p);

    pointSelectionHandles_[i]->draw(painter);
  }

  uint num_lines = sshape.numLines();

  updateLineSelectionHandles(num_lines);

  for (uint i = 0; i < num_lines; ++i) {
    const CQIllustratorShapeControlLine *line = sshape.getLine(i);

    CLine2D l = line->getLine(shape);

    double dx = fabs(l.start().x - l.end().x);
    double dy = fabs(l.start().y - l.end().y);

    lineSelectionHandles_[i]->setPosition((l.start() + l.end())/2);

    if (dx > dy)
      lineSelectionHandles_[i]->setImage(CQPixmapCacheInst->getIcon("SELECTED_HLINE"));
    else
      lineSelectionHandles_[i]->setImage(CQPixmapCacheInst->getIcon("SELECTED_VLINE"));

    lineSelectionHandles_[i]->draw(painter);
  }
}

void
CQIllustratorModeSizer::
updateSelectionHandles(const CQIllustratorSelectedShape &sshape)
{
  const CQIllustratorShape *shape = sshape.getShape();

  uint num_points = sshape.numPoints();

  updatePointSelectionHandles(num_points);

  for (uint i = 0; i < num_points; ++i) {
    const CQIllustratorShapeControlPoint *spoint = sshape.getPoint(i);

    CPoint2D p = spoint->getPoint(shape);

    pointSelectionHandles_[i]->setPosition(p);
  }

  uint num_lines = sshape.numLines();

  updateLineSelectionHandles(num_lines);

  for (uint i = 0; i < num_lines; ++i) {
    const CQIllustratorShapeControlLine *sline = sshape.getLine(i);

    CLine2D l = sline->getLine(shape);

    double dx = fabs(l.start().x - l.end().x);
    double dy = fabs(l.start().y - l.end().y);

    lineSelectionHandles_[i]->setPosition((l.start() + l.end())/2);

    if (dx > dy)
      lineSelectionHandles_[i]->setImage(CQPixmapCacheInst->getIcon("SELECTED_HLINE"));
    else
      lineSelectionHandles_[i]->setImage(CQPixmapCacheInst->getIcon("SELECTED_VLINE"));
  }
}

void
CQIllustratorModeSizer::
updatePointSelectionHandles(uint num)
{
  CQIllustrator *illustrator = mode_->getIllustrator();

  while (pointSelectionHandles_.size() < num) {
    CQIllustratorHandle *selectionHandle = new CQIllustratorHandle(illustrator);

    selectionHandle->setImage(CQPixmapCacheInst->getIcon("SELECTED_POINT"));

    pointSelectionHandles_.push_back(selectionHandle);
  }
}

void
CQIllustratorModeSizer::
updateLineSelectionHandles(uint num)
{
  CQIllustrator *illustrator = mode_->getIllustrator();

  while (lineSelectionHandles_.size() < num) {
    CQIllustratorHandle *selectionHandle = new CQIllustratorHandle(illustrator);

    selectionHandle->setImage(CQPixmapCacheInst->getIcon("SELECTED_HLINE"));

    lineSelectionHandles_.push_back(selectionHandle);
  }
}

bool
CQIllustratorModeSizer::
handleKeyPress(const KeyEvent &e)
{
  CKeyType key = e.event->getType();

  if (key == CKEY_TYPE_Up   || key == CKEY_TYPE_Down ||
      key == CKEY_TYPE_Left || key == CKEY_TYPE_Right) {
    CQIllustrator *illustrator = mode_->getIllustrator();

    CQIllustratorSelectedShapes *selection = illustrator->getSelection();

    if (selection->size() != 1) return false;

    const CQIllustratorSelectedShape &sshape = selection->front();

    uint num_points = sshape.numPoints();

    if (num_points > 0) {
      bool snap_x = false;
      bool snap_y = false;

      CPoint2D d;

      if      (key == CKEY_TYPE_Up   ) { d = CPoint2D( 0,  1); snap_y = true; }
      else if (key == CKEY_TYPE_Down ) { d = CPoint2D( 0, -1); snap_y = true; }
      else if (key == CKEY_TYPE_Left ) { d = CPoint2D(-1,  0); snap_x = true; }
      else if (key == CKEY_TYPE_Right) { d = CPoint2D( 1,  0); snap_x = true; }

      CQIllustratorSelectedShape &sshape1 =
        selection->checkoutShape(sshape, mode_->getChangeType());

      for (uint i = 0; i < num_points; ++i) {
        CQIllustratorShapeControlPoint *point = sshape1.getPoint(i);

        CPoint2D p = point->getPoint(sshape.getShape());

        p += d;

        if (snap_x) p.x = CMathRound::Round(p.x);
        if (snap_y) p.y = CMathRound::Round(p.y);

        point->setPoint(sshape1.getShape(), p);
      }

      selection->checkinShape(sshape, mode_->getChangeType());
    }

    uint num_lines = sshape.numLines();

    if (num_lines > 0) {
      bool snap_x = false;
      bool snap_y = false;

      CPoint2D d;

      if      (key == CKEY_TYPE_Up   ) { d = CPoint2D( 0,  1); snap_y = true; }
      else if (key == CKEY_TYPE_Down ) { d = CPoint2D( 0, -1); snap_y = true; }
      else if (key == CKEY_TYPE_Left ) { d = CPoint2D(-1,  0); snap_x = true; }
      else if (key == CKEY_TYPE_Right) { d = CPoint2D( 1,  0); snap_x = true; }

      CQIllustratorSelectedShape &sshape1 =
        selection->checkoutShape(sshape, mode_->getChangeType());

      for (uint i = 0; i < num_lines; ++i) {
        CQIllustratorShapeControlLine *line = sshape1.getLine(i);

        CLine2D l = line->getLine(sshape.getShape());

        CPoint2D p1 = l.start();
        CPoint2D p2 = l.end  ();

        p1 += d;
        p2 += d;

        if (snap_x) { p1.x = CMathRound::Round(p1.x); p2.x = CMathRound::Round(p2.x); }
        if (snap_y) { p1.y = CMathRound::Round(p1.y); p2.y = CMathRound::Round(p2.y); }

        line->setLine(sshape1.getShape(), CLine2D(p1, p2));
      }

      selection->checkinShape(sshape, mode_->getChangeType());
    }

    return true;
  }

  return false;
}
