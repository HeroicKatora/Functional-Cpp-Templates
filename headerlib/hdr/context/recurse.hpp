#ifndef HEADERLIB_HDR_CONTEXT_RECURSE_HPP
#define HEADERLIB_HDR_CONTEXT_RECURSE_HPP
#include "hdr/context/base.hpp"
#include "hdr/core/std.hpp"
#include "hdr/core/convert.hpp"

namespace hdr::context::recurse {
template<typename outer_replace,
         typename current_value>
struct state;
namespace _impl {
  template<typename current_value, typename context, typename inner_state> struct inner_context;
  template<typename R> struct _init;
  using init = ::hdr::std::TypeFunction<_init>;
  template<typename _state, typename arg> struct _apply;
  using apply = ::hdr::std::TypeFunction<_apply>;
  template<typename inner_con, typename result> struct _context_result;
  using context_result = ::hdr::std::TypeFunction<_context_result>;
  template<typename argument, typename replace, typename value> struct _apply_value;
  using apply_value = ::hdr::std::TypeFunction<_apply_value>;

  template<typename R> struct _init {
    using type = state<R, ::hdr::std::id>;
  };

  template<typename outer_replace,
           typename current_value,
           typename argument>
  struct _apply<state<outer_replace, current_value>, argument> { // Normal apply
    using type = ::hdr::std::Apply<apply_value, argument, outer_replace, current_value>;
  };
  template<typename outer_replace,
           typename current_value,
           typename init,
           typename apply>
  struct _apply<state<outer_replace, current_value>, Context<init, apply>> { // Enter context
    using inner_state = ::hdr::std::Apply<init, outer_replace>;
    using type = state<outer_replace, inner_context<current_value, Context<init, apply>, inner_state>>;
  };
  template<typename outer_replace,
           typename current_value,
           typename context,
           typename inner_state,
           typename argument>
  struct _apply<state<outer_replace, inner_context<current_value, context, inner_state>>, argument> { // Apply with inner context
    using inner_result = ::hdr::std::Apply<typename context::apply, inner_state, argument>;
    using inner_con = inner_context<current_value, context, inner_state>;
    using type = state<outer_replace, ::hdr::std::Apply<context_result, inner_con, inner_result>>;
  };

  template<typename current_value,
           typename context,
           typename old_state,
           typename new_state>
  struct _context_result<inner_context<current_value, context, old_state>, new_state> {
    using type = inner_context<current_value, context, new_state>;
  };
  template<typename current_value,
           typename context,
           typename old_state,
           typename result>
  struct _context_result<inner_context<current_value, context, old_state>, Result<result>> {
    using type = ::hdr::std::Apply<current_value, result>;
  };

  template<typename replace,
           typename value>
  struct _apply_value<End, replace, value> { // Exit context
    using type = Result<value>;
  };
  template<typename argument,
           typename replace,
           typename value>
  struct _apply_value { // Ordinary application
    using new_value = ::hdr::std::Apply<value, ::hdr::std::Apply<replace, argument>>;
    using type = state<replace, new_value>;
  };
}

using Recursive = Context<_impl::init, _impl::apply>;
}

#endif /* end of include guard: HEADERLIB_HDR_CONTEXT_RECURSE_HPP */
