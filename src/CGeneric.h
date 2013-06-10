#ifndef CGENERIC_H
#define CGENERIC_H

//----------

// If then else for to select one of type types using compiler evaluable expression
template<bool C, typename T1, typename T2>
class CGenericIfElse;

template<typename T1, typename T2>
class CGenericIfElse<true, T1, T2> {
 public:
  typedef T1 Result;
};

template<typename T1, typename T2>
class CGenericIfElse<false, T1, T2> {
 public:
  typedef T2 Result;
};

//----------

#include <CTypeTraits.h>

//----------

// simple type to use as function overload discrimator
template<int i>
struct Int2Type {
  enum { value = i };
};

//----------

// promote to biggest of the type types
template<typename T1, typename T2>
class CGenericTypePromotion {
 public:
  typedef typename
    CGenericIfElse<
      (sizeof(T1) > sizeof(T2)),
      T1,
      typename CGenericIfElse<
                 (sizeof(T1) < sizeof(T2)),
                  T2,
                  void
               >::Result
    >::Result Result;
};

template<typename T>
class CGenericTypePromotion<T,T> {
 public:
  typedef T Result;
};

// macros to add overloads for type promotion

#define CGenericMakePromotion(T1,T2,Tr) \
template<> \
class CGenericTypePromotion<T1, T2> { \
 public: \
  typedef Tr Result; \
}; \
\
template<> \
class CGenericTypePromotion<T2, T1> { \
 public: \
  typedef Tr Result; \
};

#include <vector>

template<typename T1, typename T2>
class CGenericTypePromotion<std::vector<T1>, std::vector<T2> > {
 public:
  typedef std::vector<typename CGenericTypePromotion<T1,T2>::Result> Result;
};

template<typename T>
class CGenericTypePromotion<std::vector<T>, std::vector<T> > {
 public:
  typedef std::vector<typename CGenericTypePromotion<T,T>::Result> Result;
};

//----------

// is fundamental type

template <typename T>
class CGenericIsFundamentalT {
 public:
  enum { Yes = 0, No = 1 };
};

#define CGenericMakeFundamental(T) \
template<> \
class CGenericIsFundamentalT<T> { \
 public: \
  enum { Yes = 1, No = 0 }; \
};

CGenericMakeFundamental(void)

CGenericMakeFundamental(bool)
CGenericMakeFundamental(char)
CGenericMakeFundamental(signed char)
CGenericMakeFundamental(unsigned char)
CGenericMakeFundamental(wchar_t)

CGenericMakeFundamental(signed short)
CGenericMakeFundamental(unsigned short)
CGenericMakeFundamental(signed int)
CGenericMakeFundamental(unsigned int)
CGenericMakeFundamental(signed long)
CGenericMakeFundamental(unsigned long)
#if LONGLONG_EXISTS
 CGenericMakeFundamental(signed long long)
 CGenericMakeFundamental(unsigned long long)
#endif

CGenericMakeFundamental(float)
CGenericMakeFundamental(double)
CGenericMakeFundamental(long double)

//------

template<typename T>
class CompoundT {
 public:
  enum { IsPtrT = 0, IsRefT = 0, IsArrayT = 0, IsFuncT = 0, IsPtrMemT = 0 };

  typedef T               BaseT;
  typedef T               BottomT;
  typedef CompoundT<void> ClassT;
};

template<typename T>
class CGenericIsFunctionT {
 private:
  typedef char                 One;
  typedef struct { One a[2]; } Two;

 public:
  template<typename U> static One test(...);
  template<typename U> static Two test(U (*)[1]);

  //enum { Yes = sizeof(CGenericIsFunctionT::test(0)) == 1 };
  enum { Yes = sizeof(CGenericIsFunctionT<T>::test<T>(0)) == 1 };
  enum { No  = !Yes };
};

template<typename T>
class CGenericIsFunctionT<T&> {
 public:
  enum { Yes = 0 };
  enum { No = !Yes };
};

template<>
class CGenericIsFunctionT<void> {
 public:
  enum { Yes = 0 };
  enum { No = !Yes };
};

template<>
class CGenericIsFunctionT<void const> {
 public:
  enum { Yes = 0 };
  enum { No = !Yes };
};

template<typename T>
class CGenericCompoundT {
 public:
  enum { IsPtrT = 0, IsRefT = 0, IsArrayT = 0,
         IsFuncT = CGenericIsFunctionT<T>::Yes,
         IsPtrMemT = 0 };

  typedef T                       BaseT;
  typedef T                       BottomT;
  typedef CGenericCompoundT<void> ClassT;
};

template<typename T>
class CGenericCompoundT<T&> {
 public:
  enum { IsPtrT = 0, IsRefT = 1, IsArrayT = 0, IsFuncT = 0, IsPtrMemT = 0 };

