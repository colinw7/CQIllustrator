#ifndef CQIllustratorSandbox_H
#define CQIllustratorSandbox_H

#include <vector>
#include <sys/types.h>
#include <CPoint2D.h>
#include <CBBox2D.h>
#include <CQIllustratorData.h>

class CQIllustrator;
class CQIllustratorShape;
class CQIllustratorShapeDrawer;

class CQIllustratorSandbox {
 public:
  typedef std::vector<CQIllustratorShape *> EditShapes;

  typedef EditShapes::iterator       shape_iterator;
  typedef EditShapes::const_iterator const_shape_iterator;

 public:
  CQIllustratorSandbox(CQIllustrator *illustrator);

  void addSelection();

  void addSelectionPath();

  uint getNumShapes() const { return edit_shapes_.size(); }

  CQIllustratorShape *frontShape() {
    if (! edit_shapes_.empty())
      return edit_shapes_.front();
    else
      return nullptr;
  }

  shape_iterator beginShape() { return edit_shapes_.begin(); }
  shape_iterator endShape  () { return edit_shapes_.end  (); }

  const_shape_iterator beginShape() const { return edit_shapes_.begin(); }
  const_shape_iterator endShape  () const { return edit_shapes_.end  (); }

  void clear();

  CBBox2D getBBox() const;

  void moveBy(const CPoint2D &d);

  void moveTo(const CPoint2D &d);

  void commit(CQIllustratorData::ChangeType changeType);

  void draw(CQIllustratorShapeDrawer *drawer);

 private:
  CQIllustrator *illustrator_ { nullptr };
  EditShapes     edit_shapes_;
};

#endif
