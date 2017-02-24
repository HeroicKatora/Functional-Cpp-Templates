#include "hdr/std.hpp"
#include "hdr/match.hpp"
#include "hdr/math.hpp"

#include "boost/type_index/ctti_type_index.hpp"

struct Foo;
struct Bar;

template<typename V>
struct TypeIndexFunction {
  using RetType = decltype(boost::typeindex::ctti_type_index::type_id<V>());
  constexpr static const RetType value = boost::typeindex::ctti_type_index::type_id<V>();
};
using typeindex = hdr::std::Function<TypeIndexFunction>;

struct CompareTypeIndex {
  template<typename IA, typename IB>
  using expr = hdr::math::IntegralConstant<bool, (IA::value < IB::value)>;
};
using compare = hdr::std::F2<CompareTypeIndex>;

using IndexFoo = hdr::std::Apply<typeindex, Foo>;
using IndexBar = hdr::std::Apply<typeindex, Bar>;
using IsFooSmaller = hdr::std::Apply<compare, IndexFoo, IndexBar>;
using IsBarSmaller = hdr::std::Apply<compare, IndexBar, IndexFoo>;
static_assert(IsFooSmaller::value ^ IsBarSmaller::value);

int main() {};
