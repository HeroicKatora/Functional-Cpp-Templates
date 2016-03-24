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
	auto max5 = bind(max, Foo{6}, _auto);
	Foo second{4};
	std::cout << max5(second);
}
