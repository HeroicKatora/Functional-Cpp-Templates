/*
 * main.cpp
 *
 *  Created on: 02.04.2015
 *      Author: Andreas Molzer
 */

#include "Permutation.hpp"
#include <cstdlib>
#include <cstdio>

int main(int argc, char **argv) {
	const auto& perm1 = makePermutation<0,3,1,2>();
	const auto& permComb = perm1.circle(perm1);
	const auto& permId = makeIdentity<7>();
	const auto& extPerm = extendWithIdentity<7>(perm1);

	perm1.print();
	permComb.print();
	permId.print();
	extPerm.print();

	unsigned permArr [] = {0,2,1,3};
	Permutation<4> perm = createDynamicInstance(perm1);
	Permutation<4> permAlt(permArr);
	Permutation<4> permCombined = perm.circle(permAlt);

	PermutationIterator<4> iterator = permCombined.iterator();
	permCombined.lock();
	printf("%u,", *(iterator++));
	printf("%u,", *(iterator++));
	printf("%u,", *(iterator++));
	printf("%u", *(iterator++));
	permCombined.unlock();

}

