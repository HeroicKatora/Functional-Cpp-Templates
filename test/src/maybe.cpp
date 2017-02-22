#include "hdr/std.hpp"
#include "hdr/types/maybe.hpp"
#include <type_traits>

using ::hdr::std::Const;
using ::hdr::std::Apply;
using ::hdr::maybe::Just;
using ::hdr::maybe::Nothing;
using ::std::is_same_v;

struct Test {
  struct FooBar;
  struct Mapped;

  struct NothingTest {
    using Function = Const<Mapped>;
    using Result = Apply<::hdr::maybe::bind, Nothing, Function>;
    static_assert(is_same_v<Result, Nothing>);
  };

  struct Bind {
    using Function = Const<Mapped>;
    using JustFooBar = Apply<::hdr::maybe::freturn, FooBar>;
    using Result = Apply<::hdr::maybe::bind, JustFooBar, Function>;
    static_assert(is_same_v<Result, Mapped>);
  };
};

int main() {};
