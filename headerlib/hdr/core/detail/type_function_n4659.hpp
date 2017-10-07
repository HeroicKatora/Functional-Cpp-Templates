#ifndef HEADERLIB_HDR_CORE_DETAIL_TYPE_FUNCTION_HPP
#define HEADERLIB_HDR_CORE_DETAIL_TYPE_FUNCTION_HPP
template<template<typename...> typename F, size_t nargs = helper::args_count<F>, typename...>
struct PartialTypeFunction;

template<template<typename...> typename F, typename... Args, typename LastArg>
struct _apply<PartialTypeFunction<F, sizeof...(Args) + 1, Args...>, LastArg> {
	using type = typename F<Args..., LastArg>::type;
};

template<template<typename...> typename F, size_t nargs, typename... Args, typename NextArg>
struct _apply<PartialTypeFunction<F, nargs, Args...>, NextArg> {
	static_assert(nargs > sizeof...(Args), "Function partially applied to more arguments than it has");
	using type = PartialTypeFunction<F, nargs, Args..., NextArg>;
};

template<template<typename ...> typename F, size_t ncount>
struct _typeFunction {
	using type = PartialTypeFunction<F, ncount>;
};
#endif /* end of include guard: HEADERLIB_HDR_CORE_DETAIL_TYPE_FUNCTION_HPP */
