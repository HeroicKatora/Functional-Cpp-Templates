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
	/**
	 * A function should be a templated struct with a member named result.
	 * An expression is a function that takes one template parameter but ignores it,
	 * it will/should generally be called with Void as the generic parameter. See them
	 * as lazy evaluated function calls, there is also a wrapper for exactly this.
	 */

	struct Void{} __attribute__((packed));
	namespace _impl{
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
		template<typename val>
		struct expression_val{
			template<typename>
			struct expr{
				using result = val;
			};
		};
		template<template<typename...> class f>
		struct function{
			template<typename ...arg>
			using expr = f<arg...>;
		};
		template<template<typename...> class F>
		using f_ = function<F>;

		template<typename f, typename g>
		struct composition{
			template<typename arg>
			struct expr{
				using _inter = typename g::template expr<arg>::result;
				using _res = typename f::template expr<_inter>::result;
				using result = _res;
			};
		};
		template<typename f, typename g>
		using c_ = composition<f, g>;

		template<bool s, typename A, typename B>
		struct conditional{
			template<typename>
			using expr = typename A::template expr<Void>;
		};
		template<typename A, typename B>
		struct conditional<false, A, B>{
			template<typename>
			using expr = typename B::template expr<Void>;
		};

		struct Sink{
			template<typename ...T>
			Sink(T&&... v){};
		};

		/**
		 * Makes binary operators more intuitive
		 */
		template<typename aarg, template<typename, typename> class op, typename barg>
		struct b_{
			using result = typename op<aarg, barg>::result;
		};

		/**
		 * Partial function application
		 */
		template<template<typename...> class f, typename ...args>
		struct p_{
			//static_assert(sizeof (par) > sizeof...(args), "Partial application implies left over parameters");
			template<typename ...missing>
			struct result_c{
				using result = typename f<args..., missing...>::result;
			};
			template<typename ...missing>
			using result = result_c<missing...>;
		};
	}

	template<typename t>
	using _false = _impl::_false<t>;
	template<typename T, T value>
	using _false_integral = _impl::_false_integral<T, value>;

	template<typename T, T value>
	using to_int_constant = _impl::to_int_constant<T, value>;
	template<unsigned u>
	using to_int_unsigned = _impl::to_int_unsigned<u>;

	template<typename T>
	using from_int_constant = _impl::from_int_constant<T>;

	template<template<typename...> class Function, typename ...args>
	using expression = _impl::expression<Function, args...>;
	template<typename value>
	using expression_val = _impl::expression_val<value>;

	template<template<typename...> class Function>
	using function = _impl::function<Function>;
	template<template<typename...> class Function>
	using f_ = _impl::f_<Function>;

	template<typename FunctionA, typename FunctionB>
	using composition = _impl::composition<FunctionA, FunctionB>;

	template<typename FunctionA, typename FunctionB>
	using c_ = _impl::c_<FunctionA, FunctionB>;

	using _impl::conditional;

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

	void sink_args(...){};
	using Sink = _impl::Sink;

	template<typename A, template<typename, typename> class Op, typename B>
	using b_ = _impl::b_<A, Op, B>;

	template<template<typename...> class Function, typename ...args>
	using p_ = _impl::p_<Function, args...>;

}
