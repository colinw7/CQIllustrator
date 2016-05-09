#ifndef CQIllustratorShape_H
#define CQIllustratorShape_H

#include <CAutoPtrVector.h>
#include <CBBox2D.h>
#include <CPoint2D.h>
#include <CPolygon2D.h>
#include <CMatrix2D.h>
#include <CRGBA.h>
#include <CLineDash.h>
#include <CLineCapType.h>
#include <CLineJoinType.h>
#include <CFillType.h>
#include <CPathPartType.h>
#include <CFont.h>
#include <CImageLib.h>
#include <CGenGradient.h>
#include <QRectF>

class CQIllustratorShape;
class CQIllustratorShapeDrawer;

#include <CQIllustratorSaveData.h>
#include <CQIllustratorData.h>

struct CQIllustratorShapeStroke {
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

//------

class CQIllustratorImageFill {
 public:
  enum Scale {
    SCALE_NONE,
    SCALE_FIT,
    SCALE_EQUAL
  };

 public:
  CQIllustratorImageFill() { }

  CQIllustratorImageFill(const CQIllustratorImageFill &image) :
   image_(image.image_), scale_(image.scale_), halign_(image.halign_), valign_(image.valign_) {
  }

  CImagePtr getImage() const { return image_; }

  void setImage(CImagePtr image) { image_ = image; }

  Scale getScale() const { return scale_; }

  void setScale(Scale scale) { scale_ = scale; }

  CHAlignType getHAlign() const { return halign_; }
  CVAlignType getVAlign() const { return valign_; }

  void setHAlign(CHAlignType halign) { halign_ = halign; }
  void setVAlign(CVAlignType valign) { valign_ = valign; }

 private:
  CImagePtr   image_;
  Scale       scale_ { SCALE_NONE };
  CHAlignType halign_ { CHALIGN_TYPE_LEFT };
  CVAlignType valign_ { CVALIGN_TYPE_BOTTOM };
};

//------

class CQIllustratorShapeFill {
 public:
  enum ImageScale {
    IMAGE_SCALE_NONE,
    IMAGE_SCALE_FIT,
    IMAGE_SCALE_EQUAL
  };

 public:
  CQIllustratorShapeFill() { }

  CQIllustratorShapeFill(const CQIllustratorShapeFill &fill) :
   filled_(fill.filled_), color_(fill.color_), opacity_(fill.opacity_), rule_(fill.rule_) {
    if (fill.gradient_)
      gradient_ = fill.gradient_->dup();

    if (fill.image_)
      image_ = new CQIllustratorImageFill(*fill.image_);
  }

  const CQIllustratorShapeFill &operator=(const CQIllustratorShapeFill &fill) {
    filled_  = fill.filled_;
    color_   = fill.color_;
    opacity_ = fill.opacity_;
    rule_    = fill.rule_;

    delete gradient_;

    if (fill.gradient_)
      gradient_ = fill.gradient_->dup();
    else
      gradient_ = 0;

    delete image_;

    if (fill.image_)
      image_ = new CQIllustratorImageFill(*fill.image_);
    else
      image_ = 0;

    return *this;
  }

 ~CQIllustratorShapeFill() {
    delete gradient_;
    delete image_;
  }

  bool isFilled() const { return filled_; }
  void setFilled(bool b) { filled_ = b; }

  const CRGBA &getColor() const { return color_; }
  void setColor(const CRGBA &color) { color_ = color; }

  double getOpacity() const { return opacity_; }
  void setOpacity(double opacity) { opacity_ = opacity; }

  double getFillRule() const { return rule_; }
  void setFillRule(CFillType rule) { rule_ = rule; }

  bool hasGradient() const { return (gradient_ != 0); }

  void setGradient(const CGenGradient *g) {
    delete gradient_;

    if (g)
      gradient_ = g->dup();
    else
      gradient_ = 0;
  }

  const CGenGradient *getGradient() const { return gradient_; }
  CGenGradient *getGradient() { return gradient_; }

  bool hasImage() const { return (image_ != 0); }

  CImagePtr getImage() const {
    if (! image_) {
      CQIllustratorShapeFill *th = const_cast<CQIllustratorShapeFill *>(this);

      th->image_ = new CQIllustratorImageFill;
    }

    return image_->getImage();
  }

  void setImage(CImagePtr image) {
    if (! image_) {
      CQIllustratorShapeFill *th = const_cast<CQIllustratorShapeFill *>(this);

      th->image_ = new CQIllustratorImageFill;
    }

    image_->setImage(image);
  }

  void resetImage() {
    delete image_;

    image_ = 0;
  }

  ImageScale getImageScale() const {
    if (! image_)
      return IMAGE_SCALE_NONE;

    return (ImageScale) image_->getScale();
  }

  void setImageScale(ImageScale scale) {
    if (! image_) {
      CQIllustratorShapeFill *th = const_cast<CQIllustratorShapeFill *>(this);

      th->image_ = new CQIllustratorImageFill;
    }

    image_->setScale((CQIllustratorImageFill::Scale) scale);
  }

  CHAlignType getImageHAlign() const {
    if (! image_)
      return CHALIGN_TYPE_LEFT;

    return image_->getHAlign();
  }

  void setImageHAlign(CHAlignType halign) {
    if (! image_) {
      CQIllustratorShapeFill *th = const_cast<CQIllustratorShapeFill *>(this);

      th->image_ = new CQIllustratorImageFill;
    }

    image_->setHAlign(halign);
  }

  CVAlignType getImageVAlign() const {
    if (! image_)
      return CVALIGN_TYPE_BOTTOM;

    return image_->getVAlign();
  }

  void setImageVAlign(CVAlignType valign) {
    if (! image_) {
      CQIllustratorShapeFill *th = const_cast<CQIllustratorShapeFill *>(this);

      th->image_ = new CQIllustratorImageFill;
    }

    image_->setVAlign(valign);
  }

  void draw(const CQIllustratorShape *shape, CQIllustratorShapeDrawer *drawer) const;

