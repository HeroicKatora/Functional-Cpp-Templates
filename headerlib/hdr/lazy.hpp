/*
 * lazy.hpp
 *
 *  Created on: 20.02.2017
 *      Author: Andreas Molzer
 */

#pragma once
#include "hdr/std.hpp"

namespace hdrstd::lazy {

using ::hdrstd::Apply;
using ::hdrstd::compose;
using ::hdrstd::fconst;
using ::hdrstd::Void;

/**	Lift from Pure to Monad
 */
HDR_FUNCTION
HDR_PURE HDR_MAP_TO HDR_MONAD
using freturn = fconst;

/**	Collapse from lazy monad to pure
 */
HDR_FUNCTION
HDR_MONAD HDR_MAP_TO HDR_PURE
template<typename T> using Eval = Apply<T, Void>;

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
using lift = hdrstd::F2<Lift1>;
using lift1 = lift;
using fmap = lift;

/** Join operator of the lazy monad
 *  (() -> () -> a) -> (() -> a)
 */
struct FJoin {
  template<typename A, typename V>
  using expr = Apply<A, V, V>;
};
using fjoin = hdrstd::F2<FJoin>;

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
using lift2 = hdrstd::F3<Lift2>;

}
