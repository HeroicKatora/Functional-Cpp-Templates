
template<template<typename...> typename F, size_t nargs = helper::args_count<F>, typename...>
struct PartialTypeFunction;
template<template<typename...> typename F, typename...Args>
struct PartialTypeFunction<F, sizeof...(Args), Args...> {
	using type = typename F<Args...>::type;
};
template<template<typename...> typename F, size_t nargs, typename... Args>
struct PartialTypeFunction {
	static_assert(nargs > sizeof...(Args), "Function partially applied to more arguments than it has");
	using type = PartialTypeFunction;
};
template<template<typename...> typename F, size_t nargs, typename... Args, typename NewArg>
struct _apply<PartialTypeFunction<F, nargs, Args...>, NewArg> {
	using type = typename PartialTypeFunction<F, nargs, Args..., NewArg>::type;
};

template<template<typename ...> typename F, size_t ncount = helper::args_count<F>>
struct _typeFunction {
	using type = PartialTypeFunction<F, ncount>;
};