 private:
  bool                    filled_ { true };
  CRGBA                   color_ { 1, 1, 1 };
  double                  opacity_ { 1 };
  CFillType               rule_ { FILL_TYPE_EVEN_ODD };
  CGenGradient           *gradient_ { 0 };
  CQIllustratorImageFill *image_ { 0 };
};

//------

#define CQIllustratorShapeFilterMgrInst CQIllustratorShapeFilterMgr::getInstance()

class CQIllustratorShapeFilter;

class CQIllustratorShapeFilterMgr {
 public:
  static CQIllustratorShapeFilterMgr *getInstance();

  CQIllustratorShapeFilter *getFilter(uint id);

  void addFilter(CQIllustratorShapeFilter *filter);

 private:
  CQIllustratorShapeFilterMgr();
 ~CQIllustratorShapeFilterMgr();

 private:
  typedef std::map<uint,CQIllustratorShapeFilter *> FilterMap;

  FilterMap filterMap_;
  uint      id_ { 0 };
};

//------

class CQIllustratorShapeFilter {
 public:
  CQIllustratorShapeFilter();

  virtual ~CQIllustratorShapeFilter();

  uint getId() const { return id_; }

 private:
  friend class CQIllustratorShapeFilterMgr;

  void setId(uint id) { id_ = id; }

 protected:
  uint id_ { 0 };
};

//------

class CQIllustratorShapeGaussianFilter : public CQIllustratorShapeFilter {
 public:
  CQIllustratorShapeGaussianFilter(double std_dev=1.0) :
   std_dev_(std_dev) {
  }

 private:
  double std_dev_;
};

//------

class CQIllustratorShapeControlLine {
 public:
  CQIllustratorShapeControlLine(uint id, const CLine2D &line=CLine2D());

  virtual ~CQIllustratorShapeControlLine() { }

  virtual CQIllustratorShapeControlLine *dup() const = 0;

  uint getId() const { return id_; }

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
  uint    id_ { 0 };
  CLine2D line_;
};

//------

class CQIllustratorShapeControlPoint {
 public:
  enum ControlPointType {
    NORMAL_POINT,
    CONTROL_POINT
  };

 public:
  CQIllustratorShapeControlPoint(uint id, const CPoint2D &point=CPoint2D(0,0),
                                 ControlPointType type=NORMAL_POINT);

  virtual ~CQIllustratorShapeControlPoint() { }

  virtual CQIllustratorShapeControlPoint *dup() const = 0;

  uint getId() const { return id_; }

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
  uint             id_ { 0 };
  CPoint2D         point_;
  ControlPointType type_;
};

//------

class CQIllustratorShapeNearestPoint2D {
 public:
  CQIllustratorShapeNearestPoint2D() :
   set_(false), dist_(0.0), p_(0) {
  }

  CQIllustratorShapeNearestPoint2D(const CQIllustratorShapeNearestPoint2D &p) :
   set_(p.set_), dist_(p.dist_), p_(p.p_ ? p.p_->dup() : 0) {
  }

  const CQIllustratorShapeNearestPoint2D &operator=(const CQIllustratorShapeNearestPoint2D &p) {
    set_  = p.set_;
    dist_ = p.dist_;
    p_    = (p.p_ ? p.p_->dup() : 0);

    return *this;
  }

 ~CQIllustratorShapeNearestPoint2D() {
    delete p_;
  }

  bool isSet() const { return set_; }

  void updatePoint(CQIllustratorShapeControlPoint *p, double dist) {
    if (! set_ || dist < dist_) {
      set_  = true;
      dist_ = dist;

      delete p_;

      p_ = p->dup();
    }
  }

  double getDist() const { return dist_; }

  CQIllustratorShapeControlPoint *getPoint() const { return p_; }

 private:
  bool                set_;
  double              dist_;
  CQIllustratorShapeControlPoint *p_;
};

//------

class CQIllustratorShapeGeometry {
 public:
  CQIllustratorShapeGeometry(bool fixed, const CPoint2D &rcenter, const CMatrix2D &m) :
   fixed_(fixed), rcenter_(rcenter), m_(m) {
  }

  virtual ~CQIllustratorShapeGeometry() { }

  virtual CQIllustratorShapeGeometry *dup() = 0;

 protected:
  bool      fixed_;
  CPoint2D  rcenter_;
  CMatrix2D m_;
};

//------

class CQIllustratorShapeLGradientControlPoint : public CQIllustratorShapeControlPoint {
 public:
  enum Position { START, END };

 public:
  CQIllustratorShapeLGradientControlPoint(Position pos, const CPoint2D &p);

  CQIllustratorShapeLGradientControlPoint *dup() const;

  Position getPosition() const { return pos_; }

  CPoint2D getPoint(const CQIllustratorShape *shape) const;

  void setPoint(CQIllustratorShape *shape, const CPoint2D &point);

 protected:
  Position pos_;
};

//------

class CQIllustratorShapeRGradientControlPoint : public CQIllustratorShapeControlPoint {
 public:
  enum Position { CENTER, FOCUS, RADIUS };

 public:
  CQIllustratorShapeRGradientControlPoint(Position pos, const CPoint2D &p);

  CQIllustratorShapeRGradientControlPoint *dup() const;

  Position getPosition() const { return pos_; }

  CPoint2D getPoint(const CQIllustratorShape *shape) const;

  void setPoint(CQIllustratorShape *shape, const CPoint2D &point);

 protected:
  Position pos_;
};

//------

#include <CPathShape.h>

class CQIllustratorShape : public QObject {
  Q_OBJECT

  Q_PROPERTY(int     id      READ getId      WRITE setId)
  Q_PROPERTY(QString name    READ getQName   WRITE setQName)
  Q_PROPERTY(QRectF  bbox    READ getQRect)
  Q_PROPERTY(bool    fixed   READ getFixed   WRITE setFixed)
  Q_PROPERTY(bool    visible READ getVisible WRITE setVisible)

 public:
  enum ControlType {
    CONTROL_GEOMETRY,
    CONTROL_LGRADIENT,
    CONTROL_RGRADIENT
  };

  typedef std::vector<CPoint2D> PointList;

  typedef CAutoPtrVector<CQIllustratorShapeControlLine>  ControlLineList;
  typedef CAutoPtrVector<CQIllustratorShapeControlPoint> ControlPointList;

