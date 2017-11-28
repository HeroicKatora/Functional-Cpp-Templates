#ifndef HDR_ELVM_CORE_HPP
#define HDR_ELVM_CORE_HPP
/** An implementation of the virtual machine from the [elvm project][1].
 *
 *  [1]: https://github.com/shinh/elvm/
 */
#include "hdr/core.hpp"
#include "hdr/math.hpp"
#include "hdr/types/array.hpp"
#include "hdr/types/list.hpp"
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

namespace _memory {
  using namespace ::hdr::array;
  using namespace ::hdr::map;
  using nullmem = Apply<empty, Apply<make_type, compare>>;
  template<
    typename Memory,
    typename Datum>
  using InsertData = Apply<insert,
    Apply<size, Memory>,
    Datum,
    Memory>;
  using insert_data = TemplateFunction<InsertData>;

  template<
    typename InitData>
  using Memory = Apply<fold,
    insert_data,
    nullmem,
    InitData>;
  using memory = TemplateFunction<Memory>;
}

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

// Opcode comparisons
namespace _cmp {
  using _value::value;
  using _save::save;

  template<typename A, typename B> using LT = Unsigned<A::value < B::value>;
  using ltF = TemplateFunction<LT>;
  template<typename A, typename B> using GT = Unsigned<(A::value > B::value)>;
  using gtF = TemplateFunction<GT>;
  template<typename A, typename B> using EQ = Unsigned<A::value == B::value>;
  using eqF = TemplateFunction<EQ>;
  template<typename A, typename B> using NE = Unsigned<A::value != B::value>;
  using neF = TemplateFunction<NE>;
  template<typename A, typename B> using LE = Unsigned<A::value <= B::value>;
  using leF = TemplateFunction<LE>;
  template<typename A, typename B> using GE = Unsigned<(A::value >= B::value)>;
  using geF = TemplateFunction<GE>;

  template<typename cmp>
  struct OpCmp {
    template<
      typename Dst,
      typename Src,
      typename Registers>
    using Op = Apply<save,
      Apply<cmp,
        Apply<value, Dst, Registers>,
        Apply<value, Src, Registers>>,
      Dst,
      Registers>;
    using op = TemplateFunction<Op>;
  };

  using lt = typename OpCmp<ltF>::op;
  using gt = typename OpCmp<gtF>::op;
  using eq = typename OpCmp<eqF>::op;
  using ne = typename OpCmp<neF>::op;
  using le = typename OpCmp<leF>::op;
  using ge = typename OpCmp<geF>::op;
}

// Opcode load
namespace _load {
  using _value::value;
  using _save::save;

  using get_memory = Apply<compose,
    Apply<compose, Apply<::hdr::maybe::fromMaybe, Unsigned<0>>>,
    ::hdr::map::find>;

  template<
    typename Dst,
    typename Src,
    typename Registers,
    typename Memory>
  using Load = Apply<save,
    Apply<get_memory,
      Apply<value,
        Src,
        Registers>,
      Memory>,
    Dst,
    Registers>;
  using load = TemplateFunction<Load>;
}

// Opcode store (caution, reversed dst/src)
namespace _store {
  using _value::value;

  using set_memory = ::hdr::map::insert;

  template<
    typename Src,
    typename Dst,
    typename Registers,
    typename Memory>
  using Store = Apply<set_memory,
    Apply<value, Dst, Registers>,
    Apply<value, Src, Registers>,
    Memory>;
  using store = TemplateFunction<Store>;
}

//Opcode putc
namespace _putc {
  using _value::value;
  using Stdout = ::hdr::list::EmptyList;

  template<
    typename Src,
    typename Registers,
    typename Buffer>
  using Putc = Apply<::hdr::list::cons,
    Apply<mod,
      Apply<value,
        Src,
        Registers>,
      Unsigned<256>>,
    Buffer>;
  using putcop = TemplateFunction<Putc>;
}

//Opcode getc. Special in that it would modify two structures, stdin and
//registers! The compiler will therefore encode this as two seperate opcodes,
//peek and advance.
namespace _getc {
  using _save::save;

  template<unsigned N, const char (&buffer)[N], unsigned pos, bool eof=(pos >= N)>
  struct Stream;
  template<unsigned N, const char (&buffer)[N], unsigned pos>
  struct Stream<N, buffer, pos, false> {
    using Peek = Unsigned<buffer[pos]>;
    template<unsigned Step>
    using Advance = Stream<N, buffer, N + Step>;
  };
  template<unsigned N, const char (&buffer)[N], unsigned pos>
  struct Stream<N, buffer, pos, true> {
    using Peek = Unsigned<0>;
    template<unsigned Step>
    using Advance = Stream;
  };

  // This workaround exists because clang doesnt accept `const char (&buf)[N]`
  // as a specialization of `auto& buf`.
  template<unsigned N> auto size(const char (&buffer)[N])
   -> ::std::integral_constant<unsigned, N> { }
  template<auto& buf> using Stdin = Stream<
    decltype(size(buf))::value,
    buf, 0>;

  template<
    typename Dst,
    typename Registers,
    typename Stream>
  using Peek = Apply<save,
    typename Stream::Peek,
    Dst,
    Registers>;
  using peek = TemplateFunction<Peek>;

  template<
    typename Stream>
  using Advance = typename Stream::template Advance<1>;
  using advance = TemplateFunction<Advance>;
}

using ::hdr::array::Array;
using _memory::nullmem;
using _memory::memory;
using _mov::mov;
using _add::add;
using _sub::sub;
using _cmp::eq;
using _cmp::ne;
using _cmp::lt;
using _cmp::gt;
using _cmp::le;
using _cmp::ge;
using _load::load;
using _store::store;
using _putc::Stdout;
using _putc::putcop; // Avoid conflict with c function
using _getc::Stdin;
using _getc::peek;
using _getc::advance;
}

#endif
