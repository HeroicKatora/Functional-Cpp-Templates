/*
 * hdrmath.hpp
 *
 *  Created on: 04.04.2016
 *      Author: andreas
 */

#pragma once
#include "hdrstd.hpp"

template<typename a, typename b>
struct plus{
	static_assert(_false<a>::value, "Can only operate on integral constants");
	using result = void;
};
template<typename _Tp, _Tp a, _Tp b>
struct plus<std::integral_constant<_Tp, a>, std::integral_constant<_Tp, b>>{
	using result = std::integral_constant<_Tp, a+b>;
};
