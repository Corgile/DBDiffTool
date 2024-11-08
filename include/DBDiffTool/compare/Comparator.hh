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
        /// 必须与 SQL 语句 ORDER BY LENGTH(xxx), xxx 效果一致
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
        name = glb::name2cn.at(name);
    }
    return "\x1B[34;1m" + name + "\x1B[0m";
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
using iterator = typename std::vector<T>::const_iterator;

template <typename T>
    requires shared_ptr_to_aggregate<T>
void CompareCommon(iterator<T> const& it_a, iterator<T> const& it_b,
                   std::stringstream& diff, std::string prefix) {
    std::stringstream ss;
    ss << "Comparing same named " << detail::nameof<T>() << ": \x1B[1m";
    std::string new_prefix{ prefix.append(1, '.').append((*it_a)->Name()) };
    ss << new_prefix << "\x1B[0m\n";
    auto const size_before{ ss.str().size() };
    if constexpr (std::is_same_v<T, schema_t>) {
        Compare<sn::tbl_t>((*it_a)->Tbl(), (*it_b)->Tbl(), ss, prefix);
        Compare<sn::seq_t>((*it_a)->Seq(), (*it_b)->Seq(), ss, prefix);
        Compare<sn::pro_t>((*it_a)->Pro(), (*it_b)->Pro(), ss, prefix);
    } else if constexpr (std::is_same_v<T, table_t>) {
        Compare<sn::col_t>((*it_a)->Col(), (*it_b)->Col(), ss, prefix);
        Compare<sn::idx_t>((*it_a)->Idx(), (*it_b)->Idx(), ss, prefix);
        Compare<sn::tgr_t>((*it_a)->Tgr(), (*it_b)->Tgr(), ss, prefix);
    }
    if (ss.str().size() not_eq size_before) {
        diff << ss.str();
    }
}

template <typename T>
    requires shared_ptr_to_aggregate<T>
void Compare(std::vector<T> const& listA, std::vector<T> const& listB,
             std::stringstream& diff, std::string prefix = "") {
    using OBJ = typename T::template element_type;
    std::stringstream ss;
    ss << T::element_type::prefix() << prefix << "." << detail::nameof<T>()
       << ":\n";
    auto const size_before{ ss.str().size() };
    auto       it_a{ listA.begin() }, it_b{ listB.begin() };
    while (it_a not_eq listA.end() and it_b not_eq listB.end()) {
        std::string_view const elem_name_a{ (*it_a)->Name() };
        std::string_view const elem_name_b{ (*it_b)->Name() };
        if (elem_name_a == elem_name_b) {
            if constexpr (std::is_same_v<T, sn::scm_t> or
                          std::is_same_v<T, sn::tbl_t>) {
                CompareCommon<T>(it_a, it_b, ss, prefix); // DFS
            }
            ++it_a;
            ++it_b;
        } else if (string_less{}(elem_name_a, elem_name_b)) {
            ss << std::setw(10 + OBJ::w()) << Present<T, Lang::EN>(elem_name_a);
            ss << " | " << Absent<T, Lang::EN>() << '\n';
            ++it_a;
        } else if (string_less{}(elem_name_b, elem_name_a)) {
            ss << std::setw(84) << Absent<T, Lang::EN>() << " | "
               << Present<T, Lang::EN>(elem_name_b) << '\n';
            ++it_b;
        }
    }
    while (it_a not_eq listA.end()) {
        ss << std::setw(10 + OBJ::w()) << Present<T, Lang::EN>((*it_a)->Name());
        ss << " | " << Absent<T, Lang::EN>() << '\n';
        ++it_a;
    }
    while (it_b not_eq listB.end()) {
        ss << std::setw(84) << Absent<T, Lang::EN>() << " | "
           << Present<T, Lang::EN>((*it_b)->Name()) << '\n';
        ++it_b;
    }
    if (ss.str().size() not_eq size_before) {
        diff << ss.str();
        diff << OBJ::prefix() << "------------------------------------------\n";
    }
}

}