  typedef std::list<CQIllustratorShape *> ShapeList;

 public:
  CQIllustratorShape(CQIllustratorShape *parent=0);
  CQIllustratorShape(const CQIllustratorShape &rhs);

  virtual ~CQIllustratorShape();

  virtual CQIllustratorShape *dup() const = 0;

  CQIllustratorData *getData() const { return data_; }
  void setData(CQIllustratorData *data) { data_ = data; }

  int getId() const { return id_; }
  void setId(int id) { id_ = id; }

  virtual const char *getClassName() const { return "shape"; }

  const std::string &getName() const { return name_; }
  void setName(const std::string &name);

  QString getQName() const { return name_.c_str(); }
  void setQName(const QString &name) { setName(name.toStdString()); }

  const CBBox2D &getBBox() const;
  QRectF getQRect() const;

  virtual CPoint2D getCenter() const;

  virtual bool getPolygon(CPolygon2D &) const = 0;

  virtual bool getPath(CPathShapePartList &) const = 0;

  const CMatrix2D &getFlatMatrix() const;

  CBBox2D getFlatBBox() const;

  const CMatrix2D &getMatrix() const { return m_; }
  void setMatrix(const CMatrix2D &m);

  virtual void addChild   (CQIllustratorShape *child);
  virtual void removeChild(CQIllustratorShape *child);

  CQIllustratorShape *getParent() const { return parent_; }
  virtual void setParent(CQIllustratorShape *newParent);

  virtual void childrenChanged() { }

  const ShapeList &getChildren() const { return shapes_; }

  CQIllustratorShapeStroke &getStroke();
  CQIllustratorShapeFill   &getFill  ();

  const CQIllustratorShapeStroke &getStroke() const;
  const CQIllustratorShapeFill   &getFill  () const;

  void setStroke(const CQIllustratorShapeStroke &stroke);
  void setFill  (const CQIllustratorShapeFill   &fill  );

  void setStrokeColor(const CRGBA &rgba);
  void setFillColor  (const CRGBA &rgba);

  void setStrokeOpacity(double opacity);
  void setFillOpacity  (double opacity);

  double getStrokeWidth() const;

  void setFilter(uint num);

  void setFilter(CQIllustratorShapeFilter *filter);

  void unsetFilter();

  CQIllustratorShapeFilter *getFilter() const;

  bool getClip() const { return clip_; }
  void setClip(bool clip);

  bool getFixed() const { return fixed_; }
  void setFixed(bool fixed) { fixed_ = fixed; }

  void setVisible(bool visible) { visible_ = visible; }
  bool getVisible() const { return visible_; }

  virtual CPoint2D getRotateCenter() const;

  void setRotateCenter(const CPoint2D &center);

  virtual double distance(const CPoint2D &p) const;

  virtual bool inside(const CPoint2D &p) const;

  virtual void getControlLines(ControlLineList &lines) const;

  virtual void setControlLine(const CQIllustratorShapeControlLine *line);

  virtual void getControlPoints(ControlPointList &points, ControlType type=CONTROL_GEOMETRY) const;

  virtual void setControlPoint(const CQIllustratorShapeControlPoint *point) = 0;

  CQIllustratorShapeNearestPoint2D nearestPoint(const CPoint2D &p,
                                                ControlType type=CONTROL_GEOMETRY) const;

  void moveTo(const CPoint2D &pos);

  virtual void moveBy(const CPoint2D &d) = 0;

  void resizeTo(double w, double h);

  virtual void resizeBy(double dw, double dh);

  virtual void movePointTo(const CQIllustratorShapeControlPoint *point, const CPoint2D &pos);
  virtual void movePointBy(const CQIllustratorShapeControlPoint *point, const CPoint2D &d);

  bool removePoint(const CQIllustratorShapeControlPoint *point);

  virtual bool doRemovePoint(const CQIllustratorShapeControlPoint *point);

  virtual void scale(const CPoint2D &c, double sx, double sy);

  virtual void rotate(double da);

  virtual void skew(double dx, double dy);

  virtual void transform(const CPoint2D &c, const CMatrix2D &m);

  virtual void setBBox(const CBBox2D &bbox);

  virtual bool flip(bool x_axis=true);

  void draw(CQIllustratorShapeDrawer *drawer) const;

  virtual void drawShape(CQIllustratorShapeDrawer *drawer) const = 0;

  void drawGaussian(CQIllustratorShapeDrawer *drawer) const;

  virtual CQIllustratorShapeGeometry *getGeometry() = 0;

  void setGeometry(const CQIllustratorShapeGeometry *geom);

  virtual void doSetGeometry(const CQIllustratorShapeGeometry *) = 0;

  virtual void saveSVG(const CQIllustratorSaveData &saveData) = 0;
  virtual void saveCmd(const CQIllustratorSaveData &saveData) = 0;

  virtual void drawSelect(CQIllustratorShapeDrawer *drawer);

  std::string getSVGStroke() const;

  std::string getSVGMatrix() const;
  std::string getSVGMatrix(const CMatrix2D &m) const;

  //------

  void lockShape();
  void unlockShape();

  int getLocked() const { return (lock_count_ > 0); }

 protected:
  void checkoutShape(CQIllustratorData::ChangeType changeType);
  void checkinShape (CQIllustratorData::ChangeType changeType);

  void invalidateBBox() const { bbox_valid_ = false; }

  virtual void updateBBox() const = 0;

  double pointDist(const CPoint2D &p1, const CPoint2D &p2) const;

 private:
  CQIllustratorShape &operator=(const CQIllustratorShape &rhs);

 protected:
  static int last_id_;

  CQIllustratorData        *data_ { 0 };
  int                       id_ { 0 };
  CQIllustratorShape       *parent_ { 0 };
  ShapeList                 shapes_;
  std::string               name_;
  CQIllustratorShapeStroke  stroke_;
  CQIllustratorShapeFill    fill_;
  uint                      filter_id_ { 0 };
  bool                      clip_ { false };
  bool                      fixed_ { false };
  bool                      visible_ { true };
  CPoint2D                  rcenter_;
  CMatrix2D                 m_;
  int                       lock_count_ { 0 };

