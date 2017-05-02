
template<template<typename...> typename F, size_t nargs = helper::args_count<F>, typename...>
struct PartialTemplateFunction;
template<template<typename...> typename F, typename...Args>
struct PartialTemplateFunction<F, sizeof...(Args), Args...> {
	using type = typename F<Args...>;
};
template<template<typename...> typename F, size_t nargs, typename... Args>
struct PartialTemplateFunction {
	static_assert(nargs > sizeof...(Args), "Function partially applied to more arguments than it has");
	using type = PartialTemplateFunction;
};
template<template<typename...> typename F, size_t nargs, typename... Args, typename NewArg>
struct _apply<PartialTemplateFunction<F, nargs, Args...>, NewArg> {
	using type = typename PartialTemplateFunction<F, nargs, Args..., NewArg>::type;
};

template<template<typename ...> typename F, size_t ncount = helper::args_count<F>>
struct _templateFunction {
	using type = PartialTypeFunction<F, ncount>;
};
