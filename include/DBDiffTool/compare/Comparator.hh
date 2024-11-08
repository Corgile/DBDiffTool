/// @file Comparator.hh
/// @author xianghongli\@hikvision.com.cn
/// @date 2024-11-08.
/// HangZhou HikVision Digital Technology Co., Ltd. All Right Reserved.
#pragma once

#include <string>

#include <DBDiffTool/object/Schema.hh>

struct string_less {
    bool operator()(std::string_view a, std::string_view b) const {
        /// 必须与 SQL 语句 ORDER BY LENGTH(xxx), xxx 效果一致
        return a.size() < b.size() or (a.size() == b.size() and a < b);
    }
};

namespace compare {
namespace detail {
template <typename T, Lang locale = Lang::EN>
auto nameof() -> std::string {
    std::string name{ typeid(T).name() };
    auto const  qualifier{ name.find_first_of(' ') + 1 };
    name = name.substr(qualifier);
    if constexpr (locale == Lang::CN) {
        return glb::name2cn.at(name);
    }
    return name;
}

template <typename T, Lang locale>
auto no_such() -> std::string {
    if constexpr (locale == Lang::CN) {
        std::string ret{ "没有此【" };
        ret.append(nameof<T, Lang::CN>());
        ret.append("】");
        return ret;
    } else if constexpr (locale == Lang::EN) {
        std::string ret{ "No such \x1b[1m" };
        ret.append(nameof<T, Lang::EN>());
        ret.append("\x1b[0m");
        return ret;
    }
    assert(false); // unreachable
    return "unreachable";
}
} // namespace detail

template <typename T, Lang locale>
auto Absent() -> std::string {
    return detail::no_such<T, locale>();
}

template <typename T, Lang locale>
auto Present(std::string_view value) -> std::string {
    return { value.data() };
}

void Compare(std::vector<schema_t> const& listA,
             std::vector<schema_t> const& listB, std::stringstream& diff);

void Compare(std::vector<table_t> const& listA,
             std::vector<table_t> const& listB, std::stringstream& diff);
void Compare(std::vector<Sequence> const& listA,
             std::vector<Sequence> const& listB, std::stringstream& diff);
void Compare(std::vector<Procedure> const& listA,
             std::vector<Procedure> const& listB, std::stringstream& diff);

void Compare(std::vector<Column> const& listA, std::vector<Column> const& listB,
             std::stringstream& diff);
void Compare(std::vector<Index> const& listA, std::vector<Index> const& listB,
             std::stringstream& diff);
void Compare(std::vector<Trigger> const& listA,
             std::vector<Trigger> const& listB, std::stringstream& diff);

}