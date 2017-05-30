#ifndef HEADERLIB_HDR_CONTEXT_BASE_H
#define HEADERLIB_HDR_CONTEXT_BASE_H
#include "hdr/core.hpp"
#include "hdr/types/list.hpp"

/** Definition for contexts, hopefully enabling many syntactic sugar expressions
 *  The idea is to provide nesting contexts which can each change every argument
 *  before it is applied and additionally keep some state. This would provide us
 *  with let and nicer matching syntax.
 *  It is not possible to 'prebuild' nested contexts, you should consider lazy
 *  functions or simply taking arguments to achieve this. The reasoning is that
 *  in order to recognize the nesting, the context needs to occurs as Context<T>
 *  but to reduce spam from explicit evaluation of every context, we can't use
 *  a context and its results interchangeably.
 */
namespace hdr::context {
/** Generic placeholder type, intended for use in contexts such as variables.
 *  This should probably never occur as an argument to ApplySingle and always
 *  be substituted by some surrounding context.
 */
template<typename T> struct Placeholder;
/** Signals the end of a context
 */
struct End;

/** Constructor for a context
 *  The function argument is the outer scopes replacer
 *    init   : (Value -> Value) -> State
 *  Apply it to an argument, advancing the state
 *    apply  : State -> Argument -> State
 *  Close the context, evaluating the state to a result
 *    close  : State -> Value
 */
template<typename _init,
         typename _apply>
struct Context {
  using init  = _init;
  using apply = _apply;
};

/** Signals the final state of a context, returning a result
 */
template<typename result> struct Result;
/*
namespace {
  using namespace ::hdr::std;
  template<typename ContextStack> struct _context_eval;  // Data definition
  template<typename Context, typename State> struct _stack_entry {
    using context = Context;
    using state   = State;
  };

  template<typename eval, typename arg>
  struct _context_eval_apply;  // Implementation via specialization
  using context_eval_apply = TypeFunction<_context_eval_apply>;
  template<typename stack, typename arg>
  struct _context_eval_apply<_context_eval<stack>, arg> {
    template<typename arg, typename entry> using replaced_arg
      = Apply<entry::context::handle_argument, entry::state, arg>;
    using replacement_fold = TemplateFunction<replaced_arg>;
    using repl_arg = Apply<::hdr::list::fold, stack, replacement_fold, arg>;
    using top = Apply<::hdr::list::first, stack>;
    using new_state = Apply<top::context::progress, top::state, repl_arg>;
    using new_stack = Apply<push, Apply<pop, stack>, _stack_entry<top::context, new_state>>;
    using type = _context_eval<new_stack>;
  };
  template<typename stack>
  struct _context_eval_apply<_context_eval<stack>, End> {
    using top = Apply<::hdr::list::first, stack>;
    using val = Apply<top::context::close, top::state>;
    using new_stack = Apply<::hdr::list::tail, stack>;
    using is_last_context = Apply<::hdr::math::same, new_stack, EmptyStack>;
    using apply_method = Apply<::hdr::std::when_else, is_last_context,
      ::hdr::std::ignore, _context_eval_apply>;
    using type = Apply<apply_method, _context_eval<new_stack>, val>;
  };
  template<typename stack, typename i, typename h, typename p, typename c>
  struct _context_eval_apply<_context_eval<stack>, Context<i, h, p, c>> {
    using new_stack = Apply<push, stack, _stack_entry<Context<i, h, p, c>, i>>;
    using type = _context_eval<new_stack>;
  };
}

using NoOpContext = Context<::hdr::std::id, ::hdr::std::ignore, ::hdr::std::apply, ::hdr::std::id>;
using ContextEval = _context_eval<::hdr::list::Cons<NoOpContext, ::hdr::list::EmptyList>>;
*/
}

#endif /* end of include guard: HEADERLIB_HDR_CONTEXT_BASE_H */
