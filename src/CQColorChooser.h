#ifndef CQColorChooser_H
#define CQColorChooser_H

#include <QWidget>
#include <CRGBA.h>

class QLineEdit;
class QPushButton;
class QToolButton;
class QLabel;
class CQAlphaButton;

class CQColorChooser : public QWidget {
  Q_OBJECT

  Q_PROPERTY(QColor  color     READ color     WRITE setColor    )
  Q_PROPERTY(QString colorName READ colorName WRITE setColorName)

 public:
  enum Style {
    Text        = (1<<0),
    ColorButton = (1<<1),
    ColorLabel  = (1<<2),
    ImageButton = (1<<3),
    AlphaButton = (1<<4)
  };

 public:
  CQColorChooser(QWidget *parent=0);

  CQColorChooser(uint styles, QWidget *parent=0);

  bool getEditable() const { return editable_; }
  void setEditable(bool editable=true);

  uint getStyles() const { return styles_; }
  void setStyles(uint styles);

  const QColor &color() const;
  void setColor(const QColor &color, double alpha=1.0);

  void setRGBA(const CRGBA &rgba);
  CRGBA getRGBA() const;

  QString colorName() const;
  void setColorName(const QString &colorName, double alpha=1.0);

  double getAlpha() const;
  void setAlpha(double alpha);

  QLineEdit *getTextWidget() const { return cedit_; }

 private:
  void init();
  void update();
  void changeColor();
  void applyColor();

 private slots:
  void textColorChanged();
  void imageButtonClicked();
  void colorButtonClicked();
  void alphaButtonChanged();

 signals:
  void colorChanged(const QColor &color);
  void colorChanged(const QString &colorName);

  void colorApplied(const QColor &color);
  void colorApplied(const QString &colorName);

 private:
  uint           styles_;
  bool           editable_;
  bool           has_alpha_;
  QColor         color_;
  QString        colorName_;
  double         alpha_;
  QLineEdit     *cedit_;   // text color name
  QToolButton   *cbutton_; // click color button
  QLabel        *clabel_;  // static color button
  CQAlphaButton *alphab_;  // alpha slider
  QToolButton   *button_;  // image click button
};

#endif
