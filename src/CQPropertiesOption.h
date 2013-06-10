#include <CQOptionToolSet.h>
#include <CQPointEdit.h>
#include <CQTableWidget.h>
#include <CQIllustratorShape.h>

#include <QPointer>

class CQIllustratorShape;
class CQPropertiesOptionDialog;
class CQControlPointsTable;
class CQIllustrator;
class CQPointEdit;
class CQMatrix2D;
class CQTransformPanel;

class QLabel;
class QLineEdit;
class QTabWidget;
class CQRealEdit;
class CQAngleSpinBox;

class CQPropertiesOptionTool : public CQOptionTool {
  Q_OBJECT

 public:
  CQPropertiesOptionTool(CQIllustrator *illustrator);

  CQIllustrator *getIllustrator() const { return illustrator_; }

  CQOptionToolDialog *getDialog();

  void setObject(CQIllustratorShape *shape);

  void emitValueChanged();

 signals:
  void valueChanged();

 private slots:
  void selectionChangedSlot();

 private:
  CQIllustrator            *illustrator_;
  CQPropertiesOptionDialog *dialog_;
  QLabel                   *label_;
};

class CQPropertiesOptionDialog : public CQOptionToolDialog {
  Q_OBJECT

 public:
  CQPropertiesOptionDialog(CQPropertiesOptionTool *tool);

  CQIllustrator *getIllustrator() const;

  void update(const CQIllustratorShape *shape);

 private:
  void initWidgets();
  void updateWidgets();

 private slots:
  void updateShape();

  void replaceMatrixSlot();
  void appendMatrixSlot();
  void resetMatrixSlot();

 private:
  CQPropertiesOptionTool *tool_;
  std::string             name_;
  QTabWidget             *tab_;
  QLineEdit              *nameEdit_;
  QLineEdit              *classEdit_;
  CQPointEdit            *posEdit_;
  CQPointEdit            *sizeEdit_;
  QLineEdit              *textEdit_;
  CQControlPointsTable   *table_;
  CQTransformPanel       *transformPanel_;
  QLineEdit              *filterEdit_;
};

//------

class CQControlPointsTable;

class CQControlPointItem : public CQTableWidgetItem {
 public:
  enum { TYPE = QTableWidgetItem::UserType + 1 };

  CQControlPointItem(CQControlPointsTable *t, const CQIllustratorShape *shape,
                     CQIllustratorShapeControlPoint *point);

 ~CQControlPointItem();

  CQIllustratorShapeControlPoint *getPoint() const { return point_; }

  CQControlPointItem *clone() const;

  QString getString();

  QWidget *createEditor(QWidget *parent) const;

  void setEditorData();

  void getEditorData(QString &str);

  bool sizeHint(const QStyleOptionViewItem &option, QSize &size) const;

 private:
  CQControlPointsTable           *table_;
  const CQIllustratorShape       *shape_;
  CQIllustratorShapeControlPoint *point_;
  mutable QPointer<CQPointEdit>   edit_;
};

class CQControlPointsTable : public CQTableWidget {
  Q_OBJECT

 public:
  CQControlPointsTable(CQPropertiesOptionDialog *dialog);

  CQIllustrator *getIllustrator() const;

  void clear();

  void addPoint(const CQIllustratorShape *shape, uint pos, CQIllustratorShapeControlPoint *point);

 private slots:
  void updatePointsSlot();

 private:
  CQPropertiesOptionDialog *dialog_;
};

class CQTransformPanel : public QWidget {
  Q_OBJECT

 public:
  CQTransformPanel(CQPropertiesOptionDialog *dialog);

  CQMatrix2D *getMatrix() const { return matrix_; }

  CMatrix2D getNewMatrix() const;

 private:
  CQPropertiesOptionDialog *dialog_;
  CQMatrix2D               *matrix_;
  QTabWidget               *tabWidget_;
  CQRealEdit               *translateXEdit_;
  CQRealEdit               *translateYEdit_;
  CQRealEdit               *scaleXEdit_;
  CQRealEdit               *scaleYEdit_;
  CQAngleSpinBox           *rotateAngleEdit_;
  CQRealEdit               *skewXEdit_;
  CQRealEdit               *skewYEdit_;
};