  typedef T                                      BaseT;
  typedef typename CGenericCompoundT<T>::BottomT BottomT;
  typedef CGenericCompoundT<void>                ClassT;
};

template<typename T>
class CGenericCompoundT<T*> {
 public:
  enum { IsPtrT = 1, IsRefT = 0, IsArrayT = 0, IsFuncT = 0, IsPtrMemT = 0 };

  typedef T                                      BaseT;
  typedef typename CGenericCompoundT<T>::BottomT BottomT;
  typedef CGenericCompoundT<void>                ClassT;
};

template<typename T, size_t N>
class CGenericCompoundT<T[N]> {
 public:
  enum { IsPtrT = 0, IsRefT = 0, IsArrayT = 1, IsFuncT = 0, IsPtrMemT = 0 };

  typedef T                                      BaseT;
  typedef typename CGenericCompoundT<T>::BottomT BottomT;
  typedef CGenericCompoundT<void>                ClassT;
};

template<typename T>
class CGenericCompoundT<T[]> {
 public:
  enum { IsPtrT = 0, IsRefT = 0, IsArrayT = 1, IsFuncT = 0, IsPtrMemT = 0 };

  typedef T                                      BaseT;
  typedef typename CGenericCompoundT<T>::BottomT BottomT;
  typedef CGenericCompoundT<void>                ClassT;
};

template<typename T, typename C>
class CGenericCompoundT<T C::*> {
 public:
  enum { IsPtrT = 0, IsRefT = 0, IsArrayT = 0, IsFuncT = 0, IsPtrMemT = 1 };

  typedef T                                      BaseT;
  typedef typename CGenericCompoundT<T>::BottomT BottomT;
  typedef C                                      ClassT;
};

template<typename R>
class CGenericCompoundT<R()> {
 public:
  enum { IsPtrT = 0, IsRefT = 0, IsArrayT = 0, IsFuncT = 1, IsPtrMemT = 0 };

  typedef R                       BaseT();
  typedef R                       BottomT();
  typedef CGenericCompoundT<void> ClassT;
};

template<typename R, typename P1>
class CGenericCompoundT<R(P1)> {
 public:
  enum { IsPtrT = 0, IsRefT = 0, IsArrayT = 0, IsFuncT = 1, IsPtrMemT = 0 };

  typedef R                       BaseT(P1);
  typedef R                       BottomT(P1);
  typedef CGenericCompoundT<void> ClassT;
};

template<typename R, typename P1>
class CGenericCompoundT<R(P1, ...)> {
 public:
  enum { IsPtrT = 0, IsRefT = 0, IsArrayT = 0, IsFuncT = 1, IsPtrMemT = 0 };

  typedef R                       BaseT(P1);
  typedef R                       BottomT(P1);
  typedef CGenericCompoundT<void> ClassT;
};

//------

struct CGenericSizeOverOne { char c[2]; };

template<typename T, bool convert_possible =
                      (! CGenericCompoundT<T>::IsFuncT &&
                       ! CGenericCompoundT<T>::IsArrayT)>
class CGenericConsumeUDC {
 public:
  operator T() const;
};

template <typename T>
class CGenericConsumeUDC<T, false> {
};

template <bool convert_possible>
class CGenericConsumeUDC<void, convert_possible> {
};

char CGenericEnumCheck(bool);
char CGenericEnumCheck(char);
char CGenericEnumCheck(signed char);
char CGenericEnumCheck(unsigned char);
char CGenericEnumCheck(wchar_t);

char CGenericEnumCheck(signed short);
char CGenericEnumCheck(unsigned short);
char CGenericEnumCheck(signed int);
char CGenericEnumCheck(unsigned int);
char CGenericEnumCheck(signed long);
char CGenericEnumCheck(unsigned long);
#if LONGLONG_EXISTS
char CGenericEnumCheck(signed long long);
char CGenericEnumCheck(unsigned long long);
#endif  // LONGLONG_EXISTS
char CGenericEnumCheck(float);
char CGenericEnumCheck(double);
char CGenericEnumCheck(long double);

CGenericSizeOverOne CGenericEnumCheck(...);

template<typename T>
class CGenericIsEnumT {
 public:
  enum { Yes = CGenericIsFundamentalT<T>::No &&
               ! CGenericCompoundT<T>::IsRefT &&
               ! CGenericCompoundT<T>::IsPtrT &&
               ! CGenericCompoundT<T>::IsPtrMemT &&
               sizeof(CGenericEnumCheck(CGenericConsumeUDC<T>()))==1 };
  enum { No = !Yes };
};

//------

template<typename T>
class CGenericIsClassT {
 public:
  enum { Yes = CGenericIsFundamentalT<T>::No &&
               CGenericIsEnumT<T>::No &&
               !CGenericCompoundT<T>::IsPtrT &&
               !CGenericCompoundT<T>::IsRefT &&
               !CGenericCompoundT<T>::IsArrayT &&
               !CGenericCompoundT<T>::IsPtrMemT &&
               !CGenericCompoundT<T>::IsFuncT };
  enum { No = !Yes };
};

