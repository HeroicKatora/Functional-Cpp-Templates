#include "hdr/core.hpp"
#include "hdr/math.hpp"
#include <type_traits>

using namespace hdr::std;
using namespace hdr::math;
using std::is_same_v;

namespace Test {
  // Improved version for test/src/std.cpp
  namespace StdTest {
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
  namespace Factorial {
    template<signed n> struct factorial {
      constexpr static const int value = n*factorial<n-1>::value;
    };
    template<> struct factorial<0> {
      constexpr static const int value = 1;
    };

    constexpr auto res = Apply<ValueTemplateFunction<factorial>, Value<5>>::value;
    static_assert(res == 120);
  };

  namespace FactorialInType {
    using namespace ::hdr::std;
    using namespace ::hdr::math;
    using namespace ::hdr::match;
    using namespace ::hdr::lambda;

    struct N; // Useful typename

    template<typename T> struct Factorial;     // Forward declare
    using factorial = TypeFunction<Factorial>; // Se we can do this

    using recfac    = Apply<compose, factorial, Apply<flip, minus, One>>;
    using matchfac  = Apply<compose, recfac,    Apply<get, N>>;

    template<typename T> struct Factorial {
      using PN = Placeholder<N>;
      using type = Match<T, With<Zero, Const <One>                              >,
                            With<PN,   Apply<compose, Apply<mult, T>, matchfac> >
                        >;
    };
    using res = Apply<factorial, Value<5>>;
    static_assert(Same<res, Value<120>>::value);
  }
};

int main() {}
