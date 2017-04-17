#ifndef HEADERLIB_HDR_TYPES_MONAD_HPP
#define HEADERLIB_HDR_TYPES_MONAD_HPP
/*
 * hdrmonad.hpp
 *
 * 	Created on: 13.07.2016
 * 		Author: andreas
 */

#pragma once
#include "hdr/core/std.hpp"


namespace hdr::monad {
	using hdr::std::Apply;
	using hdr::std::TemplateFunction;
	using hdr::std::flip;
	using hdr::std::compose;
	using hdr::std::id;

	template<typename ret_, typename bind_>
	struct MonadFromBind {
		using return_ = ret_;
		using bind 		= bind_;
		using kleisli = Apply<compose, flip, Apply<compose, bind>>;
		using fmap 		= Apply<compose, Apply<flip, bind>, Apply<compose, return_>>;
		using join    = Apply<flip, bind, id>;
	};
	using monadFromBind = TemplateFunction<MonadFromBind>;

	template<typename>
	struct MonadFromKleisli {

	};
} // namespace hdr::monad
#endif //HEADERLIB_HDR_TYPES_MONAD_HPP
