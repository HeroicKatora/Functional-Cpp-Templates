#ifndef HEADERLIB_HDR_CONTEXT_BASE_H
#define HEADERLIB_HDR_CONTEXT_BASE_H

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
}

#endif /* end of include guard: HEADERLIB_HDR_CONTEXT_BASE_H */
