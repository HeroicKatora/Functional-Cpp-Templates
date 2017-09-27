#ifndef HEADERLIB_HDR_TYPES_TUPLE_HPP
#define HEADERLIB_HDR_TYPES_TUPLE_HPP
#include "hdr/core.hpp"

namespace hdr::tuple {
using hdr::std::Apply;
using hdr::std::compose;
using hdr::std::flip;
namespace {
  template<typename A, typename B> struct Tuple;
  using tup = ::hdr::std::TemplateFunction<Tuple>;

  template<typename F, typename Tup> struct _uncurry;
  using uncurry = ::hdr::std::TypeFunction<_uncurry>;

  template<typename F, typename A, typename B>
  struct _uncurry<F, Tuple<A, B>> {
    using type = Apply<F, A, B>;
  };

  using curry = Apply<compose, Apply<flip, compose, tup>, compose>;
}
using uncurry = uncurry;
using curry = curry;
using first = Apply<uncurry, ::hdr::std::fconst>;
using second = Apply<uncurry, ::hdr::std::ignore>;
using swap = Apply<uncurry, Apply<flip, tup>>;
}

#endif
