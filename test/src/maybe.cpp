#include "hdr/std.hpp"
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
using ::hdr::maybe::freturn;
using ::std::is_same_v;

namespace Test {
  struct FooBar;
  struct Mapped;

  namespace NothingTest {
    using Function = Const<Mapped>;
    using Result   = Apply<bind, Nothing, Function>;
    using Replaced = Apply<fromMaybe, FooBar, Result>;
    static_assert(is_same_v<Result, Nothing>);
    static_assert(is_same_v<FooBar, Replaced>);
    static_assert(!Apply<isJust, Result>::value);
  };

  namespace Bind {
    using Function   = Const<Just<Mapped>>;
    using JustFooBar = Apply<freturn, FooBar>;
    using Result     = Apply<bind, JustFooBar, Function>;
    using Unpacked   = Apply<fromJust, Result>;
    using FromMaybe  = Apply<fromMaybe, Void, Result>;
    static_assert(is_same_v<Just<Mapped>, Result>);
    static_assert(is_same_v<Mapped, Unpacked>);
    static_assert(is_same_v<Mapped, FromMaybe>);
    static_assert(Apply<isJust, Result>::value);
  };

  namespace Fmap {
    using Value  = Just<FooBar>;
    using Result = Apply<fmap, Const<Mapped>, Value>;
    static_assert(is_same_v<Just<Mapped>, Result>);
  };
};

int main() {};
