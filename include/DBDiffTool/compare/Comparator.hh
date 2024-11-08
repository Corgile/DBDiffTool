/// @file Comparator.hh
/// @author xianghongli\@hikvision.com.cn
/// @date 2024-11-08.
/// HangZhou HikVision Digital Technology Co., Ltd. All Right Reserved.
#pragma once

#include <string>

#include <DBDiffTool/common/Traits.hh>
#include <DBDiffTool/object/Schema.hh>

struct string_less {
    bool operator()(std::string_view a, std::string_view b) const {
        /// 必须与 SQL 语句 ORDER BY LENGTH(xxx), xxx 效果Same
        return a.size() < b.size() or (a.size() == b.size() and a < b);
    }
};

namespace compare {
namespace detail {
template <typename T, Lang locale = Lang::EN>
    requires shared_ptr_to_aggregate<T>
auto nameof() -> std::string {
    /// name的格式应该类似于`class std::shared_ptr<struct Table>`
    std::string name{ typeid(T).name() };
    auto const  type_beg{ name.find_last_of(' ') + 1 };
    auto const  type_end{ name.find_last_of('>') };
    name = name.substr(type_beg, type_end - type_beg);
    if constexpr (locale == Lang::CN) {
        return glb::name2cn.at(name);
    }
    return name;
}

template <typename T, Lang locale>
    requires shared_ptr_to_aggregate<T>
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
    requires shared_ptr_to_aggregate<T>
auto Absent() -> std::string {
    return detail::no_such<T, locale>();
}

template <typename T, Lang locale>
auto Present(std::string_view value) -> std::string {
    return { value.data() };
}

template <typename T>
    requires shared_ptr_to_aggregate<T>
void CompareCommon(typename std::vector<T>::const_iterator const& it_a,
                   typename std::vector<T>::const_iterator const& it_b,
                   std::stringstream&                             diff) {
    std::stringstream ss;
    ss << "\x1B[1m ==== Comparing Same-named ";
    ss << detail::nameof<T>() << ":" << (*it_a)->Name() << " ====\x1B[0m";
    auto const size_before{ ss.str().size() };
    if constexpr (std::is_same_v<T, schema_t>) {
        Compare<table_t>((*it_a)->Tbl(), (*it_b)->Tbl(), diff);
        Compare<sequence_t>((*it_a)->Seq(), (*it_b)->Seq(), diff);
        Compare<procedure_t>((*it_a)->Pro(), (*it_b)->Pro(), diff);
    } else if constexpr (std::is_same_v<T, table_t>) {
        Compare<field_t>((*it_a)->Col(), (*it_b)->Col(), diff);
        Compare<index_t>((*it_a)->Idx(), (*it_b)->Idx(), diff);
        Compare<trigger_t>((*it_a)->Tgr(), (*it_b)->Tgr(), diff);
    }
    if (ss.str().size() == size_before) {
        ss << "\tSame\n";
    }
    diff << ss.str();
}

template <typename T>
    requires shared_ptr_to_aggregate<T>
void Compare(std::vector<T> const& listA, std::vector<T> const& listB,
             std::stringstream& diff) {
    std::stringstream ss;
    ss << "\x1B[1m ==== Comparing ";
    ss << detail::nameof<T>() << "s: ==== \x1B[0m\n";
    auto const size_before{ ss.str().size() };

    auto it_a{ listA.begin() }, it_b{ listB.begin() };
    while (it_a not_eq listA.end() and it_b not_eq listB.end()) {
        std::string_view const elem_name_a{ (*it_a)->Name() };
        std::string_view const elem_name_b{ (*it_b)->Name() };
        if (elem_name_a == elem_name_b) {
            if constexpr (std::is_same_v<T, schema_t> or
                          std::is_same_v<T, table_t>) {
                CompareCommon<T>(it_a, it_b, ss); // DFS
            }
            ++it_a;
            ++it_b;
        } else if (string_less{}(elem_name_a, elem_name_b)) {
            ss << std::setw(80) << Present<T, Lang::EN>(elem_name_a);
            ss << " | " << Absent<T, Lang::EN>() << '\n';
            ++it_a;
        } else if (string_less{}(elem_name_b, elem_name_a)) {
            ss << std::setw(80) << Absent<T, Lang::EN>() << " | "
               << Present<T, Lang::EN>(elem_name_b) << '\n';
            ++it_b;
        }
    }
    while (it_a not_eq listA.end()) {
        ss << std::setw(80) << Present<T, Lang::EN>((*it_a)->Name());
        ss << " | " << Absent<T, Lang::EN>() << '\n';
        ++it_a;
    }
    while (it_b not_eq listB.end()) {
        ss << std::setw(80) << Absent<T, Lang::EN>() << " | "
           << Present<T, Lang::EN>((*it_b)->Name()) << '\n';
        ++it_b;
    }
    if (ss.str().size() == size_before) {
        ss << "\tSame\n";
    }
    diff << ss.str();
}

}