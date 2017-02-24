#include <utility>
template<typename a, typename b> struct TT;
struct St {
  const char* str;
  int val;
};
template<typename A, typename B> struct StT { using str = A; using val = B;};

template<typename T>
struct TypeOfDeferred {
  using type = typename std::remove_reference<decltype(T::value)>::type;
};
template<typename T, const T& t>
struct DeferredValue {
  static constexpr const T& value = t;
};
template<typename T, typename R,
	R TypeOfDeferred<T>::type::* p>
struct DeferredSubobject {
  static constexpr const R& value = T::value.*p;
};
template<typename T, typename O>
struct DeferredPlus {
  using TType = typename std::remove_reference<decltype(T::value)>::type;
  using OType = typename std::remove_reference<decltype(O::value)>::type;
  static_assert(std::is_same_v<TType, OType>);
  using VType = TType;
  static constexpr const VType value = T::value + O::value;
};
template<typename T, std::ptrdiff_t index>
struct DeferredArrayAccess {
  static constexpr const auto& value = T::value[index];
};

template<typename R, typename T, R T::* p, const T& t>
struct MemberOf_ {
  using type = DeferredSubobject<DeferredValue<T, t>, R, p>;
};

template<typename R, typename T, R T::* p, const T& t>
using MemberOf = typename MemberOf_<R, T, p, t>::type;

template<const St& t>
using varst = StT<MemberOf<const char*, St, &St::str, t>, MemberOf<int, St, &St::val, t>>;

constexpr auto str = "this is a string";
constexpr St testSt {str, 1};
using StTestType = varst<testSt>;

constexpr decltype(str) extracted = StTestType::str::value;
static_assert(extracted == str);

using AddedValue = DeferredPlus<StTestType::val, std::integral_constant<int, 3>>;
constexpr int extracted_value = AddedValue::value;
static_assert(extracted_value == 1 + 3);
using ConstructedType = std::integral_constant<int, extracted_value>;

using InStringAccess = DeferredArrayAccess<StTestType::str, 3>;
constexpr char extracted_char = InStringAccess::value;
static_assert(extracted_char == testSt.str[3]);
