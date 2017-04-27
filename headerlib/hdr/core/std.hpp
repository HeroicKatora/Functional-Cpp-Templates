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
#include "hdr/core/template_template_args_count.hpp"
#include "hdr/core/apply.hpp"
#include "hdr/core/convert.hpp"

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

	/**	Hides a type from immediate substitution in template using declarations.
	 *  Use it as Const<Value>.
	 *  It holds that Apply<Const<V>, T> === V for any (V, T)
	 */
	template<typename val>
	struct Const;
	template<typename Val, typename FirstArg>
	struct ApplySingleStruct<Const<Val>, FirstArg> {
		using result = Val;
	};

	/**	Constructor of the above as well as standalone function.
	 *  Ignores the second argument or constructs Const<a>
	 *	(a -> b -> a)
	 */
	using fconst = TemplateFunction<Const>;

	/** Flips the order of two arguments
	 * 		(a -> b -> c) -> (b -> a -> c)
	 */
	template<typename F, typename B, typename A>
	struct Flip {
		using type = Apply<F, A, B>;
	};
	using flip   = TypeFunction<Flip>;

	/** Similar to fconst, but ignores its first argument
	 */
	using ignore = Apply<flip, fconst>;

	/** A function which will always return itself, infinitely
	 */
	struct VoidFunction;
	template<typename FirstArg>
	struct ApplySingleStruct<VoidFunction, FirstArg> {
		using result = VoidFunction;
	};

	/**	Type Definition of true, should be used as a parameter instead of bools.
	 */
	struct True : ::std::true_type , fconst {};
	/**	Type Definition of false, should be used as a parameter instead of bools.
	 */
	struct False : ::std::false_type , ignore {};

	/** Identity function for all Objects
	 *		(a -> a)
	 *	Can also be used as a functional form of apply.
	 *  We could also implement this as
	 *		template<typename T> using _id = T; using id = TemplateFunction<id>;
	 *	which would be neutral to the calling style but necessitate an additional
	 *  indirection through another type.
	 */
	struct id;
	template<typename FirstArg>
	struct ApplySingleStruct<id, FirstArg> {
		using result = FirstArg;
	};
	//Function Object of Apply
	using apply = id;

	/** Might make binary operators more intuitive, alternative to Apply. I haven't
	 *  felt the need to use it yet.
	 */
	template<typename aarg, typename op, typename barg>
	using Binary = Apply<op, aarg, barg>;
	using binary = Apply<flip, apply>;

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
	 *  here instead of a bool. You could also use the truth type directly as
	 *  a function.
	 */
	using when_else = id;
	// Pretty ugly compare to when_else, just swap arguments yourself please
	using unless    = Apply<compose, flip, when_else>;

	/** This is really just apply but code might be easier to understand this way.
	 */
	template<typename ...Args>
	using When = Apply<when_else, Args...>;

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
