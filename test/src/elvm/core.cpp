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
    using Subbed = Apply<sub, A, Unsigned<10>, Added>;
    Assert<Equal<Unsigned<0>, Apply<value, A, Subbed>>>;
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
}

int main() {}
