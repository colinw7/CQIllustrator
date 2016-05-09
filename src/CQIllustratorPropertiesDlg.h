#ifndef CQIllustratorPropertiesDlg_H
#define CQIllustratorPropertiesDlg_H

#include <QFrame>

class CQIllustrator;
class CQIllustratorShape;
class CQPropertyTree;

class CQIllustratorPropertiesDlg : public QFrame {
  Q_OBJECT

 public:
  CQIllustratorPropertiesDlg(CQIllustrator *illustrator);

  CQIllustrator *illustrator() const { return illustrator_; }

  CQPropertyTree *tree() const { return tree_; }

  void load();

  void loadShape(const QString &parentName, CQIllustratorShape *object);

  QSize sizeHint() const;

 private slots:
  void itemSelectedSlot(QObject *obj, const QString &path);

 private:
  CQIllustrator  *illustrator_ { 0 };
  CQPropertyTree *tree_        { 0 };
};

#endif
