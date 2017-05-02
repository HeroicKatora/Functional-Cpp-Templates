#ifndef HEADERLIB_HDR_CORE_DETAIL_TYPE_FUNCTION_HPP
#define HEADERLIB_HDR_CORE_DETAIL_TYPE_FUNCTION_HPP
template<template<typename...> typename F, size_t nargs = helper::args_count<F>, typename...>
struct PartialTypeFunction;
template<template<typename...> typename F, typename... Args>
struct PartialTypeFunction<F, sizeof...(Args) + 1, Args...> {
	template<typename LastArg>
	using expr = typename F<Args..., LastArg>::type;
};
template<template<typename...> typename F, size_t nargs, typename... Args>
struct PartialTypeFunction {
	static_assert(nargs > sizeof...(Args), "Function partially applied to more arguments than it has");
	template<typename NextArg>
	using expr = PartialTypeFunction<F, nargs, Args..., NextArg>;
};

template<template<typename ...> typename F, size_t ncount>
struct _typeFunction {
	using type = PartialTypeFunction<F, ncount>;
};
#endif /* end of include guard: HEADERLIB_HDR_CORE_DETAIL_TYPE_FUNCTION_HPP */
