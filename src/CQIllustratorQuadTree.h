#ifndef CQUAD_TREE_H
#define CQUAD_TREE_H

#include <list>
#include <cassert>
#include <sys/types.h>

// quad tree containing pointers to items of type DATA with an associated bbox of type BBOX
//
// the tree is split when the number of elements is greater than or equal to the auto
// split limit
//
// tree does not take ownership of data. The application must ensure elements are not
// deleted while in the tree and are deleted when required.
//
// DATA must support:
//   const BBOX &bbox = data->getFlatBBox();
//
// BBOX must support:
//   constructor BBOX(l, b, r, t);
//
//   T l = bbox.getLeft  ();
//   T b = bbox.getBottom();
//   T r = bbox.getRight ();
//   T t = bbox.getTop   ();
//
template<typename DATA, typename BBOX, typename T=double>
class CQIllustratorQuadTree {
 public:
  typedef std::list<DATA *> DataList;

 public:
  explicit CQIllustratorQuadTree(const BBOX &bbox=BBOX(1,1,-1,-1)) :
   bbox_(bbox) {
  }

 ~CQIllustratorQuadTree() {
    delete bl_tree_;
    delete br_tree_;
    delete tl_tree_;
    delete tr_tree_;
  }

 private:
  CQIllustratorQuadTree(CQIllustratorQuadTree *parent, const BBOX &bbox) :
   parent_(parent), bbox_(bbox) {
  }

 public:
  // reset quad tree
  void reset() {
    dataList_.clear();

    delete bl_tree_; bl_tree_ = 0;
    delete br_tree_; br_tree_ = 0;
    delete tl_tree_; tl_tree_ = 0;
    delete tr_tree_; tr_tree_ = 0;
  }

  // get bounding box
  const BBOX &getBBox() const { return bbox_; }

  // get data list
  const DataList &getDataList() const { return dataList_; }

  // get auto split limit
  static uint getAutoSplitLimit() {
    return *getAutoSplitLimitP();
  }

  // set auto split limit
  static void setAutoSplitLimit(uint limit) {
    *getAutoSplitLimitP() = limit;
  }

  // get auto split limit
  static uint getMinTreeSize() {
    return *getMinTreeSizeP();
  }

  // set auto split limit
  static void setTreeSize(uint limit) {
    *getMinTreeSizeP() = limit;
  }

  // get parent tree
  CQIllustratorQuadTree *getParent() const { return parent_; }

  // get sub trees
  CQIllustratorQuadTree *getBLTree() const { return bl_tree_; }
  CQIllustratorQuadTree *getBRTree() const { return br_tree_; }
  CQIllustratorQuadTree *getTLTree() const { return tl_tree_; }
  CQIllustratorQuadTree *getTRTree() const { return tr_tree_; }

  // has child trees
  bool hasChildren() const { return bl_tree_ != 0; }

  //----------

 public:
  // add data item to the tree
  void add(DATA *data) {
    const BBOX &bbox = data->getFlatBBox();

    if (! bbox.isSet()) {
      std::cerr << "No BBox for QuadTree add" << std::endl;
      return;
    }

    if (! inside(bbox))
      grow(bbox);

    addData(data, bbox);
  }

 private:
  void addData(DATA *data, const BBOX &bbox) {
    if (bl_tree_) {
      if        (bbox.getRight() <= br_tree_->bbox_.getLeft ()) {
        if        (bbox.getTop   () <= tl_tree_->bbox_.getBottom()) {
          bl_tree_->addData(data, bbox); return;
        } else if (bbox.getBottom() >= bl_tree_->bbox_.getTop   ()) {
          tl_tree_->addData(data, bbox); return;
        }
      } else if (bbox.getLeft () >= bl_tree_->bbox_.getRight()) {
        if        (bbox.getTop   () <= tr_tree_->bbox_.getBottom()) {
          br_tree_->addData(data, bbox); return;
        } else if (bbox.getBottom() >= br_tree_->bbox_.getTop   ()) {
          tr_tree_->addData(data, bbox); return;
        }
      }
    }

    dataList_.push_back(data);

    if (! bl_tree_) {
      uint limit = getAutoSplitLimit();

      if (limit > 0 && dataList_.size() > limit)
        autoSplit();
    }
  }

