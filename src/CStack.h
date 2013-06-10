#ifndef CSTACK_H
#define CSTACK_H

#include <vector>
#include <list>

template<typename T>
class CStack {
 private:
  typedef std::list<T *> List;

  List list_;

 public:
  typedef typename List::iterator               iterator;
  typedef typename List::const_iterator         const_iterator;
  typedef typename List::reverse_iterator       reverse_iterator;
  typedef typename List::const_reverse_iterator const_reverse_iterator;

 public:
  CStack() { }

  void push(T *t) { list_.push_back(t); }

  void pop() { list_.pop_back(); }

  T *peek() { return list_.back(); }

  T *front() const { return list_.front(); }
  T *back () const { return list_.back (); }

  const_iterator begin() const { return list_.begin(); }
  const_iterator end  () const { return list_.end  (); }

  iterator begin() { return list_.begin(); }
  iterator end  () { return list_.end  (); }

  const_reverse_iterator rbegin() const { return list_.rbegin(); }
  const_reverse_iterator rend  () const { return list_.rend  (); }

  reverse_iterator rbegin() { return list_.rbegin(); }
  reverse_iterator rend  () { return list_.rend  (); }

  void lower(T *t) {
    if (list_.size() < 2) return;

    typename List::iterator p2 = list_.begin();
    typename List::iterator p1 = p2++;

    while (p2 != list_.end()) {
      if (*p2 == t) {
        std::swap(*p1, *p2);
        return;
      }

      p1 = p2++;
    }
  }

  void raise(T *t) {
    if (list_.size() < 2) return;

    typename List::reverse_iterator p2 = list_.rbegin();
    typename List::reverse_iterator p1 = p2++;

    while (p2 != list_.rend()) {
      if (*p2 == t) {
        std::swap(*p1, *p2);
        return;
      }

      p1 = p2++;
    }
  }

  void toBottom(T *t) {
    remove(t);

    list_.push_front(t);
  }

  void toTop(T *t) {
    remove(t);

    list_.push_back(t);
  }

  void remove(T *t) {
    list_.remove(t);
  }

  void addAfter(const T *t, const std::vector<T *> &a) {
    typename List::iterator p1 = list_.begin();
    typename List::iterator p2 = list_.end();

    for ( ; p1 != p2 && *p1 != t; ++p1) { }

    if (p1 != p2)
      ++p1;

    uint n = a.size();

    for (uint i = 0; i < n; ++i) {
      p1 = list_.insert(p1, a[i]);

      ++p1;
    }
  }
};

#endif
