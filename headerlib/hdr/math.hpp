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
HDR_BASE
HDR_CONVERT_TO HDR_TYPE
template<typename _Tp, _Tp v>
using IntegralConstant = ::std::integral_constant<_Tp, v>;
template<unsigned u>
using Unsigned = IntegralConstant<unsigned, u>;
template<signed s>
using Signed = IntegralConstant<signed, s>;

template<typename a, typename b>
struct Plus {
	static_assert(::hdr::std::_false<a>::value, "No specialization provided for type");
};
using plus = ::hdr::std::TypeFunction2<Plus>;

template<typename _Tp, _Tp a, _Tp b>
struct Plus<IntegralConstant<_Tp, a>, IntegralConstant<_Tp, b>> {
	using type = IntegralConstant<_Tp, a+b>;
};

template<bool a, bool b>
struct Plus<IntegralConstant<bool, a>, IntegralConstant<bool, b>> {
	using type = IntegralConstant<bool, a || b>;
};

} // hdrstd::math

namespace hdr::std {

template<unsigned long v>
struct Printer<hdr::math::IntegralConstant<unsigned long, v>>{
	static void print(){
		printf("%lu", v);
	}
};

template<typename _Tp, _Tp v>
struct Printer<hdr::math::IntegralConstant<_Tp, v>>{
	static void print(){
		printf("%i", v);
	}
};

}
