#ifndef HEADERLIB_HDR_CORE_MATH_HPP
#define HEADERLIB_HDR_CORE_MATH_HPP
/*
 * math.hpp
 *
 *  Created on: 20.04.2016
 *      Author: HeroicKatora
 */
#include "hdr/core/std.hpp"
#include "hdr/core/lambda.hpp"
namespace hdr::math {

/** Test for structural equality
 */
template<typename A, typename B>
using Same  = ::hdr::std::FromStdBool<::std::is_same<A, B>::value>;
using same  = ::hdr::std::TemplateFunction<Same>;

template<typename T, T v>
struct IntegralConstant;

template<auto v>
using Value = IntegralConstant<decltype(v), v>;
using Zero  = Value<0>;
using One   = Value<1>;

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
using minus  = ::hdr::std::TypeFunction<Minus>;
using uminus = ::hdr::lambda::Lambda<minus, ::hdr::lambda::IApply<minus, ::hdr::lambda::_0, ::hdr::lambda::_0>, ::hdr::lambda::_0>;

template<typename a, typename b>
struct Div {
	static_assert(::hdr::std::_false<a>::value, "No specialization provided for type");
};
using div = ::hdr::std::TypeFunction<Div>;

template<typename a, typename b>
struct Compare {
	static_assert(::hdr::std::_false<a>::value, "No specialization provided for type");
};
using compare = ::hdr::std::TypeFunction<Compare>;

template<typename> struct ValueOf; // resolves to type with constexpr member
template<typename T> constexpr static auto value_of_v = ValueOf<T>::value;

template<>
struct ValueOf<::hdr::std::True>
	{ using type = ::std::true_type;
		constexpr static auto value = true;};
template<>
struct ValueOf<::hdr::std::False>
	{ using type = ::std::false_type;
		constexpr static auto value = false;};

template<template<auto> typename F>
struct ValueTemplateFunction {
	template<typename Arg>
	using expr = Value<F<ValueOf<Arg>::value>::value>;
};

} // hdrstd::math
#endif //HEADERLIB_HDR_MATH_HPP
