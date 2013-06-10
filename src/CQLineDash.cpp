#include <CQLineDash.h>
#include <CQUtil.h>
#include <QContextMenuEvent>
#include <QMenu>

#include <pixmaps/dot1.xpm>
#include <pixmaps/dash1.xpm>

#define IMAGE_DATA(I) I, sizeof(I)/sizeof(char *)

CQLineDash::
CQLineDash(QWidget *parent) :
 QLineEdit(parent)
{
  setToolTip("Line Dash\n(List of Dash Lengths)");

  connect(this, SIGNAL(editingFinished()), this, SLOT(dashChangedSlot()));

  CImagePtr image1 = CImageMgrInst->createImage(CImageXPMSrc(IMAGE_DATA(dot1_data)));
  CImagePtr image2 = CImageMgrInst->createImage(CImageXPMSrc(IMAGE_DATA(dash1_data)));

  menu_ = new QMenu;

  menu_->addAction(CQUtil::imageToIcon(image1), "1, 8")->setIconVisibleInMenu(true);
  menu_->addAction(CQUtil::imageToIcon(image2), "4, 4")->setIconVisibleInMenu(true);

  connect(menu_, SIGNAL(triggered(QAction *)), this, SLOT(menuItemActivated(QAction *)));
}

void
CQLineDash::
setLineDash(const CLineDash &dash)
{
  dash_ = dash;

  setText(dash_.toString().c_str());
}

CLineDash
CQLineDash::
getLineDash() const
{
  return dash_;
}

void
CQLineDash::
dashChangedSlot()
{
  CLineDash dash;

  QString str = text();

  if (dash.fromString(str.toStdString())) {
    dash_ = dash;

    emit valueChanged(dash);
  }
}

void
CQLineDash::
contextMenuEvent(QContextMenuEvent *e)
{
  menu_->exec(e->globalPos());
}

void
CQLineDash::
menuItemActivated(QAction *action)
{
  setText(action->text());

  dashChangedSlot();
}
