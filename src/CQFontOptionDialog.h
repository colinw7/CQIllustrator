#ifndef CQFontOptionDialog_H
#define CQFontOptionDialog_H

#include <CQOptionToolSet.h>

class CQFontOptionTool;
class CQFontChooser;

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

#endif
