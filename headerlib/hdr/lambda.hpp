#ifndef HEADERLIB_HDR_LAMBDA_HPP
#define HEADERLIB_HDR_LAMBDA_HPP
/*
 * @file: lambda.hpp
 * Defines a helper type (not part of the real type system, i.e. not a function nor a non-template type)
 * which helps with definition of lambdas. Many times you want to create functions on the fly, but have
 * to resort to this:
 *   template<typename T> using FunctionEval = ...;
 *   using function = TemplateFunction<FunctionEval>;
 * This header will enable you to deal with this in a standardized way, without having to rely on internals.
 * Although this type is not functionally constructible (you can't create a constructor with Constructor<Lambda>)
 *
 * Note: you can 'abuse' notation by deriving from a lambda to make a type double as a function but this
 * practise is not recommended. Rather create a function that returns the lambda from your type (and save
 * the lambda as a member or just do matching).
 *
 *  Created on: 11.04.2017
 *      Author: Andreas Molzer
 */

#include "hdr/std.hpp"

namespace hdr::lambda {
template<typename ... T> struct IApply;
template<typename ... T> struct ConcreteLambda; // Internal lambda representation
template<typename ... T> using  Lambda = typename ConcreteLambda<T...>::remove_boiler;

namespace {
 // Outside the standard type system, you shouldn't modify or pass them... I might regret this
template<unsigned t>     struct Variable;

template<typename T>     struct Terminal;
template<typename T>     struct Terminal               { constexpr static bool value = true; };
template<typename ... T> struct Terminal<IApply<T...>> { constexpr static bool value = (Terminal<T>::value && ...); };
template<unsigned t>     struct Terminal<Variable<t>>  { constexpr static bool value = false; };

template<typename A, typename T>     struct Downgrade;
using downgrade = ::hdr::std::TypeFunction<Downgrade>;
template<typename A, typename T>     struct Downgrade                  { using type = T; };
template<typename A, unsigned t>     struct Downgrade<A, Variable<t>>  { using type = Variable<t - 1>; };
template<typename A>                 struct Downgrade<A, Variable<0>>  { using type = A; };
template<typename A, typename ... T> struct Downgrade<A, IApply<T...>> { using type = typename IApply<::hdr::std::Apply<downgrade, A, T>...>::remove_boiler; };

struct SelectIApply { template<typename F, typename ...T> using type = IApply<F, T...>; };
struct SelectLApply { template<typename F, typename ...T> using type = ConcreteLambda<F, T...>; };
struct SelectSApply { template<typename F, typename ...T> using type = ::hdr::std::Apply<F, T...>; };
}

template<typename F, typename ... T> struct IApply<F, T...> {
  using IType = IApply<F, T...>;
  // if there are no variables, type is the applied function. If there are, type is this exact class
  using remove_boiler = typename ::hdr::std::Conditional<Terminal<IType>::value, SelectSApply, SelectIApply>::template type<F, T...>;
};

template<typename F, typename ... T> struct ConcreteLambda<F, T...> {
  using IType = IApply<F, T...>;
  // if there are no variables, type is the applied function. If there are, type is this exact class
  using remove_boiler = typename ::hdr::std::Conditional<Terminal<IType>::value, SelectSApply, SelectLApply>::template type<F, T...>;
  template<typename A>
  using expr = Lambda<::hdr::std::Apply<downgrade, A, F>, ::hdr::std::Apply<downgrade, A, T>...>;
};


using _0 = Variable<0>;
using _1 = Variable<1>;
using _2 = Variable<2>;
using _3 = Variable<3>;
using _4 = Variable<4>;

}

#endif //HEADERLIB_HDR_LAMBDA_HPP
