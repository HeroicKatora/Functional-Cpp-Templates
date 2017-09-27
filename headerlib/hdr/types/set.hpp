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
  using ::hdr::math::mult;
  using ::hdr::math::compare;
  using ::hdr::math::Zero;
  using ::hdr::math::One;
  using ::hdr::math::Value;
  using ::hdr::maybe::Nothing;
  using just = ::hdr::maybe::return_;


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
  using get_entry  = MatchClause<With<Node<pval,_,_,_>, pval>>;
  using get_size   = MatchClause<With<Node<_,pval,_,_>, pval>>;
  using get_left   = MatchClause<With<Node<_,_,pval,_>, pval>>;
  using get_right  = MatchClause<With<Node<_,_,_,pval>, pval>>;
  using smart_node = Lambda<node, _0, IApply<plus, One, IApply<plus, IApply<size, _1>, IApply<size, _2>>>, _1, _2>;

  template<typename Compare> struct SetType : ::hdr::math::TotalOrder<Compare> {};
  template<typename Type, typename Nodes> struct Set;
  using get_type   = MatchClause<With<Set<pval, _>, pval>>;
  using get_root   = MatchClause<With<Set<_, pval>, pval>>;

  template<typename Cmp, typename Node, typename val> struct _find;
  // cmp -> Node -> maybe result
  using find  = TypeFunction<_find>;
  template<typename cmp, typename node, typename val> struct _find {
    using recfind = Apply<find, cmp>;
    using smaller = Apply<typename cmp::smaller, val>;
    using greater = Apply<typename cmp::greater, val>;
    using type = Match<node,
                       With  <Empty,                                         Const<Nothing>>,
                       WithIf<Node<pval,_,pleft,_>,  MApply<smaller, pval>,  MApply<recfind, pleft, val>>,
                       WithIf<Node<pval,_,_,pright>, MApply<greater, pval>,  MApply<recfind, pright, val>>,
                       With  <Node<pval,_,_,_>,                              MApply<just, pval>> // !smaller && !greater === equal
                       /* This should not occur */
                      >;
  };
  using set_find = Lambda<find, IApply<get_type, _0>, IApply<get_root, _0>>;

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

  template<typename v, typename l, typename r> struct _t_tick;
  using t_tick = TypeFunction<_t_tick>;
  using overweight_value = Value<3>;

  template<typename compare, typename node, typename x> struct _add;
  using add = TypeFunction<_add>;

  template<typename set, typename el> struct _insert;
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

  template<typename v, typename l, typename r>
  struct _t_tick {
    using ln = Apply<size, l>;
    using rn = Apply<size, r>;
    using sum = Apply<plus, rn, ln>;
    using small  = Apply<compare, sum, Value<2>>;
    using lsmall = Apply<compare, Apply<mult, ln, overweight_value>, rn>;
    using rsmall = Apply<compare, Apply<mult, rn, overweight_value>, ln>;
    template<typename L, typename R>
    struct _small {
      using type = Apply<smart_node, v, L, R>;
    }; using smallf = TypeFunction<_small>;
    template<typename L, typename R>
    struct _rtoobig {
      using rln = Apply<size, Apply<get_left, R>>;
      using rrn = Apply<size, Apply<get_right, R>>;
      using type = Apply<compare, rln, rrn, single_l, double_r, v, L, R>;
    }; using rtoobig = TypeFunction<_rtoobig>;
    template<typename L, typename R>
    struct _ltoobig {
      using lln = Apply<size, Apply<get_left, L>>;
      using lrn = Apply<size, Apply<get_right, L>>;
      using type = Apply<compare, lrn, lln, single_r, double_l, v, L, R>;
    }; using ltoobig = TypeFunction<_ltoobig>;
    template<typename L, typename R>
    struct _okay {
      using type = Apply<smart_node, v, L, R>;
    }; using okayf = TypeFunction<_okay>;
    using resolver =
      Apply<small,  smallf,  /* if small */
      Apply<lsmall, rtoobig, /* else if lsmall */
      Apply<rsmall, ltoobig, /* else if rsmall */
            okayf>>>;        /* else */
    using type = Apply<resolver, l, r>;
  };

  template<typename cmp, typename x>
  struct _add<cmp, Empty, x>
    { using type = Node<x, One, Empty, Empty>; };
  template<typename cmp, typename v, typename _, typename l, typename r, typename x>
  struct _add<cmp, Node<v, _, l, r>, x> {
    template<typename X>
    struct _left {
      using nl = Apply<add, cmp, l, X>;
      using type = Apply<t_tick, v, nl, r>;
    }; using left = TypeFunction<_left>;
    template<typename X>
    struct _right {
      using nr = Apply<add, cmp, r, X>;
      using type = Apply<t_tick, v, l, nr>;
    }; using right = TypeFunction<_right>;
    template<typename X>
    struct _equal {
      using type = Apply<smart_node, X, l, r>;
    }; using equal = TypeFunction<_equal>;
    using smaller = Apply<cmp, x, v>;
    using greater = Apply<cmp, v, x>;
    static_assert(!smaller::value || !greater::value, "Comparison function is not asymetric");
    using resolver =
      Apply<smaller, left,  /* if smaller */
      Apply<greater, right, /* if greater */
            equal>>;
    using type = Apply<resolver, x>;
  };

  template<typename Type, typename root, typename el>
  struct _insert<Set<Type, root>, el>
    { using cmp = typename Type::smaller;
      using type = Set<Type, Apply<add, cmp, root, el>>; };

}

using make_type   = TemplateFunction<SetType>;
using number_type = Apply<make_type, compare>;
using empty       = Apply<::hdr::std::flip, TemplateFunction<Set>, Empty>;
using find        = set_find;
using insert      = insert;

}

#endif
