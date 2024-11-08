//
// DBDiffTool / Util.hh
// Created by bryant on 2024 Nov 06.
//

#ifndef DBDIFFTOOL_UTIL_HH
#define DBDIFFTOOL_UTIL_HH

#include <cassert>
#include <filesystem>
#include <vector>

#include <DBLayer/DBLayer.h>

#include <DBDiffTool/common/Global.hh>
#include <github/mcmtroffaes/inipp.hh>

enum struct Lang { CN, EN };

namespace util {
using namespace db_layer;
namespace fs      = std::filesystem;
using ini_section = inipp::Ini<char>::Section;

namespace detail {
void Load(ini_section const& sec, DBParam& param);
} // namespace detail

void LoadConfig(DBParam& dsa, DBParam& dsb);

void TraverseResultSet(CConnect* conn, std::function<void()> const& func);

void SplitString(std::string_view str, std::vector<std::string>& tokens);

} // namespace util

#endif // DBDIFFTOOL_UTIL_HH
