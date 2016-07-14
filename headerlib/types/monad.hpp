/*
 * hdrmonad.hpp
 *
 * 	Created on: 13.07.2016
 * 		Author: andreas
 */

#pragma once
#include "hdrstd.hpp"


namespace hdrtypes{
	template<typename T>
	struct MonadImplementation{
		static_assert(_false<T>::value, "Is not a monad");
		using implementation = Void;
	};

	struct monad{
		template<typename Monad, typename T>
		struct ret{
			using result = typename Monad::type::monad::template ret<T>::result;
		};

		template<typename Monad, typename F>
		struct bind{
			using result = typename Monad::type::monad::template bind<Monad, F>::result;
		};

		template<typename Monad, typename Fa, typename Fb>
		struct kleisli{
			using result = typename Monad::type::monad::template kleisli<Fa, Fb>::result;
		};

		template<typename F, typename Monad>
		struct functor{
			using result = typename Monad::type::monad::template functor<F, Monad>::result;
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
	};
}
