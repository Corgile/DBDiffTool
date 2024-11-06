//
// DBDiffTool / PostgreSql.hh
// Created by bryant on 2024 Nov 06.
//

#ifndef POSTGRESQL_HH
#define POSTGRESQL_HH
#include <DBDiffTool/common/Macros.hh>

namespace db {
namespace impl {
class PostgreSQL {
public:
    explicit PostgreSQL(DBParam &&param) : param_{std::move(param)} {
        DBLayer_Init(param_, instanceId_);
        connect_ = DBLayer_ApplyConn(instanceId_);
    }

    ~PostgreSQL() = default;

    void SetParam(DBParam param) { param_ = std::move(param); }

    static std::string_view Name() { return orm::PostgreSQL::Name(); }

    ND std::vector<Schema> SchemaList() const {
        std::vector<Schema> schemas;
        ENSURE_QUERY(connect_, orm::PostgreSQL::all_schema());
        Schema      schema;
        std::string curr_schema{connect_->GetString(0)};
        util::TraverseResultSet(connect_, [&]() -> void {
            auto const schema_name{connect_->GetString(0)};
            if (curr_schema not_eq schema_name) {
                schemas.emplace_back(schema);
                schema_map_.emplace(schema_name, schemas.back());
                curr_schema = schema_name;
            }
            Table table{
                connect_->GetString(0), // schema
                connect_->GetString(1), // table
                connect_->GetString(2), // fields
                connect_->GetString(3), // types
                connect_->GetString(4), // nulls
            };
            schema.AddTable(std::move(table));
        });
        return schemas;
    }

private:
    int       instanceId_{};
    bool      initialized_{};
    DBParam   param_;
    CConnect *connect_;

    std::map<std::string, Schema &> schema_map_;
};
} // namespace impl

using namespace db::impl;
} // namespace db

#endif // POSTGRESQL_HH
