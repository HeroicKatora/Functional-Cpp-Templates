#ifndef HEADERLIB_HDR_TYPES_MAYBE_HPP
#define HEADERLIB_HDR_TYPES_MAYBE_HPP
/*
 * option.hpp
 * Declares an option monad type, called Maybe in Haskell. I renamed it to maybe
 * but haven't moved the file. This shall be a features forever.
 *
 *  Created on: 14.07.2016
 * 		Author: andreas
 */
#include "hdr/core/std.hpp"
#include "hdr/types/monad.hpp"

namespace hdr::maybe {

using ::hdr::std::Apply;
using ::hdr::std::True;
using ::hdr::std::False;
using ::hdr::std::Const;
using ::hdr::std::TemplateFunction;
using ::hdr::std::TypeFunction;
using ::hdr::std::Void;

using ::hdr::std::id;
using ::hdr::std::compose;
using ::hdr::std::flip;

using ::hdr::monad::MonadFromBind;

/** Use these type aliases to construct, compare and handle Maybes
 */
struct Nothing;
template<typename type>
struct Just;

template<typename Default, typename F, typename Maybe>
struct MaybeFunc;
/** Unpacks a maybe into a default value or the function application
 *		b -> (a -> b) -> Maybe a -> b
 */
using maybe = TypeFunction<MaybeFunc>;
template<typename Default, typename F>
struct MaybeFunc<Default, F, Nothing> {
	using type = Default;
};
template<typename Default, typename F, typename V>
struct MaybeFunc<Default, F, Just<V>> {
	using type = Apply<F, V>;
};

/// Monad constructor
using return_ 	= TemplateFunction<Just>;
using bind 			= Apply<flip, Apply<maybe, Nothing>>;
using MaybeType = MonadFromBind<return_, bind>;
/// Monad fmap function, expressed with bind and return
using fmap 			= MaybeType::fmap;
using kleisli 	= MaybeType::kleisli;
using join    	= MaybeType::join;

/// True if argument is an instance of Just else False
using isJust 		= Apply<maybe, False, Const<True>>;
/// True if argument is not an instance of Just else True
using isNothing = Apply<maybe, True,  Const<False>>;

/// Exception when argument is not Just<V>
using fromJust  = Apply<maybe, Void, id>;
/** Returns the default value or an unpacked Just
 *  	(a -> Maybe a -> a)
 */
using fromMaybe = Apply<Apply<flip, maybe>, id>;

} // namespace hdrtypes::option
#endif //HEADERLIB_HDR_TYPES_MAYBE_HPP
