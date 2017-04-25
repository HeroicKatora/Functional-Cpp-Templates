#ifndef HEADERLIB_HDR_CORE_MATCH_HPP
#define HEADERLIB_HDR_CORE_MATCH_HPP
/*
 * match.hpp
 *
 *  Created on: 22.02.2017
 *      Author: Andreas Molzer
 */
#include "hdr/core/std.hpp"
#include "hdr/types/maybe.hpp"

/** Limited (!!!) core matching, as a monad.
 *    match var [>>= (with template selector function)]*
 *  Essentially works as a monad M with M::return == match which constructs
 *  an 'Unmatched' type. with constructs a bindable function returning this type.
 *  Binding F to a Matched T will skip the application of F while binding to
 *  an unmatched will do it. This works like Maybe but instead Nothing/Matched
 *  carries an additional result type and Maybe/Unmatched is very much the same.
 *
 *  Limitations of core matching:
 *  Each placeholder can only be used once in a template, this is not (!) checked
 *  for. If you need this functionality, use advanced matching based on sets
 *  instead (uniqueness check needs set mechanics for performance).
 *  [WIP]
 *  Templates are declared like the type they are supposed to match but using
 *  Placholders instead of the true type parameters. Should the type definition
 *  depends on its parameters, a special form is provided (can't be constructed).
 *  This feature is not currently implemented.
 */
namespace hdr::match {

using ::hdr::std::Apply;
using ::hdr::std::Const;
using ::hdr::std::True;
using ::hdr::std::False;
using ::hdr::std::TemplateFunction;
using ::hdr::std::TypeFunction;
using ::hdr::std::Void;
using ::hdr::std::apply;
using ::hdr::std::compose;
using ::hdr::std::id;
using ::hdr::std::fconst;
using ::hdr::std::ignore;
using ::hdr::std::flip;
using ::hdr::std::when_else;
using ::hdr::monad::MonadFromBind;

/// Type definition of Unmatched
template<typename Var>
struct Unmatched;
using unmatched = TemplateFunction<Unmatched>;
/// Type definition of Matched
template<typename Var>
struct Matched;
using matched   = TemplateFunction<Matched>;

namespace {
  template<typename U, typename M, typename V> struct _matching;
  template<typename U, typename M, typename V>
  struct _matching<U, M, Unmatched<V>> {
    using type = Apply<U, V>;
  };
  template<typename U, typename M, typename V>
  struct _matching<U, M, Matched<V>> {
    using type = Apply<M, V>;
  };
}
/// (a -> c) -> (b -> c) -> (Matching a b) -> c
using matching    = TypeFunction<_matching>;
using isUnmatched = Apply<matching, Const<True>,  Const<False>>;
using isMatched   = Apply<matching, Const<False>, Const<True>>;

/** Left side matched will always propagate, left side unmatched dominates
 *  right side unmatched.
 *    (Matching a b) -> (Matching a b) -> (Matching a b)
 *  This is a non-abelian function, beware
 */
using matchAdd    = Apply<matching, ignore, Apply<compose, fconst, matched>>;

using fromUnmatched = Apply<matching, id, Void>;
using fromMatched   = Apply<matching, Void, id>;

namespace {
  template<typename M>
  struct _bind;
  template<typename V>
  struct _bind<Matched<V>> {
    using type = Const<Matched<V>>;
  };
  template<typename V>
  struct _bind<Unmatched<V>> {
    using resultf = Apply<flip, apply, V>;
    using type    = Apply<compose, Apply<matchAdd, Unmatched<V>>, resultf>;
  };
}

using return_      = unmatched;
using bind         = TypeFunction<_bind>;
using MatchingType = MonadFromBind<return_, bind>;
using fmap         = MatchingType::fmap;
using kleisli      = MatchingType::kleisli;
using join         = MatchingType::join;

/// A placeholder with a given type as its identifier
template<typename Key>
struct Placeholder;
/// A placeholder matching anything
struct PlaceholderAny;
using _ = PlaceholderAny;

namespace {
  using ::hdr::maybe::Just;
  using ::hdr::maybe::Nothing;
  using ::hdr::maybe::fromJust;
  using ::hdr::maybe::maybe;
  using maybefmap = ::hdr::maybe::fmap;
  /** We don't want to use set/map/list since we are in core :(
   *  This might change in the future, when I get, I might inline maybe here
   *  Anyways, this implements flat map, so don't go too wild with placeholder
   *  count and we don't check anything.
   */
  template<typename _Key, typename _Val> struct KVPair;
  template<typename ... KVs> struct KVList;

  template<typename K, typename L> struct Join;
  using _join = TypeFunction<Join>;
  template<typename ... KVs, typename ... KLs>
  struct Join<KVList<KVs...>, KVList<KLs...>> {
    using type = KVList<KVs..., KLs...>;
  };

  template<typename List, typename Name> struct Get;
  using _get_maybe  = TypeFunction<Get>;
  template<typename Name>
  struct Get<KVList<>, Name> {
    using type = Nothing;
  };
  template<typename Name, typename Value, typename ... Rest>
  struct Get<KVList<KVPair<Name, Value>, Rest...>, Name> {
    using type = Just<Value>;
  };
  template<typename Name, typename First, typename ... Rest>
  struct Get<KVList<First, Rest...>, Name> {
    using type = typename Get<KVList<Rest...>, Name>::type;
  };
  using _get       = Apply<compose, Apply<compose, fromJust>, _get_maybe>;

