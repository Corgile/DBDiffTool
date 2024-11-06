//
// DBDiffTool / PostgreSql.hh
// Created by bryant on 2024 Nov 06.
//

#ifndef DBDIFFTOOL_POSTGRESQL_HH
#define DBDIFFTOOL_POSTGRESQL_HH

#include <string>

struct PostgreSQL {
    /// all schemas
    static std::string const& all_schema() {
        static const std::string sql{"SELECT 1;"};
        return sql;
    }

    /// table
    static std::string const& all_tables() {
        static const std::string sql{"SELECT 1;"};
        return sql;
    }
};

#endif //DBDIFFTOOL_POSTGRESQL_HH
