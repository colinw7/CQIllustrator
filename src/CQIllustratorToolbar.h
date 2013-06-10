#ifndef CQIllustratorToolbar_H
#define CQIllustratorToolbar_H

#include <QWidget>

class QToolButton;

class CQIllustratorMode;
class CQIllustratorShape;
class CQIllustratorShapeControlPoint;

class CQIllustratorToolbar : public QWidget {
  Q_OBJECT

 public:
  CQIllustratorToolbar(CQIllustratorMode *mode);

  virtual ~CQIllustratorToolbar() { }

  void init();

  virtual const char *getTitle() const = 0;

  virtual QIcon getIcon() = 0;

  virtual void addWidgets();

  virtual void setSelectedShape(const CQIllustratorShape *shape);

  virtual void setSelectedShapePoint(const CQIllustratorShape *shape,
                                     const CQIllustratorShapeControlPoint *point);

 private slots:
  void modeSlot();

  void createEditSlot();

 private:
  CQIllustratorMode *mode_;
  QToolButton       *createEditButton_;
};

#endif
