/*
 * main.cpp
 *
 *  Created on: 02.04.2016
 *      Author: andreas
 */

#include "hdrlist.hpp"
#include "hdrmap.hpp"

template<typename arg>
struct plus_one{
	static_assert(_false<arg>::value, "Can only operate on integral constants");
	using value = void;
};
template<typename _Tp, _Tp v>
struct plus_one<std::integral_constant<_Tp, v>>{
	using value = std::integral_constant<_Tp, v+1>;
};

int main(int argc, char **argv) {
	using tlist = _t_list<std::integral_constant<unsigned, 3>>;
	using mod = typename hdrlist::map<plus_one, tlist>::result;
	using clist = typename hdrlist::t_to_c<mod>::result;
	using print_list = hdrstd::Printer<clist>;
	print_list::print();
}


