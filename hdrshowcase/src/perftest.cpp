#include "hdrstd.hpp"
#include "hdrlist.hpp"
#include "hdrmap.hpp"
#include <functional>
#include <utility>


template<typename Map, typename Num>
struct build_map{
  using result = typename hdrmap::template add<typename Num::value_type, Num::value, Num, Map>::result;
};

template<typename T>
struct Dep{ using type = T; };

template<typename T>
struct Main {
  using _ = Dep<T>;
  using initlist = std::make_index_sequence<200>;
  using list = typename hdrlist::c_to_t<initlist>::result;
  using map = typename hdrstd::expression<hdrlist::fold_left, f_<build_map>, hdrmap::empty<typename initlist::value_type>, list>::expr<T>::result;
  using Print = hdrstd::Printer<map>;
};

int main(){
  //Main<hdrstd::Void>::Print::print();
};
