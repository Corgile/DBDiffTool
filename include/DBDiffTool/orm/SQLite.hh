/// @file SQLite.hh
/// @author xianghongli\@hikvision.com.cn
/// @date 2024-11-06.
/// HangZhou HikVision Digital Technology Co., Ltd. All Right Reserved.
#pragma once

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

    /// 第一级元数据 - 表/列
    static std::string const& table_sql(type const t) {
        static std::string table{ R"(
SELECT
    m.name AS table_name,
    GROUP_CONCAT(i.name, ',' ORDER BY LENGTH(i.name),i.name) AS table_fields,
    GROUP_CONCAT(i.type, ',' ORDER BY LENGTH(i.name),i.name) AS field_types,
    GROUP_CONCAT(CASE WHEN i."notnull" = 0 THEN 'YES'
    ELSE 'NO' END, ',' ORDER BY LENGTH(i.name),i.name) AS field_nulls
FROM sqlite_master AS m
JOIN pragma_table_info(m.name) AS i
WHERE m.type = 'table' AND m.name NOT LIKE 'sqlite_%'
GROUP BY m.name ORDER BY LENGTH(m.name),m.name;)" };
        static std::string view{ R"(
SELECT
    m.name AS table_name,
    GROUP_CONCAT(i.name, ',' ORDER BY i.name) AS table_fields,
    GROUP_CONCAT(i.type, ',' ORDER BY i.name) AS field_types,
    GROUP_CONCAT(CASE WHEN i."notnull" = 0 THEN 'YES'
    ELSE 'NO' END, ',' ORDER BY i.name) AS field_nulls
FROM sqlite_master AS m
JOIN pragma_table_info(m.name) AS i
WHERE m.type = 'view' AND m.name NOT LIKE 'sqlite_%'
GROUP BY m.name ORDER BY LENGTH(m.name),m.name;)" };
        if (t == type::view) return view;
        return table;
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
