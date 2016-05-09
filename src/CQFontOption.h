#include <CQOptionToolSet.h>

class CQFontOptionDialog;
class CQFontChooser;
class QPaintEvent;

class CQFontOptionTool : public CQOptionTool {
  Q_OBJECT

 public:
  CQFontOptionTool(QWidget *parent=0);

  CQOptionToolDialog *getDialog();

  void setFont(QFont font);

  QFont getFont() const;

 signals:
  void valueChanged(const QFont &);

 private:
  CQFontOptionDialog *dialog_ { 0 };
};

//------

class CQFontOptionDialog : public CQOptionToolDialog {
  Q_OBJECT

 public:
  CQFontOptionDialog(CQFontOptionTool *tool);

  void setFont(QFont font);

  QFont getFont() const { return font_; }

 private:
  void initWidgets();
  void updateWidgets();

 private slots:
  void fontSlot(const QFont &font);

 signals:
  void valueChanged(const QFont &);

 private:
  CQFontOptionTool *tool_ { 0 };
  QFont             font_;
  CQFontChooser    *fontChooser_ { 0 };
};
