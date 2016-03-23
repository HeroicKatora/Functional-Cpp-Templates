#pragma once

#include <cstdlib>
#include <cstdio>

template<unsigned... perms>
class Permutation;

template<unsigned... perms>
class PermutationIterator;

/**
 * Specialization for just size argument, used for run time, non-constant permutations
 */
template<unsigned size>
class Permutation<size>{
	unsigned permutation[size];
	unsigned permutationIndices[size]; //These should always be "in sync" when a valid permutation
	const size_t length;
	std::mutex stateMutex;
	std::unique_lock<typename std::mutex> stateLock;

	void initializePermutation(unsigned perms[size]){
		unsigned perm;
		for(unsigned i = 0;i<size;i++){
			perm = perms[i];
			permutation[i] = perm;
			permutationIndices[perm] = i;
		}
	}

	/**
	 * Resets the info if the array contains a permutation (is bijective)
	 */
	void resetValidity(){
		bool exists[size];
		bool valid = true;
		for(unsigned i = 0;i<size;i++){
			unsigned perm = permutation[i];
			if(exists[perm]){
				valid = false;
				break;
			}else{
				exists[perm] = true;
			}
		}
		validPermutation = valid;
	}

	void refreshIndices(){
		for(unsigned i = 0;i<size;i++){
			permutationIndices[permutation[i]] = i;
		}
	}

public:
	std::atomic<bool> validPermutation;

	Permutation(unsigned perms[size]):
		length(size),
		stateMutex(),
		stateLock(stateMutex, std::defer_lock),
		validPermutation(false){
		initializePermutation(perms);
		resetValidity();
	}

	Permutation(Permutation<size>& source):
		length(size),
		stateMutex(),
		stateLock(stateMutex, std::defer_lock),
		validPermutation(false){
		memcpy(permutation, source.permutation, size);
		memcpy(permutationIndices, source.permutationIndices, size);
		resetValidity();
	}

	Permutation(Permutation<size>&& source):
		length(size),
		stateMutex(),
		stateLock(stateMutex, std::defer_lock),
		validPermutation(false){
		std::swap(permutation, source.permutation);
		std::swap(permutationIndices, source.permutationIndices);
		resetValidity();
	}

	Permutation<size>& operator=(Permutation<size>& source){
		if(stateLock.owns_lock()&&source.stateLock.owns_lock()){
			initializePermutation(source.permutation);
			resetValidity();
		}
		return *this;
	}

	Permutation<size>& operator=(Permutation<size>&& source){
		if(stateLock.owns_lock()){
			std::swap(permutation, source.permutation);
			std::swap(permutationIndices, source.permutationIndices);
			resetValidity();
		}
		return *this;
	}

	/**
	 * Is this a permutation? This is, is the array bijective from indices to indices.
	 */
	bool isValid(){
		return validPermutation;
	}

	/**
	 * Reserves the permutation for usage, read or write
	 * While having the lock, all changes to this permutation are allowed.
	 */
	void lock(){
		stateLock.lock();
	}

	/**
	 * Releases the lock on this permutation and rechecks the validity
	 */
	void unlock(){
		resetValidity();
		stateLock.unlock();
	}

	/**
	 * Requires lock.
	 * Gets the permutation of the given index. If the index is out of range,
	 * (unsigned)(-1) is returned. Behavior is undefined if you do not
	 * own the state lock for the permutation.
	 * No exception guarantee.
	 */
	unsigned getPermutationAt(unsigned index) noexcept{
		if(!stateLock.owns_lock())
			return -1;
		if(index>=size)
			return -1;
		return permutation[index];
	}

	/**
	 * Requires lock.
	 * Gets the index of the given permutation. If the permutation is out of range,
	 * (unsigned)(-1) is returned. Behavior is undefined if you do not
	 * own the state lock for the permutation object.
	 * No exception guarantee.
	 */
	unsigned getIndexOf(unsigned index) noexcept{
		if(!stateLock.owns_lock())
			return -1;
		if(index>=size)
			return -1;
		return permutationIndices[index];
	}

	/**
	 * Requires lock.
	 * Sets the permutation at the index. Returns true if the permutation was
	 * modified and false if out of range or the lock is not owned by the
	 * modifying thread.
	 * No exception guarantee.
	 */
	bool setPermutationAt(unsigned index, unsigned value) noexcept{
		if(!stateLock.owns_lock())
			return false;
		if(index>=size||value>=size)
			return false;
		permutation[index] = value;
		refreshIndices();
		return true;
	}

	/**
	 * Requires lock.
	 * Sets the permutation to the argument. Returns true if the permutation was
	 * valid and false if not or if lock is not owned by the
	 * modifying thread.
	 * No exception guarantee.
	 */
	bool setPermutationTo(unsigned perms[size]) noexcept{
		if(!stateLock.owns_lock())
			return false;
		initializePermutation(perms);
		resetValidity();
		return isValid;
	}

	PermutationIterator<size> iterator(){
		return PermutationIterator<size>(this);
	}


//All the following methods acquire locks before executing and release them directly afterwards.

