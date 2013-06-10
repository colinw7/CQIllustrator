#include <CUndo.h>

#include <CFuncs.h>
#include <cassert>
#include <climits>
#include <algorithm>

using std::string;

CUndo::
CUndo() :
 undo_group_(0), depth_(0), locked_(false)
{
}

CUndo::
~CUndo()
{
  std::for_each(undo_list_.begin(), undo_list_.end(), CDeletePointer());
  std::for_each(redo_list_.begin(), redo_list_.end(), CDeletePointer());
}

bool
CUndo::
isInGroup() const
{
  return (depth_ > 0);
}

bool
CUndo::
startGroup()
{
  if (locked())
    return false;

  if (depth_ == 0) {
    assert(undo_group_ == NULL);

    undo_group_ = new CUndoGroup(this);
  }

  ++depth_;

  return true;
}

bool
CUndo::
endGroup()
{
  if (locked())
    return false;

  assert(depth_ > 0);

  --depth_;

  if (depth_ == 0) {
    undo_list_.push_back(undo_group_);

    undo_group_ = NULL;
  }

  return true;
}

bool
CUndo::
addUndo(CUndoData *data)
{
  if (locked())
    return false;

  std::for_each(redo_list_.begin(), redo_list_.end(), CDeletePointer());

  redo_list_.clear();

  if (undo_group_ == NULL) {
    startGroup();

    addUndo(data);

    endGroup();
  }
  else
    undo_group_->addUndo(data);

  return true;
}

bool
CUndo::
undoAll()
{
  return undo(INT_MAX);
}

bool
CUndo::
redoAll()
{
  return redo(INT_MAX);
}

bool
CUndo::
undo(uint n)
{
  bool flag = true;

  for (uint i = 0; i < n; ++i) {
    if (undo_list_.empty())
      return false;

    CUndoGroup *undo_group = undo_list_.back();

    undo_list_.pop_back();

    lock();

    if (! undo_group->undo())
      flag = false;

    unlock();

    redo_list_.push_back(undo_group);
  }

  return flag;
}

bool
CUndo::
redo(uint n)
{
  bool flag = true;

  for (uint i = 0; i < n; ++i) {
    if (redo_list_.empty())
      return false;

    CUndoGroup *undo_group = redo_list_.back();

    redo_list_.pop_back();

    lock();

    if (! undo_group->redo())
      flag = false;

    unlock();

    undo_list_.push_back(undo_group);
  }

  return flag;
}

void
CUndo::
clear()
{
  std::for_each(undo_list_.begin(), undo_list_.end(), CDeletePointer());
  std::for_each(redo_list_.begin(), redo_list_.end(), CDeletePointer());

  undo_list_.clear();
  redo_list_.clear();
}

bool
CUndo::
canUndo() const
{
  return (! undo_list_.empty());
}

bool
CUndo::
canRedo() const
{
  return (! redo_list_.empty());
}

void
CUndo::
lock()
{
  assert(! locked_);

  locked_ = true;
}

void
CUndo::
unlock()
{
  assert(locked_);

  locked_ = false;
}

//--------------------

CUndoGroup::
CUndoGroup(CUndo *) :
 desc_("")
{
}

CUndoGroup::
~CUndoGroup()
{
  std::for_each(data_list_.begin(), data_list_.end(), CDeletePointer());
}

void
CUndoGroup::
addUndo(CUndoData *data)
{
  data_list_.push_back(data);

  data->setGroup(this);
}

string
CUndoGroup::
getDesc() const
{
  if (desc_ != "") return desc_;

  if (data_list_.empty())
    return "";
  else
    return data_list_.front()->getDesc();
}

bool
CUndoGroup::
undo()
{
  CUndoData::setError(false);

  std::for_each(data_list_.rbegin(), data_list_.rend(), &CUndoData::execUndoFunc);

  return CUndoData::isError();
}

bool
CUndoGroup::
redo()
{
  CUndoData::setError(false);

  std::for_each(data_list_.begin(), data_list_.end(), &CUndoData::execRedoFunc);

  return CUndoData::isError();
}

//--------------------

CUndoData::
CUndoData() :
 group_(NULL), state_(UNDO_STATE)
{
}

CUndoData::
~CUndoData()
{
}

void
CUndoData::
execUndoFunc(CUndoData *data)
{
  data->setState(CUndoData::UNDO_STATE);

  CUndoData::setError(data->exec());
}

void
CUndoData::
execRedoFunc(CUndoData *data)
{
  data->setState(CUndoData::REDO_STATE);

  CUndoData::setError(data->exec());
}

bool
CUndoData::
setError(bool flag)
{
  static bool error;

  std::swap(flag, error);

  return flag;
}


bool
CUndoData::
isError()
{
  bool error;

  setError(error = setError(false));

  return error;
}
