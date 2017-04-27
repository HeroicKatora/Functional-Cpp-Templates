#ifndef HEADERLIB_HDR_CORE_APPLY_HPP
#define HEADERLIB_HDR_CORE_APPLY_HPP

namespace hdr::std {
/** Main point of the library, reference for the call syntax.
 *  Everything else is stacked onto this definition. This is implemented
 *  by template specialization which is what this library tries to steer you
 *  away from. Don't mind the irony.
 */
template<typename F, typename Arg> struct _apply {
	using type = typename F::template expr<Arg>;
};
/** The non-specializable version of apply that deducts the internal type.
 *  This can be used like a templated operator
 */
template<typename F, typename Arg>
using ApplySingle = typename _apply<F, Arg>::type;
}

#endif /* end of include guard: HEADERLIB_HDR_CORE_APPLY_HPP */
