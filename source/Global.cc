﻿/// @file Global.cc
/// @author xianghongli\@hikvision.com.cn
/// @date 2024-11-06.
/// HangZhou HikVision Digital Technology Co., Ltd. All Right Reserved.
//
#include <DBDiffTool/common/Global.hh>

namespace glb {
const std::unordered_map<std::string, std::string> name2cn{
    { "Index", "索引" },         { "Table", "表格" },
    { "Column", "字段" },        { "Schema", "模式" },
    { "Trigger", "触发器" },     { "Sequence", "序列" },
    { "Procedure", "存储过程" },
};

const std::unordered_map<std::string_view, DBTYPE> str2enum{
    { "", db_layer::DBType::Null },
    { "Null", db_layer::DBType::Null },
    { "SQLite", db_layer::DBType::SQLite },
    { "PostgreSQL", db_layer::DBType::PostgreSQL },
    { "SQLServer", db_layer::DBType::SQLServer },
    { "MySQL", db_layer::DBType::MySQL },
    { "Oracle", db_layer::DBType::Oracle },
    { "SQLServerODBC", db_layer::DBType::SQLServerODBC },
    { "SQLCipher3", db_layer::DBType::SQLCipher3 },
};

const std::unordered_map<DBTYPE, std::string_view> enum2str{
    { db_layer::DBType::Null, "Null" },
    { db_layer::DBType::SQLite, "SQLite" },
    { db_layer::DBType::PostgreSQL, "PostgreSQL" },
    { db_layer::DBType::SQLServer, "SQLServer" },
    { db_layer::DBType::MySQL, "MySQL" },
    { db_layer::DBType::Oracle, "Oracle" },
    { db_layer::DBType::SQLServerODBC, "SQLServerODBC" },
    { db_layer::DBType::SQLCipher3, "SQLCipher3" },
};

bool heterogeneous{false};
}
