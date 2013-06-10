#include <CQIllustratorSliceMode.h>
#include <CQIllustrator.h>
#include <CQIllustratorShapeDrawer.h>

#include <QPainter>
#include <QBoxLayout>

#include <CQUtil.h>
#include <CQMenu.h>
#include <CQSwatch.h>
#include <CQImageButton.h>

#include "xpm/slice.xpm"

#include "cursors/slice.xbm"
#include "cursors/slicemask.xbm"

CQIllustratorSliceMode::
CQIllustratorSliceMode(CQIllustrator *illustrator) :
 CQIllustratorMode(illustrator, CQIllustrator::MODE_SLICE)
{
  setCursor(slice_bits, slicemask_bits, 2, 2);
}

CQIllustratorSliceToolbar *
CQIllustratorSliceMode::
createToolbar()
{
  toolbar_ = new CQIllustratorSliceToolbar(this);

  return toolbar_;
}

CQMenuItem *
CQIllustratorSliceMode::
createMenuItem(CQMenu *menu)
{
  menuItem_ = new CQMenuItem(menu, "&Slice", CQMenuItem::CHECKABLE);

  menuItem_->setStatusTip("Slice Shape");
  menuItem_->setXPMIcon(slice_data);

  connect(menuItem_->getAction(), SIGNAL(toggled(bool)), this, SLOT(menuItemSlot()));

  return menuItem_;
}

void
CQIllustratorSliceMode::
handleMousePress(const MouseEvent &)
{
}

void
CQIllustratorSliceMode::
handleMouseRelease(const MouseEvent &e)
{
  //QPoint current_ppos = e.pixel;

  if (! moving_) {
  }
  else {
    illustrator_->startUndoGroup("Slice");

    std::vector<CQIllustratorShape *> nshapes;
    std::vector<CQIllustratorShape *> oshapes;

    CPoint2D p1 = CQUtil::fromQPoint(press_wpos_);
    CPoint2D p2 = CQUtil::fromQPoint(e.window);

    CQIllustratorSelectedShapes::iterator ps1, ps2;

    CQIllustratorSelectedShapes *selection = illustrator_->getSelection();

    for (ps1 = selection->begin(), ps2 = selection->end(); ps1 != ps2; ++ps1) {
      CQIllustratorShape *shape = (*ps1).getShape();

      CPolygon2D polygon;

      if (! shape->getPolygon(polygon))
        continue;

      //CBBox2D bbox = polygon.getBBox();

      std::vector< std::vector<CPoint2D> > opolys;

      if (! CMathGeom2D::SlicePolygonByLines(polygon.getPoints(), CLine2D(p1, p2), opolys))
        continue;

      uint num_opolys = opolys.size();

      for (uint i = 0; i < num_opolys; ++i) {
        CQIllustratorPolygonShape *pshape = illustrator_->createPolygonShape();

        pshape->setPoints(opolys[i]);

        pshape->setStroke(shape->getStroke());
        pshape->setFill  (shape->getFill ());

        illustrator_->addShape(pshape);

        nshapes.push_back(pshape);
      }

      oshapes.push_back(shape);
    }

    uint num_oshapes = oshapes.size();

    for (uint i = 0; i < num_oshapes; ++i)
      illustrator_->removeShape(oshapes[i]);

    uint num_nshapes = nshapes.size();

    if (num_nshapes > 0) {
      illustrator_->setSelectShape(NULL);

      for (uint i = 0; i < num_nshapes; ++i)
        illustrator_->addSelectShape(nshapes[i]);
    }

    illustrator_->endUndoGroup();
  }

  illustrator_->redraw();
}

void
CQIllustratorSliceMode::
handleMouseDrag(const MouseEvent &)
{
  illustrator_->redrawOverlay();
}

void
CQIllustratorSliceMode::
handleMouseMove(const MouseEvent &)
{
}

void
CQIllustratorSliceMode::
drawOverlay(CQIllustratorShapeDrawer *drawer)
{
  if (pressed_) {
    QPainter *painter = drawer->getPainter();

    QPointF current_wpos = illustrator_->getITransform().map(QPointF(curr_ppos_));

    QPen pen;

    pen.setColor(QColor(0,0,0));
    pen.setStyle(Qt::DashLine);

    painter->setPen(pen);
    painter->setBrush(Qt::NoBrush);

    painter->drawLine(press_wpos_, current_wpos);
  }
}

//------------

CQIllustratorSliceToolbar::
CQIllustratorSliceToolbar(CQIllustratorSliceMode *mode) :
 CQIllustratorToolbar(mode), mode_(mode)
{
}

QIcon
CQIllustratorSliceToolbar::
getIcon()
{
  return QIcon(QPixmap(slice_data));
}

void
CQIllustratorSliceToolbar::
addWidgets()
{
}
