#include <CQOptionToolSet.h>

class CQObjectOptionDialog;
class CQIllustrator;
class CQIllustratorShape;
class QLabel;
class QTreeWidget;
class QTreeWidgetItem;

class CQObjectOptionTool : public CQOptionTool {
  Q_OBJECT

 public:
  CQObjectOptionTool(CQIllustrator *illustrator);

  CQOptionToolDialog *getDialog();

  void selectShape(const std::string &name);

 signals:
  void valueChanged(const QString &name);

 private slots:
  void selectionChangedSlot();
  void populateSlot();

 private:
  CQIllustrator*        illustrator_ { nullptr };
  CQObjectOptionDialog* dialog_      { nullptr };
  QLabel*               label_       { nullptr };
};

//---

class CQObjectOptionDialog : public CQOptionToolDialog {
  Q_OBJECT

 public:
  CQObjectOptionDialog(CQObjectOptionTool *tool);

  void populate(CQIllustrator *illustrator);

 private:
  void initWidgets();
  void updateWidgets();

  void populateChildren(CQIllustrator *illustrator, QTreeWidgetItem *pitem,
                        const CQIllustratorShape *shape);

 signals:
  void valueChanged(const QString &name);

 private slots:
  void itemSelectedSlot(QTreeWidgetItem *item, int pos);

 private:
  CQObjectOptionTool* tool_ { nullptr };
  QTreeWidget*        list_ { nullptr };
};