  template<typename ... KVs> struct KVList {
    using get = Apply<_get, KVList<KVs...>>;
  };

  template<typename... Args> struct Flatten;
  template<>           struct Flatten<>  { using type = KVList<>; };
  template<typename K> struct Flatten<K> { using type = K; };
  template<typename K, typename ... R> struct Flatten<K, R...> {
    template<typename Ma, typename Mb>
    struct MaybeJoin {
      using mjoin = Apply<compose, Apply<compose, ::hdr::maybe::return_>, _join>;
      using first = Apply<maybe, Const<Nothing>, mjoin, Ma>;
      using type  = Apply<::hdr::maybe::bind, Mb, first>;
    };
    using T       = typename Flatten<R...>::type;
    using type    = Apply<TypeFunction<MaybeJoin>, K, T>;
  };

  /** Tries to deconstruct a type into the placeholders given into the template.
   *    Template -> Argument -> Maybe TemplateVars
   */
  template<typename Template, typename Actual>
  struct Decompose {
    template<typename T, typename S> struct IsSame { using type = False; };
    template<typename T> struct IsSame<T, T>       { using type = True;  };
    using type = Apply<when_else, typename IsSame<Template, Actual>::type, Just<KVList<>>, Nothing>;
  };
  using _decompose = TypeFunction<Decompose>;
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
    using type = typename Flatten<Apply<_decompose, TArgs, Args>...>::type;
  };

  /** Constructs a WithClause
   *    Template -> (TemplateVars -> Bool) -> (TemplateVars -> B) -> A -> Maybe B
   */
  template<typename Template, typename Selector, typename Function>
  struct _with {
    using maybe_decomp = Apply<_decompose, Template>;                     // (A -> Maybe TemplateVars)
    using boolifier    = Apply<maybefmap, Selector>;                      // (Maybe TemplateVars -> Maybe Bool)
    using get_result   = Apply<maybefmap, Function>;                      // (Maybe TemplateVars -> Maybe B)
    template<typename B>
    using _selector    = Apply<when_else, B, get_result, Const<Nothing>>;
    using selector     = TemplateFunction<_selector>;                    // (Bool -> (Maybe TemplateVars -> Maybe B))
    using select_after = Apply<flip, selector>;                          // (Maybe TemplateVars -> Bool -> Maybe B)
    template<typename MTV>
    using _result_func = Apply<::hdr::maybe::bind, Apply<boolifier, MTV>, Apply<select_after, MTV>>;
    using result_func  = TemplateFunction<_result_func>;                 // (Maybe TemplateVars -> Maybe B)
    using maybe_result = Apply<compose, result_func, maybe_decomp>;      // (A -> Maybe B)
    template<typename A>
    using match_result = Apply<maybe, Unmatched<A>, matched, Apply<maybe_result, A>>;
    using type         = TemplateFunction<match_result>;                 // (A -> Match V)
  };
}
/** Constructor function for With
 *    Template -> (TemplateVars -> Bool) -> (TemplateVars -> B) -> A -> Match B
 */
using with_if   = TypeFunction<_with>;
template<typename Template, typename Selector, typename Function>
using WithIf    = Apply<with_if, Template, Selector, Function>;
///   Template -> (TemplateVars -> B) -> A -> Match B
using with      = Apply<flip, with_if, Const<True>>;
template<typename Template, typename Function>
using With      = Apply<with, Template, Function>;
/// Template -> V -> Maybe TemplateVars
using decompose = _decompose;
/** Supply the name used in the placeholder then the template vars. Error when
 *  there is no such keyword in the TemplatVars. The argument order makes it
 *  easier to use it for maybe::bind or in lambdas (as the name should be defined
 *  ad-hoc and known to the programmer).
 *    Name -> TemplateVars -> V
 *  If you already know the TemplateVars, you can also use `TemplateVars::get`, type
 *    Name -> V
 */
using get       = Apply<flip, _get>;
template<typename Key>
struct Placeholder : Apply<get, Key> {};

namespace {
  template<typename ... W> struct _match_body;
  template<> struct _match_body<> {
    using combined_function = unmatched;
  };
  template<typename V> struct _match_body<V> {
    using combined_function = V;
  };
  template<typename V, typename ... W> struct _match_body<V, W...> {
    using recursor = typename _match_body<W...>::combined_function;
    using combined_function = Apply<kleisli, V, recursor>;
  };
  template<typename ... W>
  using _bindable_matches = typename _match_body<W...>::combined_function;
}

/** Convenience non-type version for matching, syntactic sugar for folding
 *  multiple with statements with ::hdr::match::kleisli, then binding it to
 *  Unmatched V and finally unpacking. Will fail if it didn't match
 *    V -> [with-clause]* -> W
 */
template<typename V, typename ... W>
using Match = Apply<fromMatched, Apply<bind, Unmatched<V>, _bindable_matches<W...>>>;
/** 'Flipped' version of Match, deferring application of the argument
 *    [with-clause]* -> V -> W
 */
template<typename ... W>
using MatchClause = Apply<compose, fromMatched, _bindable_matches<W...>>;

}
#endif //HEADERLIB_HDR_MATCH_HPP
