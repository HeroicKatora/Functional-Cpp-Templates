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
using ::hdr::maybe::Just;
using ::hdr::maybe::Nothing;

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

/**
 *    A -> B -> Optional TemplateVars
 */
template<typename Template, typename Actual>
struct GetMatcherVars {
  using type = Nothing;
};

/** Constructs a WithClause
 *    Template -> (TemplateVars -> Bool) -> (TemplateVars -> B) -> A -> M
 */
template<typename Template, typename Selector, typename Function>
struct With {

};

}