  //----------

 public:
  // increase size of bounding box of tree (root)
  void grow(const BBOX &bbox) {
    T l, b, r, t;

    if (bbox_.getLeft() > bbox_.getRight()) {
      l = bbox.getLeft  ();
      b = bbox.getBottom();
      r = bbox.getRight ();
      t = bbox.getTop   ();
    }
    else {
      l = std::min(bbox_.getLeft  (), bbox.getLeft  ());
      b = std::min(bbox_.getBottom(), bbox.getBottom());
      r = std::max(bbox_.getRight (), bbox.getRight ());
      t = std::max(bbox_.getTop   (), bbox.getTop   ());
    }

    if (bl_tree_) {
      const BBOX &bl_bbox = bl_tree_->getBBox();
      const BBOX &br_bbox = br_tree_->getBBox();
      const BBOX &tl_bbox = tl_tree_->getBBox();
      const BBOX &tr_bbox = tr_tree_->getBBox();

      bl_tree_->grow(BBOX(l                 , b                  ,
                          bl_bbox.getRight(), bl_bbox.getTop   ()));
      br_tree_->grow(BBOX(br_bbox.getLeft (), b                  ,
                          r                 , br_bbox.getTop   ()));
      tl_tree_->grow(BBOX(l                 , tl_bbox.getBottom(),
                          tl_bbox.getRight(), t                  ));
      tr_tree_->grow(BBOX(tr_bbox.getLeft (), tr_bbox.getBottom(),
                          r                 , t                  ));
    }

    bbox_ = BBOX(l, b, r, t);
  }

  //----------

 public:
  // remove data from tree
  void remove(DATA *data) {
    const BBOX &bbox = data->getFlatBBox();

    if (! bbox.isSet()) {
      std::cerr << "No BBox for Quad Tree remove" << std::endl;
      return;
    }

    if (! inside(bbox)) {
      std::cerr << "Quad Tree BBox mismatch" << std::endl;
      return;
    }

    removeData(data, bbox);
  }

 private:
  void removeData(DATA *data, const BBOX &bbox) {
    if (bl_tree_) {
      if      (bl_tree_->inside(bbox)) return bl_tree_->removeData(data, bbox);
      else if (br_tree_->inside(bbox)) return br_tree_->removeData(data, bbox);
      else if (tl_tree_->inside(bbox)) return tl_tree_->removeData(data, bbox);
      else if (tr_tree_->inside(bbox)) return tr_tree_->removeData(data, bbox);
    }

    dataList_.remove(data);
  }

  //----------

 public:
  // split tree at point (defining vertical and horizontal split)
  void split(T x, T y) {
    if (bbox_.getLeft() == bbox_.getRight() && bbox_.getBottom() == bbox_.getRight()) return;

    if (! bl_tree_) {
      if (x <= bbox_.getLeft()   || x >= bbox_.getRight() ||
          y <= bbox_.getBottom() || y >= bbox_.getTop()) return;

      BBOX blbbox(bbox_.getLeft(), bbox_.getBottom(), x               , y             );
      BBOX brbbox(x              , bbox_.getBottom(), bbox_.getRight(), y             );
      BBOX tlbbox(bbox_.getLeft(), y                , x               , bbox_.getTop());
      BBOX trbbox(x              , y                , bbox_.getRight(), bbox_.getTop());

      bl_tree_ = new CQIllustratorQuadTree(this, blbbox);
      br_tree_ = new CQIllustratorQuadTree(this, brbbox);
      tl_tree_ = new CQIllustratorQuadTree(this, tlbbox);
      tr_tree_ = new CQIllustratorQuadTree(this, trbbox);

      typename DataList::iterator p1 = dataList_.begin();
      typename DataList::iterator p2 = dataList_.end  ();

      while (p1 != p2) {
        const BBOX &bbox = (*p1)->getFlatBBox();

        if (! bbox.isSet()) { ++p1; continue; }

        if      (bl_tree_->inside(bbox)) bl_tree_->add(*p1);
        else if (br_tree_->inside(bbox)) br_tree_->add(*p1);
        else if (tl_tree_->inside(bbox)) tl_tree_->add(*p1);
        else if (tr_tree_->inside(bbox)) tr_tree_->add(*p1);
        else                             { ++p1; continue; }

        typename DataList::iterator p = p1++;

        dataList_.erase(p);
      }
    }
    else {
      bl_tree_->split(x, y);
      br_tree_->split(x, y);
      tl_tree_->split(x, y);
      tr_tree_->split(x, y);
    }
  }

