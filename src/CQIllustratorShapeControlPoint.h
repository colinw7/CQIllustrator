#ifndef CQIllustratorShapeControlPoint_H
#define CQIllustratorShapeControlPoint_H

#include <CPoint2D.h>

class CQIllustratorShape;

class CQIllustratorShapeControlPoint {
 public:
  enum class ControlPointType {
    NORMAL,
    CONTROL
  };

 public:
  CQIllustratorShapeControlPoint(int id, const CPoint2D &point=CPoint2D(0,0),
                                 ControlPointType type=ControlPointType::NORMAL);

  virtual ~CQIllustratorShapeControlPoint() { }

  virtual CQIllustratorShapeControlPoint *dup() const = 0;

  int getId() const { return id_; }

  virtual CPoint2D getPoint(const CQIllustratorShape *shape) const;

  virtual void setPoint(CQIllustratorShape *shape, const CPoint2D &point);

  virtual void setPoint(const CPoint2D &point);

  void updatePoint(CQIllustratorShape *shape, CQIllustratorShapeControlPoint *point);

  friend bool operator==(const CQIllustratorShapeControlPoint &p1,
                         const CQIllustratorShapeControlPoint &p2);
  friend bool operator!=(const CQIllustratorShapeControlPoint &p1,
                         const CQIllustratorShapeControlPoint &p2);

  virtual double distance(const CQIllustratorShape *shape, const CPoint2D &point) const;

  virtual void movePointBy(CQIllustratorShape *shape, const CPoint2D &d);

  virtual void movePointTo(CQIllustratorShape *shape, const CPoint2D &pos);

  static double pointDist(const CPoint2D &p1, const CPoint2D &p2);

 protected:
  int              id_ { 0 };
  CPoint2D         point_;
  ControlPointType type_;
};

#endif
