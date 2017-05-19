#ifndef HEADERLIB_HDR_TYPES_LIST_HPP
#define HEADERLIB_HDR_TYPES_LIST_HPP
/*
 * celists.hpp
 *
 *	Created on: 12.11.2015
 *			Author: Andreas Molzer
 *			Coathor: Martin Molzer
 */
#include "hdr/core.hpp"
#include "hdr/math.hpp"

namespace hdr::list {
/** List a = Nil | Cons a (List a)
*/
struct Nil;
template<typename head, typename tail>
struct Cons;

/** []
*/
using EmptyList = Nil;

template<typename L> struct Head;
using head = ::hdr::std::TypeFunction<Head>;
template<typename L> struct Last;
using last = ::hdr::std::TypeFunction<Last>;
template<typename L> struct Tail;
using tail = ::hdr::std::TypeFunction<Tail>;
template<typename L> struct Init;
using init = ::hdr::std::TypeFunction<Init>;
template<typename L> struct Len;
using len  = ::hdr::std::TypeFunction<Len>;
template<typename L, typename J> struct Concat;
using concat = ::hdr::std::TypeFunction<Concat>;
template<typename L, typename F, typename I> struct Fold;
using fold   = ::hdr::std::TypeFunction<Fold>;
// Head []
template<>
struct Head<EmptyList>; // exception?
// Head (head:tail)
template<typename head, typename tail>
struct Head<Cons<head, tail>>
  { using type = head; };

// Last []
template<>
struct Last<EmptyList>; // exception?
// Last (head:[])
template<typename head>
struct Last<Cons<head, EmptyList>>
  { using type = head; };
// Last (head:tail)
template<typename head, typename tail>
struct Last<Cons<head, tail>>
  { using type = ::hdr::std::Apply<last, tail>; };

// Tail []
template<>
struct Tail<EmptyList>; // exception?
// Tail (head:tail)
template<typename head, typename tail>
struct Tail<Cons<head, tail>>
  { using type = tail; };

// Init []
template<>
struct Init<EmptyList>; // exception?
// Init (head:[])
template<typename head>
struct Init<Cons<head, EmptyList>> { using type = EmptyList; };
// Last (head:tail)
template<typename head, typename tail>
struct Init<Cons<head, tail>>
  { using type = Cons<head, ::hdr::std::Apply<init, tail>>; };

template<>
struct Len<EmptyList>
  { using type = ::hdr::math::Zero; };
template<typename head, typename tail>
struct Len<Cons<head, tail>>
  { using type = ::hdr::std::Apply<::hdr::math::plus, ::hdr::math::One, ::hdr::std::Apply<len, tail>>; };

// Concat [] L
template<typename L>
struct Concat<EmptyList, L>
  { using type = L; };
// Concat (head:tail) L
template<typename head, typename tail, typename L>
struct Concat<Cons<head, tail>, L>
  { using type = Cons<head, ::hdr::std::Apply<concat, tail, L>>; };

// Fold [] L
template<typename F, typename I>
struct Fold<EmptyList, F, I>
  { using type = I; };
// Concat (head:tail) L
template<typename head, typename tail, typename F, typename I>
struct Fold<Cons<head, tail>, F, I>
  { using type = ::hdr::std::Apply<fold, tail, F, ::hdr::std::Apply<F, I, head>>; };
}

#endif //HEADERLIB_HDR_TYPES_LIST_HPP
