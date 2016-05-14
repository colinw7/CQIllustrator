#include <CPSViewShapeRenderer.h>
#include <CQIllustrator.h>
#include <CQIllustratorPathShape.h>

CPSViewShapeRenderer::
CPSViewShapeRenderer(CQIllustrator *illustrator) :
 illustrator_(illustrator)
{
  path_ = new CQIllustratorPathShape;
}

CPSViewShapeRenderer *
CPSViewShapeRenderer::
dup()
{
  return new CPSViewShapeRenderer(illustrator_);
}

void
CPSViewShapeRenderer::
clear(const CRGBA &)
{
}

void
CPSViewShapeRenderer::
setPen(const CPen &pen)
{
  stroke_.setColor     (pen.getColor());
  stroke_.setWidth     (pen.getWidth());
  stroke_.setLineDash  (pen.getLineDash());
  stroke_.setLineCap   (pen.getLineCap());
  stroke_.setLineJoin  (pen.getLineJoin());
  stroke_.setMitreLimit(pen.getMitreLimit());
}

void
CPSViewShapeRenderer::
setBrush(const CBrush &brush)
{
  fill_.setColor(brush.getColor());
}

void
CPSViewShapeRenderer::
pathInit()
{
  delete path_;

  path_ = new CQIllustratorPathShape;
}

void
CPSViewShapeRenderer::
pathMoveTo(const CPoint2D &p)
{
  path_->addMoveTo(convertPoint(p));
}

void
CPSViewShapeRenderer::
pathLineTo(const CPoint2D &p)
{
  path_->addLineTo(convertPoint(p));
}

void
CPSViewShapeRenderer::
pathArc(const CPoint2D &c, double xr, double yr, double angle1, double angle2)
{
  if (fabs(angle2 - angle1) > M_PI) {
    double d = (angle2 - angle1)/2;

    path_->addArcTo(convertPoint(c), xr, yr, angle1, angle1 + d);
    path_->addArcTo(convertPoint(c), xr, yr, angle1 + d, angle2);
  }
  else
    path_->addArcTo(convertPoint(c), xr, yr, angle1, angle2);
}

void
CPSViewShapeRenderer::
pathBezier2To(const CPoint2D &p1, const CPoint2D &p2)
{
  path_->addCurveTo(convertPoint(p1), convertPoint(p2));
}

void
CPSViewShapeRenderer::
pathBezier3To(const CPoint2D &p1, const CPoint2D &p2, const CPoint2D &p3)
{
  path_->addCurveTo(convertPoint(p1), convertPoint(p2), convertPoint(p3));
}

void
CPSViewShapeRenderer::
pathClose()
{
  path_->addClose();
}

void
CPSViewShapeRenderer::
pathStroke()
{
  path_->setStroke(stroke_);
  path_->setFill  (fill_);

  path_->setFillOpacity(0);

  illustrator_->addShape(path_);

  path_ = 0;
}

void
CPSViewShapeRenderer::
pathFill()
{
  path_->setStroke(stroke_);
  path_->setFill  (fill_);

  path_->setStrokeOpacity(0);

  illustrator_->addShape(path_);

  path_ = 0;
}

void
CPSViewShapeRenderer::
pathClip()
{
  path_->setClip(true);

  illustrator_->addShape(path_);

  path_ = 0;
}

CPoint2D
CPSViewShapeRenderer::
convertPoint(const CPoint2D &p)
{
  return CPoint2D(p.x, CPSView::PAGE_HEIGHT - p.y);
}
