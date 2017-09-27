#include <type_traits>
#include "hdr/core/std.hpp"
#include "hdr/core/lambda.hpp"
#include "hdr/context/lambda.hpp"

namespace Test {
  using namespace ::hdr::lambda;
  using namespace ::hdr::std;
  struct Foo;
  static_assert(::std::is_same<Foo, Lambda<Foo>>::value);
  static_assert(::std::is_same<Foo, Lambda<id, Foo>>::value);

  using lidentity = Lambda<_0>;
  static_assert(::std::is_same<Foo, Apply<lidentity, Foo>>::value);

  using cFoo = Lambda<Const<Foo>, _0>;
  static_assert(::std::is_same<Foo, Apply<cFoo, Void>>::value);

  using funl = Lambda<_0, Foo>;
  static_assert(::std::is_same<Foo,      Apply<funl, id>>::value);

  using self = Lambda<_0, _0>;
  using selfresult = Apply<self, id>;
  static_assert(::std::is_same<id, selfresult>::value);

  using twoa = Lambda<_0, _1>;
  using result = Apply<twoa, id, Foo>;
  static_assert(::std::is_same<Foo, result>::value);

  using remix  = Apply<twoa, lidentity, id>;
  using selfremix  = Apply<twoa, lidentity, lidentity>;
  static_assert(::std::is_same<id,        remix>::value);
  static_assert(::std::is_same<lidentity, selfremix>::value);

  using inner    = Lambda<IApply<fconst, _0>, IApply<fconst, _0>>;
  using innerid  = Apply<inner, id>;
  using innerfoo = Apply<inner, Foo>;
  static_assert(::std::is_same<id,        innerid>::value);
  static_assert(::std::is_same<Foo,       innerfoo>::value);
}

int main() {};