  mutable CBBox2D           bbox_;
  mutable bool              bbox_valid_ { false };
};

//------

class CQIllustratorRectGeometry : public CQIllustratorShapeGeometry {
 public:
  CQIllustratorRectGeometry(bool fixed=false, const CPoint2D &rcenter=CPoint2D(0,0),
                            const CMatrix2D m=CMatrix2D(), const CPoint2D &p1=CPoint2D(),
                            const CPoint2D &p2=CPoint2D(), double rx=0.0, double ry=0.0) :
   CQIllustratorShapeGeometry(fixed, rcenter, m), p1_(p1), p2_(p2), rx_(rx), ry_(ry) {
  }

  CQIllustratorRectGeometry(const CQIllustratorRectGeometry &geometry) :
   CQIllustratorShapeGeometry(geometry), p1_(geometry.p1_), p2_(geometry.p2_),
   rx_(geometry.rx_), ry_(geometry.ry_) {
  }

  virtual CQIllustratorRectGeometry *dup() {
    return new CQIllustratorRectGeometry(*this);
  }

  friend class CQIllustratorRectShape;

 private:
  CQIllustratorRectGeometry &operator=(const CQIllustratorRectGeometry &);

 private:
  CPoint2D p1_, p2_;
  double   rx_, ry_;
};

//------

class CQIllustratorRectShape : public CQIllustratorShape {
 public:
  enum ControlPointType {
    TYPE_LL, TYPE_UR, TYPE_RX, TYPE_RY
  };

 public:
  CQIllustratorRectShape(const CPoint2D &p1, const CPoint2D &p2, double rx=0.0, double ry=0.0);
  CQIllustratorRectShape(const CQIllustratorRectShape &rect);

  CQIllustratorRectShape *dup() const;

  const char *getClassName() const { return "rect"; }

  void getControlPoints(ControlPointList &points,
                        ControlType type=CONTROL_GEOMETRY) const;

  void setControlPoint(const CQIllustratorShapeControlPoint *point);

  double getRadiusX() const { return rx_; }
  double getRadiusY() const { return ry_; }

  void setRadiusX(double rx);
  void setRadiusY(double ry);

  CPoint2D getLL() const;
  CPoint2D getUR() const;

  CPoint2D getRXPoint() const;
  CPoint2D getRYPoint() const;

  void setLL(const CPoint2D &p);
  void setUR(const CPoint2D &p);

  void setRXPoint(const CPoint2D &p);
  void setRYPoint(const CPoint2D &p);

  bool getPolygon(CPolygon2D &polygon) const;

  bool getPath(CPathShapePartList &path) const;

  void moveBy(const CPoint2D &d);

  void resizeBy(double dw, double dh);

  bool flip(bool x_axis);

  void drawShape(CQIllustratorShapeDrawer *drawer) const;

  CQIllustratorShapeGeometry *getGeometry();

  void doSetGeometry(const CQIllustratorShapeGeometry *geom);

  void saveSVG(const CQIllustratorSaveData &saveData);
  void saveCmd(const CQIllustratorSaveData &saveData);

  double getAngle() const;

 protected:
  void updateBBox() const;

 protected:
  CPoint2D p1_, p2_;
  double   rx_, ry_;
};

//------

class CQIllustratorRectShapeControlPoint : public CQIllustratorShapeControlPoint {
 public:
  CQIllustratorRectShapeControlPoint(CQIllustratorRectShape::ControlPointType type,
                                     const CPoint2D &p);

  CQIllustratorRectShapeControlPoint *dup() const;

  CQIllustratorRectShape::ControlPointType getType() const { return type_; }

  CPoint2D getPoint(const CQIllustratorShape *shape) const;

  void setPoint(CQIllustratorShape *shape, const CPoint2D &point);

 protected:
  CQIllustratorRectShape::ControlPointType type_;
};

//------

enum CEllipseConnectType {
  CELLIPSE_CONNECT_LINE,  // Line
  CELLIPSE_CONNECT_CENTER // Center
};

class CQIllustratorEllipseGeometry : public CQIllustratorShapeGeometry {
 public:
  CQIllustratorEllipseGeometry(bool fixed=false, const CPoint2D &rcenter=CPoint2D(0,0),
                               const CMatrix2D m=CMatrix2D(), const CPoint2D &p1=CPoint2D(),
                               const CPoint2D &p2=CPoint2D(), double angle1=0.0,
                               double angle2=360.0,
                               CEllipseConnectType connectType=CELLIPSE_CONNECT_LINE) :
   CQIllustratorShapeGeometry(fixed, rcenter, m), p1_(p1), p2_(p2),
                              angle1_(angle1), angle2_(angle2), connectType_(connectType) {
  }

  CQIllustratorEllipseGeometry(const CQIllustratorEllipseGeometry &geometry) :
   CQIllustratorShapeGeometry(geometry), p1_(geometry.p1_), p2_(geometry.p2_),
   angle1_(geometry.angle1_), angle2_(geometry.angle2_) {
  }

  virtual CQIllustratorEllipseGeometry *dup() {
    return new CQIllustratorEllipseGeometry(*this);
  }

  friend class CQIllustratorEllipseShape;

 private:
  CQIllustratorEllipseGeometry &operator=(const CQIllustratorEllipseGeometry &);

 protected:
  CPoint2D            p1_, p2_;
  double              angle1_, angle2_;
  CEllipseConnectType connectType_;
};

//------

class CQIllustratorEllipseShape : public CQIllustratorShape {
 public:
  enum ControlPointType {
    TYPE_RX, TYPE_RY, TYPE_A1, TYPE_A2
  };

 public:
  CQIllustratorEllipseShape(const CPoint2D &p1, const CPoint2D &p2,
                            double angle1=0.0, double angle2=360.0,
                            CEllipseConnectType connectType=CELLIPSE_CONNECT_LINE);
  CQIllustratorEllipseShape(const CQIllustratorEllipseShape &ellipse);

  CQIllustratorEllipseShape *dup() const;

  const char *getClassName() const { return "ellipse"; }

  void setConnectType(CEllipseConnectType connectType) { connectType_ = connectType; }

  CEllipseConnectType getConnectType() const { return connectType_; }

