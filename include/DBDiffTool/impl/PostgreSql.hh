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
    explicit PostgreSQL(DBParam&& param) : param_{ std::move(param) } {
        DBLayer_Init(param_, instanceId_);
        connect_ = DBLayer_ApplyConn(instanceId_);
    }

    static std::string_view Name() { return orm::PostgreSQL::Name(); }

    ND std::vector<Schema> SchemaList(orm::type const t) const {
        std::vector<Schema> schemas;
        // 第一级元数据
        FillTableList(schemas, t);  // 与schema关联
        FillSequenceList(schemas);  // 与schema关联
        FillProcedureList(schemas); // 与schema关联
        // 下一级元数据
        FillTableIndex(); // 与table关联
        return schemas;
    }

    // TODO 别Toto了 不打算做.
    std::shared_ptr<Node> Tree() const { return {}; } // NOLINT

    ~PostgreSQL() { DBLayer_FreeConn(connect_); }

private:
    void FillTableList(std::vector<Schema>& schemas, orm::type const t) const {
        ENSURE_QUERY(connect_, orm::PostgreSQL::schema_list(t));
        Schema      schema;
        int         schema_idx{ 0 };
        std::string last_schema{ connect_->GetString(0) };
        util::TraverseResultSet(connect_, [&]() -> void {
            auto const curr_schema{ connect_->GetString(0) };
            if (curr_schema not_eq last_schema) {
                schemas.emplace_back(std::move(schema));
                schema_map_.emplace(last_schema, schema_idx++);
                last_schema = curr_schema;
            }
            schema.EmplaceTable(        //
                connect_->GetString(0), // schema_name
                connect_->GetString(1), // table_name
                connect_->GetString(2), // table_fields
                connect_->GetString(3), // field_types
                connect_->GetString(4)  // field_nulls
            );
            table_map_.emplace(connect_->GetString(1), schema.UnCachedTable());
        });
    }

    void FillSequenceList(std::vector<Schema>& schemas) const {
        ENSURE_QUERY(connect_, orm::PostgreSQL::sequence_sql());
        util::TraverseResultSet(connect_, [&]() -> void {
            auto const curr_schema{ connect_->GetString(0) };
            Schema&    schema{ schemas.at(schema_map_.at(curr_schema)) };
            schema.EmplaceSequence(     //
                connect_->GetString(1), // seq_name
                connect_->GetInt64(2),  // seq_min
                connect_->GetInt64(3),  // seq_inc
                connect_->GetInt64(4),  // seq_max
                connect_->GetBool(5)    // seq_cycle
            );
        });
    }

    void FillProcedureList(std::vector<Schema>& schemas) const {
        ENSURE_QUERY(connect_, orm::PostgreSQL::procedure_sql());
        util::TraverseResultSet(connect_, [&]() -> void {
            auto const curr_schema{ connect_->GetString(0) };
            Schema&    schema{ schemas.at(schema_map_.at(curr_schema)) };
            schema.EmplaceProcedure(    //
                connect_->GetString(1), // procedure_name
                connect_->GetString(2)   // procedure_md5
            );
        });
    }

    void FillTableIndex() const {
        ENSURE_QUERY(connect_, orm::PostgreSQL::index_sql());
        util::TraverseResultSet(connect_, [&]() -> void {
            auto const table_name{ connect_->GetString(0) };
            Table&     table{ table_map_.at(table_name) };
            table.EmplaceIndex(         //
                connect_->GetString(1), // index_name
                connect_->GetString(2), // indexed_fields
                connect_->GetString(3)  // index_md5
            );
        });
    }

private: // NOLINT
    int       instanceId_{};
    bool      initialized_{};
    DBParam   param_;
    CConnect* connect_;

    std::map<std::string, int> mutable schema_map_;
    std::map<std::string, Table&> mutable table_map_;
};
} // namespace impl

using namespace db::impl;
} // namespace db

#endif // POSTGRESQL_HH
