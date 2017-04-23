#ifndef HEADERLIB_HDR_CORE_TEMPLATE_TEMPLATE_ARGS_COUNT_HPP
#define HEADERLIB_HDR_CORE_TEMPLATE_TEMPLATE_ARGS_COUNT_HPP
#include <utility>
namespace hdr::std::helper {
template<typename>
auto _deduct_function() -> ::std::integral_constant<size_t, 0>;
template<template<typename>typename>
auto _deduct_function() -> ::std::integral_constant<size_t, 1>;
template<template<typename,typename>typename>
auto _deduct_function() -> ::std::integral_constant<size_t, 2>;
template<template<typename,typename,typename>typename>
auto _deduct_function() -> ::std::integral_constant<size_t, 3>;
template<template<typename,typename,typename,typename>typename>
auto _deduct_function() -> ::std::integral_constant<size_t, 4>;
template<template<typename,typename,typename,typename,typename>typename>
auto _deduct_function() -> ::std::integral_constant<size_t, 5>;
template<template<typename...> typename Arg>
constexpr const static size_t args_count = decltype(_deduct_function<Arg>())::value;

namespace inline_test {
template<typename> struct OneArg;
static_assert(args_count<OneArg>  == 1);
template<typename,typename,typename,typename,typename> struct FiveArg;
static_assert(args_count<FiveArg> == 5);
} /* inline_test */
}

#endif /* end of include guard: HEADERLIB_HDR_CORE_TEMPLATE_TEMPLATE_ARGS_COUNT_HPP */
