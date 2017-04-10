/*
 * option.hpp
 *
 * 	Created on: 14.07.2016
 * 		Author: andreas
 */

#pragma once
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
template<bool,typename>
struct Maybe;

template<typename type>
using Just 		= Maybe<true, type>;
using Nothing = Maybe<false, void>;

using freturn = TemplateFunction<Just>;

template<typename option, typename function>
struct Bind {
};
using bind = TypeFunction<Bind>;
using fmap = Apply<compose, Apply<flip, bind>, Apply<compose, freturn>>;

template<typename F>
struct Bind<Nothing, F> {
	using type = Nothing;
};
template<typename T, typename F>
struct Bind<Just<T>, F> {
	using type = Apply<F, T>;
};

template<bool existsB, typename typeT>
struct Maybe {
	constexpr static bool exists = existsB;
	using type = typeT;
};

/** Unpacks a maybe into a default value or the function application
 *		b -> (a -> b) -> Maybe a -> b
 */
template<typename Default, typename F, typename Maybe>
struct MaybeFunc;

template<typename Default, typename F>
struct MaybeFunc<Default, F, Nothing> {
	using type = Default;
};
template<typename Default, typename F, typename V>
struct MaybeFunc<Default, F, Just<V>> {
	using type = Apply<F, V>;
};
using maybe = TypeFunction<MaybeFunc>;

using isJust 		= Apply<maybe, False, Const<True>>;
using isNothing = Apply<maybe, True,  Const<False>>;

template<typename Maybe>
struct FromJust;
template<typename V>
struct FromJust<Just<V>> {
	using type = V;
};
using fromJust = TypeFunction<FromJust>;

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
