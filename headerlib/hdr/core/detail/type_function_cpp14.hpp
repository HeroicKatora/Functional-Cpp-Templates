template<template<typename ...> typename F, size_t ncount = helper::args_count<F>>
struct _typeFunction;
template<template<typename ...> typename F>
using TypeFunction = typename _typeFunction<F>::type;
template<template<typename> typename F>
struct _typeFunction <F, 1> {
	struct type {
		template<typename Arg>
		using expr = typename F<Arg>::type;
	};
};
template<template<typename, typename> typename F>
struct _typeFunction <F, 2> {
	template<typename IA> struct _inner {
		template<typename I1>	using _Inner = F<IA, I1>;
		using type = TypeFunction<_Inner>;
	};
	using type = TypeFunction<_inner>;
};
template<template<typename, typename, typename> typename F>
struct _typeFunction <F, 3> {
	template<typename IA> struct _inner {
		template<typename I1, typename I2> using _Inner = F<IA, I1, I2>;
		using type = TypeFunction<_Inner>;
	};
	using type = TypeFunction<_inner>;
};
template<template<typename, typename, typename, typename> typename F>
struct _typeFunction <F, 4> {
	template<typename IA, typename IB> struct _inner {
		template<typename I1, typename I2> using _Inner = F<IB, IA, I1, I2>;
		using type = TypeFunction<_Inner>;
	};
	using type = TypeFunction<_inner>;
};
