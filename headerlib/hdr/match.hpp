#ifndef HEADERLIB_HDR_MATCH_HPP
#define HEADERLIB_HDR_MATCH_HPP
/*
 * match.hpp
 *
 *  Created on: 22.02.2017
 *      Author: Andreas Molzer
 */
#include "hdr/std.hpp"
#include "hdr/types/maybe.hpp"

/** Intended usage:
 *    match var [(with template selector function)]...
 *  Essentially works as a monad M where
 *  (match var) constructs (M V) and
 *  (with template selector function) constructs a (V -> M V) and
 *  the monad overloads application to be bind
 */
namespace hdr::match {

using ::hdr::std::Apply;
using ::hdr::std::Const;
using ::hdr::std::True;
using ::hdr::std::False;
using ::hdr::std::TemplateFunction;
using ::hdr::std::TypeFunction;
using ::hdr::std::compose;
using ::hdr::std::flip;
using ::hdr::std::when_else;
using ::hdr::maybe::Just;
using ::hdr::maybe::Nothing;
using ::hdr::maybe::bind;
using ::hdr::maybe::fmap;
using ::hdr::maybe::freturn;
using ::hdr::maybe::maybe;

template<typename Var>
struct Unmatched {
  template<typename With>
  using expr = Apply<With, Var>;
};

template<typename Var>
struct Matched {
  template<typename With>
  using expr = Matched<Var>;
};

template<typename Key>
struct Placeholder { using type = Key; };
struct PlaceholderAny;
using _ = PlaceholderAny;

template<typename _Key, typename _Val> struct KVPair{
  using Key = _Key; using Val = _Val;
};
template<typename ... KVs> struct KVList;
template<typename K, typename L> struct _Join;
using _join = TypeFunction<_Join>;
template<typename ... KVs, typename ... KLs>
struct _Join<KVList<KVs...>, KVList<KLs...>> {
  using type = KVList<KVs..., KLs...>;
};

template<typename... Args> struct _Flatten;
template<>           struct _Flatten<>  { using type = KVList<>; };
template<typename K> struct _Flatten<K> { using type = K; };
template<typename K, typename ... R> struct _Flatten<K, R...> {
  template<typename Ma, typename Mb>
  struct MaybeJoin {
    template<typename L, typename M> using Mjoin = Just<Apply<_join, L, M>>;
    using mjoin = TemplateFunction<Mjoin>;
    using first = Apply<maybe, Const<Nothing>, mjoin, Ma>;
    using type  = Apply<bind, Mb, first>;
  };
  using T       = typename _Flatten<R...>::type;
  using type    = Apply<TypeFunction<MaybeJoin>, K, T>;
};

/**
 *    A -> B -> Optional TemplateVars
 */
template<typename Template, typename Actual>
struct Decompose {
  template<typename T, typename S> struct IsSame { using type = False; };
  template<typename T> struct IsSame<T, T>       { using type = True;  };
  using type = Apply<when_else, typename IsSame<Template, Actual>::type, Just<KVList<>>, Nothing>;
};
using decompose = TypeFunction<Decompose>;
template<typename PKey, typename A>
struct Decompose<Placeholder<PKey>, A> {
  using type = Just<KVList<KVPair<PKey, A>>>;
};
template<typename A>
struct Decompose<PlaceholderAny, A> {
  using type = Just<KVList<>>;
};
template<template<typename...> typename A, typename ... TArgs, typename ... Args>
struct Decompose<A<TArgs...>, A<Args...>> {
  using type = typename _Flatten<Apply<decompose, TArgs, Args>...>::type;
};

/** Constructs a WithClause
 *    Template -> (TemplateVars -> Bool) -> (TemplateVars -> B) -> A -> Maybe B
 */
template<typename Template, typename Selector, typename Function>
struct With {
  using maybe_decomp = Apply<decompose, Template>;                     // (A -> Maybe TemplateVars)
  using boolifier    = Apply<fmap, Selector>;                          // (Maybe TemplateVars -> Maybe Bool)
  using get_result   = Apply<fmap, Function>;                          // (Maybe TemplateVars -> Maybe B)
  template<typename B>
  using _selector    = Apply<when_else, B, get_result, Const<Nothing>>;
  using selector     = TemplateFunction<_selector>;                    // (Bool -> (Maybe TemplateVars -> Maybe B))
  using select_after = Apply<flip, selector>;                          // (Maybe TemplateVars -> Bool -> Maybe B)
  template<typename MTV>
  using _result_func = Apply<bind, Apply<boolifier, MTV>, Apply<select_after, MTV>>;
  using result_func  = TemplateFunction<_result_func>;                 // (Maybe TemplateVars -> Maybe B)
  using maybe_result = Apply<compose, result_func, maybe_decomp>;      // (A -> Maybe B)
  using type = maybe_result;
};
using with = TypeFunction<With>;

}
#endif //HEADERLIB_HDR_MATCH_HPP
