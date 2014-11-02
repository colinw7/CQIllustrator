#include <CQOptionToolSet.h>

#include <QLabel>
#include <QHBoxLayout>

#include <menu.xpm>
#include <menu_active.xpm>

CQOptionToolSet::
CQOptionToolSet(QWidget *parent) :
 QWidget(parent), currentTool_(NULL)
{
  QHBoxLayout *layout = new QHBoxLayout(this);
  layout->setMargin(0); layout->setSpacing(0);

  dialogArea_ = new CQOptionToolDialogArea(this);

  dialogArea_->hide();
}

void
CQOptionToolSet::
setPopupArea(QWidget *popupArea)
{
  popdown();

  popupArea_ = popupArea;

  dialogArea_->setParent(popupArea_);
}

void
CQOptionToolSet::
addOption(CQOptionTool *tool)
{
  CQOptionToolArea *area = new CQOptionToolArea(this, tool);

  tool->setArea(area);

  layout()->addWidget(area);

  toolList_.push_back(area);
}

void
CQOptionToolSet::
popup(CQOptionToolArea *tool_area)
{
  CQOptionToolDialog *dialog = tool_area->getTool()->getDialog();

  if (tool_area != currentTool_) {
    popdown();

    if (currentTool_) {
      CQOptionToolMenu *menu = currentTool_->getMenuButton();

      menu->setChecked(false);
    }

    dialogArea_->setWidget(dialog);

    currentTool_ = tool_area;
  }

  tool_area->getTool()->popup(dialogArea_, popupArea_);
}

void
CQOptionToolSet::
popdown(CQOptionToolArea *tool_area)
{
  if (tool_area == currentTool_)
    popdown();
}

void
CQOptionToolSet::
popdown()
{
  if (currentTool_) {
    currentTool_->getTool()->popdown(dialogArea_);

    CQOptionToolMenu *menu = currentTool_->getMenuButton();

    menu->setChecked(false);
  }
}

//------

CQOptionToolArea::
CQOptionToolArea(CQOptionToolSet *toolset, CQOptionTool *tool) :
 QFrame(NULL), toolset_(toolset), tool_(tool)
{
  setFrameStyle(QFrame::Panel | QFrame::Sunken);
  setLineWidth(1);

  QHBoxLayout *layout = new QHBoxLayout(this);
  layout->setMargin(0); layout->setSpacing(0);

  menuButton_ = new CQOptionToolMenu;

  layout->addWidget(tool);
  layout->setAlignment(tool, Qt::AlignBottom);

  layout->addWidget(menuButton_);
  layout->setAlignment(menuButton_, Qt::AlignBottom);

  connect(menuButton_, SIGNAL(clicked(bool)), this, SLOT(menuPressed(bool)));
}

void
CQOptionToolArea::
menuPressed(bool show)
{
  if (show)
    toolset_->popup(this);
  else
    toolset_->popdown(this);
}

//------

CQOptionToolMenu::
CQOptionToolMenu()
{
  off_pixmap_ = QPixmap((const char **) menu_data);
  on_pixmap_  = QPixmap((const char **) menu_active_data);

  setIcon(QIcon(off_pixmap_));

  setCheckable(true);

  connect(this, SIGNAL(toggled(bool)), this, SLOT(updatePixmap(bool)));

  setFixedSize(QSize(10,20));
}

void
CQOptionToolMenu::
updatePixmap(bool checked)
{
  setIcon(QIcon(checked ? on_pixmap_ : off_pixmap_));
}

//------

CQOptionToolDialogArea::
CQOptionToolDialogArea(CQOptionToolSet *set) :
 CQWinWidget(), set_(set), w_(NULL)
{
  setResizable(false);

  connect(this, SIGNAL(closed()), this, SLOT(closedSlot()));

}

void
CQOptionToolDialogArea::
setWidget(QWidget *w)
{
  setChild(w);

  w_ = w;
}

void
CQOptionToolDialogArea::
updateSize()
{
  fitChild();
}

void
CQOptionToolDialogArea::
closedSlot()
{
  set_->popdown();
}

//------

CQOptionTool::
CQOptionTool() :
 area_(NULL)
{
}

void
CQOptionTool::
setArea(CQOptionToolArea *area)
{
  area_ = area;
}

void
CQOptionTool::
popup(CQOptionToolDialogArea *dialogArea, QWidget *popupArea)
{
  emit poppingUp();

  dialogArea->updateSize();

  dialogArea->show();

  // move dialog area bottom right to tool area top right in popup area

  QPoint rt = popupArea->mapFromGlobal(area_->mapToGlobal(area_->rect().topRight()));

  int w = dialogArea->width () + 4;
  int h = dialogArea->height() + 4;

  int x = rt.x() - w;
  int y = rt.y() - h;

  if (x > popupArea->width ()) x = popupArea->width () - w;
  if (x < 0                  ) x = 0;
  if (y > popupArea->height()) y = popupArea->height() - h;
  if (y < 0                  ) y = 0;

  dialogArea->move(x, y);

  emit poppedUp();
}

void
CQOptionTool::
popdown(CQOptionToolDialogArea *area)
{
  emit poppingDown();

  area->hide();

  emit poppedDown();
}

//------

CQOptionToolDialog::
CQOptionToolDialog()
{
}
