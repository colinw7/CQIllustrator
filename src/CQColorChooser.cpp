#include <CQColorChooser.h>
#include <CQAlphaButton.h>
#include <CRGBName.h>

#include <CQUtil.h>
#include <color_dialog.xpm>

#include <QLineEdit>
#include <QPushButton>
#include <QToolButton>
#include <QLabel>
#include <QHBoxLayout>
#include <QColorDialog>

CQColorChooser::
CQColorChooser(QWidget *parent) :
 QWidget(parent), styles_(Text), editable_(true)
{
  init();
}

CQColorChooser::
CQColorChooser(uint styles, QWidget *parent) :
 QWidget(parent), styles_(styles), editable_(true)
{
  init();
}

void
CQColorChooser::
init()
{
  color_     = QColor(1,2,3);
  alpha_     = 0.456;
  colorName_ = "abcdef";

  //-----

  QHBoxLayout *layout = new QHBoxLayout(this);

  layout->setMargin (2);
  layout->setSpacing(2);

  cedit_   = new QLineEdit  (this); // text color
  cbutton_ = new QToolButton(this); // click color button
  clabel_  = new QLabel     (this); // static color button
  button_  = new QToolButton(this); // image click button
  alphab_  = new CQAlphaButton(this);

  cbutton_->setFixedSize(QSize(24,24));
  clabel_ ->setFixedSize(QSize(24,24));
  button_ ->setFixedSize(QSize(24,24));

  button_->setIcon(QIcon(QPixmap((const char **) color_dialog_data)));

  layout->addWidget(cedit_  );
  layout->addWidget(cbutton_);
  layout->addWidget(clabel_ );
  layout->addWidget(alphab_ );
  layout->addWidget(button_ );
  layout->addStretch();

  cedit_  ->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
  cbutton_->setSizePolicy(QSizePolicy::Fixed    , QSizePolicy::Fixed);
  clabel_ ->setSizePolicy(QSizePolicy::Fixed    , QSizePolicy::Fixed);
  button_ ->setSizePolicy(QSizePolicy::Fixed    , QSizePolicy::Fixed);

  connect(cedit_  , SIGNAL(editingFinished()), this, SLOT(textColorChanged()));
  connect(cbutton_, SIGNAL(clicked()), this, SLOT(colorButtonClicked()));
  connect(button_ , SIGNAL(clicked()), this, SLOT(imageButtonClicked()));
  connect(alphab_ , SIGNAL(valueChanged()), this, SLOT(alphaButtonChanged()));

  update();

  setColor(color_, alpha_);

  setFocusProxy(cedit_);
}

void
CQColorChooser::
setEditable(bool editable)
{
  editable_ = editable;

  update();
}

void
CQColorChooser::
setStyles(uint styles)
{
  styles_ = styles;

  update();
}

void
CQColorChooser::
update()
{
  cedit_  ->setVisible(styles_ & Text       );
  cbutton_->setVisible(styles_ & ColorButton);
  clabel_ ->setVisible(styles_ & ColorLabel );
  button_ ->setVisible(styles_ & ImageButton);
  alphab_ ->setVisible(styles_ & AlphaButton);

  cedit_->setReadOnly(! editable_);
}

void
CQColorChooser::
setColor(const QColor &color, double alpha)
{
  if (! color.isValid())
    return;

  QColor color1(color);

  color1.setAlpha(255);

  if (color_ == color1 && alpha_ == alpha_)
    return;

  color_     = color1;
  colorName_ = color_.name();

  cedit_->setText(colorName_);

  QPixmap pixmap(24, 24);

  pixmap.fill(color_);

  cbutton_->setIcon(QIcon(QPixmap(pixmap)));

  CQUtil::setBackground(clabel_, color_);

  alpha_ = alpha;

  alphab_->setColor(color1);
  alphab_->setAlpha(alpha_);
}

void
CQColorChooser::
setRGBA(const CRGBA &rgba)
{
  setColor(CQUtil::rgbaToColor(rgba));
}

CRGBA
CQColorChooser::
getRGBA() const
{
  return CQUtil::colorToRGBA(color());
}

void
CQColorChooser::
setColorName(const QString &colorName, double alpha)
{
  QColor color = CQUtil::rgbaToColor(CRGBName::toRGBA(colorName.toStdString()));

  if (! color.isValid())
    return;

  QColor color1(color);

  color1.setAlpha(255);

  if (color_ == color1 && alpha_ == alpha_)
    return;

  color_     = color1;
  colorName_ = colorName;

  cedit_->setText(colorName_);

  QPixmap pixmap(24, 24);

  pixmap.fill(color_);

  cbutton_->setIcon(QIcon(QPixmap(pixmap)));

  CQUtil::setBackground(clabel_, color_);

  alpha_ = alpha;

  alphab_->setAlpha(alpha_);
}

const QColor &
CQColorChooser::
color() const
{
  return color_;
}

QString
CQColorChooser::
colorName() const
{
  return colorName_;
}

void
CQColorChooser::
setAlpha(double alpha)
{
  alpha_ = alpha;

  alphab_->setAlpha(alpha_);
}

double
CQColorChooser::
getAlpha() const
{
  return alpha_;
}

void
CQColorChooser::
textColorChanged()
{
  setColorName(cedit_->text(), alpha_);

  changeColor();
}

void
CQColorChooser::
imageButtonClicked()
{
  QColor color = QColorDialog::getColor(color_, this);

  setColor(color, alpha_);

  changeColor();
}

void
CQColorChooser::
colorButtonClicked()
{
  QColor color = QColorDialog::getColor(color_, this);

  setColor(color, alpha_);

  changeColor();
}

void
CQColorChooser::
alphaButtonChanged()
{
  setAlpha(alphab_->getAlpha());

  changeColor();
}

void
CQColorChooser::
changeColor()
{
  QColor color(color_);

  color_.setAlpha(255*alpha_);

  emit colorChanged(color);
  emit colorChanged(colorName_);
}

void
CQColorChooser::
applyColor()
{
  QColor color(color_);

  color_.setAlpha(255*alpha_);

  emit colorApplied(color);
  emit colorApplied(colorName_);
}
