/*
 * bind.h
 *
 *  Created on: 21.09.2015
 *      Author: Andreas Molzer
 */
#pragma once
#include <functional>
#include <cstring>

//Index of 0 is for automatic, others refer to specific positions
template<unsigned index>
struct Fill{
};

template<unsigned index>
static constexpr Fill<index> fill = Fill<index>();

static constexpr Fill<0> _auto = {};

#include "celists.hpp"

template<typename list>
struct _Recursive_Param_Storage;

template<unsigned index>
struct _get_from_variadic{
	template<typename TFirst, typename... Types>
	static auto get(TFirst&& first,Types&&... t)->
	decltype(std::forward<decltype (_get_from_variadic<index-1>::get(t...))>(_get_from_variadic<index-1>::get(t...))){
		return std::forward<decltype (_get_from_variadic<index-1>::get(t...))>(_get_from_variadic<index-1>::get(t...));
	}
};
template<>
struct _get_from_variadic<0>{
	template<typename TFirst, typename... Types>
	static TFirst get(TFirst&& first,Types&&... t){
		return std::forward<TFirst>(first);
	}
};

enum _BindType{
	BOUND,					//A function that wants no arguments
	PLACEHOLDER,			//A function that wants its positional argument
	EVALUATION_TARGET		//A function that wants the whole evaluation context
};

/* Represents a parameter given to the bind function
 * @type How the param works
 * @Type The type for the param
 */
template<_BindType type, typename Type, unsigned pIndex>
struct _Param{
};
template<typename Type, unsigned index>
struct _Param<BOUND, Type, index>{
	Type storedValue;
	template<typename... Types>
	_Param(Types... t):storedValue(_get_from_variadic<index>::get(std::forward<Types>(t)...)){
	}
	Type& get(){
		return storedValue;
	}
};
template<unsigned placeholderIndex, typename type>
struct _placeholder_type{
	static constexpr unsigned placeHolderIndex = placeholderIndex;
	using TargetType = type;
};
template<typename Type, unsigned placeIndex, unsigned index>
struct _Param<PLACEHOLDER, _placeholder_type<placeIndex, Type>, index>{
	template<typename... Types>
	_Param(Types... t){};
	/*Type operator()(Type arg){
		return arg;
	}*/
};
template<typename Type, unsigned index>
struct _Param<EVALUATION_TARGET, Type, index>{
	Type storedValue;
	template<typename... Types>
	_Param(Types... t):storedValue(_get_from_variadic<index>::get(std::forward<Types>(t)...)){
	}
	template<typename... Types>
	auto get(Types... args)
	->decltype(storedValue(args...)){
		return storedValue(args...);
	}
};

template<typename ParamT>
struct _param_inspect{
	static_assert(_false<ParamT>::value, "Not a _Param");
};
template<_BindType btype, typename ParamT, unsigned index>
struct _param_inspect<_Param<btype, ParamT, index>>{
	using type = ParamT;
	static constexpr unsigned index_u = index;
	static constexpr _BindType bindtype = btype;
};
template<typename ParamT, unsigned placeIndex, unsigned index>
struct _param_inspect<_Param<_BindType::PLACEHOLDER, _placeholder_type<placeIndex, ParamT>, index>>{
	using type = ParamT;
	static constexpr unsigned index_u = index;
	static constexpr _BindType bindtype = _BindType::PLACEHOLDER;
	static constexpr unsigned placeholderIndex = placeIndex;
};

template<_BindType btype, typename Param>
struct _is_param_of_bound:std::false_type{
};
template<_BindType btype, typename ParamT, unsigned index>
struct _is_param_of_bound<btype, _Param<btype, ParamT, index>>:std::true_type{
};

template<_BindType btype, typename List>
struct _remove_head_bounds{
	using type = List;
};
template<_BindType btype, typename fPType, unsigned fPIndex, typename... params>
struct _remove_head_bounds<btype, _t_list<_Param<btype, fPType, fPIndex>, params...>>{
	using type = _remove_head_bounds<btype, _t_list<params...>>;
};

