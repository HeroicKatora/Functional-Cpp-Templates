#include "hdr/elvm/core.hpp"
#include "hdr/assert.hpp"

namespace Test {
  using namespace ::hdr::elvm;
  using namespace ::hdr::assert;
  using _value::value;
  using _load::get_memory;

  namespace RegisterUsage {
    using Initial = Registers<>;
    using Stored = Apply<mov, A, Unsigned<5>, Initial>;
    Assert<Equal<Unsigned<5>, Apply<value, A, Stored>>>;
    using Added = Apply<add, A, Unsigned<5>, Stored>;
    Assert<Equal<Unsigned<10>, Apply<value, A, Added>>>;
    using Subbed = Apply<sub, A, Unsigned<7>, Added>;
    Assert<Equal<Unsigned<3>, Apply<value, A, Subbed>>>;
  }

  namespace Memory {
    using Data = Array<Unsigned<42>, Unsigned<69>>;
    using Initial = Registers<>;
    using Memory = Apply<memory, Data>;
    using Loaded = Apply<load, A, Unsigned<0>, Initial, Memory>;
    Assert<Equal<Unsigned<42>, Apply<value, A, Loaded>>>;
    using Stored = Apply<store, A, Unsigned<0xBEEF>, Loaded, Memory>;
    Assert<Equal<Unsigned<42>, Apply<get_memory, Unsigned<0xBEEF>, Stored>>>;
  }

  namespace Output {
    using Initial = Apply<mov, A, Unsigned<20>, Registers<>>;
    using OBuf0 = Apply<putcop, A, Initial, Stdout>;
    Assert<Equal<Unsigned<20>, Apply<::hdr::array::last, OBuf0>>>;
    using OBuf1 = Apply<putcop, B, Initial, OBuf0>;
    Assert<Equal<Unsigned<0>, Apply<::hdr::array::last, OBuf1>>>;
  }

  namespace Input {
    constexpr char buffer[] = "!";
    using Input = Stdin<buffer>;
    using Initial = Apply<peek, A, Registers<>, Input>;
    Assert<Equal<Unsigned<'!'>, Apply<value, A, Initial>>>;
    using Advanced = Apply<advance, Input>;
    using ReadNull = Apply<peek, A, Initial, Advanced>;
    Assert<Equal<Unsigned<0>, Apply<value, A, ReadNull>>>;
    using AtEnd = Apply<advance, Advanced>;
    using ReadEOF = Apply<peek, A, ReadNull, AtEnd>;
    Assert<Equal<Unsigned<0>, Apply<value, A, ReadEOF>>>;
  }

  namespace Jump {
    using Register = Apply<mov, A, Unsigned<5>, Registers<>>;
    using Target = Apply<jmpeq, Unsigned<1>, A, Unsigned<5>, Register, Unsigned<0>>;
    Assert<Equal<Target, Unsigned<1>>>;
    using Continue = Apply<jmpeq, Unsigned<1>, A, Unsigned<0>, Register, Unsigned<0>>;
    Assert<Equal<Continue, Unsigned<0>>>;
  }
}

int main() {}
