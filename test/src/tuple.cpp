#include "hdr/types/tuple.hpp"
#include "hdr/math.hpp"

namespace Test {
  struct A; struct B;
  using namespace ::hdr::std;
  using namespace ::hdr::tuple;
  namespace create {
    using Simple = Apply<tup, A, B>;

    static_assert(::std::is_same<Simple, Tuple<A, B>>::value);
  }
  namespace retrieve {
    using Simple = Apply<tup, A, B>;
    using First  = Apply<first, Simple>;
    using Second = Apply<second, Simple>;

    static_assert(::std::is_same<First, A>::value);
    static_assert(::std::is_same<Second, B>::value);
  }
  namespace call {
    using namespace ::hdr::math;
    using Simple  = Apply<tup, Zero, One>;
    using Swapped = Apply<swap, Simple>;
    using uncurriedPlus = Apply<uncurry, plus>;
    using UncurriedSum  = Apply<uncurriedPlus, Simple>;
    using curriedPlus = Apply<curry, uncurriedPlus>;
    using CurriedSum  = Apply<curriedPlus, Value<3>, Value<39>>;

    static_assert(::std::is_same<Swapped, Tuple<One, Zero>>::value);
    static_assert(::std::is_same<UncurriedSum, One>::value);
    static_assert(::std::is_same<CurriedSum, Value<42>>::value);
  }
}

int main() {}