//Extracts either bound or unbound parameters into the list
template<_BindType btype, typename ParamList, typename ExtractedList = _t_list<>>
struct _extract_params{
	static_assert(_false<ParamList>::value, "Not a list of _Params");
};
//Specialization for an empty param list
template<_BindType btype, typename... listContent>
struct _extract_params<btype, _t_list<>, _t_list<listContent...>>{
	using type = _t_list<listContent...>;
};
//Specialization for a list with items in it
template<_BindType btype, typename firstParam, typename... listContent, typename... Params>
struct _extract_params<btype, _t_list<firstParam, Params...>, _t_list<listContent...>>{
	using _extendedList = typename _extend_list_if<_is_param_of_bound<btype, firstParam>::value, _t_list<listContent...>, firstParam>::type;
	using type = typename _extract_params<btype, _t_list<Params...>, _extendedList>::type;
};

//Extracts all params that are not of the specified _BindType
template<_BindType btype, typename ParamList, typename ExtractedList = _t_list<>>
struct _extract_not_params{
	static_assert(_false<ParamList>::value, "Not a list of _Params");
};
//Specialization for an empty param list
template<_BindType btype, typename... listContent>
struct _extract_not_params<btype, _t_list<>, _t_list<listContent...>>{
	using type = _t_list<listContent...>;
};
//Specialization for a list with items in it
template<_BindType btype, typename firstParam, typename... listContent, typename... Params>
struct _extract_not_params<btype, _t_list<firstParam, Params...>, _t_list<listContent...>>{
	using _extendedList = typename _extend_list_if<!(_is_param_of_bound<btype, firstParam>::value), _t_list<listContent...>, firstParam>::type;
	using type = typename _extract_not_params<btype, _t_list<Params...>, _extendedList>::type;
};

template<typename ParamList>
struct _extract_bound{
	static_assert(_false<ParamList>::value, "Use _t_list, you shouldn't use this at all");
};
template<typename... allParams>
struct _extract_bound<_t_list<allParams...>>{
	using type = typename _extract_params<BOUND, _t_list<allParams...>>::type;
};

template<typename ParamList>
struct _extract_unbound{
	static_assert(_false<ParamList>::value, "Use _t_list, you shouldn't use this at all");
};
template<typename... allParams>
struct _extract_unbound<_t_list<allParams...>>{
	using type = typename _extract_not_params<BOUND, _t_list<allParams...>>::type;
};

template<typename lhs, typename rhs>
struct _test_can_store{
	static_assert(std::is_base_of<lhs, rhs>::value, "Incompatible parameter given");
};

// construct a function signature from a list of _Parameters
template<typename Ret, typename ParamList>
struct _make_signature_paramlist{
	static_assert(_false<Ret>::value, "Use _t_list as ParamList, you shouldn't use this at all");
};
template<typename Ret, typename... signatures>
struct _make_signature_paramlist<Ret, _t_list<signatures...>>{
	using type = Ret(*)(typename _param_inspect<signatures>::type ...);
};

// construct a function signature from a list of typenames
template<typename Ret, typename TypeList>
struct _make_signature_raw{
	static_assert(_false<Ret>::value, "Use _t_list as TypeList, you shouldn't use this at all");
};
template<typename Ret, typename... types>
struct _make_signature_raw<Ret, _t_list<types...>>{
	using type = Ret(*)(types ...);
};

//Helper struct to generate the method signature
template<typename Ret, typename ParamList>
struct _gen_signature{
	using ReturnType = Ret;
	using ArgumentParamList = typename _extract_unbound<ParamList>::type;
	using type = typename _make_signature_paramlist<Ret, ArgumentParamList>::type;
};

//Helper struct to create the right param for the param list in _help_build_arg_types
template<typename CAType, typename AType, unsigned fullIndex>
struct _create_param{
	typedef _test_can_store<CAType, AType> compatibiltyTest;
	using type = _Param<BOUND, AType, fullIndex>;
};
//Create a filler param
template<typename CAType, unsigned index, unsigned fullIndex>
struct _create_param<CAType, Fill<index>, fullIndex>{
	using type = _Param<PLACEHOLDER, _placeholder_type<index-1, CAType>, fullIndex>;
};
//Create a filler param that is to be calculated automatically
//FIXME IMPLEMENT this
template<typename CAType, unsigned fullIndex>
struct _create_param<CAType, Fill<0>, fullIndex>{
	using type = _Param<PLACEHOLDER, _placeholder_type<0, CAType>, fullIndex>;
};

