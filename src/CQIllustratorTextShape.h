#ifndef CQIllustratorTextShape_H
#define CQIllustratorTextShape_H

#include <CQIllustratorShape.h>

class CQIllustratorTextShape : public CQIllustratorShape {
  Q_OBJECT

  Q_PROPERTY(QString text READ getQText WRITE setQText)
  Q_PROPERTY(QPointF ll   READ getQLL   WRITE setQLL  )
  Q_PROPERTY(QPointF ur   READ getQUR   WRITE setQUR  )

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

  CQIllustratorTextShape *dup() const override;

  const char *getClassName() const override { return "text"; }

  void getControlPoints(ControlPointList &points,
                        ControlType type=ControlType::GEOMETRY) const override;

  void setControlPoint(const CQIllustratorShapeControlPoint *point) override;

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

  bool getPolygon(CPolygon2D &polygon) const override;

  bool getPath(CPathShapePartList &path) const override;

  void moveBy(const CPoint2D &d) override;

  void resizeBy(double dw, double dh) override;

  bool flip(bool x_axis) override;

  void drawShape(CQIllustratorShapeDrawer *drawer) const override;

  CQIllustratorShapeGeometry *getGeometry() override;

  void doSetGeometry(const CQIllustratorShapeGeometry *geom) override;

  void saveSVG(const CQIllustratorSaveData &saveData) override;
  void saveCmd(const CQIllustratorSaveData &saveData) override;

  void drawSelect(CQIllustratorShapeDrawer *drawer) override;

 protected:
  void updateBBox() const override;

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

  CQIllustratorTextShapeControlPoint *dup() const override;

  ControlPointType getType() const { return type_; }

  CPoint2D getPoint(const CQIllustratorShape *shape) const override;

  void setPoint(CQIllustratorShape *shape, const CPoint2D &point) override;

 protected:
  ControlPointType type_;
};

#endif
