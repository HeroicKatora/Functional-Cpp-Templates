#include "hdr/core/std.hpp"
#include "hdr/core/match.hpp"
#include "hdr/math.hpp"


namespace Main {
  struct Foo;
  struct Bar;
  using namespace hdr::match;
  using ::hdr::math::Same;
  using ::hdr::maybe::isJust;
  using ::hdr::maybe::isNothing;

  using FooPlaceholder = Placeholder<Foo>;
  using BarPlaceholder = Placeholder<Bar>;
  template<typename A, typename B> struct Pair;
  template<typename A, typename B> struct Diff;

  namespace DecomposeUsage {
    /** Mostly for internal sanity testing during implementation.
     *  Testing composition of types with the decompose function, breaking them
     *  into a maybe kv-pair. This first ensure pattern matching works, then
     *  tests the actual structure of matches and related functions.
     */
    // Templates match matching patterns
    using DecFir = Apply<decompose, Pair<FooPlaceholder, BarPlaceholder>, Pair<bool, int>>;
    using DecSec = Apply<decompose, Pair<bool, BarPlaceholder>,           Pair<bool, int>>;
    using DecThi = Apply<decompose, Pair<bool, int>,  Pair<bool, int>>;
    using DecFou = Apply<decompose, PlaceholderAny,   Pair<bool, int>>;
    static_assert(Apply<isJust, DecFir>::value);
    static_assert(Apply<isJust, DecSec>::value);
    static_assert(Apply<isJust, DecThi>::value);
    static_assert(Apply<isJust, DecFou>::value);

    // Templates don't match non-matching patterns
    using FailFi = Apply<decompose, unsigned int,     unsigned short>;
    using FailSe = Apply<decompose, Pair<bool, int>,  Pair<int, bool>>;
    using FailTh = Apply<decompose, Pair<bool, int>,  unsigned short>;
    using FailFo = Apply<decompose, Pair<bool, int>,  Diff<bool, int>>;
    static_assert(Apply<isNothing, FailFi>::value);
    static_assert(Apply<isNothing, FailSe>::value);
    static_assert(Apply<isNothing, FailTh>::value);
    static_assert(Apply<isNothing, FailFo>::value);

    // How to use placeholders
    using MDecomposed = Apply<decompose, Pair<FooPlaceholder, BarPlaceholder>,
                                         Pair<bool,           int>>;
    using Decomposed  = Apply<hdr::maybe::fromJust, MDecomposed>;
    // You can access the getter function of the matched pattern type
    using MatchFoo    = Apply<Decomposed::get, Foo>;
    using MatchBar    = Apply<Decomposed::get, Bar>;
    // You can use the ::hdr::match::get function
    using GetMatchFoo = Apply<get, Foo, Decomposed>;
    using GetMatchBar = Apply<get, Bar, Decomposed>;
    // You can use placeholders like preset get functions, this is neat
    // since you can treat the essentially very similar to variables
    using GetPlaceFoo = Apply<FooPlaceholder, Decomposed>;
    using GetPlaceBar = Apply<BarPlaceholder, Decomposed>;
    static_assert(Same<bool, MatchFoo>::value);
    static_assert(Same<int,  MatchBar>::value);
    static_assert(Same<bool, GetMatchFoo>::value);
    static_assert(Same<int,  GetMatchBar>::value);
    static_assert(Same<bool, GetPlaceFoo>::value);
    static_assert(Same<int,  GetPlaceBar>::value);
  }

  namespace WithUsage {
    /** Showcases some use cases for matching, and how to use the primitives
     *  For specific uses of placeholders, see above in DecomposeUsage
     */
    using namespace hdr::match;
    using namespace hdr::std;
    using freematch  = Apply<with, PlaceholderAny, Const<Foo>>;
    using JustResult = Apply<freematch, Foo>;
    static_assert(Apply<isMatched, JustResult>::value);

    struct UniqueNoMatch;
    using nonematch  = Apply<with, UniqueNoMatch,  Const<Foo>>;
    using NoResult   = Apply<nonematch, Foo>;
    static_assert(Apply<isUnmatched, NoResult>::value);

    using truematch  = Apply<with_if, Foo, Const<True>, Const<Bar>>;
    using BarResult  = Apply<fromMatched,   Apply<truematch,  Foo>>;
    static_assert(Same<Bar, BarResult>::value);

    using falsematch = Apply<with_if, Foo, Const<False>, Const<Bar>>;
    using NotResult  = Apply<fromUnmatched, Apply<falsematch, Foo>>;
    static_assert(Same<Foo, NotResult>::value);

    using finallymatch  = Apply<kleisli, nonematch, freematch>;
    using firstmatch    = Apply<kleisli, freematch, nonematch>;
    using againmatch    = Apply<kleisli, freematch, truematch>;

    // All of the above should match any type to Foo and not to None or Bar
    static_assert(Same<Matched<Foo>, Apply<firstmatch, Foo>>::value &&
                  Same<Matched<Foo>, Apply<againmatch, Foo>>::value &&
                  Same<Matched<Foo>, Apply<finallymatch, Foo>>::value);
  }

  namespace WithSyntacticSugar {
    /** Showcases the compressed matching statement with automatic unwrapping
     *  This is what you would be most likely to write in your code if you
     *  intend to have functionality similar to other functional programming
     *  languages with matching.
     */
    using MatchedBool   = Match<     Pair<bool,           int>,
                                With<Pair<FooPlaceholder, BarPlaceholder>, Apply<get, Foo>>
                          >;
    using MatchedInt    = Match<     Pair<bool,           int>,
                                With<Pair<FooPlaceholder, BarPlaceholder>, Apply<get, Bar>>
                          >;
    using defered_match = MatchClause<
                                With<Pair<FooPlaceholder, BarPlaceholder>, Apply<get, Bar>>
                          >;
    using DeferredInt   = Apply<defered_match, Pair<bool, int>>;
    static_assert(Same<bool, MatchedBool>::value);
    static_assert(Same<int,  MatchedInt>::value);
    static_assert(Same<int,  DeferredInt>::value);
  }
};

int main() {};
