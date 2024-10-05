#ifndef CPSViewShapeRenderer_H
#define CPSViewShapeRenderer_H

#include <CPSView.h>
#include <CQIllustratorShape.h>

class CQIllustrator;

class CPSViewShapeRenderer : public CPSViewRenderer {
 public:
  CPSViewShapeRenderer(CQIllustrator *illustrator);

  CPSViewShapeRenderer *dup() override;

  void clear(const CRGBA &bg) override;

  void setPen(const CPen &pen) override;
  void setBrush(const CBrush &brush) override;

  void pathInit() override;
  void pathMoveTo(const CPoint2D &p) override;
  void pathLineTo(const CPoint2D &p) override;
  void pathArc(const CPoint2D &p, double r1, double r2, double a1, double a2) override;
  void pathBezier2To(const CPoint2D &p1, const CPoint2D &p2) override;
  void pathBezier3To(const CPoint2D &p1, const CPoint2D &p2, const CPoint2D &p3) override;
  void pathClose() override;
  void pathStroke() override;
  void pathFill() override;
  void pathClip() override;
  void pathEoclip();

 private:
  CPoint2D convertPoint(const CPoint2D &p);

 private:
  CQIllustrator            *illustrator_ { nullptr };
  CQIllustratorShapeStroke  stroke_;
  CQIllustratorShapeFill    fill_;
  CQIllustratorPathShape   *path_ { nullptr };
};

#endif
