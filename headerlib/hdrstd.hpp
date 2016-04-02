/*
 * hdrstd.hpp
 *
 *  Created on: 02.04.2016
 *      Author: andreas
 */

#pragma once
#include <utility>
#include <type_traits>

namespace hdrstd{
	struct Void{} __attribute__((packed));
	template<typename>
	struct _false:std::false_type{
	};
	template<typename T, T value>
	struct _false_integral:std::false_type{
	};


	template<typename _Tp, _Tp v>
	struct to_int_constant{
		using result = std::integral_constant<_Tp, v>;
	};
	template<unsigned u>
	struct to_int_unsigned : to_int_constant<unsigned, u>{};

	template<typename A>
	struct from_int_constant{
		static_assert(_false<A>::value, "Not an integral constant");
	};
	template<typename _Tp, _Tp v>
	struct from_int_constant<std::integral_constant<_Tp, v>>{
		constexpr static _Tp result = v;
	};

	template<bool s, template<typename> class a, template<typename> class b>
	struct conditional{
		template<typename>
		using expr = a<Void>;
	};
	template<template<typename> class a, template<typename> class b>
	struct conditional<false, a, b>{
		template<typename>
		using expr = b<Void>;
	};
	template<typename O>
	struct Printer{
		static_assert(_false<O>::value, "No printer for target class defined");
		static void print();
	};
	template<>
	struct Printer<Void>{
		static void print(){};
	};
	void sink_args(...){};
}
