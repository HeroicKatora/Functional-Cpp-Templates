#include "hdr/core.hpp"
#include "hdr/math.hpp"

using namespace hdr::std;
using namespace hdr::math;

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

    static_assert(Same<ExpectedPOneTTwo, ActualPOneTTwo>::value);
    static_assert(Same<ExpectedTTwoPOne, ActualTTwoPOne>::value);

    static_assert(Same<Value<-5>, Apply<uminus, Value<5>>>::value);
  };

  // Example from README.md
  namespace Factorial {
    template<auto n> struct factorial {
      constexpr static const int value = n*factorial<n-1>::value;
    };
    template<> struct factorial<0> {
      constexpr static const int value = 1;
    };

    constexpr auto res = Apply<ValueTemplateFunction<factorial>, Value<5>>::value;
    static_assert(res == 120);
  };

  namespace FactorialInType {
    /** Code the factorial purely with these headers and typename arguments.
     *  This makes use of the standard definition of integers with ::hdr::math::Value
     *  (which is internally represented like integral_constant) but its generic
     *  coding style would make it easy to adapt it to other integer definitions.
     *  Should one prefer Peano construction, it's fairly easy to implements it
     *  and also express factorial in its terms.
     */
    using namespace ::hdr::std;
    using namespace ::hdr::math;
    using namespace ::hdr::match;

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

  namespace FactorialTemplateMatching {
    /** This version will only work for ::hdr::math::Value but is visually
     *  incredibly close to the integral template parameter version one could
     *  code naiively. This is supposed to show that adaption of these headers
     *  is possible in reasonably nice terms.
     */
    using namespace ::hdr::std;
    using namespace ::hdr::math;
    template<typename T> struct Factorial;
    using factorial = TypeFunction<Factorial>;
    template<typename T> struct Factorial {
      using type = Apply<mult, T, Apply<factorial, Apply<minus, T, One>>>;
    };
    template<> struct Factorial<Zero> {
      using type = One;
    };
    using res = Apply<factorial, Value<5>>;
    static_assert(Same<Value<120>, res>::value);
  }
};

int main() {}
