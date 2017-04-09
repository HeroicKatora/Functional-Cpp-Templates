#include "hdr/std.hpp"
#include "hdr/match.hpp"
#include "hdr/math.hpp"

#include "boost/type_index/ctti_type_index.hpp"

struct Foo;
struct Bar;

struct PreConditiona {
  template<typename V>
  struct TypeIndexFunction {
    using RetType = decltype(boost::typeindex::ctti_type_index::type_id<V>());
    constexpr static const RetType value = boost::typeindex::ctti_type_index::type_id<V>();
  };
  using typeindex = hdr::std::TemplateFunction<TypeIndexFunction>;

  struct CompareTypeIndex {
    template<typename IA, typename IB>
    using expr = hdr::math::IntegralConstant<bool, (IA::value < IB::value)>;
  };
  using compare = hdr::std::Function<CompareTypeIndex>;

  using IndexFoo = hdr::std::Apply<typeindex, Foo>;
  using IndexBar = hdr::std::Apply<typeindex, Bar>;
  using IsFooSmaller = hdr::std::Apply<compare, IndexFoo, IndexBar>;
  using IsBarSmaller = hdr::std::Apply<compare, IndexBar, IndexFoo>;
  static_assert(IsFooSmaller::value ^ IsBarSmaller::value);
};

namespace Main {
  using namespace hdr::match;
  using FooP = Placeholder<Foo>;
  using BarP = Placeholder<Bar>;
  template<typename A, typename B> struct Pair {};

  using DecFir = typename Decompose<Pair<FooP, BarP>, Pair<bool, int>>::type;
  using DecSec = typename Decompose<Pair<bool, BarP>, Pair<bool, int>>::type;
  using DecThi = typename Decompose<Pair<bool, int>,  Pair<bool, int>>::type;
  static_assert(Apply<hdr::maybe::isJust, DecFir>::value);
  static_assert(Apply<hdr::maybe::isJust, DecSec>::value);
  static_assert(Apply<hdr::maybe::isJust, DecThi>::value);
};

int main() {};
