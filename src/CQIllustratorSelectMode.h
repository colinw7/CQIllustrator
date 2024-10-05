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

  const char *getTitle() const override { return "Select"; }

  QIcon getIcon() override;

  void addWidgets() override;

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

  const char *getTitle() const override { return "Select Shape"; }

  CQIllustratorSelectToolbar *createToolbar() override;

  CQMenuItem *createMenuItem(CQMenu *menu) override;

  bool isSelectMode() const override { return true; }

  CQIllustratorSelectToolbar *getToolbar() const override { return toolbar_; }

  void setInside(bool inside);

  void handleMousePress  (const MouseEvent &e) override;
  void handleMouseRelease(const MouseEvent &e) override;
  void handleMouseDrag   (const MouseEvent &e) override;
  void handleMouseMove   (const MouseEvent &e) override;

  bool handleKeyPress(const KeyEvent &e) override;

  void commitShapes();

  void drawOverlay(CQIllustratorShapeDrawer *drawer) override;

 private:
  CQIllustratorSelectToolbar   *toolbar_ { nullptr };
  CAutoPtr<CQIllustratorSizer>  sizer_;
  bool                          sizing_ { false };
  CBBox2D                       drag_bbox_;
  CPoint2D                      drag_center_;
  CMatrix2D                     drag_matrix_;
  bool                          inside_ { false };
};

#endif
