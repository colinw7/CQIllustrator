#ifndef CQFontOptionTool_H
#define CQFontOptionTool_H

#include <CQOptionToolSet.h>

class CQFontOptionDialog;
class CQFontChooser;
class QPaintEvent;

class CQFontOptionTool : public CQOptionTool {
  Q_OBJECT

 public:
  CQFontOptionTool(QWidget *parent=nullptr);

  CQOptionToolDialog *getDialog() override;

  void setFont(QFont font);

  QFont getFont() const;

 signals:
  void valueChanged(const QFont &);

 private:
  CQFontOptionDialog *dialog_ { 0 };
};

#endif
