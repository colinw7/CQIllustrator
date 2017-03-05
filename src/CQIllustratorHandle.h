#ifndef CQIllustratorHandle_H
#define CQIllustratorHandle_H

#include <CPoint2D.h>
#include <CIPoint2D.h>
#include <CISize2D.h>
#include <QPointF>
#include <QIcon>

class CQIllustrator;
class QPainter;
class CQIllustratorShape;
class CQIllustratorShapeControlPoint;
class CQIllustratorShapeStroke;

class CQIllustratorHandle {
 public:
  // use CSymbol2D for more symbols
  enum Style {
    IMAGE_STYLE,
    RECT_STYLE,
    CIRCLE_STYLE
  };

 public:
  CQIllustratorHandle(CQIllustrator *illustrator);

  virtual ~CQIllustratorHandle() { }

  void setImage(const QIcon &normal, const QIcon &active=QIcon());

  void setStyle(Style style);

  virtual CISize2D getSize();

  bool getActive() const { return active_; }

  bool getVisible() const { return visible_; }

  void setVisible(bool visible) { visible_ = visible; }

  const CPoint2D &getPosition() const { return pos_; }

  void setPosition(const CPoint2D &pos);

  bool updateActive(const QPointF &p);

  virtual void draw(QPainter *painter, const CPoint2D &point, const CIPoint2D &offset);
  virtual void draw(QPainter *painter, const CPoint2D &point);
  virtual void draw(QPainter *painter);

 private:
  void drawI(QPainter *painter);

 protected:
  CQIllustrator *illustrator_{ 0 };
  Style          style_ { RECT_STYLE };
  bool           active_ { false };
  bool           visible_ { true };
  CPoint2D       pos_ { 0, 0 };
  CIPoint2D      offset_ { 0, 0 };
  QIcon          icon_;
  QIcon          active_icon_;
};

class CQIllustratorControlPointHandle : public CQIllustratorHandle {
 public:
  CQIllustratorControlPointHandle(CQIllustrator *illustrator);

 ~CQIllustratorControlPointHandle();

  // draw control point (and assign current point for handle)
  // Note: can't save shape here as this is a 'const' shape (not checked out)
  void draw(const CQIllustratorShape *shape, CQIllustratorShapeControlPoint *point,
            QPainter *painter);

  // update point on 'checked out' shape
  void updatePoint(CQIllustratorShape *shape, const CPoint2D &point);

 private:
  CQIllustratorShapeControlPoint *point_;
};

#endif
