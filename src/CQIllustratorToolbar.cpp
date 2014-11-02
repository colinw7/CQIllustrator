#include <CQIllustratorToolbar.h>
#include <CQIllustrator.h>
#include <CQIllustratorMode.h>

#include <QPushButton>
#include <QLabel>
#include <QToolButton>
#include <QFrame>
#include <QHBoxLayout>

#include <xpm/CREATE_edit.xpm>
#include <xpm/create_EDIT.xpm>

CQIllustratorToolbar::
CQIllustratorToolbar(CQIllustratorMode *mode) :
 QWidget(NULL), mode_(mode)
{
}

void
CQIllustratorToolbar::
init()
{
  QHBoxLayout *layout = new QHBoxLayout(this);
  layout->setMargin(0); layout->setSpacing(2);

  QVBoxLayout *buttonLayout = new QVBoxLayout;
  buttonLayout->setMargin(0); buttonLayout->setSpacing(0);

  QPushButton *modeButton = new QPushButton;

  connect(modeButton, SIGNAL(clicked()), this, SLOT(modeSlot()));

  modeButton->setFocusPolicy(Qt::NoFocus);

  modeButton->setIcon(getIcon());

  buttonLayout->addWidget(modeButton);

  if (mode_->isCreateMode()) {
    createEditButton_ = new QToolButton;

    createEditButton_->setIcon(QIcon(QPixmap(CREATE_edit_data)));

    createEditButton_->setAutoRaise(true);
    createEditButton_->setFixedSize(QSize(20,12));

    createEditButton_->setToolTip("Switch between create or edit modes");

    connect(createEditButton_, SIGNAL(clicked()), this, SLOT(createEditSlot()));

    buttonLayout->addWidget(createEditButton_);
  }

  layout->addLayout(buttonLayout);

  QFrame *frame = new QFrame;

  frame->setFixedWidth(4);

  frame->setFrameShape(QFrame::VLine);
  frame->setFrameShadow(QFrame::Raised);

  layout->addWidget(frame);

  addWidgets();

  layout->addStretch(1);
}

void
CQIllustratorToolbar::
addWidgets()
{
}

void
CQIllustratorToolbar::
setSelectedShape(const CQIllustratorShape *)
{
}

void
CQIllustratorToolbar::
setSelectedShapePoint(const CQIllustratorShape *, const CQIllustratorShapeControlPoint *)
{
}

void
CQIllustratorToolbar::
modeSlot()
{
  mode_->getIllustrator()->setCanvasFocus();
}

void
CQIllustratorToolbar::
createEditSlot()
{
  if (mode_->getEditMode() == CQIllustratorMode::CREATE_MODE) {
    mode_->setEditMode(CQIllustratorMode::EDIT_MODE);

    createEditButton_->setIcon(QIcon(QPixmap(create_EDIT_data)));
  }
  else {
    mode_->setEditMode(CQIllustratorMode::CREATE_MODE);

    createEditButton_->setIcon(QIcon(QPixmap(CREATE_edit_data)));
  }
}