  void getControlPoints(ControlPointList &points,
                        ControlType type=CONTROL_GEOMETRY) const;

  void setControlPoint(const CQIllustratorShapeControlPoint *point);

  double getAngle1() const;
  double getAngle2() const;

  void setAngle1(double a1);
  void setAngle2(double a2);

  CPoint2D getRXPoint() const;
  CPoint2D getRYPoint() const;

  void setRXPoint(const CPoint2D &p);
  void setRYPoint(const CPoint2D &p);

  CPoint2D getAngle1Point() const;
  CPoint2D getAngle2Point() const;

  void setAngle1Point(const CPoint2D &p);
  void setAngle2Point(const CPoint2D &p);

  bool getPolygon(CPolygon2D &polygon) const;

  bool getPath(CPathShapePartList &path) const;

  void moveBy(const CPoint2D &d);

  void resizeBy(double dw, double dh);

  bool flip(bool x_axis);

  void drawShape(CQIllustratorShapeDrawer *drawer) const;

  CQIllustratorShapeGeometry *getGeometry();

  void doSetGeometry(const CQIllustratorShapeGeometry *geom);

  void saveSVG(const CQIllustratorSaveData &saveData);
  void saveCmd(const CQIllustratorSaveData &saveData);

  double getAngle() const;

 protected:
  void updateBBox() const;

 protected:
  CPoint2D            p1_, p2_;
  double              angle1_, angle2_;
  CEllipseConnectType connectType_;
};

//------

class CQIllustratorEllipseShapeControlPoint : public CQIllustratorShapeControlPoint {
 public:
  CQIllustratorEllipseShapeControlPoint(CQIllustratorEllipseShape::ControlPointType type,
                                        const CPoint2D &p);

  CQIllustratorEllipseShapeControlPoint *dup() const;

  CQIllustratorEllipseShape::ControlPointType getType() const { return type_; }

  CPoint2D getPoint(const CQIllustratorShape *shape) const;

  void setPoint(CQIllustratorShape *shape, const CPoint2D &point);

 protected:
  CQIllustratorEllipseShape::ControlPointType type_;
};

//------

class CQIllustratorNPolyGeometry : public CQIllustratorShapeGeometry {
 public:
  CQIllustratorNPolyGeometry(bool fixed=false, const CPoint2D &rcenter=CPoint2D(0,0),
                             const CMatrix2D m=CMatrix2D(), const CPoint2D &c=CPoint2D(),
                             uint n=5, double r=0.0, double a=0.0) :
   CQIllustratorShapeGeometry(fixed, rcenter, m), c_(c), n_(n), r_(r), a_(a) {
  }

  CQIllustratorNPolyGeometry(const CQIllustratorNPolyGeometry &geometry) :
   CQIllustratorShapeGeometry(geometry), c_(geometry.c_), n_(geometry.n_),
   r_(geometry.r_), a_(geometry.a_) {
  }

  virtual CQIllustratorNPolyGeometry *dup() {
    return new CQIllustratorNPolyGeometry(*this);
  }

  friend class CQIllustratorNPolyShape;

 private:
  CQIllustratorNPolyGeometry &operator=(const CQIllustratorNPolyGeometry &);

 protected:
  CPoint2D c_;
  uint     n_;
  double   r_;
  double   a_;
};

//------

class CQIllustratorNPolyShape : public CQIllustratorShape {
 public:
  enum ControlPointType {
    TYPE_CENTER, TYPE_RADIUS
  };

 public:
  CQIllustratorNPolyShape(const CPoint2D &c, uint n, double r, double a);
  CQIllustratorNPolyShape(const CQIllustratorNPolyShape &poly);

  CQIllustratorNPolyShape *dup() const;

  const char *getClassName() const { return "npoly"; }

  void getControlPoints(ControlPointList &points,
                        ControlType type=CONTROL_GEOMETRY) const;

  void setControlPoint(const CQIllustratorShapeControlPoint *point);

  CPoint2D getCenter() const { return c_; }

  CPoint2D getRotateCenter() const;

  uint getNum() const { return n_; }

  void setNum(uint n);

  double getRadius() const { return r_; }

  void setRadius(double r);

  double getAngle() const { return a_; }

  void setAngle(double a);

  CPoint2D getCenterPoint() const;

  void setCenterPoint(const CPoint2D &c);

  CPoint2D getPoint() const;

  void setPoint(const CPoint2D &p);

  bool getPolygon(CPolygon2D &polygon) const;

  bool getPath(CPathShapePartList &path) const;

  void moveBy(const CPoint2D &d);

  bool flip(bool x_axis);

  void drawShape(CQIllustratorShapeDrawer *drawer) const;

  CQIllustratorShapeGeometry *getGeometry();

  void doSetGeometry(const CQIllustratorShapeGeometry *geom);

  void saveSVG(const CQIllustratorSaveData &saveData);
  void saveCmd(const CQIllustratorSaveData &saveData);

 protected:
  void updateBBox() const;

  void getPoints(std::vector<CPoint2D> &p) const;

 protected:
  CPoint2D c_;
  uint     n_;
  double   r_;
  double   a_;
};

//------

class CQIllustratorNPolyShapeControlPoint : public CQIllustratorShapeControlPoint {
 public:
  CQIllustratorNPolyShapeControlPoint(CQIllustratorNPolyShape::ControlPointType type,
                                      const CPoint2D &p);

  CQIllustratorNPolyShapeControlPoint *dup() const;

  CQIllustratorNPolyShape::ControlPointType getType() const { return type_; }

  CPoint2D getPoint(const CQIllustratorShape *shape) const;

  void setPoint(CQIllustratorShape *shape, const CPoint2D &point);

 protected:
  CQIllustratorNPolyShape::ControlPointType type_;
};

//------

class CQIllustratorStarGeometry : public CQIllustratorShapeGeometry {
 public:
  CQIllustratorStarGeometry(bool fixed=false, const CPoint2D &rcenter=CPoint2D(0,0),
                            const CMatrix2D m=CMatrix2D(), const CPoint2D &c=CPoint2D(),
                            uint n=5, double r1=0.0, double r2=0.0, double a1=0.0, double a2=0.0) :
   CQIllustratorShapeGeometry(fixed, rcenter, m), c_(c), n_(n), r1_(r1), r2_(r2), a1_(a1), a2_(a2) {
  }

