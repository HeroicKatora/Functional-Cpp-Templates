#ifndef HEADERLIB_HDR_CORE_DETAIL_TEMPLATE_FUNCTION_HPP
#define HEADERLIB_HDR_CORE_DETAIL_TEMPLATE_FUNCTION_HPP
template<template<typename...> typename F, size_t nargs = helper::args_count<F>, typename...>
struct PartialTemplateFunction;
template<template<typename...> typename F, typename...Args>
struct PartialTemplateFunction<F, sizeof...(Args) +1, Args...> {
	template<typename LastArg>
	using expr = F<Args..., LastArg>;
};
template<template<typename...> typename F, size_t nargs, typename... Args>
struct PartialTemplateFunction {
	static_assert(nargs > sizeof...(Args), "Function partially applied to more arguments than it has");
	template<typename NextArg>
	using expr = PartialTemplateFunction<F, nargs, Args..., NextArg>;
};

template<template<typename ...> typename F, size_t ncount>
struct _templateFunction {
	using type = PartialTemplateFunction<F, ncount>;
};
#endif /* end of include guard: HEADERLIB_HDR_CORE_DETAIL_TEMPLATE_FUNCTION_HPP */
