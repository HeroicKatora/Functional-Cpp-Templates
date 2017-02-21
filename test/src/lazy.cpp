#include "hdr/lazy.hpp"
#include <functional>
#include <type_traits>

using namespace hdrstd;
using namespace hdrstd::lazy;
using std::is_same_v;
struct Test {
  using Type = Test;
  using Boxed = Apply<freturn, Type>;
  using UnBoxed = Apply<Boxed, Void>;
  static_assert(is_same_v<UnBoxed, Type>);

  template<typename T> struct DeepRecursion;
  template<typename T> struct DeepRecursion<std::integral_constant<T, T(-1)>> { using type = Void; };
  template<typename T, T t> struct DeepRecursion<std::integral_constant<T, t>> { using type = typename DeepRecursion<std::integral_constant<T, t+1>>::type; };
  using FunctionalRecursion = TypeFunction<DeepRecursion>;

  using CharArg = std::integral_constant<unsigned char, 0>;
  using LongArg = std::integral_constant<unsigned long, 0>;
  using OkCompile = Apply<FunctionalRecursion, CharArg>; // Stressful for the compiler, but still works
  // using LongCompile = Apply<FunctionalRecursion, LongArg>; // Won't work

  using LazyRecursion = Apply<lift, FunctionalRecursion>;

  using LazyCharArg = Apply<freturn, CharArg>;
  static_assert(is_same_v<CharArg, Eval<LazyCharArg>>);
  using LazyLongArg = Apply<freturn, LongArg>;
  static_assert(is_same_v<LongArg, Eval<LazyLongArg>>);

  using LazyResultChar = Apply<LazyRecursion, LazyCharArg>;
  using LazyResultLong = Apply<LazyRecursion, LazyLongArg>;

  static_assert(is_same_v<OkCompile, Eval<LazyResultChar>>);
  // static_assert(std::is_same<LongCompile, Eval<LazyResultLong>>::value); // Should not evaluate that
};

int main() {}
