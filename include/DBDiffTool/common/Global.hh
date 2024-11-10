/// @file Global.hh
/// @author xianghongli\@hikvision.com.cn
/// @date 2024-11-06.
/// HangZhou HikVision Digital Technology Co., Ltd. All Right Reserved.
#pragma once

#include <string>
#include <unordered_map>

#include <DBLayer/DBLayer.h>

namespace glb {
using DBTYPE = db_layer::DBType::Type;
extern const std::unordered_map<std::string, std::string> name2cn;
extern const std::unordered_map<std::string_view, DBTYPE> str2enum;
extern const std::unordered_map<DBTYPE, std::string_view> enum2str;

extern bool heterogeneous;
} // namespace glb
