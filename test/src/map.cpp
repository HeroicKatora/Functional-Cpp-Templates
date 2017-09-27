#include "hdr/types/map.hpp"

namespace Test {
  using namespace ::hdr::map;
  using namespace ::hdr::std;
  namespace Build {
    using namespace ::hdr::math;
    struct Mapped;
    using EmptyMap  = Apply<empty, number_type>;
    using emptycount = Apply<size, EmptyMap>;
    static_assert(::hdr::math::Same<emptycount, Value<0>>::value);

    using InsertOne = Apply<insert, Value<1>, Mapped, EmptyMap>;
    using onecount = Apply<size, InsertOne>;
    static_assert(::hdr::math::Same<onecount, Value<1>>::value);

    using InsertTwo = Apply<insert, Value<2>, Mapped, InsertOne>;
    using twocount = Apply<size, InsertTwo>;
    static_assert(::hdr::math::Same<twocount, Value<2>>::value);

    using InsertThr = Apply<insert, Value<3>, Mapped, InsertTwo>;
    using InsertFou = Apply<insert, Value<4>, Mapped, InsertThr>;
    using InsertFiv = Apply<insert, Value<5>, Mapped, InsertFou>;

    using count = Apply<size, InsertFiv>;
    static_assert(::hdr::math::Same<count, Value<5>>::value);
  }
}

int main() {}
