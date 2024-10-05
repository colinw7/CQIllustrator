#ifndef CQIllustratorShapeLGradientControlPoint_H
#define CQIllustratorShapeLGradientControlPoint_H

#include <CQIllustratorShapeControlPoint.h>

class CQIllustratorShapeLGradientControlPoint : public CQIllustratorShapeControlPoint {
 public:
  enum class Position {
    START,
    END
  };

 public:
  CQIllustratorShapeLGradientControlPoint(Position pos, const CPoint2D &p);

  CQIllustratorShapeLGradientControlPoint *dup() const override;

  Position getPosition() const { return pos_; }

  CPoint2D getPoint(const CQIllustratorShape *shape) const override;

  void setPoint(CQIllustratorShape *shape, const CPoint2D &point) override;

 protected:
  Position pos_;
};

#endif
