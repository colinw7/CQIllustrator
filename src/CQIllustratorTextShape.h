#ifndef CQIllustratorTextShape_H
#define CQIllustratorTextShape_H

#include <CQIllustratorShape.h>

class CQIllustratorTextShape : public CQIllustratorShape {
  Q_OBJECT

  Q_PROPERTY(QString text READ getQText WRITE setQText);
  Q_PROPERTY(QPointF ll   READ getQLL   WRITE setQLL  );
  Q_PROPERTY(QPointF ur   READ getQUR   WRITE setQUR  );

 public:
  enum class ControlPointType {
    LL,
    UR
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
                        ControlType type=ControlType::GEOMETRY) const;

  void setControlPoint(const CQIllustratorShapeControlPoint *point);

  //---

  CPoint2D getLL() const;
  void setLL(const CPoint2D &p);

  QPointF getQLL() const;
  void setQLL(const QPointF &p);

  //---

  CPoint2D getUR() const;
  void setUR(const CPoint2D &p);

  QPointF getQUR() const;
  void setQUR(const QPointF &p);

  //---

  std::string getText() const { return str_; }
  void setText(const std::string &str);

  QString getQText() const;
  void setQText(const QString &text);

  //---

  CFontPtr getFont() const { return font_; }
  void setFont(CFontPtr font);

  //---

  CHAlignType getHAlign() const { return halign_; }
  void setHAlign(CHAlignType halign);

  CVAlignType getVAlign() const { return valign_; }
  void setVAlign(CVAlignType valign);

  //---

  int getCursorPos() const { return cursorPos_; }
  void setCursorPos(int cursorPos) { cursorPos_ = cursorPos; }

  //---

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
  CHAlignType halign_ { CHALIGN_TYPE_CENTER };
  CVAlignType valign_ { CVALIGN_TYPE_CENTER };
  int         cursorPos_ { -1 };
};

//------

class CQIllustratorTextShapeControlPoint : public CQIllustratorShapeControlPoint {
 public:
  typedef CQIllustratorTextShape::ControlPointType ControlPointType;

 public:
  CQIllustratorTextShapeControlPoint(ControlPointType type, const CPoint2D &p);

  CQIllustratorTextShapeControlPoint *dup() const;

  ControlPointType getType() const { return type_; }

  CPoint2D getPoint(const CQIllustratorShape *shape) const;

  void setPoint(CQIllustratorShape *shape, const CPoint2D &point);

 protected:
  ControlPointType type_;
};

#endif
