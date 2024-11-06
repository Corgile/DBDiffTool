//
// DBDiffTool / Global.cc
// Created by bryant on 2024 Nov 06.
//
#include <DBDiffTool/common/Global.hh>

namespace glb {
const std::unordered_map<std::string_view, std::string_view> name2cn{
    {"Index", "索引"},
    {"Table", "表格"},
    {"Column", "字段"},
    {"Schema", "模式"},
    {"Trigger", "触发器"},
    {"Sequence", "序列"},
    {"Procedure", "存储过程"},
};

const std::unordered_map<std::string_view, db_layer::DBType> str2enum{
    {"MYSQL", db_layer::DBType::MYSQL},
    {"POSTGRESQL", db_layer::DBType::POSTGRESQL},
    {"ORACLE", db_layer::DBType::ORACLE},
    {"SQLITE3", db_layer::DBType::SQLITE3},
    {"SQLCIPHER", db_layer::DBType::SQLCIPHER},
    {"SQLSERVER", db_layer::DBType::SQLSERVER},
};

bool heterogeneous{false};
}
