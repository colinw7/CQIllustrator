#ifndef CQIllustratorCanvas_H
#define CQIllustratorCanvas_H

#include <QWidget>
#include <CDisplayRange2D.h>

class CQIllustrator;
class CQIllustratorShape;
class CQIllustratorInfo;
class CTimer;
class QMenu;

class CQIllustratorCanvas : public QWidget {
  Q_OBJECT

 public:
  CQIllustratorCanvas(CQIllustrator *illustrator);

  void mousePressEvent  (QMouseEvent *event);
  void mouseMoveEvent   (QMouseEvent *event);
  void mouseReleaseEvent(QMouseEvent *event);

  void keyPressEvent(QKeyEvent *e);

  void wheelEvent(QWheelEvent *event);

  void paintEvent (QPaintEvent  *event);
  void resizeEvent(QResizeEvent *event);

  bool getPressed() const { return pressed_; }

  const CMatrix2D &getMatrix() { return range_.getMatrix(); }

  const QTransform &getTransform () const { return transform_ ; }
  const QTransform &getITransform() const { return itransform_; }

  void updateStatus();

 private:
  void showMenu(const QPoint &pos);

 private slots:
  void timeoutSlot();

 private:
  CQIllustrator      *illustrator_ { nullptr };
  QPainter           *painter_     { nullptr };
  QMenu              *popupMenu_   { nullptr };
  CDisplayRange2D     range_;
  bool                pressed_     { false };
  QTransform          transform_;
  QTransform          itransform_;
  QTimer             *timer_       { nullptr };
  CQIllustratorShape *infoShape_   { nullptr };
  CQIllustratorInfo  *infoLabel_   { nullptr };
};

#endif
