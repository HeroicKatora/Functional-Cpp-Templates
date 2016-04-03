/*
 * hdrmap.hpp
 *
 *  Created on: 28.03.2016
 *      Author: andreas
 */

#pragma once
#include <type_traits>

#include "hdrlist.hpp"

using namespace hdrstd;

namespace{
	enum _map_type{
		Node,
		Leaf
	};
	template<bool found, typename val>
	struct _map_search_return{
		constexpr static bool value = found;
		using type = val;
	};

	////////////////////////////////////// Map type

	template<typename intType, _map_type ty, typename _t_init_list>
	struct _t_map{};

	template<typename intType, intType key, typename val, typename left, typename right>
	struct _t_map<intType, Node, _t_list<std::integral_constant<intType, key>, val, left, right>>{
		using KeyType = intType;
		using Key = std::integral_constant<intType, key>;
		using Val = val;
		using Left = left;
		using Right = right;
	};
	template<typename intType, intType key, typename val, typename left, typename right>
	using _t_map_node = _t_map<intType, Node, _t_list<std::integral_constant<intType, key>, val, left, right>>;

	template<typename intType>
	struct _t_map<intType, Leaf, _t_list<>>{
	};
	template<typename intType>
	using _t_map_leaf = _t_map<intType, Leaf, _t_list<>>;

	////////////////////////////////////// Map add

	/**
	 * Adds or sets the mapping
	 */
	template<typename intType, intType key, typename val, typename map>
	struct _map_add{
		static_assert(_false<map>::value, "Only use this with maps from this header");
		//Result of the "function call"
		using result = void;
	};
	template<typename siType, typename miType, siType key, typename val, _map_type ty, typename _t_init_list>
	struct _map_add<siType, key, val, _t_map<miType, ty, _t_init_list>>{
		static_assert(_false<siType>::value, "Key type and search type do not match");
	};

	template<typename intType, intType key, typename val>
	struct _map_add<intType, key, val, _t_map_leaf<intType>>{
		using result = _t_map_node<intType, key, val, _t_map_leaf<intType>, _t_map_leaf<intType>>;
	};
	template<typename intType, intType key, typename val, intType mKey, typename mVal, typename left, typename right>
	struct _map_add<intType, key, val, _t_map_node<intType, mKey, mVal, left, right>>{
		template<typename>
		struct smaller{
			using newLeft = typename _map_add<intType, key, val, left>::result;
			using result = _t_map_node<intType, mKey, mVal, newLeft, right>;
		};
		template<typename>
		struct larger{
			using newRight = typename _map_add<intType, key, val, right>::result;
			using result = _t_map_node<intType, mKey, mVal, left, newRight>;
		};
		template<typename>
		struct equal{
			using result = _t_map_node<intType, key, val, left, right>;
		};
		using result = typename conditional<(key < mKey), smaller,
						conditional<(key > mKey), larger, equal>::template expr>::template expr<Void>::result;
	};

	////////////////////////////////////// Map search

	template<typename intType, intType val, typename map, bool when =  true>
	struct _map_search{
		static_assert(_false<map>::value, "Only use this with maps from this header");
		//The result of the search, value has a bool, type is the result
		using result = void;
	};
	template<typename siType, typename miType, siType val, _map_type ty, typename _t_init_list, bool when>
	struct _map_search<siType, val, _t_map<miType, ty, _t_init_list>, when>{
		static_assert(_false<siType>::value, "Key type and search type do not match");
	};

	//End of search
	template<typename intType, intType val>
	struct _map_search<intType, val, _t_map_leaf<intType>, true>{
		using result = _map_search_return<false, void>;
	};
	//No match
	template<typename intType, intType val, intType key, typename type, typename left, typename right>
	struct _map_search<intType, val, _t_map_node<intType, key, type, left, right>>{
		template<typename>
		struct expleft {
			using result = typename _map_search<intType, val, left>::result;
		};
		template<typename>
		struct expright {
			using result = typename _map_search<intType, val, right>::result;
		};
		template<typename>
		struct expequal {
			using result = _map_search_return<true, type>;
		};
		using result = typename conditional<(val < key), expleft,
				conditional<(val > key), expright, expequal>::template result>::template result<Void>;
	};
}

namespace hdrmap{
	/**
	 * A type that implements mapping of an integral to a typename
	 */
	template<typename intType, intType val, typename map>
	using map_search = _map_search<intType, val, map>;

	template<typename intType, intType key, typename val, typename map>
	using map_add = _map_add<intType, key, val, map>;

	template<typename intType>
	using map_empty = _t_map_leaf<intType>;
}

namespace hdrstd{
	template<typename intType>
	struct Printer<_t_map_leaf<intType>>{
		static void print(){
			printf("()");
		}
	};

	template<typename intType, intType key, typename val>
	struct Printer<_t_map_node<intType, key, val, _t_map_leaf<intType>, _t_map_leaf<intType>>>{
		static void print(){
			printf("(%i -> ", key);
			Printer<val>::print();
			printf(")");
		}
	};

	template<typename intType, intType key, typename val, typename left, typename right>
	struct Printer<_t_map_node<intType, key, val, left, right>>{
		static void print(){
			printf("(%i -> ", key);
			Printer<val>::print();
			printf(", ");
			Printer<left>::print();
			printf(", ");
			Printer<right>::print();
			printf(")");
		}
	};
}
