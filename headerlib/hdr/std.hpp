#ifndef HEADERLIB_HDR_STD_HPP
#define HEADERLIB_HDR_STD_HPP
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
namespace hdr::std {
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
	struct _false: ::std::false_type {
	};
	template<typename T, T value>
	struct _false_integral: ::std::false_type {
	};

	/** Wraps struct types which declare a member named type to display their result
	 *	This is mostly for c++ standard classes but might be used to implement functions
	 *	via template specializations.
	 */
	template<template<typename ...> typename F>
	struct _TypeFunction;
	template<template<typename ...> typename F>
	using TypeFunction = typename _TypeFunction<F>::type;

	template<template<typename> typename F>
	struct _TypeFunction<F> {
		struct _final {
			template<typename Arg>
			using expr = typename F<Arg>::type;
		};
		using type = _final;
	};
	template<template<typename, typename> typename F>
	struct _TypeFunction <F> {
		template<typename IA> struct _inner {
			template<typename I1>	using _Inner = F<IA, I1>;
			using type = ::hdr::std::TypeFunction<_Inner>;
		};
		using type = ::hdr::std::TypeFunction<_inner>;
	};
	template<template<typename, typename, typename> typename F>
	struct _TypeFunction <F> {
		template<typename IA> struct _inner {
			template<typename I1, typename I2> using _Inner = F<IA, I1, I2>;
			using type = ::hdr::std::TypeFunction<_Inner>;
		};
		using type = ::hdr::std::TypeFunction<_inner>;
	};
	template<template<typename, typename, typename, typename> typename F>
	struct _TypeFunction <F> {
		template<typename IA, typename IB> struct _inner {
			template<typename I1, typename I2> using _Inner = F<IB, IA, I1, I2>;
			using type = ::hdr::std::TypeFunction<_Inner>;
		};
		using type = ::hdr::std::TypeFunction<_inner>;
	};

	/**	Lifts a base function to a pure one. For more than one argument, you should
	 *	generally wrap it into a class and then use F1,...
	 */
	HDR_BASE HDR_CONVERT_TO HDR_FUNCTION
	template<template<typename...> class _>
	struct _TFunction;
	template<template<typename...> class F>
	using TemplateFunction = TypeFunction<_TFunction<F>::template expr>;
	template<template<typename> typename F>
	struct _TFunction<F> {
		template<typename arg> struct expr {
			using type = F<arg>;
		};
	};
	template<template<typename,typename> typename F>
	struct _TFunction<F> {
		template<typename I1, typename I2> struct expr {
			using type = F<I1, I2>;
		};
	};
	template<template<typename,typename,typename> typename F>
	struct _TFunction<F> {
		template<typename I1, typename I2, typename I3> struct expr {
			using type = F<I1, I2, I3>;
		};
	};
	template<template<typename,typename,typename,typename> typename F>
	struct _TFunction<F> {
		template<typename I1, typename I2, typename I3, typename I4> struct expr {
			using type = F<I1, I2, I3, I4>;
		};
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
	using Function = TemplateFunction<F::template expr>;

	/**	Type Definition of true, should be used as a parameter instead of bools.
	 */
	HDR_TYPE
	using True = ::std::true_type;
	/**	Type Definition of false, should be used as a parameter instead of bools.
	 */
	HDR_TYPE
	using False = ::std::false_type;

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
	template<typename F, typename G, typename Arg>
	struct Compose{
		using type = Apply<F, Apply<G, Arg>>;
	};
	/**	Binary operator for function composition
	 *		(a -> b) -> (b -> c) -> (a -> c)
	 */
	HDR_FUNCTION
	using compose = TypeFunction<Compose>;
	HDR_FUNCTION
	using c = compose;

	/** Flips the order of two arguments
	 * 		(a -> b -> c) -> (b -> a -> c)
	 */
	template<typename F, typename A, typename B>
	struct Flip {
		using type = Apply<F, B, A>;
	};
	using flip = TypeFunction<Flip>;

	/**	Ignores the second argument
	 *	(a -> b -> a)
	 */
	HDR_FUNCTION
	HDR_PURE HDR_MAP_TO HDR_PURE HDR_MAP_TO HDR_PURE
	using fconst = TemplateFunction<Const>;

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


	struct When_Else {//Function Object of Conditional
		template<typename C>
		constexpr static const bool c = ::std::is_same_v<C, True>;
		template<typename C, typename A, typename B>
		using expr = Conditional<c<C>, A, B>;
	};
	using when_else = Function<When_Else>;

	struct Sink{
		template<typename ...T>
		Sink(T&&... v){};
	};

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
	struct Printer<False>{
		static void print(){
			printf("false");
		}
	};
	template<>
	struct Printer<True>{
		static void print(){
			printf("true");
		}
	};

	void sink_args(...){};

	template<typename A, typename Op, typename B>
	using b_ = Binary<A, Op, B>;

	template<typename>
	struct Debug;
}
#endif //HEADERLIB_HDR_STD_HPP