template<typename CAList, typename AList, typename DeducedParamList = _t_list<>>
struct _help_build_arg_types{
	static_assert(_false<CAList>::value, "Use _t_list, you shouldn't use this at all");
	using ParamList = void; // A list to hold the final list of Parameters with which the bound expression should be called
	using CParamList = void; // A list to hold the original argument types
};

// Help build for a completed matching
template<typename... fullParams>
struct _help_build_arg_types<_t_list<>, _t_list<>, _t_list<fullParams...>>{
	static constexpr size_t count = sizeof...(fullParams);
	static constexpr size_t paramCount = sizeof...(fullParams);
	static constexpr size_t originalCount = 0;
	using ParamList = _t_list<fullParams...>;
	using CParamList = _t_list<>;
};

//Help build for different types
template<typename FCA, typename FA, typename... callArgs, typename... args, typename... completedParams>
struct _help_build_arg_types<_t_list<FCA, callArgs...>, _t_list<FA, args...>, _t_list<completedParams...>>{
	static constexpr size_t count = sizeof...(completedParams);
	using CreatedParam = typename _create_param<FCA, FA, count>::type;
	using Recursion = _help_build_arg_types<_t_list<callArgs...>, _t_list<args...>, _t_list<completedParams..., CreatedParam>>;
	using ParamList = typename Recursion::ParamList;
	static constexpr size_t paramCount = Recursion::paramCount;
	static constexpr size_t originalCount = sizeof...(callArgs)+1;
	using CParamList = _t_list<FCA, callArgs...>;
};

//Help for only call arguments remaining, fill with non-bound to do
template<typename FCA, typename... callArgs, typename...completedParams>
struct _help_build_arg_types<_t_list<FCA, callArgs...>, _t_list<>, _t_list<completedParams...>>{
	static constexpr size_t count = sizeof...(completedParams);
	using CreatedParam = typename _create_param<FCA, Fill<0>, count>::type;
	using Recursion = _help_build_arg_types<_t_list<callArgs...>, _t_list<>, _t_list<completedParams..., CreatedParam>>;
	using ParamList = typename Recursion::ParamList;
	static constexpr size_t paramCount = Recursion::paramCount;
	static constexpr size_t originalCount = sizeof...(callArgs)+1;
	using CParamList = _t_list<FCA, callArgs...>;
};

//Fail for only arguments remaining
template<typename... callArgs, typename...completedParams>
struct _help_build_arg_types<_t_list<>, _t_list<callArgs...>, _t_list<completedParams...>>{
	static constexpr size_t count = sizeof...(completedParams);
	static_assert(_false<_t_list<callArgs...>>::value, "More arguments given than the function you try to bind takes");
};

/**
 * Aggregates all types that are used by a bind expression
 */
template<typename RetType, typename BuildHelper>
struct _Bind_Types;

template<typename BindTypes, typename ArgumentParamList>
struct _bind_helper{
};


template<typename BindTypes, typename... ArgumentParams>
struct _bind_helper<BindTypes, _t_list<ArgumentParams...>>{
	using SignatureHelper = typename BindTypes::SignatureHelper;
	using BuildHelper = typename BindTypes::BuildHelper;
	using FnType = typename BindTypes::FnType;
	using StorageRef = typename BindTypes::Storage &;

	using Ret = typename SignatureHelper::ReturnType;

	FnType function;

	template<typename Param>
	struct SelectionParamTreatment{
		static_assert(_false<Param>::value, "Selection only works on param type");
		template<typename... Args>
		static void select(StorageRef storage, Args... args){}; //Documentation
	};
	template<typename StoredType, unsigned pIndex>
	struct SelectionParamTreatment<_Param<_BindType::BOUND, StoredType, pIndex>>{
		template<typename... Args>
		static StoredType& select(StorageRef storage, Args... args){
			return storage.template get<pIndex>().get();
		}
	};
	template<unsigned placeholderIndex, typename PlaceType, unsigned pIndex>
	struct SelectionParamTreatment<_Param<_BindType::PLACEHOLDER, _placeholder_type<placeholderIndex, PlaceType>, pIndex>>{
		template<typename... Args>
		static PlaceType select(StorageRef storage, Args... args){
			return _get_from_variadic<placeholderIndex>::get(args...);
		}
	};
	template<typename StoredType, unsigned pIndex>
	struct SelectionParamTreatment<_Param<_BindType::EVALUATION_TARGET, StoredType, pIndex>>{
		template<typename... Args>
		static auto select(StorageRef storage, Args... args)
		->decltype(storage.template get<pIndex>().get(args...)){
			return storage.template get<pIndex>().get(args...);
		}
	};

