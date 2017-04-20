#ifndef HEADERLIB_HDR_CORE_STD_HPP
#define HEADERLIB_HDR_CORE_STD_HPP
/*
 * hdrstd.hpp
 * A set of functions to denote function application, unify declarations and some
 * very broadly needed functionality. headerlib is not strongly typed, the functional
 * types are mostly denoted in comments above declarations. One day I might
 * think about giving type hints and doing optional type checking, but this
 * will probably be after the introduction of concepts.
 *
 *  Created on: 02.04.2016
 *      Author: andreas
 */

#include <cstdio>
#include <utility>
#include <type_traits>

/** This namespace holds all core utilities (and a few symbols more).
 *  Don't create any partial specializations where not explicitely noted.
 *
 *  The declatations in this namespace will not follow recommended principles,
 *  partly because the necessary structures aren't yet available or to offer
 *  slightly more optimized version. In case you are looking for examples,
 *  view headers outside of the core namespaces which operate fully within the
 *  invented functional type system.
 */
namespace hdr::std {

	/** Main point of the library, reference for the call syntax.
	 *  Everything else is stacked onto this definition. This is implemented
	 *  by template specialization which is what this library tries to steer you
	 *  away from. Don't mind the irony.
	 */
	template<typename F, typename Arg>
	using ApplySingle = typename F::template expr<Arg>;

	template<typename F, typename... args>
	struct _Apply;
	template<typename F>
	struct _Apply<F> {
		using result = F;
	};
	template<typename F, typename FirstArg, typename... args>
	struct _Apply<F, FirstArg, args...> {
		using _applied = ApplySingle<F, FirstArg>;
		using result = typename _Apply<_applied, args...>::result;
	};
	/**
	 * (Partial) function application
	 */
	template<typename F, typename ...args>
	using Apply = typename _Apply<F, args...>::result;

	/**
	 * A function should be a templated struct with a member named result.
	 * An expression is a function that takes one template parameter but ignores it,
	 * it will/should generally be called with Void as the generic parameter. See them
	 * as lazy evaluated function calls, there is also a wrapper for exactly this.
	 */

	/**	The unit type. It CAN be inherited to signal the definition of a new type.
	 *  I'm not consistent about it, it's just here for convenience.
	 */
	struct Void {} __attribute__((packed));

	/**	Convenience to produce unevaluated, always failing static_asserts, e.g.
	 *	to create an error message in invalid partial specializations.
	 *  ```
	 *  template<typename T> struct CantInstantiate {
	 *		static_assert(_false<T>::value);
 	 *	};
	 *	```
	 *  Important, don't create any partial specializations.
	 */
	template<typename>
	struct _false: ::std::false_type {
	};
	template<auto value>
	struct _false_integral: ::std::false_type {
	};

	template<template<typename ...> typename F>
	struct _TypeFunction;
	/// Wraps struct types which declare a member named type to signal their result.
	/// This is mostly for c++ standard classes but may be used to implement functions
	/// via template specializations.
 	/// Having a templated type F declared as
 	/// ```
	///	template</*some arguments*/> struct F {
	/// 	/*implementation*/
	///		using type = /*result*/;
	///	};
	///	```
 	/// invents a type PF without template parameters that fulfills the callable
 	/// syntax, i.e. applying to it Args..., where sizeof...(Args) is the number of
 	/// template arguments of F, returns typename F<Args...>::type
	///		Apply<PF, Args...> === F<Args...>::type where F<Args...> is valid.
	/// By specializing F, one can easily fuse normal template metaprogramming with
	/// this library.
	template<template<typename ...> typename F>
	using TypeFunction = typename _TypeFunction<F>::type;

	template<template<typename> typename F>
	struct _TypeFunction<F> {
		struct type {
			template<typename Arg>
			using expr = typename F<Arg>::type;
		};
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

	template<template<typename...> class _>
	struct _TFunction;
	///	Lifts a template struct to a pure function.
	/// Having a templated type F declared as
	/// ``` template</*some arguments*/> struct F; ```
	/// invents a type without template parameters that fulfills the callable
	/// syntax. Applying to it Args..., where sizeof...(Args) is the number of
	/// template arguments of F, returns F<Args...>
	template<template<typename...> class F>
	using TemplateFunction = TypeFunction<_TFunction<F>::template expr>;
	template<template<typename...> class F>
	using Constructor      = TemplateFunction<F>;
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

	/**	Part of a set of functions to convert a function object to a pure function
	 *	This transforms a function which takes 1 argument (a type whose call member
   *  has more than one template parameter). Rarely used, but for pure function F
	 *  	Apply<Function<F>, T> === Apply<F, T> for all T
	 */
	template<typename F>
	using Function = TemplateFunction<F::template expr>;
	/** Callable form of the above, equivalent to an identity transform that only
	 *  succeeds for functions. Calling it with a non-pure function is technically
	 *  not intended nor fully defined by this library but will work fine.
	 *  Note:
	 *  This allows you to write types which can double as functions without
	 *	restricting the arity of such functions to one. Simply map them through
	 *	this function before calling. This is notational abuse, and should be
	 *  done with extreme caution, and is not supported.
	 */
	using function = TemplateFunction<Function>;

