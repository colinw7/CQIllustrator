#ifndef CQIMAGE_PREVIEW_H
#define CQIMAGE_PREVIEW_H

#include <CImageLib.h>

#include <QWidget>

class QResizeEvent;
class QPaintEvent;

class CQImagePreviewCanvas;
class CQScrollArea;

class CQImagePreview : public QWidget {
  Q_OBJECT

 public:
  CQImagePreview(QWidget *parent=0);

 ~CQImagePreview();

  CImagePtr getImage() const;

  void setImage(CImagePtr image);

  void setBackground(const CRGBA &rgba);

  CQScrollArea *getScrollArea() const { return sarea_; }

  void sendCancelSignal();

  void scrollUp(bool page=false);
  void scrollDown(bool page=false);
  void scrollLeft(bool page=false);
  void scrollRight(bool page=false);

 signals:
  void cancelSignal();

 private slots:
  void canvasUpdateSlot();

 private:
  CQImagePreviewCanvas *canvas_;
  CQScrollArea         *sarea_;
};

class CQImagePreviewCanvas : public QWidget {
  Q_OBJECT

 public:
  CQImagePreviewCanvas(CQImagePreview *view);

  virtual ~CQImagePreviewCanvas();

  CImagePtr getImage() const { return image_; }

  void setImage(CImagePtr image);

  double getZoomFactor() const { return zoom_factor_; }

  void setZoomFactor(double factor);

  void setFillScreen(bool fill_screen) { fill_screen_ = fill_screen; }
  void setKeepAspect(bool keep_aspect) { keep_aspect_ = keep_aspect; }

  bool getFillScreen() const { return fill_screen_; }
  bool getKeepAspect() const { return keep_aspect_; }

  void setBackground(const CRGBA &rgba);

  void zoomIncrease();
  void zoomDecrease();

  void resizeEvent(QResizeEvent *e);
  void paintEvent(QPaintEvent *e);

  void keyPressEvent(QKeyEvent *e);

 private:
  CQImagePreview *view_;
  CImagePtr       image_;
  double          zoom_factor_;
  bool            fill_screen_;
  bool            keep_aspect_;
  CRGBA           bg_;
};

#endif
