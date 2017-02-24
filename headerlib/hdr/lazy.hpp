/*
 * lazy.hpp
 *
 *  Created on: 20.02.2017
 *      Author: Andreas Molzer
 */

#pragma once
#include "hdr/std.hpp"

namespace hdr::lazy {

using ::hdr::std::Apply;
using ::hdr::std::Void;
using ::hdr::std::Function;
using ::hdr::std::compose;
using ::hdr::std::fconst;
using ::hdr::std::flip;

/**	Lift from Pure to Monad
 */
HDR_FUNCTION
HDR_PURE HDR_MAP_TO HDR_MONAD
using freturn = fconst;

/**	Collapse from lazy monad to pure and constructor
 */
HDR_FUNCTION
HDR_MONAD HDR_MAP_TO HDR_PURE
template<typename T> using Lazy = Apply<freturn, T>;
template<typename T, typename V=Void> using Eval = Apply<T, V>;

/** Lift from Function to Monad Space, with 1 argument
 *		(a -> b) -> (() -> a) -> (() -> b)
 */
HDR_BASE
HDR_FUNCTION HDR_MAP_TO HDR_MONAD
struct Lift1{
  template<typename F, typename Wrapped>
  using expr = Apply<compose, F, Wrapped>;
};
HDR_FUNCTION
HDR_FUNCTION HDR_MAP_TO HDR_MONAD
using lift = Function<Lift1>;
using lift1 = lift;
using fmap = lift;

/** Join operator of the lazy monad
 *  (() -> () -> a) -> (() -> a)
 */
struct FJoin {
  template<typename A, typename V>
  using expr = Apply<A, V, V>;
};
using fjoin = Function<FJoin>;

/** Utility for lazy functions taking 2 arguments, guarantees to not eval any partial
 *  of the function until the result is evaluated.
 *    (a -> b -> c) -> (() -> a) -> (() -> b) -> (() -> c)
 */
HDR_BASE
HDR_FUNCTION HDR_MAP_TO HDR_MONAD
struct Lift2{
  template<typename F, typename Wa> using L1 = Apply<lift1, F, Wa>;
  template<typename F, typename Wa, typename Wb>
  using expr = Apply<fjoin, Apply<compose, Apply<flip, L1<F, Wa>>, Wb>>;
};
using lift2 = Function<Lift2>;

}