	/**
	 * Waits for the lock and if the permutation is valid then returns the index
	 * of the given number. If this is not a valid permutation, returns (unsigned)(-1).
	 */
	unsigned indexOf(const unsigned number){
		lock();
		unsigned index;
		if(isValid()) index = getIndexOf(number);
		else index = -1;
		unlock();
		return index;
	}

	/**
	 * Waits for the lock and if the permutation is valid then returns the permutation
	 * at the given index. If this is not a valid permutation, returns (unsigned)(-1).
	 */
	unsigned permutationOf(const unsigned number){
		lock();
		unsigned permutation;
		if(isValid()) getPermutationAt(number);
		else permutation = -1;
		unlock();
		return permutation;
	}

	/**
	 * Waits for the lock to be released and then returns the permutation obtained
	 * by combining the permutation with the other (this ° other)
	 * This object is not changed.
	 * If this permutation or the other one is invalid, returns a copy of it.
	 */
	Permutation<size>
	circle(Permutation<size>& other){
		lock();
		other.lock();
		Permutation<size> perm(permutation);
		if(isValid()&&other.isValid()){
			unsigned circPerms[size];
			for(int i = 0;i<size;i++){
				unsigned part = other.permutation[i];
				unsigned full = permutation[part];
				circPerms[i] = full;
			}
			perm.initializePermutation(circPerms);
		}
		if(!other.isValid()){
			perm = other;
		}
		other.unlock();
		unlock();
		return perm;
	}

	/**
	 * Waits for the lock to be released and then returns the inverse of this permutation.
	 * This object is not changed.
	 * If this permutation is invalid, returns a copy of it.
	 */
	Permutation<size>
	inverse(){
		lock();
		Permutation<size> perm(permutation);
		if(isValid()){
			perm.initializePermutation(permutationIndices);
		}
		unlock();
		return std::move(perm);
	}

	void print(){
		lock();
		if(length > 0){
			printf("%u", permutation[0]);
			for(int i = 1;i < length;i++){
				printf(", %u",permutation[i]);
			}
		}
		unlock();
	}
};

/**
 * Random Access iterator over a mutable permutation of size size. Expects the permutation
 * to be locked on access.
 */
template<unsigned size>
class PermutationIterator<size>:std::iterator<std::random_access_iterator_tag, unsigned>{
	Permutation<size> *permPointer;
	int index;
public:
	PermutationIterator():permPointer(nullptr),
			index(0){
	}

	PermutationIterator(Permutation<size>& perm):
		permPointer(&perm),index(0){
	}

	PermutationIterator(Permutation<size> *perm):
		permPointer(perm),index(0){
	}

	PermutationIterator(Permutation<size>&& perm) = delete;

	unsigned operator*(){
		if(index<0||index>=size||permPointer==nullptr)
			return -1;
		return permPointer->getPermutationAt(index);
	}

	//FIXME allow change of the permutation through the iterator and setPermutationAt()

	PermutationIterator<size>& operator++(){
		if(index<size){
			index++;
		}
		return *this;
	}
	PermutationIterator<size> operator++(int){
		PermutationIterator<size> it (*this);
		operator++();
		return it;
	}

	PermutationIterator<size>& operator--(){
		if(index>=0){
			index--;
		}
		return *this;
	}
	PermutationIterator<size> operator--(int){
		auto it (this);
		operator--();
		return it;
	}

	bool operator==(PermutationIterator<size>& it){
		return it.index == index&&it.permPointer == permPointer;
	}
	bool operator!=(PermutationIterator<size>& it){
		return !operator ==(it);
	}

	PermutationIterator<size> operator+(int n){
		auto it (this);
		it += n;
		return it;
	}
	PermutationIterator<size> operator-(int n){
		return operator+(-n);
	}
	int operator-(PermutationIterator<size>& it){
		return index-it.index;
	}

	bool operator<(PermutationIterator<size>& it){
		return index<it.index;
	}
	bool operator>(PermutationIterator<size>& it){
		return index>it.index;
	}
	bool operator<=(PermutationIterator<size>& it){
		return index<=it.index;
	}
	bool operator>=(PermutationIterator<size>& it){
		return index>=it.index;
	}

	PermutationIterator<size>& operator+=(int i){
		index += i;
		index = (index>=size)? size : (index < 0 ? -1 : index);
		return *this;
	}
	PermutationIterator<size>& operator-=(int i){
		return operator+=(-i);
	}

	unsigned const operator[](int off){
		if(index<0||index>=size)
			return -1;
		return permPointer->getPermutationAt(index+off);
	}

	/*
	 * Special operators for this iterator, you can iterate the cycle of the permutation
	 */

	PermutationIterator<size>& operator*=(int n){
		if(n == 0)
			return *this;
		if(n < 0){
			while(n){
				index = permPointer->getIndexOf(index);
				n++;
			}
			return *this;
		}
		else{
			while(n){
				index = permPointer->getPermutationAt(index);
				n--;
			}
			return *this;
		}
	}
	PermutationIterator<size> operator*(int n){
		PermutationIterator<size> it (*this);
		it *= n;
		return it;
	}
	PermutationIterator<size>& operator/=(int n){
		return operator*=(-n);
	}
	PermutationIterator<size> operator/(int n){
		return operator*(-n);
	}
};
