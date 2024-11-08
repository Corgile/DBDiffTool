//
// DBDiffTool / Util.hh
// Created by bryant on 2024 Nov 06.
//

#ifndef DBDIFFTOOL_UTIL_HH
#define DBDIFFTOOL_UTIL_HH

#include <cassert>
#include <filesystem>

#include <DBDiffTool/common/Global.hh>
#include <DBLayer/DBLayer.h>
#include <github/mcmtroffaes/inipp.hh>

enum struct Lang { CN, EN };

namespace util {
using namespace db_layer;
namespace fs      = std::filesystem;
using ini_section = inipp::Ini<char>::Section;

namespace detail {
template <typename T, Lang locale = Lang::EN>
constexpr auto nameof() -> std::string {
    std::string_view name{ typeid(T).name() };
    auto const       qualifier{ name.find_first_of(' ') + 1 };
    name.remove_prefix(qualifier);
    if constexpr (locale == Lang::CN) {
        return std::string{ glb::name2cn.at(name) };
    }
    return std::string{ name };
}

template <typename T, Lang locale>
auto no_such() -> std::string {
    if constexpr (locale == Lang::CN) {
        std::string ret{ "Ã»ÓÐ´Ë¡¾" };
        ret.append(nameof<T, Lang::CN>());
        ret.append("¡¿");
        return ret;
    } else if constexpr (locale == Lang::EN) {
        std::string ret{ "No such[" };
        ret.append(nameof<T, Lang::EN>());
        ret.append(1, ']');
        return ret;
    }
    assert(false); // unreachable
    return "unreachable";
}

void Load(ini_section const& sec, DBParam& param);
} // namespace detail

template <typename T, Lang locale>
auto Absent() -> std::string {
    return detail::no_such<T, locale>();
}

template <typename T, Lang locale>
auto Present(std::string_view value) -> std::string {
    return { value.data() };
}

void LoadConfig(DBParam& dsa, DBParam& dsb);

void TraverseResultSet(CConnect* conn, std::function<void()> const& func);

void SplitString(std::string_view str, std::vector<std::string>& tokens);
} // namespace util

#endif // DBDIFFTOOL_UTIL_HH
