# CppLibrary

A small library of my C++ stuff built upon my header library.
The header library is meant to work similarly to the type meta programming part of Boost.Hana while alleviating some issues I had with it.
The approach is a functional one, relying on template programming instead of constexpr object representation.

The code quality is not up to Boost.Hana but afterall I explore a completely new style of functional templating, sorry for the inconvenience. A very interesting aspect of the Boost library is the usefullness of its error messages which I can currently not even come close to. Debugging statements are rare (only tested for correctness by a couple of test cases) and "type type safety" is not enforced in any way.

Paradigm
-------
The only first class citizen in headerlib are structs. Without free template parameters of any sort. It's is very complicated for functions in the template system to have side effects which is strong reasoning behind the functional approach of programming I chose for this library.

Advantages
----------
Let's look at an example as a comparison from Boost.Hana about eval_if coding the factorial function [adapted for c++17 from here](http://www.boost.org/doc/libs/1_62_0/libs/hana/doc/html/group__group-Logical.html#gab64636f84de983575aac0208f5fa840c)
```template <typename N>
template<typename N>
constexpr auto ffactorial(const N n) {
    return hana::eval_if(hana::equal(n, hana::int_c<0>),
        [] () constexpr { return hana::int_c<1>; },
        [=](auto _) constexpr { return hana::mult(n, fact(hana::minus(_(n), hana::int_c<1>))); }
    );
}
constexpr auto res = ffactorial(hana::int_c<5>);
```
There are several negative points made in the documentation and it's just as bad when using it to dynamically determining a typename.
But you know how to code the same function with template specialization already. That's where this library comes in.
```
template<int n> struct factorial {
  constexpr static const int value = n*factorial<n-1>::value;
};
template<> struct factorial<0> {
  constexpr static const int value = 1;
};
constexpr auto res = hdr::Apply<hdr::ValueTemplateFunction<factorial>, hdr::Signed<5>>::value;
```
To see this in action, see [this file](test/src/math.cpp)


Real world example
---------
Consider a templated bitstream which is able to write input symbols bitwise into an output stream for some integral type. Its internal cache should be at least two times as wide as the maximum length input symbol in order to be able to write efficiently with the use bitshifts. The larger the cache is, the fewer writes to the output stream are needed but integral types larger than the largest register will have to be emulated. Hence, we would like to construct an integral type as large as a register but larger only if necessary.
```// DoublyWideCache<uint32_t> should be uint64_t while DoublyWideCache<uint64_t> would be boost::multiprecision::uint128_t; etc.
template<typename Integral>
using SymbolOutputCache = ???
```
