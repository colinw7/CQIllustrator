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
class CQIllustratorPathShape;
class CQIllustratorShapeDrawer;
class CQIllustratorShapeGeometry;
class CQIllustratorShapeFill;
class CQIllustratorShapeStroke;
class CQIllustratorShapeFilter;
class CQIllustratorShapeControlLine;
class CQIllustratorShapeControlPoint;

#include <CQIllustratorSaveData.h>
#include <CQIllustratorData.h>
#include <CQIllustratorShapeFill.h>
#include <CQIllustratorShapeStroke.h>
#include <CQIllustratorShapeNearestPoint2D.h>
#include <CPathShape.h>

class CQIllustratorShape : public QObject {
  Q_OBJECT

  Q_PROPERTY(int     id       READ getId      WRITE setId)
  Q_PROPERTY(QString name     READ getQName   WRITE setQName)
  Q_PROPERTY(QRectF  bbox     READ getQRect)
  Q_PROPERTY(QRectF  flatBBox READ getFlatQRect)
  Q_PROPERTY(bool    visible  READ getVisible WRITE setVisible)
  Q_PROPERTY(bool    fixed    READ getFixed   WRITE setFixed)
  Q_PROPERTY(bool    clip     READ getClip    WRITE setClip)

 public:
  enum class ControlType {
    GEOMETRY,
    LGRADIENT,
    RGRADIENT
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

  CMatrix2D getFlatMatrix() const;

  CBBox2D getFlatBBox() const;
  QRectF getFlatQRect() const;

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

  virtual void getControlPoints(ControlPointList &points,
                                ControlType type=ControlType::GEOMETRY) const;

  virtual void setControlPoint(const CQIllustratorShapeControlPoint *point) = 0;

  CQIllustratorShapeNearestPoint2D nearestPoint(const CPoint2D &p,
                                                ControlType type=ControlType::GEOMETRY) const;

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

  //------

  void invalidateBBox() const { bbox_valid_ = false; }

 protected:
  void checkoutShape(CQIllustratorData::ChangeType changeType);
  void checkinShape (CQIllustratorData::ChangeType changeType);

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

#endif
