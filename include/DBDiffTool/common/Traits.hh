/// @file Traits.hh
/// @author xianghongli\@hikvision.com.cn
/// @date 2024-11-08.
/// HangZhou HikVision Digital Technology Co., Ltd. All Right Reserved.
#pragma once
#include <memory>
#include <type_traits>

template <typename T>
concept is_string_like = requires(T a) {
    { std::string{ a } } -> std::convertible_to<std::string>;
};

template <typename T> // NOLINT
struct is_shared_ptr : std::false_type {};

template <typename T>
struct is_shared_ptr<std::shared_ptr<T>> : std::true_type {};

template <typename T>
concept has_key_method = requires(T t) {
    { t.Key() } -> is_string_like;
    { t.Name() } -> is_string_like;
};

#define ELEM_TYPE typename T::element_type
template <typename T>
concept sharedptr_to_aggregate = requires {
    requires is_shared_ptr<T>::value;
} and requires {
    typename T::element_type;
    { std::is_aggregate_v<ELEM_TYPE> } -> std::convertible_to<bool>;
    requires has_key_method<ELEM_TYPE>;
} and requires {
    std::is_same_v<typename std::remove_cvref_t<T>::element_type, ELEM_TYPE>;
} and requires(T t) {
    { *t } -> std::convertible_to<ELEM_TYPE>;
};
#undef ELEM_TYPE

template <typename T>
    requires sharedptr_to_aggregate<T>
using iterator = typename std::vector<T>::const_iterator;
