#ifndef CQILLUSTARTOR_SHAPE_DRAWER_H
#define CQILLUSTARTOR_SHAPE_DRAWER_H

#include <QFont>
#include <QTransform>
#include <QImage>
#include <CBBox2D.h>
#include <CMatrix2D.h>
#include <CFont.h>
#include <CImageLib.h>
#include <CFillType.h>

class QPainter;

class CQIllustrator;
class CQIllustratorShape;
class CQIllustratorShapeStroke;
class CQIllustratorShapeFill;

class CQIllustratorShapeDrawer {
 public:
  CQIllustratorShapeDrawer(CQIllustrator *illustrator, QPainter *painter=0);

  virtual ~CQIllustratorShapeDrawer();

  CQIllustrator *getIllustrator() const { return illustrator_; }

  void setPainter(QPainter *painter);

  QPainter *getPainter() const { return painter_; }

  void setBBox(const CBBox2D &bbox);

  void pushMatrix(const CMatrix2D &m, bool combine=true);
  void popMatrix();

  void setStroke(const CQIllustratorShape *shape, const CQIllustratorShapeStroke &stroke);
  void setFill  (const CQIllustratorShape *shape, const CQIllustratorShapeFill &fill);

  void setFont(CFontPtr font);

  void drawText(const CBBox2D &rect, const std::string &str,
                CHAlignType halign, CVAlignType valign);

  void pathInit();
  void pathMoveTo(const CPoint2D &p);
  void pathLineTo(const CPoint2D &p);
  void pathBezierTo(const CPoint2D &p1, const CPoint2D &p2);
  void pathBezierTo(const CPoint2D &p1, const CPoint2D &p2, const CPoint2D &p3);
  void pathArc(const CPoint2D &c, double xr, double yr, double angle1, double angle2);
  void pathText(const CBBox2D &rect, const std::string &str,
                CHAlignType halign, CVAlignType valign);
  void pathClose();

  void pathStroke();
  void pathFill(CFillType type);
  void pathClip();

  void drawControlLine(const CPoint2D &p1, const CPoint2D &p2);

  //void drawControlPointNormal(const CPoint2D &point, bool selected);
  //void drawControlPointControl(const CPoint2D &point, bool selected);

  void drawGridLine(const CPoint2D &p1, const CPoint2D &p2);

  void drawGridSubLine(const CPoint2D &p1, const CPoint2D &p2);

  void drawImage(int x, int y, QImage image);

 protected:
  typedef std::vector<CMatrix2D> MatrixStack;

  CQIllustrator *illustrator_ { 0 };
  QPainter      *painter_ { 0 };
  CBBox2D        bbox_;
  QPainterPath  *path_ { 0 };
  QFont          qfont_;
  CMatrix2D      m_ { CMatrix2D::Type::IDENTITY };
  MatrixStack    matrices_;

  //CImagePtr control_point_image_;
  //CImagePtr control_point_active_image_;
  //CImagePtr curve_point_image_;
  //CImagePtr curve_point_active_image_;

  //QImage qi1_control_;
  //QImage qi2_control_;
  //QImage qi1_curve_;
  //QImage qi2_curve_;
};

//------

class CQIllustratorFilterShapeDrawer : public CQIllustratorShapeDrawer {
 public:
  CQIllustratorFilterShapeDrawer(CQIllustratorShapeDrawer *drawer,
                                 const CQIllustratorShape *shape);

 ~CQIllustratorFilterShapeDrawer();

  QImage &getImage();

 private:
//CQIllustrator            *illustrator_ { nullptr };
  const CQIllustratorShape *shape_ { 0 };
  QImage                    qimage_;
  CBBox2D                   bbox_;
};

#endif
