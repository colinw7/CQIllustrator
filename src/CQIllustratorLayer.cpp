#include <CQIllustratorLayer.h>
#include <CQIllustratorData.h>
#include <CStrUtil.h>
#include <QString>

CQIllustratorLayer::
CQIllustratorLayer(CQIllustratorLayerStack *stack, uint id) :
 stack_(stack), id_(id)
{
  name_ = QString("Layer%1").arg(id_).toStdString();

  data_ = new CQIllustratorData(stack->getIllustrator());

  connect(data_, SIGNAL(objectPostModify(CQIllustratorShape *, ChangeType)),
          this, SIGNAL(objectPostModify(CQIllustratorShape *, ChangeType)));
}

CQIllustratorLayer::
~CQIllustratorLayer()
{
  delete data_;
}

void
CQIllustratorLayer::
setName(const std::string &name)
{
  name_ = name;
}

//--------

CQIllustratorLayerStack::
CQIllustratorLayerStack(CQIllustrator *illustrator) :
 illustrator_(illustrator), id_(0), current_(0)
{
  addLayer();
}

uint
CQIllustratorLayerStack::
getCurrent() const
{
  return current_;
}

CQIllustratorLayer *
CQIllustratorLayerStack::
getCurrentLayer() const
{
  return getLayer(getCurrent());
}

CQIllustratorData *
CQIllustratorLayerStack::
getCurrentData() const
{
  return getCurrentLayer()->getData();
}

uint
CQIllustratorLayerStack::
addLayer(const std::string &name)
{
  ++id_;

  CQIllustratorLayer *layer = new CQIllustratorLayer(this, id_);

  connect(layer, SIGNAL(objectPostModify(CQIllustratorShape *, ChangeType)),
          this, SIGNAL(objectPostModify(CQIllustratorShape *, ChangeType)));

  if (name != "")
    layer->setName(name);

  layerMap_[id_] = layer;

  layerStack_.push_back(id_);

  if (! current_)
    current_ = id_;

  emit layersModified();

  return id_;
}

void
CQIllustratorLayerStack::
deleteLayer(uint id)
{
  assert(layerStack_.size() > 1);

  LayerMap::iterator p = layerMap_.find(id);
  assert(p != layerMap_.end());

  CQIllustratorLayer *layer = (*p).second;

  layerMap_.erase(p);

  layerStack_.remove(id);

  delete layer;

  emit layersModified();
}

CQIllustratorLayer *
CQIllustratorLayerStack::
getLayer(uint id) const
{
  LayerMap::const_iterator p = layerMap_.find(id);
  assert(p != layerMap_.end());

  return (*p).second;
}

void
CQIllustratorLayerStack::
setLayer(const std::string &name)
{
  LayerStack::const_iterator p1, p2;

  for (p1 = layerStack_.begin(), p2 = layerStack_.end(); p1 != p2; ++p1) {
    uint id = *p1;

    CQIllustratorLayer *layer = getLayer(id);

    if (layer->getName() == name) {
      current_ = id;

      emit layerChanged();

      return;
    }
  }
}
