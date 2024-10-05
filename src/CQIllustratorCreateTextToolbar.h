#ifndef CQIllustratorCreateTextToolbar_H
#define CQIllustratorCreateTextToolbar_H

#include <CQIllustratorToolbar.h>
#include <CBBox2D.h>
#include <CAlignType.h>

class CQIllustratorCreateTextMode;
class CQImageButton;
class CQRealEdit;
class QFontComboBox;
class QComboBox;
class QLineEdit;

class CQIllustratorCreateTextToolbar : public CQIllustratorToolbar {
  Q_OBJECT

 public:
  CQIllustratorCreateTextToolbar(CQIllustratorCreateTextMode *createTextMode);

  const char *getTitle() const override { return "Create Text"; }

  QFont getFont() const { return qfont_; }

  QIcon getIcon() override;

  void addWidgets() override;

  void setSelectedShape(const CQIllustratorShape *shape) override;

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
  CQIllustratorCreateTextMode *mode_ { 0 };
  QFont                        qfont_;
  int                          font_ind_ { -1 };
  int                          font_size_ { -1 };
  CHAlignType                  halign_ { CHALIGN_TYPE_CENTER };
  CVAlignType                  valign_ { CVALIGN_TYPE_CENTER };
  QFontComboBox               *fontCombo_ { 0 };
  QComboBox                   *sizeCombo_ { 0 };
  CQImageButton               *boldButton_ { 0 };
  CQImageButton               *italicButton_ { 0 };
  QLineEdit                   *textEdit_ { 0 };
  CQRealEdit                  *widthEdit_ { 0 };
  CQRealEdit                  *heightEdit_ { 0 };
  CQImageButton               *lalignButton_ { 0 };
  CQImageButton               *hcalignButton_ { 0 };
  CQImageButton               *ralignButton_ { 0 };
  CQImageButton               *talignButton_ { 0 };
  CQImageButton               *vcalignButton_ { 0 };
  CQImageButton               *balignButton_ { 0 };
};

#endif
