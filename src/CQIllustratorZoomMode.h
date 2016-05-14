#ifndef CQIllustratorZoomMode_H
#define CQIllustratorZoomMode_H

#include <CQIllustratorMode.h>
#include <CQIllustratorToolbar.h>

class CQImageButton;
class CQIllustrator;
class CQIllustratorZoomMode;

class CQIllustratorZoomToolbar : public CQIllustratorToolbar {
  Q_OBJECT

 public:
  CQIllustratorZoomToolbar(CQIllustratorZoomMode *mode);

  const char *getTitle() const { return "Zoom"; }

  QIcon getIcon();

  void addWidgets();

 private slots:
  void zoomFitSlot();
  void zoomSelSlot();
  void zoomInSlot ();
  void zoomOutSlot();

 private:
  CQIllustratorZoomMode *mode_ { 0 };
  CQImageButton         *zoomFitButton_ { 0 };
  CQImageButton         *zoomSelButton_ { 0 };
  CQImageButton         *zoomInButton_ { 0 };
  CQImageButton         *zoomOutButton_ { 0 };
};

//------

class CQIllustratorZoomMode : public CQIllustratorMode {
 public:
  CQIllustratorZoomMode(CQIllustrator *illustrator);

  const char *getTitle() const { return "Zoom"; }

  CQIllustratorZoomToolbar *createToolbar();

  CQMenuItem *createMenuItem(CQMenu *menu);

  CQIllustratorZoomToolbar *getToolbar() const { return toolbar_; }

  void handleMousePress  (const MouseEvent &e);
  void handleMouseRelease(const MouseEvent &e);
  void handleMouseDrag   (const MouseEvent &e);
  void handleMouseMove   (const MouseEvent &e);

  void drawOverlay(CQIllustratorShapeDrawer *drawer);

 private:
  CQIllustratorZoomToolbar *toolbar_ { 0 };
};

#endif
