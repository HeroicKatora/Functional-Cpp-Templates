#include "hdr/core/std.hpp"
#include <type_traits>

using namespace hdr::std;

struct Test {
  // Define some test functions, these could be done better with hdr/math but we don't include it here
  template<typename T> struct PlusOne;
  template<typename T, T t> struct PlusOne<std::integral_constant<T, t>> { using type = std::integral_constant<T, t+1>; };
  template<typename T> struct TimesTwo;
  template<typename T, T t> struct TimesTwo<std::integral_constant<T, t>> { using type = std::integral_constant<T, 2*t>; };

  using FunctionalOne = TypeFunction<PlusOne>;
  using FunctionalTwo = TypeFunction<TimesTwo>;

  using POneTTwo = Apply<compose, FunctionalTwo, FunctionalOne>;
  using TTwoPOne = Apply<compose, FunctionalOne, FunctionalTwo>;

  using Input = std::integral_constant<int, 1>;
  using ExpectedPOneTTwo = std::integral_constant<int, 4>;
  using ExpectedTTwoPOne = std::integral_constant<int, 3>;

  using ActualPOneTTwo = Apply<POneTTwo, Input>;
  using ActualTTwoPOne = Apply<TTwoPOne, Input>;

  static_assert(std::is_same<ExpectedPOneTTwo, ActualPOneTTwo>::value);
  static_assert(std::is_same<ExpectedTTwoPOne, ActualTTwoPOne>::value);
};

int main() {}