  CQIllustratorStarGeometry(const CQIllustratorStarGeometry &geometry) :
   CQIllustratorShapeGeometry(geometry), c_(geometry.c_), n_(geometry.n_), r1_(geometry.r1_),
   r2_(geometry.r2_), a1_(geometry.a1_), a2_(geometry.a2_) {
  }

  virtual CQIllustratorStarGeometry *dup() {
    return new CQIllustratorStarGeometry(*this);
  }

  friend class CQIllustratorStarShape;

 private:
  CQIllustratorStarGeometry &operator=(const CQIllustratorStarGeometry &);

 protected:
  CPoint2D c_;
  uint     n_;
  double   r1_, r2_;
  double   a1_, a2_;
};

//------

class CQIllustratorStarShape : public CQIllustratorShape {
 public:
  enum ControlPointType {
    TYPE_CENTER, TYPE_IR, TYPE_OR
  };

 public:
  CQIllustratorStarShape(const CPoint2D &c, uint n, double r1, double r2, double a1, double a2);
  CQIllustratorStarShape(const CQIllustratorStarShape &star);

  CQIllustratorStarShape *dup() const;

  const char *getClassName() const { return "star"; }

  void getControlPoints(ControlPointList &points,
                        ControlType type=CONTROL_GEOMETRY) const;

  void setControlPoint(const CQIllustratorShapeControlPoint *point);

  CPoint2D getCenter() const { return c_; }

  CPoint2D getRotateCenter() const;

  uint getNum() const { return n_; }

  void setNum(uint n);

  double getInnerRadius() const { return r1_; }
  double getOuterRadius() const { return r2_; }

  void setInnerRadius(double r);
  void setOuterRadius(double r);

  double getInnerAngle() const { return a1_; }
  double getOuterAngle() const { return a2_; }

  void setInnerAngle(double a);
  void setOuterAngle(double a);

  CPoint2D getCenterPoint() const;

  void setCenterPoint(const CPoint2D &c);

  CPoint2D getInnerPoint() const;

  void setInnerPoint(const CPoint2D &p);

  CPoint2D getOuterPoint() const;

  void setOuterPoint(const CPoint2D &p);

  bool getPolygon(CPolygon2D &polygon) const;

  bool getPath(CPathShapePartList &path) const;

  void moveBy(const CPoint2D &d);

  bool flip(bool x_axis);

  void drawShape(CQIllustratorShapeDrawer *drawer) const;

  CQIllustratorShapeGeometry *getGeometry();

  void doSetGeometry(const CQIllustratorShapeGeometry *geom);

  void saveSVG(const CQIllustratorSaveData &saveData);
  void saveCmd(const CQIllustratorSaveData &saveData);

 protected:
  void updateBBox() const;

  void getPoints(std::vector<CPoint2D> &pi, std::vector<CPoint2D> &po) const;

 protected:
  CPoint2D c_;
  uint     n_;
  double   r1_, r2_;
  double   a1_, a2_;
};

//------

class CQIllustratorStarShapeControlPoint : public CQIllustratorShapeControlPoint {
 public:
  CQIllustratorStarShapeControlPoint(CQIllustratorStarShape::ControlPointType type,
                                     const CPoint2D &p);

  CQIllustratorStarShapeControlPoint *dup() const;

  CQIllustratorStarShape::ControlPointType getType() const { return type_; }

  CPoint2D getPoint(const CQIllustratorShape *shape) const;

  void setPoint(CQIllustratorShape *shape, const CPoint2D &point);

 protected:
  CQIllustratorStarShape::ControlPointType type_;
};

//------

class CQIllustratorPolygonGeometry : public CQIllustratorShapeGeometry {
 public:
  typedef std::vector<CPoint2D> PointList;

 public:
  CQIllustratorPolygonGeometry(bool fixed=false, const CPoint2D &rcenter=CPoint2D(0,0),
                               const CMatrix2D m=CMatrix2D(), const PointList points=PointList()) :
   CQIllustratorShapeGeometry(fixed, rcenter, m), points_(points) {
  }

  CQIllustratorPolygonGeometry(const CQIllustratorPolygonGeometry &geometry) :
   CQIllustratorShapeGeometry(geometry), points_(geometry.points_) {
  }

  virtual CQIllustratorPolygonGeometry *dup() {
    return new CQIllustratorPolygonGeometry(*this);
  }

  friend class CQIllustratorPolygonShape;

 private:
  CQIllustratorPolygonGeometry &operator=(const CQIllustratorPolygonGeometry &);

 protected:
  PointList points_;
};

//------

class CQIllustratorPolygonShape : public CQIllustratorShape {
 public:
  CQIllustratorPolygonShape(const PointList &points=PointList());
  CQIllustratorPolygonShape(const CQIllustratorPolygonShape &polygon);

  CQIllustratorPolygonShape *dup() const;

  const char *getClassName() const { return "polygon"; }

  void setPoints(const CPolygon2D &polygon);

  void addPoint(const CPoint2D &p);

  double distance(const CPoint2D &p) const;

  bool inside(const CPoint2D &p) const;

  CPoint2D getPoint(uint i) const;

  void setPoint(uint i, const CPoint2D &p);

  void getControlPoints(ControlPointList &points,
                        ControlType type=CONTROL_GEOMETRY) const;

  void setControlPoint(const CQIllustratorShapeControlPoint *point);

  bool addPoint(const CQIllustratorShapeControlPoint *before_point, const CPoint2D &p);

  bool doRemovePoint(const CQIllustratorShapeControlPoint *point);

  bool getPolygon(CPolygon2D &polygon) const;

  bool getPath(CPathShapePartList &path) const;

  void moveBy(const CPoint2D &d);

  void resizeBy(double dw, double dh);

  bool flip(bool x_axis);

  void drawShape(CQIllustratorShapeDrawer *drawer) const;

  CQIllustratorShapeGeometry *getGeometry();

  void doSetGeometry(const CQIllustratorShapeGeometry *geom);

  void saveSVG(const CQIllustratorSaveData &saveData);
  void saveCmd(const CQIllustratorSaveData &saveData);

