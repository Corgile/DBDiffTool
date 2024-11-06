//
// DBDiffTool / SqLite.hh
// Created by bryant on 2024 Nov 06.
//

#ifndef DBDIFFTOOL_SQLITE_HH
#define DBDIFFTOOL_SQLITE_HH

#include <string>

struct SQLite {
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

#endif //DBDIFFTOOL_SQLITE_HH
