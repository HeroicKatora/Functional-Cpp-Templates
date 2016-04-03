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

unsigned constexpr keyFunc(unsigned val){
	return val;
}
template<unsigned val>
struct keyFunction {
	using result = typename hdrstd::to_int_unsigned<keyFunc(val)>::result;
};
template<typename map, typename num>
struct buildMap {
	using mapping = typename keyFunction<num::value>::result;
	using result = typename hdrmap::map_add<unsigned, num::value, mapping, map>::result;
};

int main(int argc, char **argv) {
	using clist = _c_list<6,3,4,5,7,10>;
	using tlist = typename hdrlist::c_to_t<clist>::result;
	using mod = typename hdrlist::map<plus_one, tlist>::result;
	using reslist = typename hdrlist::t_to_c<mod>::result;
	using print_list = hdrstd::Printer<reslist>;
	print_list::print();
	printf("\n");
	using emptyMap = typename hdrmap::map_empty<unsigned>;
	using fullMap = typename hdrlist::fold_left<buildMap, emptyMap, mod>::result;
	using print_map = hdrstd::Printer<fullMap>;
	print_map::print();
}


