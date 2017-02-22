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
using ::hdr::std::Function;
using ::hdr::std::TypeFunction2;
template<bool,typename>
struct Maybe;

template<typename type>
using Just 		= Maybe<true, type>;
using Nothing = Maybe<false, void>;

using freturn = Function<Just>;

template<typename option, typename function>
struct Bind {
};
using bind = TypeFunction2<Bind>;

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

} // namespace hdrtypes::option

namespace hdr::std {
	template<typename T>
	struct Printer<hdr::maybe::Just<T>>{
		static void print(){
			printf("%s", "Some<");
			Printer<T>::print();
			printf("%s", ">");
		}
	};
	template<>
	struct Printer<hdr::maybe::Nothing>{
		static void print(){
			printf("%s", "None");
		}
	};
}
