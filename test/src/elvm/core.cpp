#include "hdr/elvm/core.hpp"

namespace Test {
  using namespace ::hdr::elvm;
  using _value::value;

  using Initial = Registers<>;
  using Stored = Apply<mov, A, Unsigned<5>, Initial>;
  static_assert(Equal<Unsigned<5>, Apply<value, A, Stored>>::value);
  using Added = Apply<add, A, Unsigned<5>, Stored>;
  static_assert(Equal<Unsigned<10>, Apply<value, A, Added>>::value);
  using Subbed = Apply<sub, A, Unsigned<10>, Added>;
  static_assert(Equal<Unsigned<0>, Apply<value, A, Subbed>>::value);
}

int main() {}