 protected:
  void updateBBox() const;

 protected:
  PointList points_;
};

//------

class CQIllustratorPolygonShapeControlPoint : public CQIllustratorShapeControlPoint {
 public:
  CQIllustratorPolygonShapeControlPoint(uint ind, const CPoint2D &p);

  CQIllustratorPolygonShapeControlPoint *dup() const;

  CPoint2D getPoint(const CQIllustratorShape *shape) const;

  void setPoint(CQIllustratorShape *shape, const CPoint2D &point);

  int getInd() const { return ind_; }

 protected:
  int ind_;
};

//------

class CQIllustratorPathGeometry : public CQIllustratorShapeGeometry {
 public:
  CQIllustratorPathGeometry(bool fixed=false, const CPoint2D &rcenter=CPoint2D(0,0),
                            const CMatrix2D m=CMatrix2D(),
                            const CPathShapePartList &parts=CPathShapePartList()) :
   CQIllustratorShapeGeometry(fixed, rcenter, m), parts_(parts) {
  }

  CQIllustratorPathGeometry(const CQIllustratorPathGeometry &geometry) :
   CQIllustratorShapeGeometry(geometry), parts_(geometry.parts_) {
  }

  virtual CQIllustratorPathGeometry *dup() {
    return new CQIllustratorPathGeometry(*this);
  }

  friend class CPathShape;

 private:
  CQIllustratorPathGeometry &operator=(const CQIllustratorPathGeometry &);

 protected:
  CPathShapePartList parts_;
};

//------

class CPathShape : public CQIllustratorShape {
 public:
  CPathShape(const CPathShapePartList &parts=CPathShapePartList());
  CPathShape(const CPathShape &path);

  CPathShape *dup() const;

  const char *getClassName() const { return "path"; }

  const CPathShapePartList &getParts() const { return parts_; }

  bool getPolygon(CPolygon2D &polygon) const;

  bool getPath(CPathShapePartList &path) const;

  void moveBy(const CPoint2D &d);

  void startGroup();
  void endGroup();

  void setParts(const CPathShapePartList &parts);

  void addMoveTo(const CPoint2D &p);
  void addLineTo(const CPoint2D &p);
  void addCurveTo(const CPoint2D &p1, const CPoint2D &p2);
  void addCurveTo(const CPoint2D &p1, const CPoint2D &p2, const CPoint2D &p3);
  void addArcTo(const CPoint2D &c, double rx, double ry, double angle1, double angle2);
  void addClose();

  void addLineTo(uint ind, const CPoint2D &p);

  bool empty() const;

  double distance(const CPoint2D &p) const;

  bool inside(const CPoint2D &p) const;

  void getControlLines(ControlLineList &lines) const;

  void getLines(std::vector<CLine2D> &lines) const;

  void getControlPoints(ControlPointList &points,
                        ControlType type=CONTROL_GEOMETRY) const;

  void setControlPoint(const CQIllustratorShapeControlPoint *point);

  bool doRemovePoint(const CQIllustratorShapeControlPoint *point);

  CLine2D getLine(uint ind) const;

  void setLine(uint ind, const CLine2D &l);

  void setLineType(uint ind, CPathPartType type);

  CPoint2D getPoint(uint ind, uint ind1) const;

  void setPoint(uint ind, uint ind1, const CPoint2D &p);

  void setCornerPoint(uint ind);
  void setCurvePoint(uint ind);

  bool flip(bool x_axis);

  void drawShape(CQIllustratorShapeDrawer *drawer) const;

  CQIllustratorShapeGeometry *getGeometry();

  void doSetGeometry(const CQIllustratorShapeGeometry *geom);

  void saveSVG(const CQIllustratorSaveData &saveData);
  void saveCmd(const CQIllustratorSaveData &saveData);

  void processPath(CPathShapeProcess *process);

 protected:
  void updateBBox() const;

 protected:
  CPathShapePartList parts_;
  mutable int        group_ { 0 };
};

//------

class CPathShapeControlLine : public CQIllustratorShapeControlLine {
 public:
  CPathShapeControlLine(uint ind, const CLine2D &l);

  CPathShapeControlLine *dup() const;

  CLine2D getLine(const CQIllustratorShape *shape) const;

  void setLine(CQIllustratorShape *shape, const CLine2D &l);

  uint getInd() const { return ind_; }

  void setType(CQIllustratorShape *shape, CPathPartType type);

 protected:
  uint ind_;
};

//------

class CPathShapeControlPoint : public CQIllustratorShapeControlPoint {
 public:
  CPathShapeControlPoint(uint ind, const CPoint2D &p);

  CPathShapeControlPoint(uint ind, uint ind1, const CPoint2D &p);

  CPathShapeControlPoint *dup() const;

  CPoint2D getPoint(const CQIllustratorShape *shape) const;

  void setPoint(CQIllustratorShape *shape, const CPoint2D &point);

  uint getInd () const { return ind_ ; }
  uint getInd1() const { return ind1_; }

  bool isMaster() const { return (ind1_ == 0); }

  void setCornerNode(CQIllustratorShape *shape) const;
  void setCurveNode (CQIllustratorShape *shape) const;

 protected:
  uint ind_  { 0 };
  uint ind1_ { 0 };
};

//------

class CQIllustratorTextGeometry : public CQIllustratorShapeGeometry {
 public:
  CQIllustratorTextGeometry(bool fixed=false, const CPoint2D &rcenter=CPoint2D(0,0),
                            const CMatrix2D m=CMatrix2D(), const CPoint2D &p1=CPoint2D(),
                            const CPoint2D &p2=CPoint2D(), const std::string &str="",
                            CFontPtr font=CFontPtr(), CHAlignType halign=CHALIGN_TYPE_CENTER,
                            CVAlignType valign=CVALIGN_TYPE_CENTER) :
   CQIllustratorShapeGeometry(fixed, rcenter, m), p1_(p1), p2_(p2), str_(str), font_(font),
   halign_(halign), valign_(valign) {
  }

