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

template <typename T>
struct is_template_instance : std::false_type {};

template <template <typename...> class Template, typename... Args>
struct is_template_instance<Template<Args...>> : std::true_type {};

template <typename T>
struct is_container {
private:
    template <typename C>
    static constexpr bool has_begin_end(int) {
        return requires(C c) {
            { c.begin() } -> std::input_or_output_iterator;
            { c.cbegin() } -> std::input_or_output_iterator;
            { c.end() } -> std::input_or_output_iterator;
            { c.cend() } -> std::input_or_output_iterator;
        };
    }
    template <typename C>
    static constexpr bool has_begin_end(...) {
        return false;
    }

public:
    static constexpr bool value{ has_begin_end<T>(0) and !std::is_array_v<T> };
};

template <typename T>
concept has_name_method = requires(T t) {
    { t.Name() } -> is_string_like;
    // 其他比较方法
};

template <typename T>
concept shared_ptr_to_aggregate = requires {
    typename T::element_type;
    requires not is_template_instance<typename T::element_type>::value;
    requires not is_container<typename T::element_type>::value;
    requires std::is_class_v<typename T::element_type>;
    requires std::is_same_v<std::shared_ptr<typename T::element_type>, T>;
    requires has_name_method<typename T::element_type>;
};
