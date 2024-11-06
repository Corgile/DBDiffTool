//
// DBDiffTool / SqLite.hh
// Created by bryant on 2024 Nov 06.
//

#ifndef DBDIFFTOOL_SQLITE_HH
#define DBDIFFTOOL_SQLITE_HH

#include <string>

namespace orm {
struct SQLite {
    static std::string_view Name() {
        static std::string name{"SQLite"};
        return name;
    }

    /// all schemas
    static std::string_view all_schema() {
        static const std::string sql{"SELECT 1;"};
        return sql;
    }

    /// table
    static std::string_view all_tables() {
        static const std::string sql{"SELECT 1;"};
        return sql;
    }
};
}

#endif //DBDIFFTOOL_SQLITE_HH
