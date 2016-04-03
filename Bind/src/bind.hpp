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

#include "hdrlist.hpp"
using namespace hdrstd;
using hdrlist::t_list;

template<typename list>
struct _recursive_param_storage;

template<typename Ret>
struct Evaluation_Target;

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

enum _bindType{
	BOUND,					//A function that wants no arguments
	PLACEHOLDER,			//A function that wants its positional argument
	EVALUATION_TARGET		//A function that wants the whole evaluation context
};

/* Represents a parameter given to the bind function
 * @type How the param works
 * @Type The type for the param
 */
template<_bindType type, typename Type, unsigned pIndex>
struct _param{
};
template<typename Type, unsigned index>
struct _param<BOUND, Type, index>{
	Type storedValue;
	template<typename... Types>
	_param(Types... t):storedValue(_get_from_variadic<index>::get(std::forward<Types>(t)...)){
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
struct _param<PLACEHOLDER, _placeholder_type<placeIndex, Type>, index>{
	template<typename... Types>
	_param(Types... t){};
	/*Type operator()(Type arg){
		return arg;
	}*/
};
template<typename Type, unsigned index>
struct _param<EVALUATION_TARGET, Type, index>{
	Type storedValue;
	template<typename... Types>
	_param(Types... t):storedValue(_get_from_variadic<index>::get(std::forward<Types>(t)...)){
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
template<_bindType btype, typename ParamT, unsigned index>
struct _param_inspect<_param<btype, ParamT, index>>{
	using type = ParamT;
	static constexpr unsigned index_u = index;
	static constexpr _bindType bindtype = btype;
};
template<typename ParamT, unsigned placeIndex, unsigned index>
struct _param_inspect<_param<_bindType::PLACEHOLDER, _placeholder_type<placeIndex, ParamT>, index>>{
	using type = ParamT;
	static constexpr unsigned index_u = index;
	static constexpr _bindType bindtype = _bindType::PLACEHOLDER;
	static constexpr unsigned placeholderIndex = placeIndex;
};

template<_bindType btype, typename Param>
struct _is_param_of_bound:std::false_type{
};
template<_bindType btype, typename ParamT, unsigned index>
struct _is_param_of_bound<btype, _param<btype, ParamT, index>>:std::true_type{
};

template<_bindType btype, typename List>
struct _remove_head_bounds{
	using type = List;
};
template<_bindType btype, typename fPType, unsigned fPIndex, typename... params>
struct _remove_head_bounds<btype, t_list<_param<btype, fPType, fPIndex>, params...>>{
	using type = _remove_head_bounds<btype, t_list<params...>>;
};

//Extracts either bound or unbound parameters into the list
template<_bindType btype, typename ParamList, typename ExtractedList = t_list<>>
struct _extract_params{
	static_assert(_false<ParamList>::value, "Not a list of _Params");
};
//Specialization for an empty param list
template<_bindType btype, typename... listContent>
struct _extract_params<btype, t_list<>, t_list<listContent...>>{
	using type = t_list<listContent...>;
};
//Specialization for a list with items in it
template<_bindType btype, typename firstParam, typename... listContent, typename... Params>
struct _extract_params<btype, t_list<firstParam, Params...>, t_list<listContent...>>{
	using _extendedList = typename hdrlist::extend_if<_is_param_of_bound<btype, firstParam>::value, t_list<listContent...>, firstParam>::type;
	using type = typename _extract_params<btype, t_list<Params...>, _extendedList>::type;
};

//Extracts all params that are not of the specified _BindType
template<_bindType btype, typename ParamList, typename ExtractedList = t_list<>>
struct _extract_not_params{
	static_assert(_false<ParamList>::value, "Not a list of _Params");
};
//Specialization for an empty param list
template<_bindType btype, typename... listContent>
struct _extract_not_params<btype, t_list<>, t_list<listContent...>>{
	using type = t_list<listContent...>;
};
//Specialization for a list with items in it
template<_bindType btype, typename firstParam, typename... listContent, typename... Params>
struct _extract_not_params<btype, t_list<firstParam, Params...>, t_list<listContent...>>{
	using _extendedList = typename hdrlist::extend_if<!(_is_param_of_bound<btype, firstParam>::value), t_list<listContent...>, firstParam>::type;
	using type = typename _extract_not_params<btype, t_list<Params...>, _extendedList>::type;
};

template<typename ParamList>
struct _extract_bound{
	static_assert(_false<ParamList>::value, "Use t_list, you shouldn't use this at all");
};
template<typename... allParams>
struct _extract_bound<t_list<allParams...>>{
	using type = typename _extract_params<BOUND, t_list<allParams...>>::type;
};

template<typename ParamList>
struct _extract_unbound{
	static_assert(_false<ParamList>::value, "Use t_list, you shouldn't use this at all");
};
template<typename... allParams>
struct _extract_unbound<t_list<allParams...>>{
	using type = typename _extract_not_params<BOUND, t_list<allParams...>>::type;
};

template<typename lhs, typename rhs>
struct _test_can_store{
	static_assert(std::is_base_of<lhs, rhs>::value, "Incompatible parameter given");
};

// construct a function signature from a list of _Parameters
template<typename Ret, typename ParamList>
struct _make_signature_paramlist{
	static_assert(_false<Ret>::value, "Use t_list as ParamList, you shouldn't use this at all");
};
template<typename Ret, typename... signatures>
struct _make_signature_paramlist<Ret, t_list<signatures...>>{
	using type = Ret(*)(typename _param_inspect<signatures>::type ...);
};

// construct a function signature from a list of typenames
template<typename Ret, typename TypeList>
struct _make_signature_raw{
	static_assert(_false<Ret>::value, "Use t_list as TypeList, you shouldn't use this at all");
};
template<typename Ret, typename... types>
struct _make_signature_raw<Ret, t_list<types...>>{
	using type = Ret(*)(types ...);
};

//Helper struct to generate the method signature
template<typename Ret, typename ParamList>
struct _gen_signature{
	using ReturnType = Ret;
	using ArgumentParamList = typename _extract_params<PLACEHOLDER, ParamList>::type;
	using NestedTargets = typename _extract_params<EVALUATION_TARGET, ParamList>::type;
	using type = typename _make_signature_paramlist<Ret, ArgumentParamList>::type;
};

//Helper struct to create the right param for the param list in _help_build_arg_types
template<typename CAType, typename AType, typename Built>
struct _create_param{
	static_assert(_false<CAType>::value, "This should not be constructed");
};
template<typename CallArgT, typename ArgT, typename ...BuiltT>
struct _create_param<CallArgT, ArgT, t_list<BuiltT...>>{
	static constexpr unsigned index = sizeof...(BuiltT);
	typedef _test_can_store<CallArgT, ArgT> compatibiltyTest;
	using type = _param<BOUND, ArgT, index>;
};
//Create a filler param
template<typename CAType, unsigned index, typename ...BuiltT>
struct _create_param<CAType, Fill<index>, t_list<BuiltT...>>{
	static constexpr unsigned fullIndex = sizeof...(BuiltT);
	using type = _param<PLACEHOLDER, _placeholder_type<index-1, CAType>, fullIndex>;
};

template<typename list>
struct _next_auto_fill{
	static_assert(_false<list>::value, "list should be of type t_list");
};
template<>
struct _next_auto_fill<t_list<>>{
	static constexpr unsigned value = 0;
};
template<typename HeadT, typename... params>
struct _next_auto_fill<t_list<HeadT, params...>>{
	static constexpr unsigned value = _next_auto_fill<t_list<params...>>::value;
};
template<unsigned placeholderIndex, typename CAType, unsigned index, typename... params>
struct _next_auto_fill<t_list<_param<PLACEHOLDER, _placeholder_type<placeholderIndex, CAType>, index>, params...>>{
	static constexpr unsigned rec_value = _next_auto_fill<t_list<params...>>::value;
	static constexpr unsigned value = rec_value > placeholderIndex ? rec_value : placeholderIndex +1;
};
//Create a filler param that is to be calculated automatically
template<typename CAType, typename ...BuiltT>
struct _create_param<CAType, Fill<0>, t_list<BuiltT...>>{
	static constexpr unsigned fullIndex = sizeof...(BuiltT);
	static constexpr unsigned placeholderIndex = _next_auto_fill<t_list<BuiltT...>>::value;
	using type = _param<PLACEHOLDER, _placeholder_type<placeholderIndex, CAType>, fullIndex>;
};

template<typename CAList, typename AList, typename DeducedParamList = t_list<>>
struct _help_build_arg_types{
	static_assert(_false<CAList>::value, "Use t_list, you shouldn't use this at all");
	using ParamList = void; // A list to hold the final list of Parameters with which the bound expression should be called
	using CParamList = void; // A list to hold the original argument types
};
template<typename L>
using _count_placeholder = _next_auto_fill<L>;
// Help build for a completed matching
template<typename... fullParams>
struct _help_build_arg_types<t_list<>, t_list<>, t_list<fullParams...>>{
	static constexpr size_t count = sizeof...(fullParams);
	static constexpr size_t paramCount = sizeof...(fullParams);
	static constexpr size_t placeholderCount = _count_placeholder<t_list<fullParams...>>::value;
	static constexpr size_t originalCount = 0;
	using ParamList = t_list<fullParams...>;
	using CParamList = t_list<>;
};

//Help build for different types
template<typename CallArgT, typename ArgT, typename... callArgs, typename... args, typename... completedParams>
struct _help_build_arg_types<t_list<CallArgT, callArgs...>, t_list<ArgT, args...>, t_list<completedParams...>>{
	static constexpr size_t count = sizeof...(completedParams);
	using DeducedParamList =  t_list<completedParams...>;
	using CParamList = t_list<CallArgT, callArgs...>;
	using CreatedParam = typename _create_param<CallArgT, ArgT, DeducedParamList>::type;
	using CreatedParamList =  t_list<completedParams..., CreatedParam>;
	using Recursion = _help_build_arg_types<t_list<callArgs...>, t_list<args...>, CreatedParamList>;
	using ParamList = typename Recursion::ParamList;
	static constexpr size_t paramCount = Recursion::paramCount;
	static constexpr size_t placeholderCount = _count_placeholder<CreatedParamList>::value;;
	static constexpr size_t originalCount = sizeof...(callArgs)+1;
};

//Help for only call arguments remaining, fill with non-bound to do
template<typename FCA, typename... callArgs, typename...completedParams>
struct _help_build_arg_types<t_list<FCA, callArgs...>, t_list<>, t_list<completedParams...>>{
	static constexpr size_t count = sizeof...(completedParams);
	using DeducedParamList =  t_list<completedParams...>;
	using CParamList = t_list<FCA, callArgs...>;
	using CreatedParam = typename _create_param<FCA, Fill<0>, DeducedParamList>::type;
	using CreatedParamList =  t_list<completedParams..., CreatedParam>;
	using Recursion = _help_build_arg_types<t_list<callArgs...>, t_list<>, CreatedParamList>;
	using ParamList = typename Recursion::ParamList;
	static constexpr size_t paramCount = Recursion::paramCount;
	static constexpr size_t placeholderCount = _count_placeholder<CreatedParamList>::value;
	static constexpr size_t originalCount = sizeof...(callArgs)+1;
};

//Fail for only arguments remaining
template<typename... callArgs, typename...completedParams>
struct _help_build_arg_types<t_list<>, t_list<callArgs...>, t_list<completedParams...>>{
	static constexpr size_t count = sizeof...(completedParams);
	static_assert(_false<t_list<callArgs...>>::value, "More arguments given than the function you try to bind takes");
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
struct _bind_helper<BindTypes, t_list<ArgumentParams...>>{
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
	struct SelectionParamTreatment<_param<_bindType::BOUND, StoredType, pIndex>>{
		template<typename... Args>
		static StoredType& select(StorageRef storage, Args... args){
			return storage.template get<pIndex>().get();
		}
	};
	template<unsigned placeholderIndex, typename PlaceType, unsigned pIndex>
	struct SelectionParamTreatment<_param<_bindType::PLACEHOLDER, _placeholder_type<placeholderIndex, PlaceType>, pIndex>>{
		template<typename... Args>
		static PlaceType select(StorageRef storage, Args... args){
			return _get_from_variadic<placeholderIndex>::get(args...);
		}
	};
	template<typename StoredType, unsigned pIndex>
	struct SelectionParamTreatment<_param<_bindType::EVALUATION_TARGET, StoredType, pIndex>>{
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
		return function(SelectionParamTreatment<typename hdrlist::get_index<constExpCounting, typename BuildHelper::ParamList>::type>::template select(storage, args...) ...);
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
	using Storage = _recursive_param_storage<typename BuildHelper::ParamList>;

};

template<typename RetType, typename BuildHelper>
struct _bind_expression{
	using BindTypes = _Bind_Types<RetType, BuildHelper>;
	using BindHelper = _bind_helper<BindTypes, typename BindTypes::SignatureHelper::ArgumentParamList>;

	BindHelper bindHelper;
	typename BindTypes::Storage storage;

	template<typename... ArgTypes>
	_bind_expression(typename BindTypes::FnType function, ArgTypes&&... args):bindHelper(function), storage(args...){
	}

	template<typename... Parameters>
	RetType operator()(Parameters&&... params){
		return bindHelper(std::make_index_sequence<BuildHelper::paramCount>{},
				storage, params...);
	}
};

template<typename Ret, typename... CallArgsT, typename... ArgsT>
static auto bind(Ret (*f)(CallArgsT...), ArgsT... args)
->_bind_expression<Ret, _help_build_arg_types<t_list<CallArgsT...>, t_list<ArgsT...>>>{
	using Builder = _help_build_arg_types<t_list<CallArgsT...>, t_list<ArgsT...>>;
	using BindExpression = _bind_expression<Ret, Builder>;
	return BindExpression(f, args...);
}

template<typename list>
struct _recursive_param_storage{
	void get() = 0;
};

template<typename Storage, unsigned index>
struct _get_recursive{
	static_assert(index <= Storage::depth, "Requested element outside of storage");
	auto get(Storage * st)
	->typename hdrlist::get_index<index, typename Storage::StoredTypeList>::type&{
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
struct _recursive_param_storage<t_list<Head, Rest...>>{
	using StoredTypeList = t_list<Head, Rest...>;
	using StoredType = Head;
	using ThisType = _recursive_param_storage<StoredTypeList>;
	using RecursionType = _recursive_param_storage<t_list<Rest...>>;
	static constexpr size_t depth = sizeof...(Rest)+1;

	Head value;
	RecursionType recursive;

	template<unsigned index>
	constexpr auto get()
	->typename hdrlist::get_index<index, t_list<Head, Rest...>>::type&{
		return _get_recursive<ThisType, index>::get(this);
	}

	template<typename... Args>
	_recursive_param_storage(Args&&... args):value(std::forward<Args>(args)...),
			recursive(std::forward<Args>(args)...){
	}
};
template<>
struct _recursive_param_storage<t_list<>>{
	template<unsigned index>
	void get(){
		static_assert(_false_integral<unsigned, index>::value, "Can't get from empty storage");
	}

	template<typename... Args>
	_recursive_param_storage(Args&&... args){};
};
