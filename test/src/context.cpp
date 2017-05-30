#include "hdr/context/voider.hpp"
#include "hdr/context/recurse.hpp"
#include "hdr/core/math.hpp"

namespace Test {
  using namespace hdr::std;
  using namespace hdr::context;
  using hdr::math::Same;
  namespace Voider {
    struct Arbitrary;
    using namespace hdr::context::voider;
    using context = hdr::context::voider::Voider;
    using state   = Apply<context::init, Void>;
    using other_state = Apply<context::apply, state, Arbitrary>;
    using third_state = Apply<context::apply, other_state, Arbitrary>;
    using result = Apply<context::apply, third_state, End>;
    static_assert(Same<result, Result<Void>>::value);
  }

  namespace Recurse {
    struct Arbitrary;
    using namespace hdr::context::recurse;
    using context = hdr::context::recurse::Recursive;
    using state   = Apply<context::init, id>; // Outer context does no modification
    using initial_state = Apply<context::apply, state, id>;

    namespace Recurse /*Joy of recursive naming*/ {
      using inner_state = Apply<context::apply, initial_state, hdr::context::voider::Voider>;
      using inner_apply = Apply<context::apply, inner_state, Arbitrary>;
      using inner_end   = Apply<context::apply, inner_apply, End>;
      using result      = Apply<context::apply, inner_end, End>;
      static_assert(Same<result, Result<Void>>::value);
    }

    using applied_state = Apply<context::apply, initial_state, Arbitrary>;
    using result = Apply<context::apply, applied_state, End>;
    static_assert(Same<result, Result<Arbitrary>>::value);

    namespace Transform {
      using tstate = Apply<context::init, Const<id>>;
      using initial_state = Apply<context::apply, tstate, Void>;
      using applied_state = Apply<context::apply, initial_state, Arbitrary>;
      using result = Apply<context::apply, applied_state, End>;
      static_assert(Same<result, Result<id>>::value);
    }
  }
}

int main() {}
