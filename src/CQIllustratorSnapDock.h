#ifndef CQIllustratorSnapDock_H
#define CQIllustratorSnapDock_H

#include <QWidget>

class CQIllustrator;
class CQRealSpin;
class QCheckBox;

class CQIllustratorSnapDock : public QWidget {
  Q_OBJECT

 public:
  CQIllustratorSnapDock(CQIllustrator *illustrator);

 private slots:
  void enabledSlot();
  void xPitchSlot(double xpitch);
  void yPitchSlot(double ypitch);

 private:
  CQIllustrator* illustrator_  { nullptr };
  QCheckBox*     enabledCheck_ { nullptr };
  CQRealSpin*    xPitch_       { nullptr };
  CQRealSpin*    yPitch_       { nullptr };
};

#endif
