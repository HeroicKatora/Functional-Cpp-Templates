#ifndef HEADERLIB_HDR_CONTEXT_VOIDER_HPP
#define HEADERLIB_HDR_CONTEXT_VOIDER_HPP
#include "hdr/context/base.hpp"
#include "hdr/core/std.hpp"
#include "hdr/core/convert.hpp"

namespace hdr::context::voider {
namespace impl {
  template<typename S, typename Arg> struct _apply;
  using apply = ::hdr::std::TypeFunction<_apply>;

  template<typename Arg>
  struct _apply<::hdr::std::Void, Arg> {
    using type = ::hdr::std::Void;
  };
  template<>
  struct _apply<::hdr::std::Void, End> {
    using type = Result<::hdr::std::Void>;
  };
}
using Voider = Context<::hdr::std::Const<::hdr::std::Void>, impl::apply>;
}

#endif /* end of include guard: HEADERLIB_HDR_CONTEXT_VOIDER_HPP */
