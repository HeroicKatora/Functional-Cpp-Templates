#include "hdr/std.hpp"
#include "hdr/match.hpp"
#include "hdr/math.hpp"

#include "boost/type_index/ctti_type_index.hpp"

struct Foo;
struct Bar;

namespace PreConditiona {
  using namespace hdr::std;
  using namespace hdr::math;
  template<typename V>
  struct TypeIndexFunction {
    using RetType = decltype(boost::typeindex::ctti_type_index::type_id<V>());
    constexpr static const RetType value = boost::typeindex::ctti_type_index::type_id<V>();
  };
  using typeindex = TemplateFunction<TypeIndexFunction>;

  struct CompareTypeIndex {
    template<typename IA, typename IB>
    using expr = Bool<(IA::value < IB::value)>;
  };
  using compare = Function<CompareTypeIndex>;

  using IndexFoo     = Apply<typeindex, Foo>;
  using IndexBar     = Apply<typeindex, Bar>;
  using IsFooSmaller = Apply<compare, IndexFoo, IndexBar>;
  using IsBarSmaller = Apply<compare, IndexBar, IndexFoo>;
  static_assert(IsFooSmaller::value ^ IsBarSmaller::value);
};

namespace Main {
  using namespace hdr::match;
  using FooP = Placeholder<Foo>;
  using BarP = Placeholder<Bar>;
  template<typename A, typename B> struct Pair {};
  template<typename A, typename B> struct Diff {};

  using DecFir = Apply<decompose, Pair<FooP, BarP>, Pair<bool, int>>;
  using DecSec = Apply<decompose, Pair<bool, BarP>, Pair<bool, int>>;
  using DecThi = Apply<decompose, Pair<bool, int>,  Pair<bool, int>>;
  using DecFou = Apply<decompose, PlaceholderAny,   Pair<bool, int>>;
  static_assert(Apply<hdr::maybe::isJust, DecFir>::value);
  static_assert(Apply<hdr::maybe::isJust, DecSec>::value);
  static_assert(Apply<hdr::maybe::isJust, DecThi>::value);
  static_assert(Apply<hdr::maybe::isJust, DecFou>::value);

  using FailFi = Apply<decompose, unsigned int,     unsigned short>;
  using FailSe = Apply<decompose, Pair<bool, int>,  Pair<int, bool>>;
  using FailTh = Apply<decompose, Pair<bool, int>,  unsigned short>;
  using FailFo = Apply<decompose, Pair<bool, int>,  Diff<bool, int>>;
  static_assert(Apply<hdr::maybe::isNothing, FailFi>::value);
  static_assert(Apply<hdr::maybe::isNothing, FailSe>::value);
  static_assert(Apply<hdr::maybe::isNothing, FailTh>::value);
  static_assert(Apply<hdr::maybe::isNothing, FailFo>::value);
};

int main() {};
