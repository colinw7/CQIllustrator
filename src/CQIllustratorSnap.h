#include <QWidget>

class CQIllustrator;
class QCheckBox;
class QDoubleSpinBox;

class CQIllustratorSnapDock : public QWidget {
  Q_OBJECT

 public:
  CQIllustratorSnapDock(CQIllustrator *illustrator);

 private slots:
  void enabledSlot();
  void xPitchSlot(double xpitch);
  void yPitchSlot(double ypitch);

 private:
  CQIllustrator  *illustrator_;
  QCheckBox      *enabledCheck_;
  QDoubleSpinBox *xPitch_;
  QDoubleSpinBox *yPitch_;
};
