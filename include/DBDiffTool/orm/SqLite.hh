//
// DBDiffTool / SqLite.hh
// Created by bryant on 2024 Nov 06.
//

#ifndef DBDIFFTOOL_SQLITE_HH
#define DBDIFFTOOL_SQLITE_HH

#include <string>

#include <DBDiffTool/orm/Type.hh>

namespace orm {
struct SQLite {
    static std::string_view Name() {
        static std::string name{ "SQLite" };
        return name;
    }
    /// 第一级元数据 - schema, sqlite的话直接跳过
    static std::string const& schema_sql(type const t) {
        static std::string sql{ "SELECT 1;" };
        return sql;
    }

    /// 第一级元数据 - schema_sequence
    static std::string const& sequence_sql() {
        static std::string sql{ R"(
SELECT name AS seq_name, 1 AS seq_min, 1 AS seq_incr,
       CAST(0x7FFFFFFFFFFFFFFF AS INTEGER) AS seq_max, "false" AS cycle
FROM sqlite_sequence ORDER BY LENGTH(name),name;)" };
        return sql;
    }

    /// 第一级元数据 - schema_procedure
    static std::string const& procedure_sql() {
        static std::string sql{ R"(SELECT 1;)" };
        return sql;
    }

    /// 第二级元数据 - 索引
    static std::string const& index_sql() {
        static std::string sql{
            R"(
SELECT
  tbl_name AS table_name, name AS index_name,
  REPLACE(SUBSTR(sql, INSTR(sql, '(') + 1, INSTR(sql, ')')
  - INSTR(sql, '(') - 1), ' ', ', ') AS indexed_fields,
  LOWER(sql) AS index_detail
FROM sqlite_master WHERE type = 'index' AND name NOT LIKE 'sqlite_%'
ORDER BY LENGTH(tbl_name), tbl_name, LENGTH(name),name;)"
        };
        return sql;
    }

    /// 第二级元数据 - 触发器
    static std::string const& trigger_sql() {
        static std::string sql{ R"(
SELECT tbl_name AS table_name, name AS trigger_name, sql AS trigger_detail
FROM sqlite_master WHERE type = 'trigger'
ORDER BY LENGTH(tbl_name), tbl_name, LENGTH(name),name;)" };
        return sql;
    }
};
}

#endif //DBDIFFTOOL_SQLITE_HH