	_bind_helper(FnType function):function(function){
	};

	template<size_t... constExpCounting>
	Ret operator()(std::index_sequence<constExpCounting...>&& a,
			StorageRef storage, typename _param_inspect<ArgumentParams>::type&& ... args){
		return function(SelectionParamTreatment<typename list_get_index<constExpCounting, typename BuildHelper::ParamList>::type>::template select(storage, args...) ...);
	}
};

template<typename RetType, typename _BuildHelper>
struct _Bind_Types{
	using ReturnType = RetType;
	using BuildHelper = _BuildHelper;
	using ParamList = typename BuildHelper::ParamList;
	using SignatureHelper = _gen_signature<RetType, ParamList>;

	using Signature = typename SignatureHelper::type;
	using FnType = typename _make_signature_raw<RetType, typename BuildHelper::CParamList>::type;
	using Storage = _Recursive_Param_Storage<typename BuildHelper::ParamList>;

};

template<typename RetType, typename BuildHelper>
struct _bind_expression{
	using BindTypes = _Bind_Types<RetType, BuildHelper>;
	using BindHelper = _bind_helper<BindTypes, typename BindTypes::SignatureHelper::ArgumentParamList>;

	BindHelper bindHelper;
	typename BindTypes::Storage storage;

	template<typename... ArgTypes>
	_bind_expression(typename BindTypes::FnType function, ArgTypes... args):bindHelper(function), storage(args...){
	}

	template<typename... Parameters>
	RetType operator()(Parameters&&... params){
		return bindHelper(std::make_index_sequence<BuildHelper::paramCount>{},
				storage, params...);
	}
};

template<typename Ret, typename... CallArgsT, typename... ArgsT>
static auto bind(Ret (*f)(CallArgsT...), ArgsT... args)
->_bind_expression<Ret, _help_build_arg_types<_t_list<CallArgsT...>, _t_list<ArgsT...>>>{
	using Builder = _help_build_arg_types<_t_list<CallArgsT...>, _t_list<ArgsT...>>;
	using BindExpression = _bind_expression<Ret, Builder>;
	return BindExpression(f, args...);
}

template<typename list>
struct _Recursive_Param_Storage{
	void get() = 0;
};

template<typename Storage, unsigned index>
struct _get_recursive{
	static_assert(index <= Storage::depth, "Requested element outside of storage");
	auto get(Storage * st)
	->typename list_get_index<index, typename Storage::StoredTypeList>::type&{
		return st->recursive.template get<index-1>();
	}
};
template<typename Storage>
struct _get_recursive<Storage, 0>{
	static typename Storage::StoredType& get(Storage *st){
		return st->value;
	}
};

template<typename Head, typename... Rest>
struct _Recursive_Param_Storage<_t_list<Head, Rest...>>{
	using StoredTypeList = _t_list<Head, Rest...>;
	using StoredType = Head;
	using ThisType = _Recursive_Param_Storage<StoredTypeList>;
	using RecursionType = _Recursive_Param_Storage<_t_list<Rest...>>;
	static constexpr size_t depth = sizeof...(Rest)+1;

	Head value;
	RecursionType recursive;

	template<unsigned index>
	constexpr auto get()
	->typename list_get_index<index, _t_list<Head, Rest...>>::type&{
		return _get_recursive<ThisType, index>::get(this);
	}

	template<typename... Args>
	_Recursive_Param_Storage(Args&&... args):value(std::forward<Args>(args)...),
			recursive(std::forward<Args>(args)...){
	}
};
template<>
struct _Recursive_Param_Storage<_t_list<>>{
	template<unsigned index>
	void get(){
		static_assert(_false_integral<unsigned, index>::value, "Can't get from empty storage");
	}

	template<typename... Args>
	_Recursive_Param_Storage(Args&&... args){};
};
