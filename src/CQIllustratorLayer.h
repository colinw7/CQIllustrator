#ifndef CQIllustratorLayer_H
#define CQIllustratorLayer_H

#include <map>
#include <list>

#include <QObject>
#include <CQIllustratorData.h>

class CQIllustrator;
class CQIllustratorShape;
class CQIllustratorLayerStack;

class CQIllustratorLayer : public QObject {
  Q_OBJECT

 public:
  typedef CQIllustratorData::ChangeType ChangeType;

 private:
  CQIllustratorLayer(CQIllustratorLayerStack *stack, uint id);
 ~CQIllustratorLayer();

  friend class CQIllustratorLayerStack;

 public:
  const std::string &getName() const { return name_; }

  CQIllustratorData *getData() const { return data_; }

  void setName(const std::string &name);

 signals:
  void objectPostModify(CQIllustratorShape *, ChangeType);

 private:
  CQIllustratorLayerStack *stack_;
  uint                     id_;
  std::string              name_;
  CQIllustratorData       *data_;
};

class CQIllustratorLayerStack : public QObject {
  Q_OBJECT

 public:
  typedef CQIllustratorData::ChangeType ChangeType;
  typedef std::list<uint>               LayerStack;

 public:
  CQIllustratorLayerStack(CQIllustrator *illustrator);

  CQIllustrator *getIllustrator() { return illustrator_; }

  const LayerStack &getLayerStack() const { return layerStack_; }

  uint getCurrent() const;

  CQIllustratorLayer *getCurrentLayer() const;

  CQIllustratorData *getCurrentData() const;

  uint addLayer(const std::string &name="");

  void deleteLayer(uint id);

  CQIllustratorLayer *getLayer(uint id) const;

  void setLayer(const std::string &name);

 signals:
  void objectPostModify(CQIllustratorShape *, ChangeType);

  void layerChanged();

  void layersModified();

 private:
  typedef std::map<uint,CQIllustratorLayer *> LayerMap;

  CQIllustrator *illustrator_;
  uint           id_;
  uint           current_;
  LayerMap       layerMap_;
  LayerStack     layerStack_;
};

#endif
