/*
 * hdrmath.hpp
 *
 *  Created on: 04.04.2016
 *      Author: andreas
 */

#pragma once
#include "hdr/std.hpp"
namespace hdrstd::math {

/**	Utility to convert integers to types
 */
HDR_BASE
HDR_CONVERT_TO HDR_TYPE
template<typename _Tp, _Tp v>
using to_int_constant = Const<std::integral_constant<_Tp, v>>;
template<unsigned u>
struct to_int_unsigned : to_int_constant<unsigned, u>{};

template<typename a, typename b>
struct plus{
	static_assert(_false<a>::value, "Can only operate on integral constants");
	using result = void;
};
template<typename _Tp, _Tp a, _Tp b>
struct plus<std::integral_constant<_Tp, a>, std::integral_constant<_Tp, b>>{
	using result = std::integral_constant<_Tp, a+b>;
};

} // hdrstd::math
