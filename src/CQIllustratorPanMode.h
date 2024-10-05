#ifndef CQIllustratorPanMode_H
#define CQIllustratorPanMode_H

#include <CQIllustratorMode.h>
#include <CQIllustratorToolbar.h>

class CQIllustrator;
class CQIllustratorPanMode;

class CQIllustratorPanToolbar : public CQIllustratorToolbar {
  Q_OBJECT

 public:
  CQIllustratorPanToolbar(CQIllustratorPanMode *mode);

  const char *getTitle() const override { return "Pan"; }

  QIcon getIcon() override;

 private:
  CQIllustratorPanMode *panMode_;
};

class CQIllustratorPanMode : public CQIllustratorMode {
 public:
  CQIllustratorPanMode(CQIllustrator *illustrator);

  const char *getTitle() const override { return "Pan"; }

  CQIllustratorPanToolbar *createToolbar() override;

  CQMenuItem *createMenuItem(CQMenu *menu) override;

  CQIllustratorPanToolbar *getToolbar() const override { return toolbar_; }

  void handleMousePress  (const MouseEvent &e) override;
  void handleMouseRelease(const MouseEvent &e) override;
  void handleMouseDrag   (const MouseEvent &e) override;
  void handleMouseMove   (const MouseEvent &e) override;

  QCursor getCursor() const override;

 private:
  CQIllustratorPanToolbar *toolbar_ { nullptr };
};

#endif
