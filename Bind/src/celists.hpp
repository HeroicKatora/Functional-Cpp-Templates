/*
 * celists.hpp
 *
 *  Created on: 12.11.2015
 *      Author: Andreas Molzer
 */
#pragma once
#include <utility>

template<typename>
struct _false:std::false_type{
};
template<typename T, T value>
struct _false_integral:std::false_type{
};


template<typename...>
struct _t_list{};

template<bool toggle, typename List, typename Extension>
struct _extend_list_if{
	using type = List;
};
template<typename... listItems, typename Extension>
struct _extend_list_if<true, _t_list<listItems...>, Extension>{
	using type = _t_list<listItems..., Extension>;
};

template<typename List>
struct _list_head{
	static_assert(_false<List>::value, "Not a list");
};
template<typename head, typename... listItems>
struct _list_head<_t_list<head, listItems...>>{
	using type = head;
};

template<bool condition, typename List>
struct _remove_tail_if{
	using type = List;
};
template<typename tail, typename... listItems>
struct _remove_tail_if<false, _t_list<listItems..., tail>>{
	using type = _t_list<listItems...>;
};

template<bool condition, typename List>
struct _remove_head_if{
	using type = List;
};
template<typename head, typename... listItems>
struct _remove_head_if<false, _t_list<head, listItems...>>{
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
struct list_get_index{
	static_assert(_false<list>::value, "Not a list");
};
template<typename First, typename... Types>
struct list_get_index<0, _t_list<First, Types...>>{
	using type = First;
};
template<unsigned index, typename First, typename... Types>
struct list_get_index<index, _t_list<First, Types...>>{
	using type = typename list_get_index<index-1, _t_list<Types...>>::type;
};
template<unsigned index>
struct list_get_index<index, _t_list<>>{
	static_assert(_false_integral<unsigned, index>::value, "Index out of range, List exhausted");
};

template<unsigned... t>
using _u_list = std::integer_sequence<size_t, t...>;

