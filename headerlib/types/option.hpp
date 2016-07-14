/*
 * option.hpp
 *
 * 	Created on: 14.07.2016
 * 		Author: andreas
 */

#pragma once
#include "hdrstd.hpp"
#include "monad.hpp"

namespace hdrtypes{
namespace option{
	namespace _impl{
		template<typename C>
		using MonadBind = hdrtypes::monad::MonadBind<C>;

		template<bool,typename>
		struct option;

		template<typename type>
		using some = option<true, type>;
		using none = option<false, void>;
		
		struct _option_monad_impl{
			template<typename T>
			struct ret {
			   	using result = some<T>;
			};

			template<typename option, typename function>
			struct bind{
			};
			template<typename function>
			struct bind<none, function>{
				using result = none;
			};
			template<typename type, typename function>
			struct bind<some<type>, function>{
				using result = typename function::template expr<type>::result;
			};
		};

		using _option_monad = MonadBind<_option_monad_impl>;
		struct _option_type{
			using monad = _option_monad;
		};

		template<bool existsB, typename typeT>
		struct option{
			constexpr static bool exists = existsB;
			using Type = typeT;
			using type = _option_type;
		};
	}

	template<typename type>
	using some = _impl::some<type>;
	using none = _impl::none;

	using option_type = _impl::_option_type;
}
}
namespace hdrstd{
	template<typename T>
	struct Printer<hdrtypes::option::some<T>>{
		static void print(){
			printf("%s", "Some<");
			Printer<T>::print();
			printf("%s", ">");
		}
	};
	template<>
	struct Printer<hdrtypes::option::none>{
		static void print(){
			printf("%s", "None");
		}
	};
}
