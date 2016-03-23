#pragma once

#include <cstdlib>
#include <cstdio>
#include <mutex>
#include <atomic>

#include "PermutationMutable.hpp"

template<unsigned... perms>
class Permutation;

template<unsigned... perms>
const Permutation<perms... >& makePermutation();

template<unsigned... perms>
class PermutationIterator;

namespace{

	template<typename... T>
	void _helper(T... types){
	};

	template<size_t length, unsigned... perms>
	struct _perm_int;

	template<unsigned highest, unsigned current, unsigned... nbrs>
	struct _extend_struct;

	template<unsigned... nbrs>
	struct _perm_help_list;

	/**
	 * Front or mid permutation element
	 */
	template<size_t length, unsigned perm1, unsigned... perms>
	struct _perm_int<length, perm1, perms...>{
		const static unsigned int permutation = perm1;
		typedef _perm_int < length, perms... > partialPerm;

		static_assert(!(partialPerm::hasNumber(permutation)), "Permutations are injective");
		static_assert(permutation < length, "Permutations are bijective");
	
		static constexpr
		bool hasNumber(const unsigned number){
			return (number == permutation) || partialPerm::hasNumber(number);
		}

		static constexpr
		unsigned indexOf(const unsigned number, unsigned base){
			return (number==permutation)?base:partialPerm::indexOf(number, base+1);
		}

		static constexpr
		unsigned permAt(unsigned position){
			return position==0?permutation:partialPerm::permAt(position-1);
		}

		template<unsigned... oPerms>
		static constexpr
		auto&
		circle(const _perm_int<length, oPerms...>& other){
			return makePermutation<permAt(oPerms)...>();
		};

	};

	/**
	 * End of permutation element
	 */
	template<size_t length,unsigned perm>
	struct _perm_int < length, perm > {
		constexpr static unsigned permutation = perm;
		static_assert(permutation < length, "Permutations are bijective");

		static constexpr
		bool hasNumber(const unsigned number){
			return (number == permutation);
		}

		static constexpr
		unsigned indexOf(const unsigned number, unsigned base){
			return number==permutation?base:throw std::out_of_range("Number to big");
		}

		static constexpr
		unsigned permAt(unsigned position){
			return position==0?permutation:throw std::out_of_range("Position no valid index");
		}

		template<unsigned oPerm>
		static constexpr
		auto&
		circle(const _perm_int<length, oPerm>& other){
			return makePermutation<other.permAt(perm)>();
		};
	};

	template<unsigned... nbrs>
	struct _perm_help_list{
		template<unsigned length>
		using Extension = typename _extend_struct<length,sizeof...(nbrs), nbrs...>::type;

		typedef const Permutation<nbrs...>& PermutationType;
		static PermutationType correspondingPerm;
	};

	template<unsigned... nbrs>
	typename _perm_help_list<nbrs...>::PermutationType _perm_help_list<nbrs...>::correspondingPerm = makePermutation<nbrs...>();

	template<unsigned highest, unsigned current, unsigned... nbrs>
	struct _extend_struct{
		static_assert(highest > current, "Biggest number must be greater than or equal to current size");
		typedef typename _extend_struct<highest, current+1, nbrs..., current>::type type;
	};

	template<unsigned bound, unsigned... nbrs>
	struct _extend_struct<bound, bound, nbrs...>{
		typedef _perm_help_list<nbrs...> type;
	};
}

template<unsigned... perms>
class Permutation:public _perm_int < sizeof...(perms), perms... > {
	typedef _perm_int<sizeof...(perms), perms...> Super_T;
	Permutation(){}

	template<unsigned first, unsigned... rest>
	static void _print(){
		printf("%u",first);
		_helper<decltype(printf(",%u", rest))...>(printf(",%u", rest)...);
	}

public:
	static const size_t size = sizeof...(perms);
	static const Permutation<perms...> instance;

	Permutation(Permutation& o)=delete;
	Permutation& operator=(Permutation& perm)=delete;

