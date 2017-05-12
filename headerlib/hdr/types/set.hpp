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
  using ::hdr::match::MApply;
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
  using smart_node = Lambda<node, _0, IApply<plus, One, IApply<plus, IApply<size, _1>, IApply<size, _2>>>, _1, _2>;

  template<typename Cmp, typename Node> struct _find;
  // camp -> Node -> maybe result
  using find  = TypeFunction<_find>;
  template<typename cmp, typename node> struct _find {
    using recfind = Apply<find, cmp>;
    using type = Match<node,
                       With  <Empty,                                        Const<Nothing>>,
                       WithIf<Node<pval,_,pleft,_>,  typename cmp::smaller, MApply<recfind, pleft>>,
                       WithIf<Node<pval,_,_,pright>, typename cmp::greater, MApply<recfind, pright>>,
                       With  <Node<pval,_,_,_>,                             MApply<just, pval>> // !smaller && !greater === equal
                       /* This should not occur */
                      >;
  };

  template<typename Node> struct _min;
  using min = TypeFunction<_min>;
  template<typename node> struct _min {
    using type = Match<node,
                       With<Node<pval,_,Empty,_>, pval>,
                       With<Node<pval,_,pleft,_>, Apply<compose, min, pleft>>
                      >;
  };

  template<typename a, typename x, typename rnode> struct _single_l;
  using single_l = TypeFunction<_single_l>;
  template<typename a, typename x, typename rnode> struct _double_l;
  using double_l = TypeFunction<_double_l>;
  template<typename a, typename lnode, typename x> struct _single_r;
  using single_r = TypeFunction<_single_r>;
  template<typename a, typename lnode, typename x> struct _double_r;
  using double_r = TypeFunction<_double_r>;

  template<typename val, typename left, typename right> struct _balance_node;
  using balance_node = TypeFunction<_balance_node>;

  template<typename El, typename Set>
  struct _insert;
  using insert = TypeFunction<_insert>;

  template<typename a, typename x, typename b, typename _, typename y, typename z>
  struct _single_l<a, x, Node<b, _, y, z>>
    { using type = Apply<smart_node, b, Apply<smart_node, a, x, y>, z>; };
  template<typename a, typename x, typename b, typename _, typename y, typename z>
  struct _single_r<b, Node<a, _, x, y>, z>
    { using type = Apply<smart_node, a, x, Apply<smart_node, b, y, z>>; };
  template<typename a, typename x, typename c, typename b, typename y1, typename y2, typename z, typename _, typename __>
  struct _double_l<a, x, Node<c, _, Node<b, __, y1, y2>, z>>
    { using type = Apply<smart_node, b, Apply<smart_node, a, x, y1>, Apply<smart_node, c, y2, z>>; };
  template<typename a, typename x, typename c, typename b, typename y1, typename y2, typename z, typename _, typename __>
  struct _double_r<c, Node<a, _, x, Node<b, __, y1, y2>>, z>
    { using type = Apply<smart_node, b, Apply<smart_node, a, x, y1>, Apply<smart_node, c, y2, z>>; };
}

using make_type   = TemplateFunction<SetType>;
using number_type = Apply<make_type, ::hdr::math::compare>;
using empty       = Empty;
using find        = find;
using insert      = insert;

}

#endif
