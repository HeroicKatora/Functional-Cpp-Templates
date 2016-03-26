/*
 * main.c
 *
 *  Created on: 03.06.2014
 *      Author: Andreas Molzer
 */

#include <iostream>
#include "bind.hpp"

struct Foo{
	int val;
};

int max (Foo& a, Foo& b){
	return a.val > b.val?a.val:b.val;
}

int main(int argc, char **argv){
	auto max6 = bind(max, Foo{6}, _auto);
	Foo firstCmp{4};
	std::cout << "Expecting 6:" << max6(firstCmp) << std::endl;
	Foo secondCmp{19};
	std::cout << "Expecting 19:" << max6(secondCmp) << std::endl;
}