	template<unsigned... oPerms>
	constexpr static
	auto
	circle(const Permutation< oPerms...>& other)
	->decltype(Super_T::circle(static_cast<const _perm_int<size, oPerms...>&>(other))){
		static_assert(size==sizeof...(oPerms), "Only permutations of same size can be combined");
		return Super_T::circle(static_cast<const _perm_int<size, oPerms...>&>(other));
	}

	constexpr static unsigned indexOf(const unsigned number){
		return Super_T::indexOf(number,0);
	}

	constexpr static unsigned permutationOf(const unsigned number){
		return Super_T::permAt(number);
	}

	constexpr static
	auto
	inverse()
	->decltype(makePermutation<indexOf(indexOf(perms))...>()){
		return makePermutation<indexOf(indexOf(perms))...>();
	}

	static void print(char terminator){
		_print<perms...>();
		printf("%c",terminator);
	}

	static void print(){
		print('\n');
	}

	static PermutationIterator<perms...> iterator(){
		return PermutationIterator<perms...>();
	}
};

template<unsigned... perms>
const Permutation<perms...> Permutation<perms...>::instance;

/**
 * Constant random access iterator over a constant permutation
 */
template<unsigned... perms>
class PermutationIterator:std::iterator<std::random_access_iterator_tag, unsigned>{
	const Permutation<perms...>& permutation;
	int index;
	unsigned value;
public:
	/*
	 * Standard operator requirements for random access iterators
	 */
	PermutationIterator():permutation(makePermutation<perms...>()),
			index(0),
			value(permutation.permutationOf(index)){
	}

	PermutationIterator(PermutationIterator<perms...>& it):
		permutation(makePermutation<perms...>()){
		index = it.index;
		value = it.value;
	}

	PermutationIterator<perms...>& operator=(PermutationIterator<perms...>& it){
		index = it.index;
		value = it.value;
		return *this;
	}

	unsigned const operator*(){
		return value;
	}

	PermutationIterator<perms...>& operator++(){
		if(index<sizeof...(perms)){
			index++;
			value = permutation.permutationOf(index);
		}
		return *this;
	}
	PermutationIterator<perms...> operator++(int){
		auto it (this);
		operator++();
		return it;
	}

	PermutationIterator<perms...>& operator--(){
		if(index>=0){
			index--;
			value = permutation.permutationOf(index);
		}
		return *this;
	}
	PermutationIterator<perms...> operator--(int){
		PermutationIterator<perms...> it (*this);
		operator--();
		return it;
	}

	bool operator==(PermutationIterator<perms...>& it){
		return it.index == index&&it.value==value;
	}
	bool operator!=(PermutationIterator<perms...>& it){
		return it.index != index||it.value!=value;
	}

	PermutationIterator<perms...> operator+(int n){
		PermutationIterator<perms...> it (*this);
		it += n;
		return it;
	}
	PermutationIterator<perms...> operator-(int n){
		return operator+(-n);
	}
	int operator-(PermutationIterator<perms...>& it){
		return index-it.index;
	}

	bool operator<(PermutationIterator<perms...>& it){
		return index<it.index;
	}
	bool operator>(PermutationIterator<perms...>& it){
		return index>it.index;
	}
	bool operator<=(PermutationIterator<perms...>& it){
		return index<=it.index;
	}
	bool operator>=(PermutationIterator<perms...>& it){
		return index>=it.index;
	}

	PermutationIterator<perms...>& operator+=(int i){
		index += i;
		index = (index>=sizeof...(perms))? sizeof...(perms) : (index < 0 ? -1 : index);
		value = permutation.permutationOf(index);
		return *this;
	}
	PermutationIterator<perms...>& operator-=(int i){
		return operator+=(-i);
	}

	unsigned const operator[](int off){
		if(index<0||index>sizeof...(perms))
			return -1;
		return permutation.permutationOf(index+off);
	}

	/*
	 * Special operators for this iterator, you can iterate the cycle of the permutation
	 */

