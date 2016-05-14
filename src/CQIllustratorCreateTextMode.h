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

  void drawHandles(QPainter *painter, const CQIllustratorShape *shape);

 private:
  CQIllustratorControlPointHandle *ll_handle_ { 0 };
  CQIllustratorControlPointHandle *ur_handle_ { 0 };
};

//------

class CQIllustratorCreateTextMode : public CQIllustratorMode {
  Q_OBJECT

 public:
  CQIllustratorCreateTextMode(CQIllustrator *illustrator);

  const char *getTitle() const { return "Create Text"; }

  CQIllustratorCreateTextToolbar *getToolbar() { return toolbar_; }

  CQIllustratorCreateTextToolbar *createToolbar();

  CQIllustratorCreateTextSizer *createSizer();

  CQMenuItem *createMenuItem(CQMenu *menu);

  bool isCreateMode() const { return true; }

  CQIllustratorCreateTextToolbar *getToolbar() const { return toolbar_; }

  void handleMouseRelease(const MouseEvent &e);
  void handleMouseDrag   (const MouseEvent &e);

  bool handleKeyPress(const KeyEvent &e);

  void drawOverlay(CQIllustratorShapeDrawer *drawer);

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
