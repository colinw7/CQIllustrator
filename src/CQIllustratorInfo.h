#include <QWidget>

class QLabel;
class CQIllustratorCanvas;
class CQIllustratorShape;

class CQIllustratorInfo : public QWidget {
  Q_OBJECT

 public:
  CQIllustratorInfo(CQIllustratorCanvas *canvas);

  void setShape(const CQIllustratorShape *shape);

 private:
  CQIllustratorCanvas *canvas_;
  QLabel              *label_;
};
