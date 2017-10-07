#ifndef HEADERLIB_HDR_TYPES_ARRAY_HPP
#define HEADERLIB_HDR_TYPES_ARRAY_HPP
#include "hdr/core.hpp"

namespace hdr::array {

template<typename ...> struct Array;

using Empty = Array<>;

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
template<typename I, typename F, typename L> struct Fold;
using fold   = ::hdr::std::TypeFunction<Fold>;
template<typename F, typename L> struct Map;
using map    = ::hdr::std::TypeFunction<Map>;

// Head (head:tail)
template<typename head, typename ... tail>
struct Head<Array<head, tail...>>
  { using type = head; };

template<typename head>
struct Last<Array<head>>
  { using type = head; };
template<typename head, typename ... tail>
struct Last<Array<head, tail...>>
  { using type = ::hdr::std::Apply<last, Array<tail...>>; };

// Tail (head:tail)
template<typename head, typename ... tail>
struct Tail<Array<head, tail...>>
  { using type = Array<tail...>; };

template<typename head>
struct Init<Array<head>>
  { using type = Empty; };
template<typename head, typename ... tail>
struct Init<Array<head, tail...>>
  { using rec = ::hdr::std::Apply<init, Array<tail...>>;
    using type = ::hdr::std::Apply<concat, Array<head>, rec>; };

template<typename ... elements>
struct Len<Array<elements...>>
  { using type = ::hdr::math::Value<sizeof...(elements)>; };

template<typename ... first, typename ... second>
struct Concat<Array<first...>, Array<second...>>
  { using type = Array<first..., second...>; };

// Fold F I []
template<typename F, typename I>
struct Fold<F, I, Empty>
  { using type = I; };
// Fold F I (head:tail)
template<typename F, typename I, typename head, typename ... tail>
struct Fold<F, I, Array<head, tail...>>
  { using type = ::hdr::std::Apply<fold, F, ::hdr::std::Apply<F, I, head>, Array<tail...>>; };

template<typename F, typename ... elements>
struct Map<F, Array<elements...>>
  { using type = Array<::hdr::std::Apply<F, elements>...>; };

template<typename T> struct _array_return {
  using type = Array<T>;
};
using array_return = ::hdr::std::TypeFunction<_array_return>;
using array_join   = ::hdr::std::Apply<fold, concat, Empty>;
using array_bind   = ::hdr::std::Apply<
                        ::hdr::std::compose,
                        ::hdr::std::Apply<::hdr::std::compose, array_join>,
                        ::hdr::std::Apply<::hdr::std::flip, map>>;
using ArrayMonadType = ::hdr::monad::MonadFromBind<array_return, array_bind>;
using return_ = ArrayMonadType::return_;
using bind    = ArrayMonadType::bind;
using kleisli = ArrayMonadType::kleisli;
using fmap    = ArrayMonadType::fmap;
using join    = ArrayMonadType::join;

}

#endif /* end of include guard: HEADERLIB_HDR_TYPES_ARRAY_HPP */
