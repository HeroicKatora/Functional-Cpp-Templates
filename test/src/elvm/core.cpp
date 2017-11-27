#include "hdr/elvm/core.hpp"

namespace Test {
  using namespace ::hdr::elvm;
  using _value::value;

  namespace RegisterUsage {
    using Initial = Registers<>;
    using Stored = Apply<mov, A, Unsigned<5>, Initial>;
    static_assert(Equal<Unsigned<5>, Apply<value, A, Stored>>::value);
    using Added = Apply<add, A, Unsigned<5>, Stored>;
    static_assert(Equal<Unsigned<10>, Apply<value, A, Added>>::value);
    using Subbed = Apply<sub, A, Unsigned<10>, Added>;
    static_assert(Equal<Unsigned<0>, Apply<value, A, Subbed>>::value);
  }

  namespace Memory {
    using Data = Array<Unsigned<42>, Unsigned<69>>;
    using Initial = Registers<>;
    using Memory = Apply<memory, Data>;
    using Loaded = Apply<load, A, Unsigned<0>, Initial, Memory>;
    static_assert(Equal<Unsigned<42>, Apply<value, A, Loaded>>::value);
  }
}

int main() {}
