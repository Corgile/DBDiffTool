/// @file Comparator.hh
/// @author xianghongli\@hikvision.com.cn
/// @date 2024-11-08.
/// HangZhou HikVision Digital Technology Co., Ltd. All Right Reserved.
#pragma once

#include <string>

#include <DBDiffTool/common/common.hh>
#include <DBDiffTool/object/object.hh>

#define ABSENT_PRESENT(ss, it, l, r)                                           \
    do {                                                                       \
        ss << REDB << std::setw(16) << l << RST ": " << Absent<T>() << " | ";  \
        ss << GRNB << r << RST ": " << Present<T, Lang::EN>((*it)->Name())     \
           << '\n';                                                            \
        ++it;                                                                  \
    } while (false)

namespace compare {
namespace detail {
template <typename T, Lang locale = Lang::CN>
    requires sharedptr_to_aggregate<T>
auto nameof() -> std::string {
    /// name的格式应该类似于`class std::shared_ptr<struct Table>`
    std::string name{ typeid(T).name() };
    auto const  type_beg{ name.find_last_of(' ') + 1 };
    auto const  type_end{ name.find_last_of('>') };
    name = name.substr(type_beg, type_end - type_beg);
    if constexpr (locale == Lang::CN) {
        name = glb::name2cn.at(name);
    }
    return name;
}

template <typename T, Lang locale>
    requires sharedptr_to_aggregate<T>
auto no_such() -> std::string {
    std::string       ret{};
    std::string const name{ nameof<T, locale>() };
    if constexpr (locale == Lang::CN) {
        ret.append("没有此[" YLWB);
    } else if constexpr (locale == Lang::EN) {
        ret.append("No such [" YLWB);
    }
    ret.append(name);
    ret.append(RST "]");
    return ret;
}
} // namespace detail

template <typename T, Lang locale = Lang::EN>
    requires sharedptr_to_aggregate<T>
auto Absent() -> std::string {
    return detail::no_such<T, locale>();
}

template <typename T, Lang locale> // NOLINT
auto Present(std::string_view value) -> std::string_view {
    return value; // NOLINT
}

template <typename T>
    requires sharedptr_to_aggregate<T>
struct Less {
    bool operator()(iterator<T> const& a, iterator<T> const& b) const {
        /// 必须与 SQL 语句 ORDER BY LENGTH(xxx), xxx 效果一致
        auto const val_a{ (*a)->Key() };
        auto const val_b{ (*b)->Key() };
        return val_a.size() < val_b.size() or
            (val_a.size() == val_b.size() and val_a < val_b);
    }
};

template <typename T>
    requires sharedptr_to_aggregate<T>
struct Equal {
    bool operator()(iterator<T> const& a, iterator<T> const& b) const {
        auto const key_a{ (*a)->Key() };
        auto const key_b{ (*b)->Key() };
        return key_a == key_b;
    }
};

template <typename T>
    requires sharedptr_to_aggregate<T>
void Compare(std::vector<T> const& listA, std::vector<T> const& listB,
             std::stringstream& diff, std::string_view dsa,
             std::string_view dsb, std::string_view prefix = "");

template <typename T>
    requires sharedptr_to_aggregate<T>
void CompareCommon(iterator<T> const& it_a, iterator<T> const& it_b,
                   std::stringstream& diff, std::string_view dsa,
                   std::string_view dsb, std::string_view pfx) {
    std::stringstream ss;
    ss << WHTB "Comparing same named " CYNB << detail::nameof<T, Lang::EN>()
       << RST ":";
    ss << BOLD << pfx << '.' << (*it_a)->Name() << RST << "\n";
    auto const size_before{ ss.str().size() };
    if constexpr (std::is_same_v<T, schema_t>) {
        Compare<sn::tbl_t>((*it_a)->Tbl(), (*it_b)->Tbl(), ss, dsa, dsb, pfx);
        Compare<sn::seq_t>((*it_a)->Seq(), (*it_b)->Seq(), ss, dsa, dsb, pfx);
        Compare<sn::pro_t>((*it_a)->Pro(), (*it_b)->Pro(), ss, dsa, dsb, pfx);
    } else if constexpr (std::is_same_v<T, table_t>) {
        Compare<sn::col_t>((*it_a)->Col(), (*it_b)->Col(), ss, dsa, dsb, pfx);
        Compare<sn::idx_t>((*it_a)->Idx(), (*it_b)->Idx(), ss, dsa, dsb, pfx);
        Compare<sn::tgr_t>((*it_a)->Tgr(), (*it_b)->Tgr(), ss, dsa, dsb, pfx);
    }
    if (ss.str().size() not_eq size_before) {
        diff << ss.str();
    }
}

template <typename T>
    requires sharedptr_to_aggregate<T>
void Compare(std::vector<T> const& listA, std::vector<T> const& listB,
             std::stringstream& diff, std::string_view dsa,
             std::string_view dsb, std::string_view prefix) {
    using OBJ = typename T::template element_type;
    std::stringstream ss;
    ss << OBJ::prefix() << prefix << "." << BLUB
       << detail::nameof<T, Lang::EN>() << RST ":\n";
    auto const size_before{ ss.str().size() };
    auto       it_a{ listA.begin() }, it_b{ listB.begin() };
    while (it_a not_eq listA.end() and it_b not_eq listB.end()) {
        if (Equal<T>{}(it_a, it_b)) {
            if constexpr (std::is_same_v<T, sn::scm_t> or
                          std::is_same_v<T, sn::tbl_t>) {
                CompareCommon<T>(it_a, it_b, ss, dsa, dsb, prefix); // DFS
            }
            ++it_a;
            ++it_b;
        } else if (Less<T>{}(it_a, it_b)) {
            ABSENT_PRESENT(ss, it_a, dsb, dsa);
        } else if (Less<T>{}(it_b, it_a)) {
            ABSENT_PRESENT(ss, it_b, dsa, dsb);
        }
    }
    // clang-format off
    while (it_a not_eq listA.end()) { ABSENT_PRESENT(ss, it_a, dsb, dsa); }
    while (it_b not_eq listB.end()) { ABSENT_PRESENT(ss, it_b, dsa, dsb); }
    // clang-format on
    if (ss.str().size() not_eq size_before) {
        diff << ss.str();
        diff << OBJ::prefix() << "------------------------------------------\n";
    }
}

} // namespace compare
#undef ABSENT_PRESENT