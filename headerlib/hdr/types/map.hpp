#ifndef HEADERLIB_HDR_TYPES_MAP_HPP
#define HEADERLIB_HDR_TYPES_MAP_HPP
#include "hdr/types/set.hpp"
#include "hdr/types/tuple.hpp"
/**
 * @file: map.hpp
 * A map type
 */
namespace hdr::map {
using ::hdr::std::Apply;
using ::hdr::std::compose;
using ::hdr::std::flip;

// (a -> a -> B) -> ((a, _) -> (a, _) -> B)
// Pointfree.io suggests flip flip fst . ((.) .) . (. fst) but thats even less readable
struct compare_first {
  template<typename F> struct _inner {
    template<typename A, typename B> struct templ {
      using type = Apply<F, Apply<::hdr::tuple::first, A>, Apply<::hdr::tuple::first, B>>;
    };
  };
  template<typename F>
  using expr = ::hdr::std::TypeFunction<_inner<F>::template templ>;
};
using make_type = Apply<compose, ::hdr::set::make_type, compare_first>;
using number_type = Apply<make_type, ::hdr::math::compare>;
using empty  = ::hdr::set::empty;
using size   = ::hdr::set::size;

// Key -> Value -> Map -> Map
using insert = Apply<::hdr::tuple::curry, ::hdr::set::insert>;

struct Unvalue;
// Key -> Map -> Bool
using contains = Apply<compose, ::hdr::set::contains, Apply<flip, ::hdr::tuple::tup, Unvalue>>;
using find = Apply<compose,
  Apply<compose, Apply<::hdr::maybe::fmap, ::hdr::tuple::second>>,
  Apply<compose, ::hdr::set::find, Apply<flip, ::hdr::tuple::tup, Unvalue>>>;
}

#endif
