#ifndef CQIllustratorSetImageMode_H
#define CQIllustratorSetImageMode_H

#include <CQIllustratorMode.h>
#include <CQIllustratorToolbar.h>
#include <CQIllustrator.h>
#include <CAutoPtr.h>

class QComboBox;
class CQImageButton;
class CQIllustrator;
class CQIllustratorSetImageMode;

class CQIllustratorImageSizer : public CQIllustratorModeSizer {
 public:
  CQIllustratorImageSizer(CQIllustratorSetImageMode *mode);

  void drawHandles(QPainter *painter, const CQIllustratorShape *shape) override;
};

class CQIllustratorSetImageToolbar : public CQIllustratorToolbar {
  Q_OBJECT

 public:
  CQIllustratorSetImageToolbar(CQIllustratorSetImageMode *mode);

  const char *getTitle() const override { return "Set Image"; }

  QIcon getIcon() override;

  void addWidgets() override;

  void setSelectedShape(const CQIllustratorShape *shape) override;

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
  CQIllustratorSetImageMode          *mode_ { nullptr };
  CQIllustratorShapeFill::ImageScale  scale_;
  CHAlignType                         halign_;
  CVAlignType                         valign_;
  QComboBox                          *scaleCombo_ { nullptr };
  CQImageButton                      *imageButton_ { nullptr };
  CQImageButton                      *lalignButton_ { nullptr };
  CQImageButton                      *hcalignButton_ { nullptr };
  CQImageButton                      *ralignButton_ { nullptr };
  CQImageButton                      *talignButton_ { nullptr };
  CQImageButton                      *vcalignButton_ { nullptr };
  CQImageButton                      *balignButton_ { nullptr };
};

//---

class CQIllustratorSetImageMode : public CQIllustratorMode {
  Q_OBJECT

 public:
  CQIllustratorSetImageMode(CQIllustrator *illustrator);

  const char *getTitle() const override { return "Set Image"; }

  CQIllustratorSetImageToolbar *createToolbar() override;

  CQIllustratorImageSizer *createSizer() override;

  CQMenuItem *createMenuItem(CQMenu *menu) override;

  CQIllustratorSetImageToolbar *getToolbar() const override { return toolbar_; }

  void handleMouseRelease(const MouseEvent &e) override;
  void handleMouseDrag   (const MouseEvent &e) override;

  void drawOverlay(CQIllustratorShapeDrawer *drawer) override;

  QCursor getCursor() const override;

 private:
  CQIllustratorSetImageToolbar*     toolbar_ { nullptr };
  CAutoPtr<CQIllustratorImageSizer> sizer_;
};

#endif
