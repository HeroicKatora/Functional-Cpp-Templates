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
    using Overwritten = Apply<mov, A, B, Subbed>;
    Assert<Equal<Unsigned<0>, Apply<value, A, Overwritten>>>;
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
    constexpr char buffer[] = "!#";
    using Input = Stdin<buffer>;
    using Initial = Apply<peek, A, Registers<>, Input>;
    Assert<Equal<Unsigned<'!'>, Apply<value, A, Initial>>>;
    using Step = Apply<advance, Input>;
    using Advanced = Apply<advance, Step>;
    using ReadNull = Apply<peek, A, Initial, Advanced>;
    Assert<Equal<Unsigned<0>, Apply<value, A, ReadNull>>>;
    using AtEnd = Apply<advance, Advanced>;
    using ReadEOF = Apply<peek, A, ReadNull, AtEnd>;
    Assert<Equal<Unsigned<0>, Apply<value, A, ReadEOF>>>;
  }

  namespace JumpEq {
    using Register = Apply<mov, A, Unsigned<5>, Registers<>>;
    using Target = Apply<jmpeq, Unsigned<0xFF>, A, Unsigned<5>, Register, Unsigned<0xAA>>;
    Assert<Equal<Target, Unsigned<0xFF>>>;
    using Continue = Apply<jmpeq, Unsigned<0xFF>, A, Unsigned<0>, Register, Unsigned<0xAA>>;
    Assert<Equal<Continue, Unsigned<0xAA>>>;
  }

  namespace JumpNe {
    using Register = Apply<mov, A, Unsigned<5>, Registers<>>;
    using Target = Apply<jmpne, Unsigned<0xFF>, A, Unsigned<5>, Register, Unsigned<0xAA>>;
    Assert<Equal<Target, Unsigned<0xAA>>>;
    using Continue = Apply<jmpne, Unsigned<0xFF>, A, Unsigned<0>, Register, Unsigned<0xAA>>;
    Assert<Equal<Continue, Unsigned<0xFF>>>;
  }

  namespace Compare {
    using Register = Apply<mov, A, Unsigned<0xFF>, Registers<>>;
    using CmpEqual = Apply<eq, B, C, Register>;
    using CmpEqualN = Apply<eq, A, C, Register>;
    Assert<Equal<Unsigned<1>, Apply<value, B, CmpEqual>>>;
    Assert<Equal<Unsigned<0>, Apply<value, A, CmpEqualN>>>;
    using Unequal = Apply<ne, B, A, Register>;
    using UnequalN = Apply<ne, B, C, Register>;
    Assert<Equal<Unsigned<1>, Apply<value, B, Unequal>>>;
    Assert<Equal<Unsigned<0>, Apply<value, B, UnequalN>>>;
    using Smaller = Apply<lt, B, A, Register>;
    using SmallerN = Apply<lt, B, C, Register>;
    Assert<Equal<Unsigned<1>, Apply<value, B, Smaller>>>;
    Assert<Equal<Unsigned<0>, Apply<value, B, SmallerN>>>;
    using Greater = Apply<gt, A, B, Register>;
    using GreaterN = Apply<gt, B, C, Register>;
    Assert<Equal<Unsigned<1>, Apply<value, A, Greater>>>;
    Assert<Equal<Unsigned<0>, Apply<value, B, GreaterN>>>;
    using SmallerEqual = Apply<le, B, A, Register>;
    using SmallerEqualN = Apply<le, A, C, Register>;
    Assert<Equal<Unsigned<1>, Apply<value, B, SmallerEqual>>>;
    Assert<Equal<Unsigned<0>, Apply<value, A, SmallerEqualN>>>;
    using GreaterEqual = Apply<ge, B, C, Register>;
    using GreaterEqualN = Apply<ge, B, A, Register>;
    Assert<Equal<Unsigned<1>, Apply<value, B, GreaterEqual>>>;
    Assert<Equal<Unsigned<0>, Apply<value, B, GreaterEqualN>>>;
  }
}

int main() {}
