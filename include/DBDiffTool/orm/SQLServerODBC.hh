/// @file SQLServerODBC.hh
/// @author xianghongli\@hikvision.com.cn
/// @date 2024-11-10.
/// HangZhou HikVision Digital Technology Co., Ltd. All Right Reserved.
#pragma once

#include <string>

#include <DBDiffTool/orm/Type.hh>

namespace orm {
struct SQLServerODBC {
    static std::string_view Name() {
        static std::string name{ "SQLServerODBC" };
        return name;
    }
    /// 第一级元数据 - schema_table / schema_view
    static std::string const& schema_sql(type const t) {
        static const std::string table{ R"(SELECT 1;)" };
        static const std::string view{ R"(SELECT 1;)" };
        if (t == type::table) return table;
        if (t == type::view) return view;
        return table; // unreachable.
    }

    /// 第一级元数据 - schema_sequence
    static std::string const& sequence_sql() {
        static std::string sql{ R"(SELECT 1;)" };
        return sql;
    }

    /// 第一级元数据 - schema_procedure
    static std::string const& procedure_sql() {
        static std::string sql{ R"(SELECT 1;)" };
        return sql;
    }

    /// 第二级元数据 - 索引
    static std::string const& index_sql() {
        static std::string sql{ R"(SELECT 1;)" };
        return sql;
    }

    /// 第二级元数据 - 触发器
    static std::string const& trigger_sql() {
        static std::string sql{ R"(SELECT 1;)" };
        return sql;
    }
};
} // namespace orm
