#ifndef HEADERLIB_HDR_CORE_OPERATORS_HPP
#define HEADERLIB_HDR_CORE_OPERATORS_HPP
#include "hdr/core/apply.hpp"

namespace hdr::operators {
/** Allows folding a variadic number of arguments with an operator. This is
 *  intended only to allow syntactic sugar and not pimarily as part of types.
 */
template<typename Op> struct BinaryOperator;
template<typename Op> struct BinaryOperator<Op> {
  /// Fold to the right, so left associative
  template<typename Init, typename ...> struct _right;
  template<typename Init> struct _right<Init>
    { using type = Init; };
  template<typename Init, typename First, typename ... Rest> struct _right<Init, First, Rest...>
    { using result = ::hdr::std::ApplySingle<::hdr::std::ApplySingle<Op, Init>, First>;
      using type   = typename _right<result, Rest...>::type; };

  template<typename Init, typename ...> struct _left;
  template<typename Init> struct _left<Init>
    { using type = Init; };
  template<typename Init, typename First, typename ... Rest> struct _left<Init, First, Rest...>
    { using result = typename _left<First, Rest...>::type;
      using type   = ::hdr::std::ApplySingle<::hdr::std::ApplySingle<Op, Init>, result>; };

  template<typename Init, typename ... Pack>
  using FoldRight = typename _right<Init, Pack...>::type;
  template<typename Init, typename ... Pack>
  using FoldLeft  = typename _left<Init, Pack...>::type;
};

}

#endif /* end of include guard: HEADERLIB_HDR_CORE_OPERATORS_HPP */
