#ifndef CQIllustratorCreateTextMode_H
#define CQIllustratorCreateTextMode_H

#include <CQIllustratorMode.h>
#include <CQIllustratorToolbar.h>

class QFontComboBox;
class QComboBox;
class CQRealEdit;
class QLineEdit;
class CQImageButton;
class CQIllustrator;
class CQIllustratorControlPointHandle;
class CQIllustratorCreateTextMode;
class CQIllustratorCreateTextSizer;

class CQIllustratorCreateTextToolbar : public CQIllustratorToolbar {
  Q_OBJECT

 public:
  CQIllustratorCreateTextToolbar(CQIllustratorCreateTextMode *createTextMode);

  const char *getTitle() const { return "Create Text"; }

  QFont getFont() const { return qfont_; }

  QIcon getIcon();

  void addWidgets();

  void setSelectedShape(const CQIllustratorShape *shape);

  void setSize(const CBBox2D &bbox);

 private:
  void updateWidgets();
  void updateFontWidgets();
  void updateFontFromWidgets();

 private slots:
  void updateShape();

  void fontComboSlot();
  void sizeComboSlot();
  void boldSlot();
  void italicSlot();

  void lalignSlot();
  void hcalignSlot();
  void ralignSlot();

  void talignSlot();
  void vcalignSlot();
  void balignSlot();

 private:
  CQIllustratorCreateTextMode *mode_;
  QFont                        qfont_;
  int                          font_ind_;
  int                          font_size_;
  CHAlignType                  halign_;
  CVAlignType                  valign_;
  QFontComboBox               *fontCombo_;
  QComboBox                   *sizeCombo_;
  CQImageButton               *boldButton_;
  CQImageButton               *italicButton_;
  QLineEdit                   *textEdit_;
  CQRealEdit                  *widthEdit_;
  CQRealEdit                  *heightEdit_;
  CQImageButton               *lalignButton_;
  CQImageButton               *hcalignButton_;
  CQImageButton               *ralignButton_;
  CQImageButton               *talignButton_;
  CQImageButton               *vcalignButton_;
  CQImageButton               *balignButton_;
};

class CQIllustratorCreateTextSizer : public CQIllustratorModeSizer {
 public:
  CQIllustratorCreateTextSizer(CQIllustratorCreateTextMode *mode);

  void drawHandles(QPainter *painter, const CQIllustratorShape *shape);

 private:
  CQIllustratorControlPointHandle *ll_handle_;
  CQIllustratorControlPointHandle *ur_handle_;
};

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
  CQIllustratorCreateTextToolbar *toolbar_;
  CQIllustratorCreateTextSizer   *sizer_;
  bool                            equalSize_;
  QString                         text_;
  int                             cursor_pos_;
};

#endif
