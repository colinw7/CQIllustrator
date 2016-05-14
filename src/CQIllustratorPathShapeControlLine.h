#ifndef CQIllustratorPathShapeControlLine_H
#define CQIllustratorPathShapeControlLine_H

#include <CQIllustratorShapeControlLine.h>

class CQIllustratorPathShapeControlLine : public CQIllustratorShapeControlLine {
 public:
  CQIllustratorPathShapeControlLine(uint ind, const CLine2D &l);

  CQIllustratorPathShapeControlLine *dup() const;

  CLine2D getLine(const CQIllustratorShape *shape) const;

  void setLine(CQIllustratorShape *shape, const CLine2D &l);

  uint getInd() const { return ind_; }

  void setType(CQIllustratorShape *shape, CPathPartType type);

 protected:
  uint ind_ { 0 };
};

#endif
