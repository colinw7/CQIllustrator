#ifndef CQIllustratorCreateTextMode_H
#define CQIllustratorCreateTextMode_H

#include <CQIllustratorMode.h>
#include <CQIllustratorCreateTextToolbar.h>

class QFontComboBox;
class QComboBox;
class CQRealEdit;
class QLineEdit;
class CQImageButton;
class CQIllustrator;
class CQIllustratorControlPointHandle;
class CQIllustratorCreateTextMode;
class CQIllustratorCreateTextToolbar;

class CQIllustratorCreateTextSizer : public CQIllustratorModeSizer {
 public:
  CQIllustratorCreateTextSizer(CQIllustratorCreateTextMode *mode);

  void drawHandles(QPainter *painter, const CQIllustratorShape *shape) override;

 private:
  CQIllustratorControlPointHandle *ll_handle_ { nullptr };
  CQIllustratorControlPointHandle *ur_handle_ { nullptr };
};

//------

class CQIllustratorCreateTextMode : public CQIllustratorMode {
  Q_OBJECT

 public:
  CQIllustratorCreateTextMode(CQIllustrator *illustrator);

  const char *getTitle() const override { return "Create Text"; }

  CQIllustratorCreateTextToolbar *getToolbar() { return toolbar_; }

  CQIllustratorCreateTextToolbar *createToolbar() override;

  CQIllustratorCreateTextSizer *createSizer() override;

  CQMenuItem *createMenuItem(CQMenu *menu) override;

  bool isCreateMode() const override { return true; }

  CQIllustratorCreateTextToolbar *getToolbar() const override { return toolbar_; }

  void handleMouseRelease(const MouseEvent &e) override;
  void handleMouseDrag   (const MouseEvent &e) override;

  bool handleKeyPress(const KeyEvent &e) override;

  void drawOverlay(CQIllustratorShapeDrawer *drawer) override;

  const QString &getText() const { return text_; }

  void setText(const QString &text);

 private:
  CQIllustratorCreateTextToolbar *toolbar_ { 0 };
  CQIllustratorCreateTextSizer   *sizer_ { 0 };
  bool                            equalSize_ { false };
  QString                         text_;
  int                             cursor_pos_ { 0 };
};

#endif
