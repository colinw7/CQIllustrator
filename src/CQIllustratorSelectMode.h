#ifndef CQIllustratorSelectMode_H
#define CQIllustratorSelectMode_H

#include <CQIllustratorMode.h>
#include <CQIllustratorToolbar.h>
#include <CQIllustratorSizer.h>
#include <CAutoPtr.h>

class CQImageButton;
class CQIllustratorSelectMode;

class CQIllustratorSelectToolbar : public CQIllustratorToolbar {
  Q_OBJECT

 public:
  CQIllustratorSelectToolbar(CQIllustratorSelectMode *mode);

  const char *getTitle() const { return "Select"; }

  QIcon getIcon();

  void addWidgets();

 private slots:
  void selectAllSlot();
  void selectNoneSlot();
  void setMode(int id);

 private:
  CQIllustratorSelectMode *mode_;
  CQImageButton           *selectAllButton_;
  CQImageButton           *selectNoneButton_;
};

class CQIllustratorSelectMode : public CQIllustratorMode {
  Q_OBJECT

 public:
  CQIllustratorSelectMode(CQIllustrator *illustrator);

  const char *getTitle() const { return "Select Shape"; }

  CQIllustratorSelectToolbar *createToolbar();

  CQMenuItem *createMenuItem(CQMenu *menu);

  bool isSelectMode() const { return true; }

  CQIllustratorSelectToolbar *getToolbar() const { return toolbar_; }

  void setInside(bool inside);

  void handleMousePress  (const MouseEvent &e);
  void handleMouseRelease(const MouseEvent &e);
  void handleMouseDrag   (const MouseEvent &e);
  void handleMouseMove   (const MouseEvent &e);

  bool handleKeyPress(const KeyEvent &e);

  void commitShapes();

  void drawOverlay(CQIllustratorShapeDrawer *drawer);

 private:
  CQIllustratorSelectToolbar   *toolbar_;
  CAutoPtr<CQIllustratorSizer>  sizer_;
  bool                          sizing_;
  CBBox2D                       drag_bbox_;
  CPoint2D                      drag_center_;
  CMatrix2D                     drag_matrix_;
  bool                          inside_;
};

#endif
