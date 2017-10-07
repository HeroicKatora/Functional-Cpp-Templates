#include "hdr/types/array.hpp"
#include "hdr/math.hpp"
#include "hdr/assert.hpp"

namespace Test {
  using namespace ::hdr::std;
  using namespace ::hdr::assert;
  using namespace ::hdr::math;
  using namespace ::hdr::array;
  namespace ArrayLength {
    using empty = Empty;
    using oneel = Array<empty>;
    using twoel = Array<oneel, oneel>;
    Assert<Equal<Apply<len, twoel>, Value<2>>>;

    struct Element;
    using longlist = Array<Element, Element, Element>;
    Assert<Equal<Apply<len, longlist>,   Value<3>>>;
    using longerlist = Apply<concat, longlist, longlist>;
    Assert<Equal<Apply<len, longerlist>, Value<6>>>;
  }

  namespace ArrayFolding {
    using ElementList = Array<Value<1>, Value<2>, Value<3>>;
    using sum = Apply<fold, plus, Zero, ElementList>;
    Assert<Equal<sum, Value<6>>>;
  }

  namespace ArrayMapping {
    using ElementList = Array<Value<1>, Value<2>, Value<3>>;
    using AddedOne    = Apply<map, Apply<plus, One>, ElementList>;
    using sum         = Apply<fold, plus, Zero, AddedOne>;
    Assert<Equal<sum, Value<9>>>;
  }

  namespace ArrayMonad {
    template<typename T> struct Double
      { using type = Array<T, T>; };
    using double_elements = TypeFunction<Double>;
    using simple_list = Apply<return_, Value<1>>;
    Assert<Same<simple_list, Array<Value<1>>>>;

    using bound_once = Apply<bind, simple_list, double_elements>;
    Assert<Same<bound_once, Array<Value<1>, Value<1>>>>;

    using operator_list = Array<double_elements>;
    using two_op_list   = Apply<bind, operator_list, double_elements>;
    using four_op_list  = Apply<bind, two_op_list,   double_elements>;
    using eight_op_list = Apply<bind, four_op_list,  double_elements>;
    using op_256x_elements = Apply<fold, kleisli, return_, eight_op_list>;

    using extensive_list = Apply<bind, simple_list, op_256x_elements>;
    Assert<Equal<Apply<len, extensive_list>, Value<256>>>;

    using crash_test = Apply<bind, extensive_list, op_256x_elements>;
    Assert<Equal<Apply<len, crash_test>, Value<(1 << 16)>>>;
  }

}

int main() {}
