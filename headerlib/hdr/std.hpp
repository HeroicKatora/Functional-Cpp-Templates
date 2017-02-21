/*
 * hdrstd.hpp
 *
 *  Created on: 02.04.2016
 *      Author: andreas
 */

#pragma once
#include <cstdio>
#include <utility>
#include <type_traits>

/** Signal that the following symbol is a base function, i.e. of the style
 *		template<Args...> using function = <result>;
 */
#define HDR_BASE

/**	A simple type, i.e of the style
 *		typedef A <type>;
 *	or
 *		using <type> = A;
 */
#define HDR_TYPE

/** Signal that the following symbol is a function object, i.e. of the style
 *		struct { template<Args...> using expr = <result>; };
 */
#define HDR_OBJECT

/** Signal that the following symbol is a pure function object, i.e. of the style
 *		struct { template<Arg> using expr = <result>; };
 */
#define HDR_FUNCTION

/**	The symbol is a Type or Pure, it can be fully references by just its typename
 */
#define HDR_PURE

/**	Signal that the following symbol is a monad function object, i.e. of the style
 *		FMonad (a -> b) := () -> (a -> b)
 *	In general, you should use these for your own implementations as they force the
 *	compiler to do lazy evaluation of results.
 */
#define HDR_MONAD

/** The following symbol performs a conversion between the above types.
 *	You should read it as an operator a -> b.
 */
#define HDR_CONVERT_TO

/**	Designates the symbol as a mapping some things to others.
 *	Read this as an operator as well.
 */
#define HDR_MAP_TO
namespace hdrstd{
	/**
	 * A function should be a templated struct with a member named result.
	 * An expression is a function that takes one template parameter but ignores it,
	 * it will/should generally be called with Void as the generic parameter. See them
	 * as lazy evaluated function calls, there is also a wrapper for exactly this.
	 */

	/**	The unit type. It CAN be inherited to signal a definition of a new type.
	 */
	struct Void {} __attribute__((packed));

	/**	Convenience to produce unevaluated, always failing static_asserts, e.g.
	 *	to create an error message in invalid partial specializations.
	 */
	template<typename>
	struct _false:std::false_type {
	};
	template<typename T, T value>
	struct _false_integral:std::false_type {
	};

	/**	Lifts a base function to a pure one. For more than one argument, you should
	 *	generally wrap it into a class and then use F1,...
	 */
	HDR_BASE HDR_CONVERT_TO HDR_FUNCTION
	template<template<typename> class f>
	struct Function {
		template<typename arg>
		using expr = f<arg>;
	};

	template<template<typename> class f>
	struct TypeFunction {
		template<typename arg>
		using expr = typename f<arg>::type;
	};

	/**	Hides a type from immediate substitution in template using declarations.
	 */
	HDR_BASE
	HDR_PURE HDR_CONVERT_TO HDR_MONAD
	template<typename val>
	struct Const {
		template<typename>
		using expr = val;
	};

	/**	Part of a set of functions to convert a function object to a pure function
	 *	This transforms a function which takes 1 argument.
	 */
	HDR_BASE
	HDR_OBJECT HDR_CONVERT_TO HDR_FUNCTION
	template<typename F>
	using F1 = F;

	/**	Part of a set of functions to convert a function object to a pure function
	 *	This transforms a function which takes 2 argument.
	 */
	HDR_BASE
	HDR_OBJECT HDR_CONVERT_TO HDR_FUNCTION
	template<typename F>
	struct F2 {
		template<typename A1> struct _rec {
			template<typename A2>
			using expr = typename F::template expr<A1, A2>;
		};
		template<typename Arg>
		using expr = _rec<Arg>;
	};

	/**	Part of a set of functions to convert a function object to a pure function
	 *	This transforms a function which takes 3 argument.
	 */
	HDR_BASE
	HDR_OBJECT HDR_CONVERT_TO HDR_FUNCTION
	template<typename F>
	struct F3 {
		template<typename A1>	struct _rec {
			template<typename A2, typename A3>
			using expr = typename F::template expr<A1, A2, A3>;
		};
		template<typename Arg>
		using expr = F2<_rec<Arg>>;
	};

	/**	Part of a set of functions to convert a function object to a pure function
	 *	This transforms a function which takes 4 argument.
	 */
	HDR_BASE
	HDR_OBJECT HDR_CONVERT_TO HDR_FUNCTION
	template<typename F>
	struct F4 {
		template<typename A1>	struct _rec {
			template<typename A2, typename A3, typename A4>
			using expr = typename F::template expr<A1, A2, A3, A4>;
		};
		template<typename Arg>
		using expr = F3<_rec<Arg>>;
	};

