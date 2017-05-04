#ifndef HEADERLIB_HDR_TYPES_LIST_HPP
#define HEADERLIB_HDR_TYPES_LIST_HPP
/*
 * celists.hpp
 *
 *	Created on: 12.11.2015
 *			Author: Andreas Molzer
 *			Coathor: Martin Molzer
 */
#include "hdr/core/std.hpp"

using namespace hdrstd;

namespace hdr::list {
/** List a = Nil | Cons a (List a)
*/
struct Nil;

template<typename head, typename tail>
struct Cons;

/** []
*/
using EmptyList = Nil;

template<typename L>
struct Head;
using head = ::hdr::std::TypeFunction<Head>;
template<typename L>
struct Last;
using last = ::hdr::std::TypeFunction<Last>;
template<typename L>
struct Tail;
using tail = ::hdr::std::TypeFunction<Tail>;
template<typename L>
struct Init;
using init = ::hdr::std::TypeFunction<Init>;

// Head []
template<>
struct Head<EmptyList>; // exception?
// Head (head:tail)
template<typename head, typename tail>
struct Head<Cons<head, tail>> { using type = head; };

// Last []
template<>
struct Last<EmptyList>; // exception?
// Last (head:[])
template<typename head>
struct Last<Cons<head, EmptyList>> { using type = head; };
// Last (head:tail)
template<typename head, typename tail>
struct Last<Cons<head, tail>> { using type = Last<tail>; };

// Tail []
template<>
struct Tail<EmptyList>; // exception?
// Tail (head:tail)
template<typename head, typename tail>
struct Tail<Cons<head, tail>> { using type = tail; };

// Init []
template<>
struct Init<EmptyList>; // exception?
// Init (head:[])
template<typename head>
struct Init<Cons<head, EmptyList>> { using type = EmptyList; };
// Last (head:tail)
template<typename head, typename tail>
struct Init<Cons<head, tail>> { using type = Cons<head, Init<tail>>; };

}

#endif //HEADERLIB_HDR_TYPES_LIST_HPP
