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

namespace{

	template<typename...>
	struct _t_list{};

	template<size_t... t>
	using _c_list = std::integer_sequence<size_t, t...>;

	template<bool toggle, typename List, typename Extension>
	struct _extend_if{
		using type = List;
		using result = type;
	};
	template<typename... listItems, typename Extension>
	struct _extend_if<true, _t_list<listItems...>, Extension>{
		using type = _t_list<listItems..., Extension>;
		using result = type;
	};

	template<typename List>
	struct _head{
		static_assert(_false<List>::value, "Not a list");
	};
	template<>
	struct _head<_t_list<>>{
	};
	template<typename head, typename... tail>
	struct _head<_t_list<head, tail...>>{
		using result = head;
	};

	template<typename List>
	struct _tail{
		static_assert(_false<List>::value, "Not a list");
	};
	template<>
	struct _tail<_t_list<>>{
	};
	template<typename head, typename ...tail>
	struct _tail<_t_list<head, tail...>>{
		using result = _t_list<tail...>;
	};

	template<bool condition, typename List>
	struct _remove_tail_if{
		using type = List;
		using result = type;
	};
	template<typename tail, typename... listItems>
	struct _remove_tail_if<false, _t_list<listItems..., tail>>{
		using type = _t_list<listItems...>;
		using result = type;
	};

	template<bool condition, typename List>
	struct _remove_head_if{
		using type = List;
		using result= type;
	};
	template<typename head, typename... listItems>
	struct _remove_head_if<false, _t_list<head, listItems...>>{
		using type = _t_list<listItems...>;
		using result = type;
	};

	template<typename ListA, typename ListB>
	struct _join{
		static_assert(_false<ListA>::value, "Not two lists");
	};
	template<typename... listAItems, typename... listBItems>
	struct _join<_t_list<listAItems...>, _t_list<listBItems...>>{
		using type = _t_list<listAItems..., listBItems...>;
		using result = type;
	};

	template<unsigned index, typename list>
	struct _get_index{
		static_assert(_false<list>::value, "Not a list");
	};
	template<typename First, typename... Types>
	struct _get_index<0, _t_list<First, Types...>>{
		using type = First;
		using result = type;
	};
	template<unsigned index, typename First, typename... Types>
	struct _get_index<index, _t_list<First, Types...>>{
		using type = typename _get_index<index-1, _t_list<Types...>>::type;
		using result = type;
	};
	template<unsigned index>
	struct _get_index<index, _t_list<>>{
		static_assert(_false_integral<unsigned, index>::value, "Index out of range, List exhausted");
	};

	template<typename Integral, typename list>
	struct _nth{
	};
	template<size_t integral, typename list>
	struct _nth<std::integral_constant<size_t, integral>, list>{
		using result = typename _get_index<integral, list>::result;
	};

	template<typename>
	struct _c_to_t{
	};
	template<size_t ...num>
	struct _c_to_t<_c_list<num...>>{
		using result = _t_list<typename to_int_constant<size_t, num>::result...>;
	};

	template<typename>
	struct _t_to_c{
	};
	template<typename ...t>
	struct _t_to_c<_t_list<t...>>{
		using result = _c_list<from_int_constant<t>::result...>;
	};

	/*Iteration functions*/

	template<typename f, typename list>
	struct _map{
		static_assert(_false<list>::value, "Not a list");
	};
	template<typename f, typename... listItems>
	struct _map<f, _t_list<listItems...>>{
		template <typename arg>
		using _call = typename f::template expr<arg>::result;
		using result = _t_list<_call<listItems>...>;
	};

	template<typename f, typename list>
	struct _mapi{
		static_assert(_false<list>::value, "Not a list");
	};
	template<typename f, typename... listItems>
	struct _mapi<f, _t_list<listItems...>>{
		using list = _t_list<listItems...>;
		using indices = std::index_sequence_for<listItems...>;
		using index_list = typename _c_to_t<indices>::result;
		template <typename index>
		struct _call{
		   using result = typename f::template expr<index, _nth<index, list>>::result;
		};
		using result = typename _map<function<_call>, index_list>::result;
	};

	template<typename f, typename, typename list>
	struct _fold_left{
		static_assert(_false<list>::value, "Last parameter needs to be a list");
	};
	template<typename f, typename base>
	struct _fold_left<f, base, _t_list<>>{
		using result = base;
	};
	template<typename f, typename base, typename head, typename ...tail>
	struct _fold_left<f, base, _t_list<head, tail...>>{
		using fold_once = typename f::template expr<base, head>::result;
		using result = typename _fold_left<f, fold_once, _t_list<tail...>>::result;
	};

	/*Scanning functions*/


	/*Searching functions*/
	template<typename f, typename list>
	struct _filter{
		static_assert(_false<list>::value, "Last parameter needs to be a list");
	};
	template<typename f>
	struct _filter<f, _t_list<>>{
		using result = _t_list<>;
	};
	template<typename f, typename hd, typename ...tail>
	struct _filter<f, _t_list<hd, tail...>>{
		struct y {
			template<typename>
			struct expr{
				using result = typename _join<_t_list<hd>, typename _filter<f, _t_list<tail...>>::result>::result;
			};
		};
		using n = expression<::_filter, f, _t_list<tail...>>;
		constexpr static bool fresult = f::template expr<hd>::result;
		using result = typename conditional<fresult, y, n>::template expr<Void>::result;
	};

	template<typename f, typename list>
	struct _find{
		using hd = typename _head<list>::result;
		using tl = typename _tail<list>::result;
		using rec = expression<_find, tl>;
		constexpr static bool condition = f::template expr<hd>::result;
		using result = typename conditional<condition, expression_val<hd>, rec>::template expr<Void>::result;
	};

	template<typename Sep, typename...>
	struct _list_printer;
	template<typename Sep>
	struct _list_printer<Sep>{
		static void print(){}
	};
	template<typename Sep, typename Head>
	struct _list_printer<Sep, Head>{
		static void print(){
			Printer<Head>::print();
		}
	};
	template<typename Sep, typename Head, typename ...Tail>
	struct _list_printer<Sep, Head, Tail...>{
		static void print(){
			Printer<Head>::print();
			Sep::print();
			_list_printer<Sep, Tail...>::print();
		}
	};

	struct _separator{
		static void print(){
			printf(", ");
		}
	};
}

namespace hdrlist{
	template<typename ...arg>
	using t_list = _t_list<arg...>;

	template<size_t... t>
	using c_list = _c_list<t...>;

	template<bool toggle, typename List, typename Extension>
	using extend_if = _extend_if<toggle, List, Extension>;

	template<typename List>
	using head = _head<List>;

	template<bool condition, typename List>
	using remove_tail_if = _remove_tail_if<condition, List>;

	template<bool condition, typename List>
	using remove_head_if = _remove_head_if<condition, List>;

	template<typename ListA, typename ListB>
	using join = _join<ListA, ListB>;

	template<typename ListA, typename ListB>
	using concat = join<ListA, ListB>;

	template<unsigned index, typename list>
	using get_index = _get_index<index, list>;

	template<typename f, typename list>
	using map = _map<f, list>;

	template<typename f, typename list>
	using filter = _filter<f, list>;

	template<typename l>
	using c_to_t = _c_to_t<l>;

	template<typename l>
	using t_to_c = _t_to_c<l>;

	template<typename f, typename b, typename list>
	using fold_left = _fold_left<f, b, list>;

	template<typename ...Arg>
	using list_printer = _list_printer<Arg...>;

	using separator = _separator;
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

