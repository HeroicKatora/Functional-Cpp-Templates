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
using IntegralConstant = std::integral_constant<_Tp, v>;
template<unsigned u>
using Unsigned = IntegralConstant<unsigned, u>;
template<signed s>
using Signed = IntegralConstant<signed, s>;

template<typename a, typename b>
struct Plus {
	static_assert(_false<a>::value, "No specialization provided for type");
};
using plus = hdrstd::F2<hdrstd::TypeFunction2<Plus>>;

template<typename _Tp, _Tp a, _Tp b>
struct Plus<IntegralConstant<_Tp, a>, IntegralConstant<_Tp, b>> {
	using type = IntegralConstant<_Tp, a+b>;
};

template<bool a, bool b>
struct Plus<IntegralConstant<bool, a>, IntegralConstant<bool, b>> {
	using type = IntegralConstant<bool, a || b>;
};

} // hdrstd::math
