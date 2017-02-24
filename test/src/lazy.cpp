#include "hdr/std.hpp"
#include "hdr/lazy.hpp"
#include "hdr/math.hpp"
#include <functional>
#include <type_traits>
#include <limits>

using namespace hdr::std;
using namespace hdr::lazy;
using namespace hdr::math;
using std::is_same_v;
struct Test {
  struct TestBoxing {
    using Type = Test;
    using Boxed = Apply<freturn, Type>;
    using UnBoxed = Apply<Boxed, Void>;
    static_assert(is_same_v<UnBoxed, Type>);
  };

  struct TestLaziness {
    template<typename T> struct DeepRecursion;
    template<typename T> struct DeepRecursion<IntegralConstant<T, T(-1)>> { using type = Void; };
    template<typename T, T t> struct DeepRecursion<IntegralConstant<T, t>> { using type = typename DeepRecursion<IntegralConstant<T, t+1>>::type; };
    using FunctionalRecursion = TypeFunction<DeepRecursion>;

    using CharArg = IntegralConstant<unsigned char, 0>;
    using LongArg = IntegralConstant<unsigned long, 0>;
    using OkCompile = Apply<FunctionalRecursion, CharArg>; // Stressful for the compiler, but still works
    // using LongCompile = Apply<FunctionalRecursion, LongArg>; // Won't work

    using LazyRecursion = Apply<lift, FunctionalRecursion>;

    using LazyCharArg = Lazy<CharArg>;
    static_assert(is_same_v<CharArg, Eval<LazyCharArg>>);
    using LazyLongArg = Lazy<LongArg>;
    static_assert(is_same_v<LongArg, Eval<LazyLongArg>>);

    using LazyResultChar = Apply<LazyRecursion, LazyCharArg>;
    using LazyResultLong = Apply<LazyRecursion, LazyLongArg>;

    static_assert(is_same_v<OkCompile, Eval<LazyResultChar>>);
    // static_assert(std::is_same<LongCompile, Eval<LazyResultLong>>::value); // Should not evaluate that
  };

  struct TestLift2 {
    using NumberFour = Signed<4>;
    using NumberEight = Signed<8>;
    using LazyPlus = Apply<lift2, plus>;
    using LazyResult = Apply<LazyPlus, Lazy<NumberFour>, Lazy<NumberFour>>;

    static_assert(is_same_v<NumberEight, Eval<LazyResult>>);
  };
};

int main() {}
