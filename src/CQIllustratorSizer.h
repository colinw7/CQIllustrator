#ifndef CQIllustratorSizer_H
#define CQIllustratorSizer_H

#include <QPointF>
#include <QTransform>
#include <CBBox2D.h>

class CQIllustrator;
class CQIllustratorShape;
class CQIllustratorHandle;
class CQIllustratorShapeDrawer;

class CQIllustratorSizer {
 public:
  enum HandleType {
    HANDLE_NONE = 0,
    HANDLE_L    = (1<<0),
    HANDLE_B    = (1<<1),
    HANDLE_R    = (1<<2),
    HANDLE_T    = (1<<3),
    HANDLE_RC   = (1<<4),
    HANDLE_BL   = (HANDLE_B | HANDLE_L),
    HANDLE_BR   = (HANDLE_B | HANDLE_R),
    HANDLE_TL   = (HANDLE_T | HANDLE_L),
    HANDLE_TR   = (HANDLE_T | HANDLE_R)
  };

  enum OpType {
    OP_RESIZE,
    OP_ROTATE
  };

 public:
  CQIllustratorSizer(CQIllustrator *illustrator);

  HandleType getHandleType() const { return handle_; }

  HandleType getPressHandleType() const { return handle_; }

  void setPressHandleType  () { press_handle_ = handle_; }
  void resetPressHandleType() { press_handle_ = HANDLE_NONE; }

  OpType getOpType() const { return op_; }

  bool mousePress  (const QPointF &p);
  void mouseRelease(const QPointF &p);

  void updateShape(CQIllustratorShape *shape, const QPointF &oldPoint,
                   const QPointF &newPoint, bool equal_scale=false);

  bool updateActive(const QPointF &p);

  void toggleOp();

  void draw(CQIllustratorShapeDrawer *drawer, const CQIllustratorShape *shape);

 private:
  CQIllustrator *illustrator_;

  QTransform transform_;
  HandleType handle_;
  HandleType press_handle_;
  OpType     op_;

  CQIllustratorHandle *bl_corner_handle_;
  CQIllustratorHandle *br_corner_handle_;
  CQIllustratorHandle *tl_corner_handle_;
  CQIllustratorHandle *tr_corner_handle_;

  CQIllustratorHandle *bl_rotate_handle_;
  CQIllustratorHandle *br_rotate_handle_;
  CQIllustratorHandle *tl_rotate_handle_;
  CQIllustratorHandle *tr_rotate_handle_;

  CQIllustratorHandle *l_side_handle_;
  CQIllustratorHandle *b_side_handle_;
  CQIllustratorHandle *r_side_handle_;
  CQIllustratorHandle *t_side_handle_;

  CQIllustratorHandle *rcenter_handle_;
};

#endif
