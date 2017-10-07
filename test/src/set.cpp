#include "hdr/math.hpp"
#include "hdr/types/set.hpp"
#include "hdr/assert.hpp"

namespace Test {
  using namespace ::hdr::set;
  using namespace ::hdr::std;
  using namespace ::hdr::math;
  using namespace ::hdr::assert;
  namespace Build {
    using EmptySet  = Apply<empty, number_type>;
    using emptycount = Apply<size, EmptySet>;
    Assert<Same<emptycount, Value<0>>>;

    using InsertOne = Apply<insert, Value<1>, EmptySet>;
    using onecount = Apply<size, InsertOne>;
    Assert<Same<onecount, Value<1>>>;

    using InsertTwo = Apply<insert, Value<2>, InsertOne>;
    using twocount = Apply<size, InsertTwo>;
    Assert<Same<twocount, Value<2>>>;

    using InsertThr = Apply<insert, Value<3>, InsertTwo>;
    using InsertFou = Apply<insert, Value<4>, InsertThr>;
    using InsertFiv = Apply<insert, Value<5>, InsertFou>;

    using count = Apply<size, InsertFiv>;
    Assert<Same<EmptySet, Set<number_type, Empty>>>;
    Assert<::hdr::math::Same<count, Value<5>>>;
  }
  namespace FindElement {
    using EmptySet  = Apply<empty, number_type>;
    using InsertOne = Apply<insert, Value<1>, EmptySet>;
    using InsertTwo = Apply<insert, Value<2>, InsertOne>;
    using Origin    = Apply<insert, Value<3>, InsertTwo>;
    using FindOne   = Apply<::hdr::set::find, Value<1>, Origin>;
    using FindFour  = Apply<::hdr::set::find, Value<4>, Origin>;

    Assert<Same<FindOne, ::hdr::maybe::Just<Value<1>>>>;
    Assert<Same<FindFour, ::hdr::maybe::Nothing>>;
    Assert<Apply<contains, Value<1>, Origin>>;
    AssertFalse<Apply<contains, Value<4>, Origin>>;
  }
}

#if __has_include("boost/type_index/ctti_type_index.hpp")
#include "boost/type_index/ctti_type_index.hpp"
namespace tti {
  /// Tests type ordering by boost type index (which has a constexpr typeid)
  /// This could theoretically be used to implement efficient sets for arbitrary
  /// types without the need for a user defined comparison function
  struct Foo;
  struct Bar;
  using namespace hdr::std;
  using namespace hdr::math;
  template<typename V>
  struct TypeIndex {
    using RetType = decltype(boost::typeindex::ctti_type_index::type_id<V>());
    constexpr static const RetType value = boost::typeindex::ctti_type_index::type_id<V>();
  };
  using typeindex = TemplateFunction<TypeIndex>;
}
namespace hdr::math {
  template<typename A, typename B>
  struct Compare<tti::TypeIndex<A>, tti::TypeIndex<B>> {
    using type = ::hdr::std::FromStdBool<(tti::TypeIndex<A>::value < tti::TypeIndex<B>::value)>;
  };
}
namespace TestTTI {
  using namespace tti;
  using cmp = ::hdr::math::natural_order;
  using IndexFoo     = TypeIndex<Foo>;
  using IndexBar     = TypeIndex<Bar>;
  using IsFooSmaller = Apply<cmp::smaller, IndexFoo, IndexBar>;
  using IsBarSmaller = Apply<cmp::smaller, IndexBar, IndexFoo>;
  using IsNotEqual   = Apply<cmp::unequal, IndexBar, IndexFoo>;
  static_assert(::hdr::math::ValueOf<IsFooSmaller>::value ^ ::hdr::math::ValueOf<IsBarSmaller>::value);
  static_assert(::hdr::math::ValueOf<IsNotEqual>::value);
};
#endif

int main() {}
