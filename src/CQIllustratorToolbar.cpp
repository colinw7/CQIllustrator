#include <CQIllustratorToolbar.h>
#include <CQIllustrator.h>
#include <CQIllustratorMode.h>

#include <QPushButton>
#include <QLabel>
#include <QToolButton>
#include <QFrame>
#include <QHBoxLayout>
#include <QStyle>

#include <svg/create_edit_svg.h>
#include <svg/edit_create_svg.h>

CQIllustratorToolbar::
CQIllustratorToolbar(CQIllustratorMode *mode) :
 QWidget(0), mode_(mode)
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
  modeButton->setObjectName("mode");

  connect(modeButton, SIGNAL(clicked()), this, SLOT(modeSlot()));

  modeButton->setFocusPolicy(Qt::NoFocus);

  modeButton->setIcon(getIcon());

  buttonLayout->addWidget(modeButton);

  if (mode_->isCreateMode()) {
    int is = style()->pixelMetric(QStyle::PM_LargeIconSize);

    createEditButton_ = new QToolButton;
    createEditButton_->setObjectName("create_edit");
    createEditButton_->setIcon(CQPixmapCacheInst->getIcon("CREATE_EDIT"));

    createEditButton_->setAutoRaise(true);
    createEditButton_->setIconSize(QSize(is, is/2));
    createEditButton_->setFixedSize(QSize(is + 4, is/2 + 4));

    createEditButton_->setToolTip("Switch between create or edit modes");

    connect(createEditButton_, SIGNAL(clicked()), this, SLOT(createEditSlot()));

    buttonLayout->addWidget(createEditButton_);
  }

  layout->addLayout(buttonLayout);

  QFrame *frame = new QFrame;
  frame->setObjectName("frame");

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
  if (mode_->getEditMode() == CQIllustratorMode::EditMode::CREATE) {
    mode_->setEditMode(CQIllustratorMode::EditMode::EDIT);

    createEditButton_->setIcon(CQPixmapCacheInst->getIcon("EDIT_CREATE"));
  }
  else {
    mode_->setEditMode(CQIllustratorMode::EditMode::CREATE);

    createEditButton_->setIcon(CQPixmapCacheInst->getIcon("CREATE_EDIT"));
  }
}