  CQIllustratorTextGeometry(const CQIllustratorTextGeometry &geometry) :
   CQIllustratorShapeGeometry(geometry), p1_(geometry.p1_), p2_(geometry.p2_), str_(geometry.str_),
   font_(geometry.font_), halign_(geometry.halign_), valign_(geometry.valign_) {
  }

  virtual CQIllustratorTextGeometry *dup() {
    return new CQIllustratorTextGeometry(*this);
  }

  friend class CQIllustratorTextShape;

 private:
  CQIllustratorTextGeometry &operator=(const CQIllustratorTextGeometry &);

 protected:
  CPoint2D    p1_, p2_;
  std::string str_;
  CFontPtr    font_;
  CHAlignType halign_;
  CVAlignType valign_;
};

//------

class CQIllustratorTextShape : public CQIllustratorShape {
 public:
  enum ControlPointType {
    TYPE_LL, TYPE_UR
  };

 public:
  CQIllustratorTextShape(const CPoint2D &p1, const CPoint2D &p2,
                         const std::string &str, CFontPtr font=CFontPtr(),
                         CHAlignType halign=CHALIGN_TYPE_CENTER,
                         CVAlignType valign=CVALIGN_TYPE_CENTER);
  CQIllustratorTextShape(const CQIllustratorTextShape &text);

  CQIllustratorTextShape *dup() const;

  const char *getClassName() const { return "text"; }

  void getControlPoints(ControlPointList &points,
                        ControlType type=CONTROL_GEOMETRY) const;

  void setControlPoint(const CQIllustratorShapeControlPoint *point);

  CPoint2D getLL() const;
  CPoint2D getUR() const;

  void setLL(const CPoint2D &p);
  void setUR(const CPoint2D &p);

  std::string getText() const { return str_; }

  void setText(const std::string &str);

  CFontPtr getFont() const { return font_; }

  void setFont(CFontPtr font);

  CHAlignType getHAlign() const { return halign_; }
  CVAlignType getVAlign() const { return valign_; }

  void setHAlign(CHAlignType halign);
  void setVAlign(CVAlignType valign);

  int getCursorPos() const { return cursorPos_; }

  void setCursorPos(int cursorPos) { cursorPos_ = cursorPos; }

  bool getPolygon(CPolygon2D &polygon) const;

  bool getPath(CPathShapePartList &path) const;

  void moveBy(const CPoint2D &d);

  void resizeBy(double dw, double dh);

  bool flip(bool x_axis);

  void drawShape(CQIllustratorShapeDrawer *drawer) const;

  CQIllustratorShapeGeometry *getGeometry();

  void doSetGeometry(const CQIllustratorShapeGeometry *geom);

  void saveSVG(const CQIllustratorSaveData &saveData);
  void saveCmd(const CQIllustratorSaveData &saveData);

  void drawSelect(CQIllustratorShapeDrawer *drawer);

 protected:
  void updateBBox() const;

 protected:
  CPoint2D    p1_;
  CPoint2D    p2_;
  std::string str_;
  CFontPtr    font_;
  CHAlignType halign_;
  CVAlignType valign_;
  int         cursorPos_;
};

//------

class CQIllustratorTextShapeControlPoint : public CQIllustratorShapeControlPoint {
 public:
  CQIllustratorTextShapeControlPoint(CQIllustratorTextShape::ControlPointType type,
                                     const CPoint2D &p);

  CQIllustratorTextShapeControlPoint *dup() const;

  CQIllustratorTextShape::ControlPointType getType() const { return type_; }

  CPoint2D getPoint(const CQIllustratorShape *shape) const;

  void setPoint(CQIllustratorShape *shape, const CPoint2D &point);

 protected:
  CQIllustratorTextShape::ControlPointType type_;
};

//------

class CQIllustratorGroupGeometry : public CQIllustratorShapeGeometry {
 public:
  CQIllustratorGroupGeometry(bool fixed=false, const CPoint2D &rcenter=CPoint2D(0,0),
                             const CMatrix2D m=CMatrix2D()) :
   CQIllustratorShapeGeometry(fixed, rcenter, m) {
  }

  CQIllustratorGroupGeometry(const CQIllustratorGroupGeometry &geometry) :
   CQIllustratorShapeGeometry(geometry) {
  }

  virtual CQIllustratorGroupGeometry *dup() {
    return new CQIllustratorGroupGeometry(*this);
  }

  friend class CQIllustratorGroupShape;

 private:
  CQIllustratorGroupGeometry &operator=(const CQIllustratorGroupGeometry &);
};

//------

class CQIllustratorGroupShape : public CQIllustratorShape {
 public:
  CQIllustratorGroupShape();
  CQIllustratorGroupShape(const CQIllustratorGroupShape &group);

  CQIllustratorGroupShape *dup() const;

  const char *getClassName() const { return "group"; }

  void childrenChanged();

  void getControlPoints(ControlPointList &points,
                        ControlType type=CONTROL_GEOMETRY) const;

  void setControlPoint(const CQIllustratorShapeControlPoint *point);

  void scale(const CPoint2D &c, double sx, double sy);

  void rotate(double da);

  void transform(const CPoint2D &c, const CMatrix2D &m);

  bool getPolygon(CPolygon2D &polygon) const;

  bool getPath(CPathShapePartList &path) const;

  void getPolygons(std::vector<CPolygon2D> &polygons) const;

  void moveBy(const CPoint2D &d);

  bool flip(bool x_axis);

  void drawShape(CQIllustratorShapeDrawer *drawer) const;

  CQIllustratorShapeGeometry *getGeometry();

  void doSetGeometry(const CQIllustratorShapeGeometry *geom);

  void saveSVG(const CQIllustratorSaveData &saveData);
  void saveCmd(const CQIllustratorSaveData &saveData);

 protected:
  void updateBBox() const;
};

//------

class CQIllustratorGroupShapeControlPoint : public CQIllustratorShapeControlPoint {
 public:
  CQIllustratorGroupShapeControlPoint(uint ind, const CPoint2D &p);

  CQIllustratorGroupShapeControlPoint *dup() const;

  CPoint2D getPoint(const CQIllustratorShape *shape) const;

  void setPoint(CQIllustratorShape *shape, const CPoint2D &point);

  int getInd() const { return ind_; }

 protected:
  int ind_;
};

#endif
