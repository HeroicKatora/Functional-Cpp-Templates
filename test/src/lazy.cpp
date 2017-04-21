#include "hdr/core.hpp"
#include "hdr/lazy.hpp"
#include "hdr/math.hpp"
#include <functional>
#include <type_traits>
#include <limits>

using namespace hdr::std;
using namespace hdr::lazy;
using namespace hdr::math;
struct Test {
  struct TestBoxing {
    using Type = Test;
    using Boxed = Apply<freturn, Type>;
    using UnBoxed = Apply<Boxed, Void>;
    static_assert(Same<UnBoxed, Type>::value);
  };

  struct TestLaziness {
    template<typename T> struct DeepRecursion;
    template<typename T> struct DeepRecursion<IntegralConstant<T, T(std::numeric_limits<T>::max())>> { using type = Void; };
    template<typename T, T t> struct DeepRecursion<IntegralConstant<T, t>> { using type = typename DeepRecursion<IntegralConstant<T, t+1>>::type; };
    using FunctionalRecursion = TypeFunction<DeepRecursion>;

    using CharArg = IntegralConstant<unsigned char, 0>;
    using LongArg = IntegralConstant<unsigned long, 0>;
    using OkCompile = Apply<FunctionalRecursion, CharArg>; // Stressful for the compiler, but still works
    // using LongCompile = Apply<FunctionalRecursion, LongArg>; // Won't work

    using LazyRecursion = Apply<lift, FunctionalRecursion>;

    using LazyCharArg = Lazy<CharArg>;
    static_assert(Same<CharArg, Eval<LazyCharArg>>::value);
    using LazyLongArg = Lazy<LongArg>;
    static_assert(Same<LongArg, Eval<LazyLongArg>>::value);

    using LazyResultChar = Apply<LazyRecursion, LazyCharArg>;
    using LazyResultLong = Apply<LazyRecursion, LazyLongArg>;

    static_assert(Same<OkCompile, Eval<LazyResultChar>>::value);
    // static_assert(std::is_same<LongCompile, Eval<LazyResultLong>>::value); // Should not evaluate that
  };

  struct TestLift2 {
    using NumberFour = Signed<4>;
    using NumberEight = Signed<8>;
    using LazyPlus = Apply<lift2, plus>;
    using LazyResult = Apply<LazyPlus, Lazy<NumberFour>, Lazy<NumberFour>>;

    static_assert(Same<NumberEight, Eval<LazyResult>>::value);
  };
};

int main() {}
