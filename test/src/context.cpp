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
}

int main() {}
