#include <CQIllustratorShapeFilter.h>
#include <CQIllustratorShapeGaussianFilter.h>

CQIllustratorShapeFilterMgr *
CQIllustratorShapeFilterMgr::
getInstance()
{
  static CQIllustratorShapeFilterMgr *instance;

  if (! instance)
    instance = new CQIllustratorShapeFilterMgr;

  return instance;
}

CQIllustratorShapeFilterMgr::
CQIllustratorShapeFilterMgr()
{
  addFilter(new CQIllustratorShapeGaussianFilter(2.0));
}

CQIllustratorShapeFilterMgr::
~CQIllustratorShapeFilterMgr()
{
}

CQIllustratorShapeFilter *
CQIllustratorShapeFilterMgr::
getFilter(int id)
{
  FilterMap::const_iterator p = filterMap_.find(id);

  if (p == filterMap_.end())
    return nullptr;

  return (*p).second;
}

void
CQIllustratorShapeFilterMgr::
addFilter(CQIllustratorShapeFilter *filter)
{
  filter->setId(++id_);

  filterMap_[id_] = filter;
}

//---

CQIllustratorShapeFilter::
CQIllustratorShapeFilter()
{
}

CQIllustratorShapeFilter::
~CQIllustratorShapeFilter()
{
}