	/**	Hides a type from immediate substitution in template using declarations.
	 *  Use it as Const<Value>.
	 *  It holds that Apply<Const<V>, T> === V for any (V, T)
	 */
	template<typename val>
	struct Const;
	template<typename Val, typename FirstArg, typename... args>
	struct _Apply<Const<Val>, FirstArg, args...> {
		using result = typename _Apply<Val, args...>::result;
	};

	/**	Constructor of the above as well as standalone function.
	 *  Ignores the second argument or constructs Const<a>
	 *	(a -> b -> a)
	 */
	using fconst = TemplateFunction<Const>;

	/** A function which will always return itself, infinitely
	 */
	struct VoidFunction;
	template<typename FirstArg, typename... args>
	struct _Apply<VoidFunction, FirstArg, args...> {
		using result = typename _Apply<VoidFunction, args...>::result;
	};

	/**	Type Definition of true, should be used as a parameter instead of bools.
	 */
	using True = ::std::true_type;
	/**	Type Definition of false, should be used as a parameter instead of bools.
	 */
	using False = ::std::false_type;

	/** Identity function for all Objects
	 *		(a -> a)
	 *	Can also be used as a functional form of apply.
	 *  We could also implement this as
	 *		template<typename T> using _id = T; using id = TemplateFunction<id>;
	 *	which would be neutral to the calling style but necessitate an additional
	 *  indirection through another type.
	 */
	struct id;
	template<typename FirstArg, typename... args>
	struct _Apply<id, FirstArg, args...> {
		using result = typename _Apply<FirstArg, args...>::result;
	};
	//Function Object of Apply
	using apply = id;

	/** Might make binary operators more intuitive, alternative to Apply. I haven't
	 *  felt the need to use it yet.
	 */
	template<typename aarg, typename op, typename barg>
	using Binary = Apply<op, aarg, barg>;

	/**	Evaluates function composition, you might instead want to use
	 *		Apply<compose, F, G>
	 *	for clarity since it doesn't mix up Pures and Objects.
	 */
	template<typename F, typename G, typename Arg>
	struct Compose {
		using type = Apply<F, Apply<G, Arg>>;
	};
	/**	Binary operator for function composition
	 *		(a -> b) -> (b -> c) -> (a -> c)
	 */
	using compose = TypeFunction<Compose>;
	using c = compose;

	/** Flips the order of two arguments
	 * 		(a -> b -> c) -> (b -> a -> c)
	 */
	template<typename F, typename B, typename A>
	struct Flip {
		using type = Apply<F, A, B>;
	};
	using flip   = TypeFunction<Flip>;
	using binary = Apply<flip, apply>;

	template<bool c, typename A, typename B>
	struct _Conditional {
		using result = A;
	};
	template<typename A, typename B>
	struct _Conditional<false, A, B> {
		using result = B;
	};
	/** Evaluates (c ? A : B) as a convenience type constructor.
	 */
	template<bool c, typename A, typename B>
	using Conditional = typename _Conditional<c, A, B>::result;
	template<bool b>
	using FromStdBool = Conditional<b, True, False>;

	/** Specialize this for a custom bool operation
	 */
	template<typename T>
	struct Boolify { using type = FromStdBool<T::value>; };
	using boolify = TypeFunction<Boolify>;

	/** Functional Object of Conditional, use hdr::std::True and hdr::std::False
	 *  here instead of a bool.
	 */
	struct When_Else {
		template<typename C>
		constexpr static const bool c = Apply<boolify, C>::value;
		template<typename C, typename A, typename B>
		using expr = Conditional<c<C>, A, B>;
	};
	using when_else = Function<When_Else>;
	using unless    = Apply<compose, flip, when_else>;

	/** Type constexpr constructible from any arguments, could be useful.
	 */
	struct Sink {
		constexpr Sink(...){};
	};
	/** Same thing as the type as a function. If void is not an acceptable return
	 *  type for the context, I suggest comma operator, e.g.
	 *		(sink_args(args...), 0)
	 *	to return 0.
	 */
	constexpr void sink_args(...){ };

	/** Ideas for a type system, I haven't gotten very far here.
	 *  Specialize this struct for your own structures.
	 */
	template<typename Instance>
	struct Type {
		static_assert(_false<Instance>::value, "No type information available");
		using type = Void;
	};
	using typeof = TypeFunction<Type>;

	/** Void does not inherit from any type, hence it is its own type.
	 */
	template<>
	struct Type<Void>{
		using type = Void;
	};

	/** declare auto _ = Debug<Type>; to get your compiler to give you printable
	 *  information about Type.
	 */
	template<typename>
	struct Debug;

	/** This should be in its own file/namespace */
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

}
#endif //HEADERLIB_HDR_STD_HPP
