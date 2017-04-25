#ifndef HEADERLIB_HDR_TYPES_SET_HPP
#define HEADERLIB_HDR_TYPES_SET_HPP
#include "hdr/core.hpp"
#include "hdr/math.hpp"
#include "hdr/types/maybe.hpp"
/**
 * @file: set.hpp
 * A set type
 * Based on https://groups.csail.mit.edu/mac/users/adams/BB/ which is also origin of haskells implementation
 */
namespace hdr::set {
using ::hdr::std::TemplateFunction;
using ::hdr::std::TypeFunction;

namespace {
using ::hdr::std::Apply;
using ::hdr::std::Const;
using ::hdr::std::compose;
using ::hdr::lambda::Lambda;
using ::hdr::lambda::IApply;
using ::hdr::lambda::_0;
using ::hdr::lambda::_1;
using ::hdr::lambda::_2;
using ::hdr::match::_;
using ::hdr::match::Placeholder;
using ::hdr::match::Match;
using ::hdr::match::MatchClause;
using ::hdr::match::With;
using ::hdr::match::WithIf;
using ::hdr::match::get;
using ::hdr::math::plus;
using ::hdr::math::Zero;
using ::hdr::math::One;
using ::hdr::maybe::Nothing;
using just = ::hdr::maybe::return_;

template<typename Compare> struct SetType;
struct Empty;
template<typename Entry, typename Count, typename Left, typename Right> struct Node;
using node = TemplateFunction<Node>;
// Named matching parameters for nodes
struct _val;   using pval   = Placeholder<_val>;
struct _count; using pcount = Placeholder<_count>;
struct _left;  using pleft  = Placeholder<_left>;
struct _right; using pright = Placeholder<_right>;

using size = MatchClause<With<Empty,              Const<Zero>>,
                         With<Node<_,pcount,_,_>, pcount>
                        >;
using smart_node = Lambda<node, _0, IApply<plus, One, IApply<plus, _1, _2>>, _1, _2>;

template<typename Cmp, typename Node>
struct _find;
// camp -> Node -> maybe result
using find  = TypeFunction<_find>;

template<typename cmp, typename node>
struct _find {
  using recfind = Apply<find, cmp>;
  using type = Match<node,
                     With<Empty,                                          Const<Nothing>>,
                     WithIf<Node<pval,_,_,_>,      typename cmp::equal,   Apply<compose, just, pval>>,
                     WithIf<Node<pval,_,pleft,_>,  typename cmp::smaller, Apply<compose, recfind, pleft>>,
                     WithIf<Node<pval,_,_,pright>, typename cmp::greater, Apply<compose, recfind, pright>>
                           /* This should not occur */
                    >;
};

}


using make_type   = TemplateFunction<SetType>;
using number_type = Apply<make_type, ::hdr::math::compare>;
using empty       = ::hdr::std::Void;

template<typename El, typename Set>
struct Insert {

};
using insert = TypeFunction<Insert>;

}

#endif
