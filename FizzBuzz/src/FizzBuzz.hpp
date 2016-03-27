/*
 * FizzBuzz.hpp
 *
 *  Created on: 27.03.2016
 *      Author: andreas
 */

#pragma once
#include <iostream>

template<unsigned n>
struct FizzBuzz;

template<bool ...vals>
struct FPrinter;

template<unsigned n>
struct FizzBuzz{
	using Printer = FPrinter<n%3 == 0, n%5 == 0>;
	static void print(){
		FizzBuzz<n-1>::print();
		std::cout << n << " ";
		Printer::print();
	};
};
template<>
struct FizzBuzz<0>{
	static void print(){};
};

template<bool threeDiv, bool fiveDiv>
struct _p_string{
	constexpr static const char * string = "\n";
};
template<bool fiveDiv>
struct _p_string<true, fiveDiv>{
	constexpr static const char * string = "Fizz\n";
};
template<bool threeDiv>
struct _p_string<threeDiv, true>{
	constexpr static const char * string = "Buzz\n";
};
template<>
struct _p_string<true, true>{
	constexpr static const char * string = "FizzBuzz\n";
};

template<bool ...vals>
struct FPrinter{
	static void print(){
		std::cout << _p_string<vals...>::string;
	}
};

