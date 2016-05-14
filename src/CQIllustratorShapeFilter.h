#ifndef CQIllustratorShapeFilter_H
#define CQIllustratorShapeFilter_H

#include <map>

#define CQIllustratorShapeFilterMgrInst CQIllustratorShapeFilterMgr::getInstance()

class CQIllustratorShapeFilter;

class CQIllustratorShapeFilterMgr {
 public:
  static CQIllustratorShapeFilterMgr *getInstance();

  CQIllustratorShapeFilter *getFilter(int id);

  void addFilter(CQIllustratorShapeFilter *filter);

 private:
  CQIllustratorShapeFilterMgr();
 ~CQIllustratorShapeFilterMgr();

 private:
  typedef std::map<int,CQIllustratorShapeFilter *> FilterMap;

  FilterMap filterMap_;
  int       id_ { 0 };
};

//------

class CQIllustratorShapeFilter {
 public:
  CQIllustratorShapeFilter();

  virtual ~CQIllustratorShapeFilter();

  int getId() const { return id_; }

 private:
  friend class CQIllustratorShapeFilterMgr;

  void setId(int id) { id_ = id; }

 protected:
  int id_ { 0 };
};

#endif