 public:
  // automatically split tree (1 or more times)
  // the split point is automatically determined from the data
  bool autoSplit(uint n=1) {
    if (n == 0) return false;

    if (! bl_tree_) {
      if (dataList_.size() <= getMinTreeSize()) return false;

      T x, y;

      if (getSplitPoint(x, y)) {
        split(x, y);

        if (bl_tree_) {
          bl_tree_->autoSplit(n - 1);
          br_tree_->autoSplit(n - 1);
          tl_tree_->autoSplit(n - 1);
          tr_tree_->autoSplit(n - 1);
        }
      }
    }
    else {
      bl_tree_->autoSplit(n);
      br_tree_->autoSplit(n);
      tl_tree_->autoSplit(n);
      tr_tree_->autoSplit(n);
    }

    return true;
  }

 private:
  bool getSplitPoint(T &x, T &y) {
    x = (bbox_.getRight() + bbox_.getLeft  ())/2;
    y = (bbox_.getTop  () + bbox_.getBottom())/2;

    return (x != bbox_.getLeft() && y != bbox_.getBottom());
  }

  //-------

 public:
  // get data items inside the specified bounding box
  void getDataInsideBBox(const BBOX &bbox, DataList &dataList) const {
    dataList.clear();

    addDataInsideBBox(bbox, dataList);
  }

  void addDataInsideBBox(const BBOX &bbox, DataList &dataList) const {
    if (! overlaps(bbox))
      return;

    // if tree completely inside, add all items
    if (inside(bbox_, bbox))
      addTreeDataToList(dataList);
    else {
      typename DataList::const_iterator p1, p2;

      for (p1 = dataList_.begin(), p2 = dataList_.end(); p1 != p2; ++p1) {
        const BBOX &bbox1 = (*p1)->getFlatBBox();

        if (! bbox1.isSet())
          continue;

        if (inside(bbox1, bbox))
          dataList.push_back(*p1);
      }

      if (bl_tree_) {
        bl_tree_->addDataInsideBBox(bbox, dataList);
        br_tree_->addDataInsideBBox(bbox, dataList);
        tl_tree_->addDataInsideBBox(bbox, dataList);
        tr_tree_->addDataInsideBBox(bbox, dataList);
      }
    }
  }

  //-------

 public:
  // get data items touching the specified bounding box
  void getDataTouchingBBox(const BBOX &bbox, DataList &dataList) const {
    dataList.clear();

    if (overlaps(bbox))
      addDataTouchingBBox(bbox, dataList);
  }

