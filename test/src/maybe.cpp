#include "hdr/core.hpp"
#include "hdr/types/maybe.hpp"
#include "hdr/assert.hpp"
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
  using namespace hdr::assert;
  struct FooBar;
  struct Mapped;

  namespace NothingTest {
    using Function = Const<Mapped>;
    using Result   = Apply<bind, Nothing, Function>;
    using Replaced = Apply<fromMaybe, FooBar, Result>;
    Assert<Same<Result, Nothing>>;
    Assert<Same<FooBar, Replaced>>;
    AssertFalse<Apply<isJust, Result>>;
  };

  namespace Bind {
    using Function   = Const<Just<Mapped>>;
    using JustFooBar = Apply<return_, FooBar>;
    using Result     = Apply<bind, JustFooBar, Function>;
    using Unpacked   = Apply<fromJust, Result>;
    using FromMaybe  = Apply<fromMaybe, Void, Result>;
    Assert<Same<Just<Mapped>, Result>   >;
    Assert<Same<Mapped,       Unpacked> >;
    Assert<Same<Mapped,       FromMaybe>>;
    Assert<Apply<isJust, Result>>;
  };

  namespace Fmap {
    using Value  = Just<FooBar>;
    using Result = Apply<fmap, Const<Mapped>, Value>;
    Assert<Same<Just<Mapped>, Result>>;
  };

  namespace Kleisli {
    using Value  = Just<FooBar>;
    using fn     = Const<Just<Mapped>>;
    using right  = Apply<kleisli, return_, fn>;
    using left   = Apply<kleisli, fn, return_>;

    using resfn  = Apply<bind, Value, fn>;
    using resri  = Apply<bind, Value, right>;
    using resle  = Apply<bind, Value, left>;
    Assert<Same<resfn, resri>>;
    Assert<Same<resfn, resle>>;
  };

  namespace Join {
    using Value  = Just<FooBar>;
    using result = Apply<join, Apply<fmap, return_, Value>>;
    Assert<Same<Value, result>>;
  };
};

int main() {};
