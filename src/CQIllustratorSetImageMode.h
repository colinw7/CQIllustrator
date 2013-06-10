#ifndef CQIllustratorSetImageMode_H
#define CQIllustratorSetImageMode_H

#include <CAutoPtr.h>

#include <CQIllustratorMode.h>
#include <CQIllustratorToolbar.h>
#include <CQIllustrator.h>

class QComboBox;
class CQImageButton;
class CQIllustrator;
class CQIllustratorSetImageMode;

class CQIllustratorImageSizer : public CQIllustratorModeSizer {
 public:
  CQIllustratorImageSizer(CQIllustratorSetImageMode *mode);

  void drawHandles(QPainter *painter, const CQIllustratorShape *shape);
};

class CQIllustratorSetImageToolbar : public CQIllustratorToolbar {
  Q_OBJECT

 public:
  CQIllustratorSetImageToolbar(CQIllustratorSetImageMode *mode);

  const char *getTitle() const { return "Set Image"; }

  QIcon getIcon();

  void addWidgets();

  void setSelectedShape(const CQIllustratorShape *shape);

  void updateShape();

  void updateWidgets();

  void setShapeImage(CQIllustratorShape *shape, CImagePtr image);

 private slots:
  void setImageSlot();
  void setScaleSlot(int);

  void lalignSlot();
  void hcalignSlot();
  void ralignSlot();

  void talignSlot();
  void vcalignSlot();
  void balignSlot();

 private:
  CQIllustratorSetImageMode          *mode_;
  CQIllustratorShapeFill::ImageScale  scale_;
  CHAlignType                         halign_;
  CVAlignType                         valign_;
  QComboBox                          *scaleCombo_;
  CQImageButton                      *imageButton_;
  CQImageButton                      *lalignButton_;
  CQImageButton                      *hcalignButton_;
  CQImageButton                      *ralignButton_;
  CQImageButton                      *talignButton_;
  CQImageButton                      *vcalignButton_;
  CQImageButton                      *balignButton_;
};

class CQIllustratorSetImageMode : public CQIllustratorMode {
  Q_OBJECT

 public:
  CQIllustratorSetImageMode(CQIllustrator *illustrator);

  const char *getTitle() const { return "Set Image"; }

  CQIllustratorSetImageToolbar *createToolbar();

  CQIllustratorImageSizer *createSizer();

  CQMenuItem *createMenuItem(CQMenu *menu);

  CQIllustratorSetImageToolbar *getToolbar() const { return toolbar_; }

  void handleMouseRelease(const MouseEvent &e);
  void handleMouseDrag   (const MouseEvent &e);

  void drawOverlay(CQIllustratorShapeDrawer *drawer);

  QCursor getCursor() const;

 private:
  CQIllustratorSetImageToolbar      *toolbar_;
  CAutoPtr<CQIllustratorImageSizer>  sizer_;
};

#endif
