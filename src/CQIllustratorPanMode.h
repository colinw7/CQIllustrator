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

  const char *getTitle() const { return "Pan"; }

  QIcon getIcon();

 private:
  CQIllustratorPanMode *panMode_;
};

class CQIllustratorPanMode : public CQIllustratorMode {
 public:
  CQIllustratorPanMode(CQIllustrator *illustrator);

  const char *getTitle() const { return "Pan"; }

  CQIllustratorPanToolbar *createToolbar();

  CQMenuItem *createMenuItem(CQMenu *menu);

  CQIllustratorPanToolbar *getToolbar() const { return toolbar_; }

  void handleMousePress  (const MouseEvent &e);
  void handleMouseRelease(const MouseEvent &e);
  void handleMouseDrag   (const MouseEvent &e);
  void handleMouseMove   (const MouseEvent &e);

  QCursor getCursor() const;

 private:
  CQIllustratorPanToolbar *toolbar_;
};

#endif
