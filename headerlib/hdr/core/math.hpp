#ifndef HEADERLIB_HDR_CORE_MATH_HPP
#define HEADERLIB_HDR_CORE_MATH_HPP
/*
 * math.hpp
 *
 *  Created on: 20.04.2016
 *      Author: HeroicKatora
 */
#include "hdr/core/std.hpp"
namespace hdr::math {

/** Test for structural equality
 */
template<typename A, typename B>
using Same  = ::hdr::std::FromStdBool<::std::is_same_v<A, B>>;
using same  = ::hdr::std::TemplateFunction<Same>;

template<typename T, T v>
using IntegralConstant = ::std::integral_constant<T, v>;
template<auto v>
using Value = IntegralConstant<decltype(v), v>;

template<typename a, typename b>
struct Plus {
	static_assert(::hdr::std::_false<a>::value, "No specialization provided for type");
};
using plus = ::hdr::std::TypeFunction<Plus>;

template<typename a, typename b>
struct Mult {
	static_assert(::hdr::std::_false<a>::value, "No specialization provided for type");
};
using mult = ::hdr::std::TypeFunction<Mult>;

template<typename a, typename b>
struct Minus {
	static_assert(::hdr::std::_false<a>::value, "No specialization provided for type");
};
using mult = ::hdr::std::TypeFunction<Mult>;

template<typename a, typename b>
struct Div {
	static_assert(::hdr::std::_false<a>::value, "No specialization provided for type");
};
using mult = ::hdr::std::TypeFunction<Mult>;

template<template<auto> typename F>
struct ValueTemplateFunction {
	template<typename Arg>
	using expr = Value<F<Arg::value>::value>;
};

} // hdrstd::math
#endif //HEADERLIB_HDR_MATH_HPP
