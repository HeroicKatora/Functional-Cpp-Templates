#ifndef HEADERLIB_HDR_LIST_HPP
#define HEADERLIB_HDR_LIST_HPP
/*
 * celists.hpp
 *
 *	Created on: 12.11.2015
 *			Author: Andreas Molzer
 */
#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <typeinfo>
#include "hdr/std.hpp"

using namespace hdrstd;

namespace hdr::list{

	struct ListSentry;
	template<typename Head, typename Tail> struct List;

	template<typename> struct Head;
	template<typename> struct Tail;
	using head = TypeFunction<Head>;
	using tail = TypeFunction<Tail>;

	template<typename H, typename T>
	struct Head<List<H, T>> {
		using type = H;
	};
	template<typename H, typename T>
	struct Tail<List<H, T>> {
		using type = T;
	};

}

namespace hdrstd{
	template<typename... ob>
	struct Printer<_t_list<ob...>>{
		static void print(){
			printf("(");
			hdrlist::list_printer<hdrlist::separator, ob...>::print();
			printf(")");
		}
	};

	template<size_t... num>
	struct Printer<_c_list<num...>>{
		using print_list = typename hdrlist::c_to_t<_c_list<num...>>::result;
		static void print(){
			Printer<print_list>::print();
		}
	};
}
#endif //HEADERLIB_HDR_LIST_HPP
