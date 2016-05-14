#ifndef CQIllustratorShapeControlLine_H
#define CQIllustratorShapeControlLine_H

#include <CLine2D.h>
#include <CPathPartType.h>

class CQIllustratorShape;

class CQIllustratorShapeControlLine {
 public:
  CQIllustratorShapeControlLine(int id, const CLine2D &line=CLine2D());

  virtual ~CQIllustratorShapeControlLine() { }

  virtual CQIllustratorShapeControlLine *dup() const = 0;

  int getId() const { return id_; }

  virtual CLine2D getLine(const CQIllustratorShape *shape) const;

  virtual void setLine(CQIllustratorShape *shape, const CLine2D &line);

  virtual void setLine(const CLine2D &line);

  void updateLine(CQIllustratorShape *shape, CQIllustratorShapeControlLine *line);

  virtual void setType(CQIllustratorShape *shape, CPathPartType type);

  friend bool operator==(const CQIllustratorShapeControlLine &p1,
                         const CQIllustratorShapeControlLine &p2);
  friend bool operator!=(const CQIllustratorShapeControlLine &p1,
                         const CQIllustratorShapeControlLine &p2);

  virtual void moveLineTo(CQIllustratorShape *shape, const CLine2D &line);

 protected:
  int     id_ { 0 };
  CLine2D line_;
};

#endif
