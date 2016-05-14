#ifndef CQIllustratorShapeStroke_H
#define CQIllustratorShapeStroke_H

#include <CLineDash.h>
#include <CLineCapType.h>
#include <CLineJoinType.h>
#include <CRGBA.h>

class CQIllustratorShape;
class CQIllustratorShapeDrawer;

class CQIllustratorShapeStroke {
 public:
  CQIllustratorShapeStroke() { }

  CQIllustratorShapeStroke(const CQIllustratorShapeStroke &stroke) :
   stroked_(stroke.stroked_), color_(stroke.color_), width_(stroke.width_),
   opacity_(stroke.opacity_), dash_(stroke.dash_), cap_(stroke.cap_), join_(stroke.join_),
   mlimit_(stroke.mlimit_) {
  }

  virtual ~CQIllustratorShapeStroke() { }

  const CQIllustratorShapeStroke &operator=(const CQIllustratorShapeStroke &stroke) {
    stroked_ = stroke.stroked_;
    color_   = stroke.color_;
    width_   = stroke.width_;
    opacity_ = stroke.opacity_;
    dash_    = stroke.dash_;
    cap_     = stroke.cap_;
    join_    = stroke.join_;
    mlimit_  = stroke.mlimit_;

    return *this;
  }

  bool isStroked() const { return stroked_; }
  void setStroked(bool b) { stroked_ = b; }

  const CRGBA &getColor() const { return color_; }
  void setColor(const CRGBA &color) { color_ = color; }

  double getWidth() const { return width_; }
  void setWidth(double width) { width_ = width; }

  double getOpacity() const { return opacity_; }
  void setOpacity(double opacity) { opacity_ = opacity; }

  const CLineDash &getLineDash() const { return dash_; }
  void setLineDash(const CLineDash &dash) { dash_ = dash; }

  const CLineCapType &getLineCap() const { return cap_; }
  void setLineCap(const CLineCapType &cap) { cap_ = cap; }

  const CLineJoinType &getLineJoin() const { return join_; }
  void setLineJoin(const CLineJoinType &join) { join_ = join; }

  double getMitreLimit() const { return mlimit_; }
  void setMitreLimit(double mlimit) { mlimit_ = mlimit; }

  void draw(const CQIllustratorShape *shape, CQIllustratorShapeDrawer *drawer) const;

 private:
  bool          stroked_ { false };
  CRGBA         color_ { 0, 0, 0 };
  double        width_ { 1 };
  double        opacity_ { 1 };
  CLineDash     dash_;
  CLineCapType  cap_ { LINE_CAP_TYPE_SQUARE };
  CLineJoinType join_ { LINE_JOIN_TYPE_MITRE };
  double        mlimit_ { 3 };
};

#endif
