/// @file Global.hh
/// @author xianghongli\@hikvision.com.cn
/// @date 2024-11-06.
/// HangZhou HikVision Digital Technology Co., Ltd. All Right Reserved.
#pragma once

#include <filesystem>
#include <functional>
#include <vector>

#include <DBLayer/DBLayer.h>

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
