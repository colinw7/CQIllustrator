#ifndef CPSViewShapeRenderer_H
#define CPSViewShapeRenderer_H

#include <CPSView/CPSView.h>
#include <CQIllustratorShape.h>

class CQIllustrator;

class CPSViewShapeRenderer : public CPSViewRenderer {
 public:
  CPSViewShapeRenderer(CQIllustrator *illustrator);

  CPSViewShapeRenderer *dup();

  void clear(const CRGBA &bg);

  void setPen(const CPen &pen);
  void setBrush(const CBrush &brush);

  void pathInit();
  void pathMoveTo(const CPoint2D &p);
  void pathLineTo(const CPoint2D &p);
  void pathArc(const CPoint2D &p, double r1, double r2, double a1, double a2);
  void pathBezier2To(const CPoint2D &p1, const CPoint2D &p2);
  void pathBezier3To(const CPoint2D &p1, const CPoint2D &p2, const CPoint2D &p3);
  void pathClose();
  void pathStroke();
  void pathFill();
  void pathClip();
  void pathEoclip();

 private:
  CPoint2D convertPoint(const CPoint2D &p);

 private:
  CQIllustrator            *illustrator_;
  CQIllustratorShapeStroke  stroke_;
  CQIllustratorShapeFill    fill_;
  CPathShape               *path_;
};

#endif
