#include <type_traits>
#include "hdr/std.hpp"
#include "hdr/lambda.hpp"

namespace Test {
  using namespace ::hdr::lambda;
  using namespace ::hdr::std;
  struct Foo;
  static_assert(::std::is_same_v<Foo, Lambda<Foo>>);
  static_assert(::std::is_same_v<Foo, Lambda<id, Foo>>);

  using lidentity = Lambda<_0>;
  static_assert(::std::is_same_v<Foo, Apply<lidentity, Foo>>);

  using cFoo = Lambda<Const<Foo>, _0>;
  static_assert(::std::is_same_v<Foo, Apply<cFoo, Void>>);

  using funl = Lambda<_0, Foo>;
  static_assert(::std::is_same_v<Foo,      Apply<funl, id>>);

  using self = Lambda<_0, _0>;
  using selfresult = Apply<self, id>;
  static_assert(::std::is_same_v<id, selfresult>);

  using twoa = Lambda<_0, _1>;
  using result = Apply<twoa, id, Foo>;
  static_assert(::std::is_same_v<Foo, result>);

  using remix  = Apply<twoa, lidentity, id>;
  static_assert(::std::is_same_v<id, remix>);
}

int main() {};
