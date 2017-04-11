#ifndef HEADERLIB_HDR_MATH_HPP
#define HEADERLIB_HDR_MATH_HPP
/*
 * hdrmath.hpp
 *
 *  Created on: 04.04.2016
 *      Author: andreas
 */

#pragma once
#include "hdr/std.hpp"
namespace hdr::math {

/**	Utility to convert integers to types
 */
template<typename T, T v>
using IntegralConstant = ::std::integral_constant<T, v>;
template<auto v>
using Value = IntegralConstant<decltype(v), v>;
template<unsigned u> using Unsigned = Value<u>;
template<signed s> 	 using Signed 	= Value<s>;
template<bool b> 		 using Bool 		= Value<b>;

template<typename a, typename b>
struct Plus {
	static_assert(::hdr::std::_false<a>::value, "No specialization provided for type");
};
using plus = ::hdr::std::TypeFunction<Plus>;
//using plus = GenericTypeFunction<Plus>;

template<typename _Tp, _Tp a, _Tp b>
struct Plus<Value<a>, Value<b>> {
	using type = Value<a+b>;
};

template<bool a, bool b>
struct Plus<Bool<a>, Bool<b>> {
	using type = Bool<a || b>;
};

template<typename a, typename b>
struct Mult {
	static_assert(::hdr::std::_false<a>::value, "No specialization provided for type");
};
using mult = ::hdr::std::TypeFunction<Mult>;

template<typename _Tp, _Tp a, _Tp b>
struct Mult<Value<a>, Value<b>> {
	using type = Value<a*b>;
};

template<bool a, bool b>
struct Mult<Bool<a>, Bool<b>> {
	using type = Bool<a && b>;
};

template<template<auto> typename F>
struct ValueTemplateFunction {
	template<typename Arg>
	using expr = Value<F<Arg::value>::value>;
};

} // hdrstd::math

namespace hdr::std {

template<unsigned long v>
struct Printer<hdr::math::Value<v>>{
	static void print(){
		printf("%lu", v);
	}
};

template<typename _Tp, _Tp v>
struct Printer<hdr::math::Value<v>>{
	static void print(){
		printf("%i", v);
	}
};

}
#endif //HEADERLIB_HDR_MATH_HPP
