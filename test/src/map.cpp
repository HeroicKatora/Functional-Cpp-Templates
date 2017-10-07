#include "hdr/types/map.hpp"
#include "hdr/assert.hpp"

namespace Test {
  using namespace ::hdr::map;
  using namespace ::hdr::std;
  using namespace ::hdr::assert;
  namespace Build {
    using namespace ::hdr::math;
    struct Mapped;
    using EmptyMap  = Apply<empty, number_type>;
    using emptycount = Apply<size, EmptyMap>;
    Assert<Same<emptycount, Value<0>>>;

    using InsertOne = Apply<insert, Value<1>, Mapped, EmptyMap>;
    using onecount = Apply<size, InsertOne>;
    Assert<Same<onecount, Value<1>>>;

    using InsertTwo = Apply<insert, Value<2>, Mapped, InsertOne>;
    using twocount = Apply<size, InsertTwo>;
    Assert<Same<twocount, Value<2>>>;

    using InsertThr = Apply<insert, Value<3>, Mapped, InsertTwo>;
    using InsertFou = Apply<insert, Value<4>, Mapped, InsertThr>;
    using InsertFiv = Apply<insert, Value<5>, Mapped, InsertFou>;

    using count = Apply<size, InsertFiv>;
    Assert<Same<count, Value<5>>>;
  }
}

int main() {}
