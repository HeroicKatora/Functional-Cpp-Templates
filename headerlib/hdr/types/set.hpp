#ifndef HEADERLIB_HDR_TYPES_SET_HPP
#define HEADERLIB_HDR_TYPES_SET_HPP
#include "hdr/core.hpp"
#include "hdr/math.hpp"
#include "hdr/types/maybe.hpp"
/**
 * @file: set.hpp
 * A set type
 * Based on https://groups.csail.mit.edu/mac/users/adams/BB/ which is also origin of haskells implementation
 */
namespace hdr::set {
using ::hdr::std::TemplateFunction;
using ::hdr::std::TypeFunction;

namespace {
using ::hdr::std::Apply;
using ::hdr::std::Const;
using ::hdr::lambda::Lambda;
using ::hdr::lambda::IApply;
using ::hdr::lambda::_0;
using ::hdr::lambda::_1;
using ::hdr::lambda::_2;
using ::hdr::match::_;
using ::hdr::match::Placeholder;
using ::hdr::match::MatchClause;
using ::hdr::match::With;
using ::hdr::match::get;
using ::hdr::math::plus;
using ::hdr::math::Zero;

template<typename Compare> struct SetType;
struct Empty;
template<typename Entry, typename Count, typename Left, typename Right> struct Node;
using node = TemplateFunction<Node>;

using size = MatchClause<With<Empty,                         Const<Zero>>,
                         With<Node<_,Placeholder<Zero>,_,_>, Apply<get, Zero>>
                        >;
using smart_node = Lambda<node, _0, IApply<plus, _1, IApply<plus, _1, _2>>, _1, _2>;
}


using make_type   = TemplateFunction<SetType>;
//using number_type = Apply<make_type, ::hdr::math::>;
using empty       = ::hdr::std::Void;

template<typename El, typename Set>
struct Insert {

};
using insert = TypeFunction<Insert>;


}

#endif
