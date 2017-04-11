#ifndef HEADERLIB_HDR_TYPES_MAYBE_HPP
#define HEADERLIB_HDR_TYPES_MAYBE_HPP
/*
 * option.hpp
 * Declares an option monad type, called Maybe in Haskell. I renamed it to maybe
 * but haven't moved the file. This shall be a features forever.
 *
 *  Created on: 14.07.2016
 * 		Author: andreas
 */
#include "hdr/std.hpp"

namespace hdr::maybe {

using ::hdr::std::Apply;
using ::hdr::std::True;
using ::hdr::std::False;
using ::hdr::std::Const;
using ::hdr::std::TemplateFunction;
using ::hdr::std::TypeFunction;

using ::hdr::std::id;
using ::hdr::std::compose;
using ::hdr::std::flip;

/** Use these type aliases to construct, compare and handle Maybes
 */
struct Nothing;
template<typename type>
struct Just;

/// Monad constructor
using freturn = TemplateFunction<Just>;

template<typename option, typename function>
struct Bind;
/// Monad bind function
using bind = TypeFunction<Bind>;
/// Monad fmap function, expressed with bind and return
using fmap = Apply<compose, Apply<flip, bind>, Apply<compose, freturn>>;

template<typename F>
struct Bind<Nothing, F> {
	using type = Nothing;
};
template<typename T, typename F>
struct Bind<Just<T>, F> {
	using type = Apply<F, T>;
};


template<typename Default, typename F, typename Maybe>
struct MaybeFunc;
/** Unpacks a maybe into a default value or the function application
 *		b -> (a -> b) -> Maybe a -> b
 */
using maybe = TypeFunction<MaybeFunc>;

template<typename Default, typename F>
struct MaybeFunc<Default, F, Nothing> {
	using type = Default;
};
template<typename Default, typename F, typename V>
struct MaybeFunc<Default, F, Just<V>> {
	using type = Apply<F, V>;
};

/// True if argument is an instance of Just else False
using isJust 		= Apply<maybe, False, Const<True>>;
/// True if argument is not an instance of Just else True
using isNothing = Apply<maybe, True,  Const<False>>;

template<typename Maybe>
struct FromJust;
template<typename V>
struct FromJust<Just<V>> {
	using type = V;
};
/// Exception when argument is not Just<V>
using fromJust = TypeFunction<FromJust>;
/** Returns the default value or an unpacked Just
 *  	(a -> Maybe a -> a)
 */
using fromMaybe = Apply<Apply<flip, maybe>, id>;

} // namespace hdrtypes::option

namespace hdr::std {
	template<typename T>
	struct Printer<::hdr::maybe::Just<T>>{
		static void print(){
			printf("%s", "Some<");
			Printer<T>::print();
			printf("%s", ">");
		}
	};
	template<>
	struct Printer<::hdr::maybe::Nothing>{
		static void print(){
			printf("%s", "None");
		}
	};
}
#endif //HEADERLIB_HDR_TYPES_MAYBE_HPP