  void addDataTouchingBBox(const BBOX &bbox, DataList &dataList) const {
    // if tree completely inside, add all items
    if (inside(bbox_, bbox))
      addTreeDataToList(dataList);
    else {
      typename DataList::const_iterator p1, p2;

      for (p1 = dataList_.begin(), p2 = dataList_.end(); p1 != p2; ++p1) {
        const BBOX &bbox1 = (*p1)->getFlatBBox();

        if (! bbox1.isSet())
          continue;

        if (overlaps(bbox1, bbox))
          dataList.push_back(*p1);
      }

      if (bl_tree_) {
        if (bbox.getLeft() <= br_tree_->bbox_.getLeft()) {
          if (bbox.getBottom() <= tl_tree_->bbox_.getBottom())
            bl_tree_->addDataTouchingBBox(bbox, dataList);
          if (bbox.getTop   () >= bl_tree_->bbox_.getTop())
            tl_tree_->addDataTouchingBBox(bbox, dataList);
        }

        if (bbox.getRight() >= bl_tree_->bbox_.getRight()) {
          if (bbox.getBottom() <= tr_tree_->bbox_.getBottom())
            br_tree_->addDataTouchingBBox(bbox, dataList);
          if (bbox.getTop   () >= br_tree_->bbox_.getTop())
            tr_tree_->addDataTouchingBBox(bbox, dataList);
        }
      }
    }
  }

  //-------

 public:
  // get data items which have the specified point inside them
  void getDataAtPoint(T x, T y, DataList &dataList) const {
    dataList.clear();

    addDataAtPoint(x, y, dataList);
  }

  void addDataAtPoint(T x, T y, DataList &dataList) const {
    if (x < bbox_.getLeft  () || x > bbox_.getRight() ||
        y < bbox_.getBottom() || y > bbox_.getTop  ())
      return;

    typename DataList::const_iterator p1, p2;

    for (p1 = dataList_.begin(), p2 = dataList_.end(); p1 != p2; ++p1) {
      const BBOX &bbox = (*p1)->getFlatBBox();

      if (! bbox.isSet())
        continue;

      if (x >= bbox.getLeft  () && x <= bbox.getRight() &&
          y >= bbox.getBottom() && y <= bbox.getTop  ())
        dataList.push_back(*p1);
    }

    if (bl_tree_) {
      if (x <= br_tree_->bbox_.getLeft()) {
        if (y <= tl_tree_->bbox_.getBottom())
          bl_tree_->addDataAtPoint(x, y, dataList);
        if (y >= bl_tree_->bbox_.getTop())
          tl_tree_->addDataAtPoint(x, y, dataList);
      }

      if (x >= bl_tree_->bbox_.getRight()) {
        if (y <= tr_tree_->bbox_.getBottom())
          br_tree_->addDataAtPoint(x, y, dataList);
        if (y >= br_tree_->bbox_.getTop())
          tr_tree_->addDataAtPoint(x, y, dataList);
      }
    }
  }

  //-------

 public:
  // get tree which has the specified point inside it
  const CQIllustratorQuadTree *getTreeAtPoint(T x, T y) const {
    if (x < bbox_.getLeft  () || x > bbox_.getRight() ||
        y < bbox_.getBottom() || y > bbox_.getTop  ())
      return 0;

    typename DataList::const_iterator p1, p2;

    for (p1 = dataList_.begin(), p2 = dataList_.end(); p1 != p2; ++p1) {
      const BBOX &bbox = (*p1)->getFlatBBox();

      if (! bbox.isSet())
        continue;

      if (x >= bbox.getLeft  () && x <= bbox.getRight() &&
          y >= bbox.getBottom() && y <= bbox.getTop  ())
        return this;
    }

    if (bl_tree_) {
      const CQIllustratorQuadTree *tree = 0;

      if ((tree = bl_tree_->getTreeAtPoint(x, y)) != 0) return tree;
      if ((tree = br_tree_->getTreeAtPoint(x, y)) != 0) return tree;
      if ((tree = tl_tree_->getTreeAtPoint(x, y)) != 0) return tree;
      if ((tree = tr_tree_->getTreeAtPoint(x, y)) != 0) return tree;
    }

    return this;
  }

  //-------