	/**	Type Definition of true, should be used as a parameter instead of bools.
	 */
	HDR_TYPE
	using True = std::true_type;
	/**	Type Definition of false, should be used as a parameter instead of bools.
	 */
	HDR_TYPE
	using False = std::false_type;

	/**
	 * Partial function application
	 */
	template<typename F, typename... args>
	struct _Apply;
	template<typename F>
	struct _Apply<F> {
		using result = F;
	};
	template<typename F, typename FirstArg, typename... args>
	struct _Apply<F, FirstArg, args...> {
		using _applied = typename F::template expr<FirstArg>;
		using result = typename _Apply<_applied, args...>::result;
	};
	HDR_BASE
	HDR_PURE HDR_PURE /*...*/ HDR_MAP_TO HDR_PURE
	template<typename F, typename ...args>
	using Apply = typename _Apply<F, args...>::result;

	/** Identity function for all Objects
	 *		(a -> a)
	 *	Can also be used as a functional form of apply
	 */
	HDR_FUNCTION
	struct id {
		template<typename F>
		using expr = F;
	};
	//Function Object of Apply
	using apply = id;

	/**
	 * Might make binary operators more intuitive, alternative to Apply
	 */
	HDR_BASE
	template<typename aarg, typename op, typename barg>
	using Binary = Apply<op, aarg, barg>;

	/**	Evaluates function composition, you might instead want to use
	 *		Apply<compose, F, G>
	 *	for clarity since it doesn't mix up Pures and Objects.
	 */
	HDR_OBJECT
	HDR_FUNCTION HDR_FUNCTION HDR_MAP_TO HDR_FUNCTION
	struct Compose{
		template<typename F, typename G, typename Arg>
		using expr = Apply<F, Apply<G, Arg>>;
	};
	/**	Binary operator for function composition
	 *		(a -> b) -> (b -> c) -> (a -> c)
	 */
	HDR_FUNCTION
	using compose = F3<Compose>;
	HDR_FUNCTION
	using c = compose;

	/** Flips the order of two arguments
	 * 		(a -> b -> c) -> (b -> a -> c)
	 */
	struct Flip {
		template<typename F, typename A, typename B>
		using expr = Apply<F, B, A>;
	};
	using flip = F3<Flip>;

	/**	Ignores the second argument
	 *	(a -> b -> a)
	 */
	HDR_FUNCTION
	HDR_PURE HDR_MAP_TO HDR_PURE HDR_MAP_TO HDR_PURE
	using fconst = Function<Const>;

	template<bool c, typename A, typename B>
	struct _Conditional{
		using result = A;
	};
	template<typename A, typename B>
	struct _Conditional<false, A, B>{
		using result = B;
	};
	template<bool c, typename A, typename B>
	using Conditional = typename _Conditional<c, A, B>::result;


	struct When_Else{//Function Object of Conditional
		template<typename C, typename T>
		using CVal = typename C::template expr<T>;
		template<typename C, typename T>
		constexpr static const bool c = std::is_same<CVal<C, T>, True>::value;
		template<typename C, typename A, typename B, typename T>
		using expr = typename Conditional<c<C, T>, A, B>::template expr<T>;
	};
	using when_else = F4<When_Else>;

	struct Sink{
		template<typename ...T>
		Sink(T&&... v){};
	};

	// template<typename t>
	// using _false = _convert::_false<t>;
	// template<typename T, T value>
	// using _false_integral = _convert::_false_integral<T, value>;
	//
	// template<typename T, T value>
	// using to_int_constant = _convert::to_int_constant<T, value>;
	// template<unsigned value>
	// using to_int_unsigned = _convert::to_int_constant<unsigned, value>;
	//
	// template<typename value>
	// using Const = _convert::Const<value>;
	//
	// template<template<typename> class RawFunction>
	// using Function = _convert::template Function<RawFunction>;
	//
	// template<typename PseudoFunction>
	// using F1 = _convert::F1<PseudoFunction>;
	// template<typename PseudoFunction>
	// using F2 = _convert::F2<PseudoFunction>;
	// template<typename PseudoFunction>
	// using F3 = _convert::F3<PseudoFunction>;
	// template<typename PseudoFunction>
	// using F4 = _convert::F4<PseudoFunction>;
	//
	// template<typename T, typename... args>
	// using Apply = _impl::template Apply<T, args...>;
	//
	// using freturn = _impl::freturn;
	// using _impl::apply;
	// using _impl::compose;
	// using _impl::when_else;

	template<typename Instance>
	struct Type{
		static_assert(_false<Instance>::value, "No type information available");
		template<typename TypeKey>
		struct Impl;
	};

	template<>
	struct Type<Void>{
		template<typename TypeKey>
		struct Impl;
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

	void sink_args(...){};

	template<typename A, typename Op, typename B>
	using b_ = Binary<A, Op, B>;

	template<typename>
	struct Debug;
}