//------

template <typename T>
class CGenericTypeT {
 public:
  enum { IsFundaT  = CGenericIsFundamentalT<T>::Yes,
         IsPtrT    = CGenericCompoundT<T>::IsPtrT,
         IsRefT    = CGenericCompoundT<T>::IsRefT,
         IsArrayT  = CGenericCompoundT<T>::IsArrayT,
         IsFuncT   = CGenericCompoundT<T>::IsFuncT,
         IsPtrMemT = CGenericCompoundT<T>::IsPtrMemT,
         IsEnumT   = CGenericIsEnumT<T>::Yes,
         IsClassT  = CGenericIsClassT<T>::Yes };
};

#define CGenericIsFundamental(T) CGenericTypeT<T>::IsFundaT
#define CGenericIsFunction(T)    CGenericTypeT<T>::IsFuncT
#define CGenericIsEnum(T)        CGenericTypeT<T>::IsEnumT
#define CGenericIsClass(T)       CGenericTypeT<T>::IsClassT

//------

// class to check if D is derived from B as either
// a compile time or runtime check:
//
// For compile time check use enum:
//
//  CIsDerivedFrom<D,B>::Value
//
// For runtime check just derive from this class e.g.
// to check if specified class supports TraitClass interface
//
//  template<class T>
//  class MyClass : CIsDerivedFrom<T,TraitClass> {
//  }
template <typename D, typename B>
class CIsDerivedFrom {
 private:
  class No { };
  class Yes { No no[2]; }; // sizeof(Yes) is bigger than sizeof(No)

  // update function overloading to get best match for D *
  // if can cast B * then the first function will be chosen
  // otherwise the second (which will match anything)
  // The return argument of the matching function can
  // then be used to initialize an enum using sizeof for compile
  // time check
  static Yes Test(B *); // not defined
  static No  Test(...); // not defined

  // static function to test if D* can be converted to B* for runtime check
  static void Constraints(D *p) {
    B *pb = p; // check id p of type D* can be converted to a B*
    pb = p; // suppress unused compiler warning
  }

 public:
  // enum will be 1 if D* can be cast to B* otherwise 0
  enum { Value = sizeof(Test(static_cast<D *>(0))) == sizeof(Yes) };
};

// specialize for same type
template <typename T>
class CIsDerivedFrom<T, T> {
  public:
  // enum will be 1 if D* can be cast to B* otherwise 0
  enum { Value = 1 };

  enum { Exists = 1 };
  enum { TwoWay = 1 };
  enum { Same   = 1 };
};

#define CGenericIsDerived(D,B) CIsDerivedFrom<D,B>::Value

// similar to CIsDerivedFrom: test if convertable
template <typename T, typename U>
class CCanConvert {
 private:
  class No { };
  class Yes { No no[2]; }; // sizeof(Yes) is bigger than sizeof(No)

  static Yes Test(U); // not defined
  static No  Test(...); // not defined

  T MakeT(); // not defined

 public:
  enum { Exists = sizeof(Test(MakeT())) == sizeof(Yes) };
  enum { TwoWay = Exists && CCanConvert<U,T>::Exists };
  enum { Same   = 0 };
};

// specialize for same type
template <typename T>
class CCanConvert<T, T> {
  public:
  enum { Exists = 1 };
  enum { TwoWay = 1 };
  enum { Same   = 1 };
};

// test if D inherits from B or are same type
#define CGenericIsInherited(B,D) \
  (CCanConvert<const D *,const B *>::Exists && ! CCanConvert<const B *,const void *>::Same)

// test if D inherits from B (not same type)
#define CGenericIsInheritedStrict(B,D) \
  (CGenericIsInherited(B,D) && ! CCanConvert<const B,const D>::Same)

//------

#if 0
#define CGENERIC_ASSERT(C) \
  typedef struct _AssertMsg_s { char _assert_msg_[(C) ? 1 : -1]; } _AssertMsg_t;
#endif

namespace CGenericStaticAssert {
  template <bool> struct STATIC_ASSERT_FAILURE;
  template <> struct STATIC_ASSERT_FAILURE<true> { enum { value = 1 }; };

  template<int x> struct Test { };
}

#define CGENERIC_JOIN(X, Y) CGENERIC_JOIN2(X,Y)
#define CGENERIC_JOIN2(X, Y) X##Y

#define CGENERIC_ASSERT(C) \
  typedef ::CGenericStaticAssert::Test< \
    sizeof(::CGenericStaticAssert::STATIC_ASSERT_FAILURE<(bool)(C)>)> \
      CGENERIC_JOIN(_static_assert_typedef_, __LINE__);
#endif
