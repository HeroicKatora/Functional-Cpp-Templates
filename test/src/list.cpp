#include "hdr/types/list.hpp"
#include "hdr/math.hpp"

namespace Test {
  using namespace ::hdr::std;
  using namespace ::hdr::math;
  using namespace ::hdr::list;
  using empty = EmptyList;
  using oneel = Cons<empty, empty>;
  using twoel = Cons<oneel, oneel>;
  static_assert(Same<Apply<len, twoel>, Value<2>>::value);

  struct Element;
  using longlist = Cons<Element, Cons<Element, Cons<Element, empty>>>;
  static_assert(Same<Apply<len, longlist>,   Value<3>>::value);
  using longerlist = Apply<concat, longlist, longlist>;
  static_assert(Same<Apply<len, longerlist>, Value<6>>::value);

}

int main() {}
