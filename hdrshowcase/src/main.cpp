/*
 * main.cpp
 *
 *  Created on: 02.04.2016
 *      Author: andreas
 */

#include "hdrlist.hpp"
#include "hdrmap.hpp"

template<typename arg>
struct inc{
	static_assert(_false<arg>::value, "Can only operate on integral constants");
	using result = void;
};
template<typename _Tp, _Tp a>
struct inc<std::integral_constant<_Tp, a>>{
	using arg = std::integral_constant<_Tp, a>;
	using result = std::integral_constant<_Tp, a+1>;
};

unsigned constexpr keyFunc(unsigned val){
	return val*2+1;
}
template<unsigned val>
struct keyFunction {
	using result = typename hdrstd::to_int_unsigned<keyFunc(val)>::result;
};
template<typename map, typename num>
struct buildMap {
	using mapping = typename keyFunction<num::value>::result;
	using result = typename hdrmap::add<unsigned, num::value, mapping, map>::result;
};

using emptyMap = typename hdrmap::empty<unsigned>;

template<typename val>
using createSingularMap = typename p<buildMap, emptyMap>::result<val>;;

int main(int argc, char **argv) {
	using clist = _c_list<6,3,4,5,7,10>;
	using tlist = typename hdrlist::c_to_t<clist>::result;
	using mod = typename hdrlist::map<inc, tlist>::result;
	using reslist = typename hdrlist::t_to_c<mod>::result;
	using print_list = hdrstd::Printer<reslist>;
	print_list::print();
	printf("\n");

	using emptyMap = typename hdrmap::empty<unsigned>;
	using fullMap = typename hdrlist::fold_left<buildMap, emptyMap, mod>::result;
	using print_map = hdrstd::Printer<fullMap>;
	print_map::print();
	printf("\n");

	using map_3 = typename createSingularMap<typename to_int_unsigned<3>::result>::result;
	using print_map_3 = hdrstd::Printer<map_3>;
	print_map_3::print();
	printf("\n");
}


