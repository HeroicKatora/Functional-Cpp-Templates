#include "hdr/core.hpp"
#include "hdr/types/maybe.hpp"
#include <type_traits>

using ::hdr::std::Const;
using ::hdr::std::Apply;
using ::hdr::std::Void;
using ::hdr::maybe::Just;
using ::hdr::maybe::Nothing;

using ::hdr::maybe::isJust;
using ::hdr::maybe::fromJust;
using ::hdr::maybe::fromMaybe;
using ::hdr::maybe::bind;
using ::hdr::maybe::fmap;
using ::hdr::maybe::join;
using ::hdr::maybe::kleisli;
using ::hdr::maybe::return_;
using ::hdr::math::Same;

namespace Test {
  struct FooBar;
  struct Mapped;

  namespace NothingTest {
    using Function = Const<Mapped>;
    using Result   = Apply<bind, Nothing, Function>;
    using Replaced = Apply<fromMaybe, FooBar, Result>;
    static_assert(Same<Result, Nothing>::value);
    static_assert(Same<FooBar, Replaced>::value);
    static_assert(!Apply<isJust, Result>::value);
  };

  namespace Bind {
    using Function   = Const<Just<Mapped>>;
    using JustFooBar = Apply<return_, FooBar>;
    using Result     = Apply<bind, JustFooBar, Function>;
    using Unpacked   = Apply<fromJust, Result>;
    using FromMaybe  = Apply<fromMaybe, Void, Result>;
    static_assert(Same<Just<Mapped>, Result>   ::value);
    static_assert(Same<Mapped,       Unpacked> ::value);
    static_assert(Same<Mapped,       FromMaybe>::value);
    static_assert(Apply<isJust, Result>::value);
  };

  namespace Fmap {
    using Value  = Just<FooBar>;
    using Result = Apply<fmap, Const<Mapped>, Value>;
    static_assert(Same<Just<Mapped>, Result>::value);
  };

  namespace Kleisli {
    using Value  = Just<FooBar>;
    using fn     = Const<Just<Mapped>>;
    using right  = Apply<kleisli, return_, fn>;
    using left   = Apply<kleisli, fn, return_>;

    using resfn  = Apply<bind, Value, fn>;
    using resri  = Apply<bind, Value, right>;
    using resle  = Apply<bind, Value, left>;
    static_assert(Same<resfn, resri>::value &&
                  Same<resfn, resle>::value);
  };

  namespace Join {
    using Value  = Just<FooBar>;
    using result = Apply<join, Apply<fmap, return_, Value>>;
    static_assert(Same<Value, result>::value);
  };
};

int main() {};
