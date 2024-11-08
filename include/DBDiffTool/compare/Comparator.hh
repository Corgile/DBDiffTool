/// @file Comparator.hh
/// @author xianghongli\@hikvision.com.cn
/// @date 2024-11-08.
/// HangZhou HikVision Digital Technology Co., Ltd. All Right Reserved.
#pragma once

#include <string>

#include <DBDiffTool/common/AnsiColor.hh>
#include <DBDiffTool/common/Traits.hh>
#include <DBDiffTool/object/Schema.hh>

#define ABSENT_PRESENT(ss, it, l, r)                                           \
    do {                                                                       \
        ss << std::setw(16) << l << ": " << Absent<T>() << " | ";              \
        ss << r << ": " << Present<T, Lang::EN>((*it)->Name()) << '\n';        \
        ++it;                                                                  \
    } while (false)

namespace compare {
namespace detail {
template <typename T, Lang locale = Lang::EN>
    requires shared_ptr_to_aggregate<T>
auto nameof() -> std::string {
    /// name�ĸ�ʽӦ��������`class std::shared_ptr<struct Table>`
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
    requires shared_ptr_to_aggregate<T>
auto no_such() -> std::string {
    if constexpr (locale == Lang::CN) {
        std::string ret{ "û�дˡ�" };
        ret.append(nameof<T, Lang::CN>());
        ret.append("��");
        return ret;
    } else if constexpr (locale == Lang::EN) {
        std::string ret{ "No such " BLKB };
        ret.append(nameof<T, Lang::EN>());
        ret.append(RST);
        return ret;
    }
    assert(false); // unreachable
    return "unreachable";
}
} // namespace detail

template <typename T, Lang locale = Lang::EN>
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
struct Less {
    bool operator()(iterator<T> const& a, iterator<T> const& b) const {
        /// ������ SQL ��� ORDER BY LENGTH(xxx), xxx Ч��һ��
        auto const val_a{ (*a)->Key() }, val_b{ (*b)->Key() };
        return val_a.size() < val_b.size() or
            (val_a.size() == val_b.size() and val_a < val_b);
    }
};

template <typename T>
    requires shared_ptr_to_aggregate<T>
struct Equal {
    bool operator()(iterator<T> const& a, iterator<T> const& b) const {
        auto const key_a{ (*a)->Key() }, key_b{ (*b)->Key() };
        return key_a == key_b;
    }
};

template <typename T>
    requires shared_ptr_to_aggregate<T>
void CompareCommon(iterator<T> const& it_a, iterator<T> const& it_b,
                   std::stringstream& diff, std::string_view a,
                   std::string_view b, std::string prefix) {
    std::stringstream ss;
    ss << WHTB "Comparing same named " CYNB << detail::nameof<T>() << RST ":";
    std::string new_prefix{ prefix.append(1, '.').append((*it_a)->Name()) };
    ss << BOLD << new_prefix << RST << "\n";
    auto const size_before{ ss.str().size() };
    if constexpr (std::is_same_v<T, schema_t>) {
        Compare<sn::tbl_t>((*it_a)->Tbl(), (*it_b)->Tbl(), ss, a, b, prefix);
        Compare<sn::seq_t>((*it_a)->Seq(), (*it_b)->Seq(), ss, a, b, prefix);
        Compare<sn::pro_t>((*it_a)->Pro(), (*it_b)->Pro(), ss, a, b, prefix);
    } else if constexpr (std::is_same_v<T, table_t>) {
        Compare<sn::col_t>((*it_a)->Col(), (*it_b)->Col(), ss, a, b, prefix);
        Compare<sn::idx_t>((*it_a)->Idx(), (*it_b)->Idx(), ss, a, b, prefix);
        Compare<sn::tgr_t>((*it_a)->Tgr(), (*it_b)->Tgr(), ss, a, b, prefix);
    }
    if (ss.str().size() not_eq size_before) {
        diff << ss.str();
    }
}

template <typename T>
    requires shared_ptr_to_aggregate<T>
void Compare(std::vector<T> const& listA, std::vector<T> const& listB,
             std::stringstream& diff, std::string_view dsa,
             std::string_view dsb, std::string prefix = "") {
    using OBJ = typename T::template element_type;
    std::stringstream ss;
    ss << OBJ::prefix() << prefix << "." << detail::nameof<T>() << ":\n";
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
            ss << std::setw(16) << dsb << ": " << Absent<T>() << " | ";
            ss << dsa << ": " << Present<T, Lang::EN>((*it_a)->Name()) << '\n';
            ++it_a;
        } else if (Less<T>{}(it_b, it_a)) {
            ss << std::setw(16) << dsa << ": " << Absent<T>() << " | ";
            ss << dsb << ": " << Present<T, Lang::EN>((*it_b)->Name()) << '\n';
            ++it_b;
        }
    }
    while (it_a not_eq listA.end()) {
        ss << std::setw(16) << dsb << ": " << Absent<T>() << " | ";
        ss << dsa << ": " << Present<T, Lang::EN>((*it_a)->Name()) << '\n';
        ++it_a;
    }
    while (it_b not_eq listB.end()) {
        ss << std::setw(16) << dsa << ": " << Absent<T>() << " | ";
        ss << dsb << ": " << Present<T, Lang::EN>((*it_b)->Name()) << '\n';
        ++it_b;
    }
    if (ss.str().size() not_eq size_before) {
        diff << ss.str();
        diff << OBJ::prefix() << "------------------------------------------\n";
    }
}

}
