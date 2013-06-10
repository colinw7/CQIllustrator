#ifndef CUNDO_H
#define CUNDO_H

#include <sys/types.h>
#include <string>
#include <vector>

class CUndoGroup;
class CUndoData;

class CUndo {
 public:
  typedef std::vector<CUndoGroup *> GroupList;

 public:
  CUndo();

  virtual ~CUndo();

  virtual bool isInGroup() const;

  virtual bool startGroup();
  virtual bool endGroup();

  virtual bool addUndo(CUndoData *data);

  virtual bool undoAll();
  virtual bool redoAll();

  virtual bool undo(uint n=1);
  virtual bool redo(uint n=1);

  virtual void clear();

  virtual bool canUndo() const;
  virtual bool canRedo() const;

  bool locked() const { return locked_; }

  const GroupList &getUndoList() { return undo_list_; }
  const GroupList &getRedoList() { return redo_list_; }

  CUndoGroup *getCurrentGroup() const { return undo_group_; }

 protected:
  void lock  ();
  void unlock();

 private:
  CUndo(const CUndo &rhs);
  CUndo &operator=(const CUndo &rhs);

 protected:
  GroupList   undo_list_;
  GroupList   redo_list_;
  CUndoGroup *undo_group_;
  int         depth_;
  bool        locked_;
};

class CUndoGroup {
 public:
  typedef std::vector<CUndoData *> DataList;

 public:
  CUndoGroup(CUndo *undo);
 ~CUndoGroup();

  void addUndo(CUndoData *data);

  bool undo();
  bool redo();

  virtual std::string getDesc() const;

  void setDesc(const std::string &str) { desc_ = str; }

  const DataList &getDataList() const { return data_list_; }

 private:
  CUndoGroup(const CUndoGroup &rhs);
  CUndoGroup &operator=(const CUndoGroup &rhs);

 private:
  CUndo       *undo_;
  std::string  desc_;
  DataList     data_list_;
};

class CUndoData {
 public:
  enum State {
    UNDO_STATE,
    REDO_STATE
  };

 public:
  CUndoData();

  virtual ~CUndoData();

  virtual bool exec() = 0;

  virtual std::string getDesc() const { return ""; }

  CUndoGroup *getGroup() const { return group_; }

  void setGroup(CUndoGroup *group) { group_ = group; }

  State getState() const { return state_; }

  void setState(State state) { state_ = state; }

  static void execUndoFunc(CUndoData *data);
  static void execRedoFunc(CUndoData *data);

  static bool setError(bool flag);
  static bool isError();

 private:
  CUndoData(const CUndoData &rhs);
  CUndoData &operator=(const CUndoData &rhs);

 private:
  CUndoGroup *group_;
  State       state_;
};

#endif
