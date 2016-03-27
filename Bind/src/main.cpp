/*
 * main.c
 *
 *  Created on: 03.06.2014
 *      Author: Andreas Molzer
 */

#include <iostream>
#include "bind.hpp"

struct Foo{
	Foo(const int val):val(val){
		std::cout << "Foo value instantiation" << std::endl;
	};
	Foo(Foo& ref):val(ref.val){
		std::cout << "Foo copy constructor" << std::endl;
	};
	Foo(const Foo& ref):val(ref.val){
		std::cout << "Foo const copy constructor" << std::endl;
	}
	Foo(Foo&& ref):val(ref.val){
		std::cout << "Foo move constructor" << std::endl;
	};
	Foo(const Foo&& ref):val(ref.val){
		std::cout << "Foo const move constructor" << std::endl;
	}
	int val;
};

int max (Foo& a, Foo& b){
	return a.val > b.val?a.val:b.val;
}

int add3 (int& a, int& b, int& c){
	return a + b + c;
}

int main(int argc, char **argv){
	std::cout << "Constructing bind" << std::endl;
	auto max6 = bind(max, Foo{6}, _auto);
	std::cout << "Constructing argument" << std::endl;
	Foo firstCmp{4};
	std::cout << "Invoking bound expression" << std::endl;
	std::cout << "Expecting 6:" << max6(firstCmp) << std::endl;
	Foo secondCmp{19};
	std::cout << "Expecting 19:" << max6(secondCmp) << std::endl;
	auto add4 = bind(add3, 4, _auto, _auto);
	auto add4D = bind(add3, 4, fill<1>, fill<1>);
	std::cout << "Expecting 15:" << add4(5,6) << std::endl;
	std::cout << "Expecting 14:" << add4D(5,10000) << std::endl;
}
