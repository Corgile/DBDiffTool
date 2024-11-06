//
// DBDiffTool / Global.hh
// Created by bryant on 2024 Nov 06.
//

#ifndef DBDIFFTOOL_GLOBAL_HH
#define DBDIFFTOOL_GLOBAL_HH

#include <string>
#include <unordered_map>
#include <DBLayer/DBLayer.h>

namespace glb {
extern const std::unordered_map<std::string_view, std::string_view> name2cn;
extern const std::unordered_map<std::string_view, db_layer::DBType> str2enum;
extern bool heterogeneous;
}
#endif //DBDIFFTOOL_GLOBAL_HH
