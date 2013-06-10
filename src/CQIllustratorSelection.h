#ifndef CQIllustratorSelection_H
#define CQIllustratorSelection_H

#include <QObject>
#include <CQIllustratorShape.h>

class CQIllustratorSelectedShape {
 protected:
  friend class CQIllustratorSelectedShapes;

  CQIllustratorSelectedShape(CQIllustratorShape *shape=NULL);

 public:
 ~CQIllustratorSelectedShape();

  CQIllustratorShape *getShape();

  const CQIllustratorShape *getShape() const;

  uint numPoints() const;

  const CQIllustratorShapeControlPoint *getPoint(uint i) const;

  CQIllustratorShapeControlPoint *getPoint(uint i);

  bool exists(const CQIllustratorShapeControlPoint *point) const;

  uint numLines() const;

  const CQIllustratorShapeControlLine *getLine(uint i) const;

  CQIllustratorShapeControlLine *getLine(uint i);

  bool exists(const CQIllustratorShapeControlLine *line) const;

 protected:
  void clearLines();

  void addLine(CQIllustratorShapeControlLine *line);

  void removeLine(CQIllustratorShapeControlLine *line);

  //----

  void clearPoints();

  void addPoint(CQIllustratorShapeControlPoint *point);

  void removePoint(CQIllustratorShapeControlPoint *point);

  //----

 private:
  typedef std::vector<CQIllustratorShapeControlLine  *> LineList;
  typedef std::vector<CQIllustratorShapeControlPoint *> PointList;

  CQIllustratorShape *shape_;
  LineList            lines_;
  PointList           points_;
};

class CQIllustratorSelectedShapes : public QObject {
  Q_OBJECT

 private:
  typedef std::list<CQIllustratorSelectedShape> SelectedShapes;

 public:
  typedef SelectedShapes::iterator       iterator;
  typedef SelectedShapes::const_iterator const_iterator;

 public:
  CQIllustratorSelectedShapes(CQIllustrator *illustrator);

  bool empty() const { return shapes_.empty(); }

  uint size() const { return shapes_.size(); }

  iterator begin() { return shapes_.begin(); }
  iterator end  () { return shapes_.end  (); }

  const_iterator begin() const { return shapes_.begin(); }
  const_iterator end  () const { return shapes_.end  (); }

  CQIllustratorSelectedShape &front() { return shapes_.front(); }

  const CQIllustratorSelectedShape &front() const { return shapes_.front(); }

  void clear();

  void startSelect();
  void endSelect  ();

  void add(CQIllustratorShape *shape);

  void remove(iterator i);

  bool exists(const CQIllustratorShape *shape) const;

  bool exists(const CQIllustratorShape *shape, const CQIllustratorShapeControlPoint *point) const;

  CQIllustratorSelectedShape &get(CQIllustratorShape *shape);

  CQIllustratorSelectedShape &checkoutShape(const CQIllustratorSelectedShape &sshape,
                                            CQIllustratorData::ChangeType changeType);

  void checkinShape(const CQIllustratorSelectedShape &sshape,
                    CQIllustratorData::ChangeType changeType);

  //----

  void clearShapeLines(CQIllustratorSelectedShape &shape);

  void addShapeLine(CQIllustratorSelectedShape &shape, CQIllustratorShapeControlLine *line);

  void removeShapeLine(CQIllustratorSelectedShape &shape, CQIllustratorShapeControlLine *line);

  void setShapeLine(CQIllustratorSelectedShape &shape, uint i, const CLine2D &l);

  //----

  void clearShapePoints(CQIllustratorSelectedShape &shape);

  void addShapePoint(CQIllustratorSelectedShape &shape, CQIllustratorShapeControlPoint *point);

  void removeShapePoint(CQIllustratorSelectedShape &shape, CQIllustratorShapeControlPoint *point);

 signals:
  void selectionChanged();

 private:
  CQIllustrator  *illustrator_;
  SelectedShapes  shapes_;
  bool            locked_;
};

#endif
