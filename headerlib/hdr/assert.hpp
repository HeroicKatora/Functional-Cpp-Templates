#ifndef HEADERLIB_HDR_ASSERT_HPP
#define HEADERLIB_HDR_ASSERT_HPP
#include "hdr/core/math.hpp"

namespace hdr::assert {

template<
  typename V,
  typename Val = typename ::hdr::math::ValueOf<V>,
  typename condition = typename ::std::enable_if<Val::value>::type>
struct Assert;

template<
  typename V,
  typename Val = typename ::hdr::math::ValueOf<V>,
  typename condition = typename ::std::enable_if<!Val::value>::type>
struct AssertFalse;

}

#endif /* end of include guard: HEADERLIB_HDR_ASSERT_HPP */
