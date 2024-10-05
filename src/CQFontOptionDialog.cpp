#include <CQFontOptionDialog.h>
#include <CQFontOptionTool.h>
#include <CQFontChooser.h>

#include <QGridLayout>
#include <QLabel>

CQFontOptionDialog::
CQFontOptionDialog(CQFontOptionTool *tool) :
 CQOptionToolDialog(), tool_(tool), fontChooser_(nullptr)
{
  initWidgets();
}

void
CQFontOptionDialog::
setFont(QFont font)
{
  font_ = font;

  updateWidgets();
}

void
CQFontOptionDialog::
initWidgets()
{
  QGridLayout *gridLayout = new QGridLayout(this);

  gridLayout->addWidget(new QLabel("Font"), 0, 0);

  fontChooser_ = new CQFontChooser;

  connect(fontChooser_, SIGNAL(fontChanged(const QFont &)),
          this, SLOT(fontSlot(const QFont &)));

  gridLayout->addWidget(fontChooser_, 0, 1);
}

void
CQFontOptionDialog::
updateWidgets()
{
  fontChooser_->setFont(font_);
}

void
CQFontOptionDialog::
fontSlot(const QFont &font)
{
  font_ = font;

  tool_->update();

  emit valueChanged(font_);
}
