#include "hdr/types/list.hpp"
#include "hdr/math.hpp"
#include "hdr/assert.hpp"

namespace Test {
  using namespace ::hdr::std;
  using namespace ::hdr::math;
  using namespace ::hdr::list;
  using namespace ::hdr::assert;
  namespace ListLength {
    using empty = EmptyList;
    using oneel = Cons<empty, empty>;
    using twoel = Cons<oneel, oneel>;
    Assert<Equal<Apply<len, twoel>, Value<2>>>;

    struct Element;
    using longlist = Cons<Element, Cons<Element, Cons<Element, empty>>>;
    Assert<Equal<Apply<len, longlist>,   Value<3>>>;
    using longerlist = Apply<concat, longlist, longlist>;
    Assert<Equal<Apply<len, longerlist>, Value<6>>>;
  }

  namespace ListFolding {
    using ElementList = Cons<Value<1>, Cons<Value<2>, Cons<Value<3>, EmptyList>>>;
    using sum = Apply<fold, plus, Zero, ElementList>;
    Assert<Equal<sum, Value<6>>>;
  }

  namespace ListMapping {
    using ElementList = Cons<Value<1>, Cons<Value<2>, Cons<Value<3>, EmptyList>>>;
    using AddedOne    = Apply<map, Apply<plus, One>, ElementList>;
    using sum         = Apply<fold, plus, Zero, AddedOne>;
    Assert<Equal<sum, Value<9>>>;
  }

  namespace ListMonad {
    template<typename T> struct Double
      { using type = Cons<T, Cons<T, EmptyList>>; };
    using double_elements = TypeFunction<Double>;
    using simple_list = Apply<return_, Value<1>>;
    Assert<Same<simple_list, Cons<Value<1>, EmptyList>>>;

    using bound_once = Apply<bind, simple_list, double_elements>;
    Assert<Same<bound_once, Cons<Value<1>, Cons<Value<1>, EmptyList>>>>;

    using operator_list = Cons<double_elements, EmptyList>;
    using two_op_list   = Apply<bind, operator_list, double_elements>;
    using four_op_list  = Apply<bind, two_op_list,   double_elements>;
    using eight_op_list = Apply<bind, four_op_list,  double_elements>;
    using op_256x_elements = Apply<fold, kleisli, return_, eight_op_list>;

    using extensive_list = Apply<bind, simple_list, op_256x_elements>;
    Assert<Equal<Apply<len, extensive_list>, Value<256>>>;

    // using crash_test = Apply<bind, extensive_list, op_256x_elements>;
    // Assert<Equal<Apply<len, crash_test>, Value<(1 << 16)>>>;
  }
}

int main() {}
