/*
 * traits.hpp
 */
#pragma once
#include <functional>
#include <type_traits>
#include <typeindex>

namespace {

	/**
	 * Constexpr list of booleans as variadic
	 */
	template<bool...>
	struct _bool_list;

	template<typename>
	struct _all_of_impl;
	/**
	 * Specialization for a bool list with more than one bool, performs an &&
	 * of the first element and the following ones and set static attribute 'value'
	 */
	template<bool head, bool... cond>
	struct _all_of_impl<_bool_list<head, cond...>>
	{
		constexpr static bool value = head && _all_of_impl<_bool_list<cond...>>::value;
	};
	/**
	 * Specialization for exactly one bool. Sets static attribute 'value' accordingly.
	 */
	template<bool only>
	struct _all_of_impl<_bool_list<only>>
	{ constexpr static bool value = only; };
	/**
	 * Specialization for an empty list. Defaults static attribute 'value' to true.
	 */
	template<>
	struct _all_of_impl<_bool_list<>>
	{ constexpr static bool value = true; };

	template<bool... cond>
	struct _all_of : _all_of_impl<_bool_list<cond...>>{};

	template<typename _Tp, bool... cond>
	struct _where_impl
	{
		static_assert(_all_of<cond...>::value, "Not all conditions served");
		typedef _Tp type;
	};

	struct _Weak_Type_Token_impl{
	protected:
		const size_t id;
	public:
		_Weak_Type_Token_impl(size_t id):id(id){};
	};

//	template<typename Base, typename Child, bool = std::is_base_of<Base, Child>::value>
//	struct _is_base_impl : std::false_type{};
//
//	template<typename B, typename C>
//	struct _is_base_impl<B,C, true> : std::true_type{};

}

/**
 * Possibility to do compiler port, re-declaration of std::is_base_of<Base, Child>
 */
template<typename Base, typename Child>
using IsBase = std::is_base_of<Base, Child>;

/**
 * Defines attribute type as _Tp when all conditions are fulfilled.
 * Otherwise gives an assertion error.
 */
template<typename _Tp, bool... cond>
using Enable_Where_All = _where_impl<_Tp, cond...>;

/**
 * Alias to Enable_Where_All<void, cond...>
 * It can therefore be used as a type in template variadics that gets dropped.
 */
template<bool... cond>
using Where_All = Enable_Where_All<void, cond...>;

/**
 * A type token unique for each Type
 */
typedef std::type_info TypeToken;
template<typename T>
const TypeToken& getTypeToken(T &ptr){
	return typeid(ptr);
}

template<typename T>
const TypeToken& getTypeToken(){
	return typeid(T);
}

struct WeakTypeToken final:public _Weak_Type_Token_impl{
	WeakTypeToken()=delete;
	WeakTypeToken(WeakTypeToken& ref)=delete;
	WeakTypeToken& operator=(WeakTypeToken& ref)=delete;

	bool operator==(WeakTypeToken& ref)=delete;
	bool operator!=(WeakTypeToken& ref){
		return id != ref.id;
	};

	size_t getAmbiguousID(){
		return id;
	}
};

template<typename T>
WeakTypeToken& getWeakTypeToken(){
	static _Weak_Type_Token_impl _token_impl(typeid(T).hash_code());
	static WeakTypeToken& token = static_cast<WeakTypeToken&>(_token_impl);
	return token;
}

template<typename T>
WeakTypeToken& getWeakTypeToken(T& ptr){
	return getWeakTypeToken<T>();
}