 private:
  void addTreeDataToList(DataList &dataList) const {
    typename DataList::const_iterator p1, p2;

    for (p1 = dataList_.begin(), p2 = dataList_.end(); p1 != p2; ++p1)
      dataList.push_back(*p1);

    if (bl_tree_) {
      bl_tree_->addTreeDataToList(dataList);
      br_tree_->addTreeDataToList(dataList);
      tl_tree_->addTreeDataToList(dataList);
      tr_tree_->addTreeDataToList(dataList);
    }
  }

  //-------

 private:
  // is bbox inside tree
  bool inside(const BBOX &bbox) const {
    assert(bbox.getLeft() <= bbox.getRight() && bbox.getBottom() <= bbox.getTop());

    return inside(bbox, bbox_);
  }

  // does bbox overlap tree
  bool overlaps(const BBOX &bbox) const {
    assert(bbox.getLeft() <= bbox.getRight() && bbox.getBottom() <= bbox.getTop());

    return overlaps(bbox, bbox_);
  }

  // is bbox1 inside bbox2
  static bool inside(const BBOX &bbox1, const BBOX &bbox2) {
    return ((bbox1.getLeft  () >= bbox2.getLeft  () && bbox1.getRight() <= bbox2.getRight()) &&
            (bbox1.getBottom() >= bbox2.getBottom() && bbox1.getTop  () <= bbox2.getTop  ()));
  }

  // does bbox1 overlap bbox2
  static bool overlaps(const BBOX &bbox1, const BBOX &bbox2) {
    return ((bbox1.getRight() >= bbox2.getLeft  () && bbox1.getLeft  () <= bbox2.getRight()) &&
            (bbox1.getTop  () >= bbox2.getBottom() && bbox1.getBottom() <= bbox2.getTop  ()));
  }

  //-------

 public:
  uint numElements() const {
    uint n = dataList_.size();

    if (bl_tree_)
      n = bl_tree_->numElements() + br_tree_->numElements() +
          tl_tree_->numElements() + tr_tree_->numElements();

    return n;
  }

  uint minElements() const {
    uint n = dataList_.size();

    if (bl_tree_)
      n = std::min(std::min(bl_tree_->minElements(), br_tree_->minElements()),
                   std::min(tl_tree_->minElements(), tr_tree_->minElements()));

    return n;
  }

  uint maxElements() const {
    uint n = dataList_.size();

    if (bl_tree_)
      n = std::max(std::max(bl_tree_->maxElements(), br_tree_->maxElements()),
                   std::max(tl_tree_->maxElements(), tr_tree_->maxElements()));

    return n;
  }

  uint getDepth() const {
    return (parent_ ? parent_->getDepth() + 1 : 1);
  }

  uint maxBorder() const {
    if (bl_tree_) {
      uint nd = dataList_.size();
      uint nl = bl_tree_->maxBorder();
      uint nr = br_tree_->maxBorder();
      uint nb = tl_tree_->maxBorder();
      uint nt = tr_tree_->maxBorder();

      return std::max(nd, std::max(nl, std::max(nr, std::max(nb, nt))));
    }
    else
      return 0;
  }

 private:
  static uint *getAutoSplitLimitP() {
    static uint autoSplitLimit = 16;

    return &autoSplitLimit;
  }

  // if tree contains less than or equal to this number of elements don't bother splitting
  static uint *getMinTreeSizeP() {
    static uint minTreeSize = 16;

    return &minTreeSize;
  }

 private:
  CQIllustratorQuadTree *parent_  { 0 }; // parent tree (0 if root)
  BBOX                   bbox_;          // bounding box of tree
  DataList               dataList_;      // data list
  CQIllustratorQuadTree *bl_tree_ { 0 }; // bottom left sub tree
  CQIllustratorQuadTree *br_tree_ { 0 }; // bottom right sub tree
  CQIllustratorQuadTree *tl_tree_ { 0 }; // top left sub tree
  CQIllustratorQuadTree *tr_tree_ { 0 }; // top right sub tree
};

#endif
