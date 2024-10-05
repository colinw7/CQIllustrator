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

  const char *getTitle() const override { return "Zoom"; }

  QIcon getIcon() override;

  void addWidgets() override;

 private slots:
  void zoomFitSlot();
  void zoomSelSlot();
  void zoomInSlot ();
  void zoomOutSlot();

 private:
  CQIllustratorZoomMode *mode_ { nullptr };
  CQImageButton         *zoomFitButton_ { nullptr };
  CQImageButton         *zoomSelButton_ { nullptr };
  CQImageButton         *zoomInButton_ { nullptr };
  CQImageButton         *zoomOutButton_ { nullptr };
};

//------

class CQIllustratorZoomMode : public CQIllustratorMode {
 public:
  CQIllustratorZoomMode(CQIllustrator *illustrator);

  const char *getTitle() const override { return "Zoom"; }

  CQIllustratorZoomToolbar *createToolbar() override;

  CQMenuItem *createMenuItem(CQMenu *menu) override;

  CQIllustratorZoomToolbar *getToolbar() const override { return toolbar_; }

  void handleMousePress  (const MouseEvent &e) override;
  void handleMouseRelease(const MouseEvent &e) override;
  void handleMouseDrag   (const MouseEvent &e) override;
  void handleMouseMove   (const MouseEvent &e) override;

  void drawOverlay(CQIllustratorShapeDrawer *drawer) override;

 private:
  CQIllustratorZoomToolbar *toolbar_ { nullptr };
};

#endif
