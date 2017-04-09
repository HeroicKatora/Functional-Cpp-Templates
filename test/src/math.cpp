#include "hdr/std.hpp"
#include "hdr/math.hpp"
#include <type_traits>

using namespace hdr::std;
using namespace hdr::math;
using std::is_same_v;

template<signed n> struct factorial {
  constexpr static const int value = n*factorial<n-1>::value;
};

template<> struct factorial<0> {
  constexpr static const int value = 1;
};

struct Test {
  // Improved version for test/src/std.cpp
  struct StdTest {
    using FunctionalOne = Apply<plus, Value<1>>;
    using FunctionalTwo = Apply<mult, Value<2>>;

    using POneTTwo = Apply<compose, FunctionalTwo, FunctionalOne>;
    using TTwoPOne = Apply<compose, FunctionalOne, FunctionalTwo>;

    using Input = Value<1>;
    using ExpectedPOneTTwo = Value<4>;
    using ExpectedTTwoPOne = Value<3>;

    using ActualPOneTTwo = Apply<POneTTwo, Input>;
    using ActualTTwoPOne = Apply<TTwoPOne, Input>;

    static_assert(is_same_v<ExpectedPOneTTwo, ActualPOneTTwo>);
    static_assert(is_same_v<ExpectedTTwoPOne, ActualTTwoPOne>);
  };

  // Example from README.md
  struct Factorial {
    using ffactorial = ValueTemplateFunction<factorial>;
    constexpr static auto res = Apply<ffactorial, Value<5>>::value;
    static_assert(res == 120);
  };
};

int main() {}
