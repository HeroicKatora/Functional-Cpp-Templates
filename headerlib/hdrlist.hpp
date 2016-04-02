/*
 * celists.hpp
 *
 *  Created on: 12.11.2015
 *      Author: Andreas Molzer
 */
#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <typeinfo>
#include "hdrstd.hpp"

using namespace hdrstd;

template<typename...>
struct _t_list{};

template<size_t... t>
using _c_list = std::integer_sequence<size_t, t...>;

namespace hdrlist{
	template<bool toggle, typename List, typename Extension>
	struct extend_if{
		using type = List;
	};
	template<typename... listItems, typename Extension>
	struct extend_if<true, _t_list<listItems...>, Extension>{
		using type = _t_list<listItems..., Extension>;
	};

	template<typename List>
	struct head{
		static_assert(_false<List>::value, "Not a list");
	};
	template<>
	struct head<_t_list<>>{
	};
	template<typename headI, typename... listItems>
	struct head<_t_list<headI, listItems...>>{
		using type = headI;
	};

	template<bool condition, typename List>
	struct remove_tail_if{
		using type = List;
	};
	template<typename tail, typename... listItems>
	struct remove_tail_if<false, _t_list<listItems..., tail>>{
		using type = _t_list<listItems...>;
	};

	template<bool condition, typename List>
	struct remove_head_if{
		using type = List;
	};
	template<typename head, typename... listItems>
	struct remove_head_if<false, _t_list<head, listItems...>>{
		using type = _t_list<listItems...>;
	};

	template<typename ListA, typename ListB>
	struct join{
		static_assert(_false<ListA>::value, "Not two lists");
	};
	template<typename... listAItems, typename... listBItems>
	struct join<_t_list<listAItems...>, _t_list<listBItems...>>{
		using type = _t_list<listAItems..., listBItems...>;
	};

	template<unsigned index, typename list>
	struct get_index{
		static_assert(_false<list>::value, "Not a list");
	};
	template<typename First, typename... Types>
	struct get_index<0, _t_list<First, Types...>>{
		using type = First;
	};
	template<unsigned index, typename First, typename... Types>
	struct get_index<index, _t_list<First, Types...>>{
		using type = typename get_index<index-1, _t_list<Types...>>::type;
	};
	template<unsigned index>
	struct get_index<index, _t_list<>>{
		static_assert(_false_integral<unsigned, index>::value, "Index out of range, List exhausted");
	};

	template<template<typename> class f, typename list>
	struct map{
		static_assert(_false<list>::value, "Not a list");
	};
	template<template<typename> class f, typename... listItems>
	struct map<f, _t_list<listItems...>>{
		template <typename arg>
		using _call = typename f<arg>::value;
		using result = _t_list<_call<listItems>...>;
	};

	template<typename>
	struct c_to_t{
	};
	template<unsigned ...num>
	struct c_to_t<_c_list<num...>>{
		using result = _t_list<typename to_int_constant<unsigned, num>::result...>;
	};

	template<typename>
	struct t_to_c{
	};
	template<typename ...t>
	struct t_to_c<_t_list<t...>>{
		using result = _c_list<from_int_constant<t>::result...>;
	};

}

namespace hdrstd{
	template<typename... ob>
	struct Printer<_t_list<ob...>>{
		static void print(){
			sink_args(printf("%s,", typeid(ob).name())...);
		}
	};

	template<size_t... num>
	struct Printer<_c_list<num...>>{
		static void print(){
			sink_args(printf("%lu,", num)...);
		}
	};
}

