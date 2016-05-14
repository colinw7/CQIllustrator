#ifndef CQIllustratorShapeRGradientControlPoint_H
#define CQIllustratorShapeRGradientControlPoint_H

#include <CQIllustratorShapeControlPoint.h>

class CQIllustratorShapeRGradientControlPoint : public CQIllustratorShapeControlPoint {
 public:
  enum class Position {
    CENTER,
    FOCUS,
    RADIUS
  };

 public:
  CQIllustratorShapeRGradientControlPoint(Position pos, const CPoint2D &p);

  CQIllustratorShapeRGradientControlPoint *dup() const;

  Position getPosition() const { return pos_; }

  CPoint2D getPoint(const CQIllustratorShape *shape) const;

  void setPoint(CQIllustratorShape *shape, const CPoint2D &point);

 protected:
  Position pos_;
};

#endif