	PermutationIterator<perms...>& operator*=(int n){
		if(n == 0)
			return *this;
		if(n < 0){
			while(n){
				value = index;
				index = permutation.indexOf(value);
				n++;
			}
			return *this;
		}
		else{
			while(n){
				index = value;
				value = permutation.permutationOf(index);
				n--;
			}
			return this;
		}
	}
	PermutationIterator<perms...> operator*(int n){
		PermutationIterator<perms...> it (*this);
		it *= n;
		return it;
	}
	PermutationIterator<perms...>& operator/=(int n){
		return operator*=(-n);
	}
	PermutationIterator<perms...> operator/(int n){
		return operator*(-n);
	}
};

template<unsigned... perms>
const Permutation<perms...>&
makePermutation() {
	static_assert(sizeof...(perms)>1,"Dynamic permutations should be constructed accordingly");
	return Permutation<perms...>::instance;
}

template<unsigned... perms>
auto
createDynamicInstance(const Permutation<perms...>& permutation){
	constexpr unsigned size = sizeof...(perms);
	static_assert(size>1,"Permutation is already of dynamic type");
	unsigned permArr[size];
	auto fn = [&](unsigned* arr, unsigned val){arr[permutation.indexOf(val)]=val;return 0;};

	_helper<decltype(fn(permArr, perms))...>(fn(permArr, perms)...);

	return Permutation<size>(permArr);
}

template<unsigned newLength, unsigned... perms>
constexpr typename _perm_help_list<perms...>::template Extension<newLength>::PermutationType
extendWithIdentity(const Permutation<perms...>& perm){
	return _perm_help_list<perms...>::template Extension<newLength>::correspondingPerm;
}

template<unsigned length>
constexpr typename _perm_help_list<>::template Extension<length>::PermutationType
makeIdentity(){
	return _perm_help_list<>::template Extension<length>::correspondingPerm;
}

/**
 * Applies a permutation to an array by generating a reordered version in the target location.
 * The behavior is undefined if the array overlap.
 */
template<typename T, unsigned... perms>
void applyPermutation(const Permutation<perms...>& permutation,
		T original[sizeof...(perms)], T target[sizeof...(perms)]){
	for(unsigned i = 0;i<permutation.size;i++){
		target[permutation.permutationOf(i)] = original[i];
	}
}

/**
 * Applies a permutation to a source that has a forward iterator into a target
 * that is accessed by a random access iterator.
 * The behavior is undefined if the iterators elements overlap or if the range of
 * one iterator is exceeded.
 */
template<typename T, unsigned... perms>
void applyPermutation(const Permutation<perms...>& permutation,
		std::iterator<std::forward_iterator_tag,T>& source,
		std::iterator<std::random_access_iterator_tag, T>& target){
	for(int i = 0;i<sizeof...(perms);i++){
		target[permutation.permutationOf(i)] = *(source++);
	}
	target += sizeof...(perms);
}

/**
 * Applies a permutation to a source that has a random access iterator into a target
 * that is accessed by a forward iterator.
 * The behavior is undefined if the iterators elements overlap or if the range of
 * one iterator is exceeded.
 */
template<typename T, unsigned... perms>
void applyPermutation(const Permutation<perms...>& permutation,
		std::iterator<std::random_access_iterator_tag, T>& source,
		std::iterator<std::forward_iterator_tag,T>& target){
	for(int i = 0;i<sizeof...(perms);i++){
		*(target++) = source[permutation.indexOf(i)];
	}
	source += sizeof...(perms);
}

/**
 * Applies a permutation in place. Behavior is undefined if the range of the iterator
 * is exceeded by the permutation. T has to be movable
 * If this is called for two permutation a,b it has the effect b=b°a
 */
template<typename T, unsigned... perms>
void applyPermutationInPlace(const Permutation<perms...>& permutation,
		std::iterator<std::random_access_iterator_tag, T>& target){
	auto iterator = permutation.iterator();
	constexpr unsigned size = sizeof...(perms);
	bool hasAccessed[size+1];
	int index = 0;
	while(index<size){
		unsigned orig = *iterator, trgt;
		for(auto iteratorCycle = iterator*1;iterator!=iteratorCycle;iteratorCycle*=1){
			trgt = *iteratorCycle;
			std::swap(target[orig], target[trgt]);
			hasAccessed[trgt];
		}
		hasAccessed[orig];
		while(hasAccessed[index]){
			index++;
			iterator++;
		}
	}
}
