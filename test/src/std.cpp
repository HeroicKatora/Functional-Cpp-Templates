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

  // And here are some more advanced tests, where we use functional concepts.
  using UntilInstant = Apply<until, Const<True>, id, Input>;
  static_assert(std::is_same<UntilInstant, Input>::value);

  // We can convert the standard concept into a function yielding functional
  // bools, which can be used like an `if`
  using is_same_f = Apply<compose,
    Apply<compose, boolify>,
    TemplateFunction<std::is_same>>;
  using Four = std::integral_constant<int, 4>;
  using is_four = Apply<is_same_f, Four>;
  using Incremented = Apply<until, is_four, FunctionalOne, Input>;
  static_assert(std::is_same<Incremented, Four>::value);
};

int main() {}
