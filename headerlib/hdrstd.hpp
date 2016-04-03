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

	template<template<typename...> class f, typename ...arg>
	struct expression{
		template<typename>
		using expr = f<arg...>;
	};

	template<bool s, template<typename> class A, template<typename> class B>
	struct conditional{
		template<typename>
		using expr = A<Void>;
	};
	template<template<typename> class A, template<typename> class B>
	struct conditional<false, A, B>{
		template<typename>
		using expr = B<Void>;
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
	template<>
	struct Printer<std::false_type>{
		static void print(){
			printf("false");
		}
	};
	template<>
	struct Printer<std::true_type>{
		static void print(){
			printf("true");
		}
	};
	template<unsigned long v>
	struct Printer<std::integral_constant<unsigned long, v>>{
		static void print(){
			printf("%lu", v);
		}
	};
	template<typename _Tp, _Tp v>
	struct Printer<std::integral_constant<_Tp, v>>{
		static void print(){
			printf("%i", v);
		}
	};

	void sink_args(...){
	};
#define SINK_ARRAY(T, s, l) sink_args(new T[s] l)

}
