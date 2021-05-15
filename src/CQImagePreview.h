#ifndef CQIMAGE_PREVIEW_H
#define CQIMAGE_PREVIEW_H

#include <CImageLib.h>

#include <QWidget>

class QResizeEvent;
class QPaintEvent;

class CQImagePreviewCanvas;
class CQScrollArea;

//---

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

  void scrollUp   (bool page=false);
  void scrollDown (bool page=false);
  void scrollLeft (bool page=false);
  void scrollRight(bool page=false);

 signals:
  void cancelSignal();

 private slots:
  void canvasUpdateSlot();

 private:
  CQImagePreviewCanvas* canvas_ { nullptr };
  CQScrollArea*         sarea_  { nullptr };
};

//---

class CQImagePreviewCanvas : public QWidget {
  Q_OBJECT

 public:
  CQImagePreviewCanvas(CQImagePreview *view);

  virtual ~CQImagePreviewCanvas();

  CImagePtr getImage() const { return image_; }

  void setImage(CImagePtr image);

  double getZoomFactor() const { return zoomFactor_; }

  void setZoomFactor(double factor);

  void setFillScreen(bool fillScreen) { fillScreen_ = fillScreen; }
  void setKeepAspect(bool keepAspect) { keepAspect_ = keepAspect; }

  bool getFillScreen() const { return fillScreen_; }
  bool getKeepAspect() const { return keepAspect_; }

  void setBackground(const CRGBA &rgba);

  void zoomIncrease();
  void zoomDecrease();

  void resizeEvent(QResizeEvent *e) override;
  void paintEvent(QPaintEvent *e) override;

  void keyPressEvent(QKeyEvent *e) override;

 private:
  CQImagePreview* view_       { nullptr };
  CImagePtr       image_;
  double          zoomFactor_ { 1.0 };
  bool            fillScreen_ { false };
  bool            keepAspect_ { true };
  CRGBA           bg_;
};

#endif
