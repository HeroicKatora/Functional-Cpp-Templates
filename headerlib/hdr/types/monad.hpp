#ifndef HEADERLIB_HDR_TYPES_MONAD_HPP
#define HEADERLIB_HDR_TYPES_MONAD_HPP
/*
 * hdrmonad.hpp
 *
 * 	Created on: 13.07.2016
 * 		Author: andreas
 */

#pragma once
#include "hdrstd.hpp"


namespace hdrtypes{ namespace monad{


	template<typename Instance>
	using Type = hdrstd::Type<Instance>;
	struct MonadType {};

	template<typename Monad, typename T>
	struct ret{
		using result = typename Type<Monad>::template Impl<MonadType>::template ret<T>::result;
	};

	template<typename Monad, typename F>
	struct bind{
		using result = typename Type<Monad>::template Impl<MonadType>::template bind<Monad, F>::result;
	};

	template<typename Monad, typename Fa, typename Fb>
	struct kleisli{
		using result = typename Type<Monad>::template Impl<MonadType>::template kleisli<Fa, Fb>::result;
	};

	template<typename F, typename Monad>
	struct functor{
		using result = typename Type<Monad>::template Impl<MonadType>::template functor<F, Monad>::result;
	};

	template<typename C>
	struct MonadBind{
		template<typename T>
		using ret = typename C::template ret<T>;

		template<typename op, typename f>
		using bind = typename C::template bind<op, f>;

		template<typename Fa, typename Fb>
		struct kleisli{
			template<typename a>
			struct expr{
				using step = typename Fa::template expr<a>::result;
				using result = MonadBind::bind<step, Fb>;
			};
			using result = f_<expr>;
		};

		template<typename f, typename op>
		struct functor{
			using _ret_function = f_<MonadBind::ret>;
			using result = typename MonadBind::template bind<op, c_<_ret_function, f>>::result;
		};
	};
} // namespace monad
} // namespace hdrtypes
#endif //HEADERLIB_HDR_TYPES_MONAD_HPP
