
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
