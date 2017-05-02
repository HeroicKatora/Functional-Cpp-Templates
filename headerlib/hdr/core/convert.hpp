#ifndef HEADERLIB_HDR_CORE_CONVERT_HPP
#define HEADERLIB_HDR_CORE_CONVERT_HPP

namespace hdr::std {

/// Wraps struct types which declare a member named type to signal their result.
/// This is mostly for c++ standard classes but may be used to implement functions
/// via template specializations.
/// Having a templated type F declared as
/// ```
///	template</*some arguments*/> struct F {
/// 	/*implementation*/
///		using type = /*result*/;
///	};
///	```
/// invents a type PF without template parameters that fulfills the callable
/// syntax, i.e. applying to it Args..., where sizeof...(Args) is the number of
/// template arguments of F, returns typename F<Args...>::type
///		Apply<PF, Args...> === F<Args...>::type where F<Args...> is valid.
/// By specializing F, one can easily fuse normal template metaprogramming with
/// this library, since F is completely under the users control.
///
/// Note [21-04-2017] Since clang brought up matching of template template parameters
/// and I verified that the standard is indeed ambiguous at the moment, this is
/// a messed up work-around I found.
/// Have a look at "template_template_args_count.hpp", we basically resolve
/// the situation by matching against several functions which each take a
/// single, specific number of arguments in its template template parameter.
/// We could even expand on this, and do some sfinae to match the special
/// template<typename...> template template parameter as well, into a constant
/// but that is for later, if needed.
///
/// A dense example of this in action can be found here
/// https://godbolt.org/g/uhQ4Cr
template<template<typename ...> typename F, size_t ncount = helper::args_count<F>>
struct _typeFunction;
template<template<typename ...> typename F>
using TypeFunction = typename _typeFunction<F>::type;

#if __cplusplus > 201701L
#  include "hdr/core/detail/type_function_n4659.hpp"
#elif __cplusplus > 201402L
#  include "hdr/core/detail/type_function_cpp14.hpp"
#else
#  error "Standards before c++14 are not supported"
#endif

///	Lifts a template struct to a pure function.
/// Having a templated type F declared as
/// ``` template</*some arguments*/> struct F; ```
/// invents a type without template parameters that fulfills the callable
/// syntax. Applying to it Args..., where sizeof...(Args) is the number of
/// template arguments of F, returns F<Args...>
/// Uses the same shenanigans as TypeFunction to match different
/// template template parameters
template<template<typename...> class F, size_t nargs = helper::args_count<F>>
struct _tFunction;
template<template<typename...> class F>
using TemplateFunction = TypeFunction<_tFunction<F>::template expr>;
template<template<typename...> class F>
using Constructor      = TemplateFunction<F>;
template<template<typename> typename F>
struct _tFunction<F, 1> {
	template<typename arg> struct expr {
		using type = F<arg>;
	};
};
template<template<typename,typename> typename F>
struct _tFunction<F, 2> {
	template<typename I1, typename I2> struct expr {
		using type = F<I1, I2>;
	};
};
template<template<typename,typename,typename> typename F>
struct _tFunction<F, 3> {
	template<typename I1, typename I2, typename I3> struct expr {
		using type = F<I1, I2, I3>;
	};
};
template<template<typename,typename,typename,typename> typename F>
struct _tFunction<F, 4> {
	template<typename I1, typename I2, typename I3, typename I4> struct expr {
		using type = F<I1, I2, I3, I4>;
	};
};

/**	Part of a set of functions to convert a function object to a pure function
 *	This transforms a function which takes 1 argument (a type whose call member
 *  has more than one template parameter). Rarely used, but for pure function F
 *  	Apply<Function<F>, T> === Apply<F, T> for all T
 */
template<typename F>
using Function = TemplateFunction<F::template expr>;
/** Callable form of the above, equivalent to an identity transform that only
 *  succeeds for functions. Calling it with a non-pure function is technically
 *  not intended nor fully defined by this library but will work fine.
 *  Note:
 *  This allows you to write types which can double as functions without
 *	restricting the arity of such functions to one. Simply map them through
 *	this function before calling. This is notational abuse, and should be
 *  done with extreme caution, and is not supported.
 */
using function = TemplateFunction<Function>;

}

#endif /* end of include guard: HEADERLIB_HDR_CORE_CONVERT_HPP */
