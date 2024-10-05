#include <CQOptionToolSet.h>

class CQLayerOptionDialog;
class CQIllustrator;
class CQIllustratorShape;
class QLabel;
class QListWidget;
class QListWidgetItem;

class CQLayerOptionTool : public CQOptionTool {
  Q_OBJECT

 public:
  CQLayerOptionTool(CQIllustrator *illustrator);

  CQIllustrator *getIllustrator() const { return illustrator_; }

  CQOptionToolDialog *getDialog() override;

  void updateLayer();

  void reloadLayers();

 signals:
  void valueChanged(const QString &name);

 private slots:
  void populateSlot();

 private:
  CQIllustrator       *illustrator_;
  CQLayerOptionDialog *dialog_;
  QLabel              *label_;
};

class CQLayerOptionDialog : public CQOptionToolDialog {
  Q_OBJECT

 public:
  CQLayerOptionDialog(CQLayerOptionTool *tool);

  void populate();

 private:
  void initWidgets();

 signals:
  void valueChanged(const QString &name);

 private slots:
  void itemSelectedSlot(QListWidgetItem *item);

  void addLayerSlot();

 private:
  CQLayerOptionTool *tool_;
  QListWidget       *list_;
};
