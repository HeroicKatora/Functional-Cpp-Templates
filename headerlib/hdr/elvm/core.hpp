#ifndef HDR_ELVM_CORE_HPP
#define HDR_ELVM_CORE_HPP
/** An implementation of the virtual machine from the [elvm project][1].
 *
 *  [1]: https://github.com/shinh/elvm/
 */
#include "hdr/core.hpp"
#include "hdr/math.hpp"
#include "hdr/types/map.hpp"

namespace hdr::elvm {
using namespace ::hdr::std;
using namespace ::hdr::math;

template<
  typename A=Unsigned<0>,
  typename B=Unsigned<0>,
  typename C=Unsigned<0>,
  typename D=Unsigned<0>,
  typename SP=Unsigned<0>,
  typename BP=Unsigned<0>>
struct Registers;

struct A; struct B; struct C; struct D; struct SP; struct BP;

// Get the value of an immediate or from a register
namespace _value {
  template<
    typename Src /* register or immediate*/,
    typename Registers>
  struct Value;
  using value = TypeFunction<Value>;

  template<
    typename a, typename b, typename c, typename d, typename sp, typename bp>
  struct Value<A, Registers<a, b, c, d, sp, bp>> {
    using type = a;
  };

  template<
    typename a, typename b, typename c, typename d, typename sp, typename bp>
  struct Value<B, Registers<a, b, c, d, sp, bp>> {
    using type = b;
  };

  template<
    typename a, typename b, typename c, typename d, typename sp, typename bp>
  struct Value<C, Registers<a, b, c, d, sp, bp>> {
    using type = c;
  };

  template<
    typename a, typename b, typename c, typename d, typename sp, typename bp>
  struct Value<D, Registers<a, b, c, d, sp, bp>> {
    using type = d;
  };

  template<
    typename a, typename b, typename c, typename d, typename sp, typename bp>
  struct Value<SP, Registers<a, b, c, d, sp, bp>> {
    using type = sp;
  };

  template<
    typename a, typename b, typename c, typename d, typename sp, typename bp>
  struct Value<BP, Registers<a, b, c, d, sp, bp>> {
    using type = bp;
  };

  template<
    unsigned v,
    typename Registers>
  struct Value<Unsigned<v>, Registers> {
    using type = Unsigned<v>;
  };
}

// Store the value of an immediate into a register
namespace _save {
  template<
    typename Src /* immediate */,
    typename Dst /* register */,
    typename Registers>
  struct Save;
  using save = TypeFunction<Save>;

  template<
    typename v,
    typename _, typename b, typename c, typename d, typename sp, typename bp>
  struct Save<v, A, Registers<_, b, c, d, sp, bp>> {
    using type = Registers<v, b, c, d, sp, bp>;
  };

  template<
    typename v,
    typename a, typename _, typename c, typename d, typename sp, typename bp>
  struct Save<v, B, Registers<a, _, c, d, sp, bp>> {
    using type = Registers<a, v, c, d, sp, bp>;
  };

  template<
    typename v,
    typename a, typename b, typename _, typename d, typename sp, typename bp>
  struct Save<v, C, Registers<a, b, _, d, sp, bp>> {
    using type = Registers<a, b, v, d, sp, bp>;
  };

  template<
    typename v,
    typename a, typename b, typename c, typename _, typename sp, typename bp>
  struct Save<v, D, Registers<a, b, c, _, sp, bp>> {
    using type = Registers<a, b, c, v, sp, bp>;
  };

  template<
    typename v,
    typename a, typename b, typename c, typename d, typename _, typename bp>
  struct Save<v, SP, Registers<a, b, c, d, _, bp>> {
    using type = Registers<a, b, c, d, v, bp>;
  };

  template<
    typename v,
    typename a, typename b, typename c, typename d, typename sp, typename _>
  struct Save<v, BP, Registers<a, b, c, d, sp, _>> {
    using type = Registers<a, b, c, d, sp, v>;
  };
}

// Opcode mov
namespace _mov {
  using _value::value;
  using _save::save;
  template<
    typename Dst,
    typename Src,
    typename Registers>
  using Mov = Apply<save, Apply<value, Src, Registers>, Dst, Registers>;
  using mov = TemplateFunction<Mov>;
}

// Opcode add
namespace _add {
  using _value::value;
  using _save::save;
  template<
    typename Dst,
    typename Src,
    typename Registers>
  using Add = Apply<save,
    Apply<plus,
      Apply<value, Src, Registers>,
      Apply<value, Dst, Registers>>,
    Dst,
    Registers>;
  using add = TemplateFunction<Add>;
}

// Opcude sub
namespace _sub {
  using _value::value;
  using _save::save;
  template<
    typename Dst,
    typename Src,
    typename Registers>
  using Sub = Apply<save,
    Apply<minus,
      Apply<value, Src, Registers>,
      Apply<value, Dst, Registers>>,
    Dst,
    Registers>;
  using sub = TemplateFunction<Sub>;
}

using _mov::mov;
using _add::add;
using _sub::sub;
}

#endif
