#ifndef CQIllustratorUndo_H
#define CQIllustratorUndo_H

#include <QObject>
#include <CUndo.h>
#include <CPoint2D.h>
#include <CQIllustratorData.h>
#include <CQIllustratorShape.h>

class CQIllustrator;

class CQIllustratorUndo : public QObject, public CUndo {
  Q_OBJECT

 public:
  typedef CQIllustratorData::ChangeType ChangeType;

  CQIllustratorUndo(CQIllustrator *illustrator);

  CQIllustrator *getIllustrator() const { return illustrator_; }

  bool addUndo(CUndoData *data);

  bool undo(uint n=1);
  bool redo(uint n=1);

  void emitUndoChanged();

 signals:
  void undoChanged();

 private slots:
  void objectCreated(CQIllustratorShape *shape);
  void objectDeleted(CQIllustratorShape *shape);

  void objectPreModify (CQIllustratorShape *shape, ChangeType changeType);
  void objectPostModify(CQIllustratorShape *shape, ChangeType changeType);

 private:
  CQIllustrator *illustrator_;
};

class CQIllustratorUndoSetShapeGeometry : public CUndoData {
 public:
  CQIllustratorUndoSetShapeGeometry(CQIllustratorUndo *undo, CQIllustratorShape *shape);

 ~CQIllustratorUndoSetShapeGeometry() { }

  bool exec();

  std::string getDesc() const;

 private:
  CQIllustratorUndo          *undo_;
  uint                        id_;
  CQIllustratorShapeGeometry *geom_;
};

class CQIllustratorUndoSetShapeStroke : public CUndoData {
 public:
  CQIllustratorUndoSetShapeStroke(CQIllustratorUndo *undo, CQIllustratorShape *shape);

 ~CQIllustratorUndoSetShapeStroke() { }

  bool exec();

  std::string getDesc() const;

 private:
  CQIllustratorUndo        *undo_;
  uint                      id_;
  CQIllustratorShapeStroke  stroke_;
};

class CQIllustratorUndoSetShapeFill : public CUndoData {
 public:
  CQIllustratorUndoSetShapeFill(CQIllustratorUndo *undo, CQIllustratorShape *shape);

 ~CQIllustratorUndoSetShapeFill() { }

  bool exec();

  std::string getDesc() const;

 private:
  CQIllustratorUndo      *undo_;
  uint                    id_;
  CQIllustratorShapeFill  fill_;
};

class CQIllustratorUndoCreateShape : public CUndoData {
 public:
  CQIllustratorUndoCreateShape(CQIllustratorUndo *undo, CQIllustratorShape *shape, bool create);

 ~CQIllustratorUndoCreateShape() { }

  bool exec();

  std::string getDesc() const;

 private:
  CQIllustratorUndo  *undo_;
  bool                create_;
  CQIllustratorShape *shape_;
  uint                id_;
};

#endif
