#include <CQLayerOption.h>
#include <CQIllustrator.h>
#include <CQImageButton.h>

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QListWidget>

#include <xpm/layer_up.xpm>
#include <xpm/layer_down.xpm>
#include <xpm/layer_add.xpm>
#include <xpm/layer_remove.xpm>

CQLayerOptionTool::
CQLayerOptionTool(CQIllustrator *illustrator) :
 CQOptionTool(), illustrator_(illustrator), dialog_(NULL)
{
  dialog_ = new CQLayerOptionDialog(this);

  QVBoxLayout *layout = new QVBoxLayout(this);
  layout->setMargin(0); layout->setSpacing(0);

  QLabel *label = new QLabel("<small><b>Layer</b></small>");

  layout->addWidget(label);

  label_ = new QLabel;

  QFontMetrics fm(font());

  int width = fm.width("ABCDEFGHIJKLM");

  label_->setMinimumWidth(width);

  layout->addWidget(label_);

  connect(dialog_, SIGNAL(valueChanged(const QString &)),
          this, SIGNAL(valueChanged(const QString &)));

  connect(this, SIGNAL(poppingUp()), this, SLOT(populateSlot()));

  //------

  updateLayer();
}

CQOptionToolDialog *
CQLayerOptionTool::
getDialog()
{
  return dialog_;
}

void
CQLayerOptionTool::
updateLayer()
{
  CQIllustratorLayerStack &layerStack = illustrator_->getLayerStack();

  CQIllustratorLayer *layer = layerStack.getCurrentLayer();

  label_->setText(layer->getName().c_str());
}

void
CQLayerOptionTool::
reloadLayers()
{
  dialog_->populate();
}

void
CQLayerOptionTool::
populateSlot()
{
  dialog_->populate();
}

//----------

CQLayerOptionDialog::
CQLayerOptionDialog(CQLayerOptionTool *tool) :
 CQOptionToolDialog(), tool_(tool)
{
  initWidgets();
}

void
CQLayerOptionDialog::
initWidgets()
{
  QVBoxLayout *layout = new QVBoxLayout(this);

  list_ = new QListWidget;

  layout->addWidget(list_);

  //list_->headerItem()->setHidden(true);

  connect(list_, SIGNAL(itemClicked(QListWidgetItem *)),
          this, SLOT(itemSelectedSlot(QListWidgetItem *)));

  QHBoxLayout *buttonLayout = new QHBoxLayout;

  CQImageButton *upButton     = new CQImageButton(up_data);
  CQImageButton *downButton   = new CQImageButton(down_data);
  CQImageButton *addButton    = new CQImageButton(plus_data);
  CQImageButton *removeButton = new CQImageButton(minus_data);

  connect(addButton, SIGNAL(clicked()), this, SLOT(addLayerSlot()));

  buttonLayout->addWidget(upButton);
  buttonLayout->addWidget(downButton);
  buttonLayout->addStretch();
  buttonLayout->addWidget(addButton);
  buttonLayout->addWidget(removeButton);
  buttonLayout->addStretch();

  layout->addLayout(buttonLayout);
}

void
CQLayerOptionDialog::
itemSelectedSlot(QListWidgetItem *item)
{
  QString name = item->text();

  CQIllustrator *illustrator = tool_->getIllustrator();

  illustrator->setLayer(name.toStdString());
}

void
CQLayerOptionDialog::
addLayerSlot()
{
  CQIllustrator *illustrator = tool_->getIllustrator();

  illustrator->addLayer();
}

void
CQLayerOptionDialog::
populate()
{
  CQIllustrator *illustrator = tool_->getIllustrator();

  list_->clear();

  const CQIllustratorLayerStack &layerStack = illustrator->getLayerStack();

  const CQIllustratorLayerStack::LayerStack &layers = layerStack.getLayerStack();

  CQIllustratorLayerStack::LayerStack::const_iterator p1, p2;

  for (p1 = layers.begin(), p2 = layers.end(); p1 != p2; ++p1) {
    uint id = *p1;

    CQIllustratorLayer *layer = layerStack.getLayer(id);

    std::string name = layer->getName();

    QListWidgetItem *item = new QListWidgetItem(name.c_str());

    list_->addItem(item);
  }
}
