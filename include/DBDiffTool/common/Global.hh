//
// DBDiffTool / Global.hh
// Created by bryant on 2024 Nov 06.
//

#ifndef DBDIFFTOOL_GLOBAL_HH
#define DBDIFFTOOL_GLOBAL_HH

#include <DBLayer/DBLayer.h>
#include <string>
#include <unordered_map>

namespace glb {
using DBTYPE = db_layer::DBType::Type;
extern const std::unordered_map<std::string, std::string> name2cn;
extern const std::unordered_map<std::string_view, DBTYPE> str2enum;
extern const std::unordered_map<DBTYPE, std::string_view> enum2str;

extern bool heterogeneous;
} // namespace glb
#endif // DBDIFFTOOL_GLOBAL_HH
