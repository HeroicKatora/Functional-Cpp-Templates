/*
 * main.cpp
 *
 *  Created on: 02.04.2016
 *      Author: andreas
 */

#include "hdrlist.hpp"
#include "hdrmap.hpp"
#include "types/option.hpp"

template<typename val>
struct filter_function{};

template<typename _Tp, _Tp a>
struct filter_function<std::integral_constant<_Tp, a>>{
	constexpr static bool result = a % 2 == 0;
};

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
using createSingularMap = typename p_<buildMap, emptyMap>::result<val>;;

int main(int argc, char **argv) {
	printf("Mapping 6,3,4,5,7,10 with x+1\n");
	using clist = _c_list<6,3,4,5,7,10>;
	using tlist = typename hdrlist::c_to_t<clist>::result;
	using mod = typename hdrlist::map<hdrstd::function<inc>, tlist>::result;
	using reslist = typename hdrlist::t_to_c<mod>::result;
	using print_list = hdrstd::Printer<reslist>;
	print_list::print();
	printf("\n");

	printf("Filtering for even numbers\n");
	using filtered = typename hdrlist::filter<hdrstd::function<filter_function>, tlist>::result;
	using print_filtered = hdrstd::Printer<filtered>;
	print_filtered::print();
	printf("\n");

	printf("Building a map from it\n");
	using emptyMap = typename hdrmap::empty<unsigned>;
	using fullMap = typename hdrlist::fold_left<f_<buildMap>, emptyMap, mod>::result;
	using print_map = hdrstd::Printer<fullMap>;
	print_map::print();
	printf("\n");

	printf("Trying map search for 6 and then for 9\n");
	using search_6 = typename hdrmap::search<unsigned, 6, fullMap>::result;
	hdrstd::Printer<search_6>::print(); printf("%s", "\n");
	using search_9 = typename hdrmap::search<unsigned, 9, fullMap>::result;
	hdrstd::Printer<search_9>::print(); printf("%s", "\n");

	printf("Trying to apply the option monad by calling fmap inc some<13>\n");
	using applied_6 = typename hdrtypes::monad::functor<f_<inc>, search_6>::result;
	hdrstd::Printer<applied_6>::print(); printf("%s", "\n");

	printf("Trying sth else\n");
	using map_3 = typename createSingularMap<typename to_int_unsigned<3>::result>::result;
	using print_map_3 = hdrstd::Printer<map_3>;
	print_map_3::print();
	printf("\n");
}


