//
// DBDiffTool / Util.hh
// Created by bryant on 2024 Nov 06.
//

#ifndef DBDIFFTOOL_UTIL_HH
#define DBDIFFTOOL_UTIL_HH

#include <DBDiffTool/common/Global.hh>
#include <DBLayer/DBLayer.h>
#include <cassert>
#include <github/mcmtroffaes/inipp.hh>

enum struct Lang { CN, EN };

namespace util {
using namespace db_layer;
using ini_section = inipp::Ini<char>::Section;

namespace detail {
template <typename T, Lang locale = Lang::EN>
constexpr std::string nameof() {
    std::string_view name{typeid(T).name()};
    auto const       qualifier{name.find_first_of(' ') + 1};
    name.remove_prefix(qualifier);
    if constexpr (locale == Lang::CN) {
        return std::string{glb::name2cn.at(name)};
    }
    return std::string{name};
}

template <typename T, Lang locale>
std::string no_such() {
    if constexpr (locale == Lang::CN) {
        std::string ret{"Ã»ÓÐ´Ë¡¾"};
        ret.append(nameof<T, Lang::CN>());
        ret.append("¡¿");
        return ret;
    } else if constexpr (locale == Lang::EN) {
        std::string ret{"No such["};
        ret.append(nameof<T, Lang::EN>());
        ret.append(1, ']');
        return ret;
    }
    assert(false); // unreachable
}

void Load(ini_section const &sec, DBParam &param);
} // namespace detail

template <typename T, Lang locale>
std::string absent() {
    return detail::no_such<T, locale>();
}

template <typename T, Lang locale>
std::string present(std::string_view value) {
    return {value.data()};
}

void LoadConfig(DBParam &dsa, DBParam &dsb);

void TraverseResultSet(CConnect *conn, const std::function<void()> &func);

void SplitString(std::string_view str, std::vector<std::string> &tokens);
} // namespace util

#endif // DBDIFFTOOL_UTIL_HH
