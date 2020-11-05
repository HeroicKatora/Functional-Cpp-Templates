#include "hdr/types/array.hpp"
#include "hdr/math.hpp"

namespace Test {
  using namespace ::hdr::std;
  using namespace ::hdr::math;
  using namespace ::hdr::array;
  namespace ArrayLength {
    using empty = Empty;
    using oneel = Array<empty>;
    using twoel = Array<oneel, oneel>;
    static_assert(Equal<Apply<len, twoel>, Value<2>>::value);

    struct Element;
    using longlist = Array<Element, Element, Element>;
    static_assert(Equal<Apply<len, longlist>,   Value<3>>::value);
    using longerlist = Apply<concat, longlist, longlist>;
    static_assert(Equal<Apply<len, longerlist>, Value<6>>::value);
  }

  namespace ArrayFolding {
    using ElementList = Array<Value<1>, Value<2>, Value<3>>;
    using sum = Apply<fold, plus, Zero, ElementList>;
    static_assert(Equal<sum, Value<6>>::value);
  }

  namespace ArrayMapping {
    using ElementList = Array<Value<1>, Value<2>, Value<3>>;
    using AddedOne    = Apply<map, Apply<plus, One>, ElementList>;
    using sum         = Apply<fold, plus, Zero, AddedOne>;
    static_assert(Equal<sum, Value<9>>::value);
  }

  namespace ArrayMonad {
    template<typename T> struct Double
      { using type = Array<T, T>; };
    using double_elements = TypeFunction<Double>;
    using simple_list = Apply<return_, Value<1>>;
    static_assert(Same<simple_list, Array<Value<1>>>::value);

    using bound_once = Apply<bind, simple_list, double_elements>;
    static_assert(Same<bound_once, Array<Value<1>, Value<1>>>::value);

    using operator_list = Array<double_elements>;
    using two_op_list   = Apply<bind, operator_list, double_elements>;
    using four_op_list  = Apply<bind, two_op_list,   double_elements>;
    using eight_op_list = Apply<bind, four_op_list,  double_elements>;
    using op_256x_elements = Apply<fold, kleisli, return_, eight_op_list>;

    using extensive_list = Apply<bind, simple_list, op_256x_elements>;
    static_assert(Same<Apply<len, extensive_list>, Value<size_t(256)>>::value);

    using crash_test = Apply<bind, extensive_list, op_256x_elements>;
    static_assert(Same<Apply<len, crash_test>, Value<size_t(1 << 16)>>::value);
  }
  
}

int main() {}
