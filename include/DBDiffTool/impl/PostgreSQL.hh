/// @file PostgreSQL.hh
/// @author xianghongli\@hikvision.com.cn
/// @date 2024-11-06.
/// HangZhou HikVision Digital Technology Co., Ltd. All Right Reserved.
#pragma once

#include <DBLayer/DBLayer.h>

#include <DBDiffTool/common/Macros.hh>
#include <DBDiffTool/orm/orm.hh>

namespace db {
namespace impl {
class PostgreSQL {
public:
    explicit PostgreSQL(DBParam&& param) : param_{ std::move(param) } {
        DBLayer_Init(param_);
        Module_Register(param_, INSTANCE(PostgreSQL));
        connect_ = DBLayer_ApplyConn(INSTANCE(PostgreSQL));
    }

    static std::string_view Name() { return orm::PostgreSQL::Name(); }

    ND std::vector<schema_t> SchemaList(orm::type const t) const {
        std::vector<schema_t> schemas;
        // 第一级元数据
        FillTableList(schemas, t); // 与schema关联
        FillSequenceList();        // 与schema关联
        FillProcedureList();       // 与schema关联
        // 第二级元数据
        FillTableIndex();   // 与table关联
        FillTableTrigger(); // 与table关联
        return schemas;
    }

    ~PostgreSQL() { DBLayer_FreeConn(connect_); }

private:
    void FillTableList(std::vector<schema_t>& schemas,
                       orm::type const        orm_type) const {
        ENSURE_QUERY(connect_, orm::PostgreSQL::schema_sql(orm_type));
        schema_t    schema{ std::make_unique<Schema>() };
        std::string last_schema{ connect_->GetString(0) };
        util::TraverseResultSet(connect_, [&]() -> void {
            auto const curr_schema{ connect_->GetString(0) };
            if (curr_schema not_eq last_schema) {
                schema->SetName(last_schema);
                auto& last{ schemas.emplace_back(std::move(schema)) };
                schema = std::make_shared<Schema>();
                scm_map_.emplace(last_schema, last);
                last_schema = curr_schema;
            }
            auto const table_name{ connect_->GetString(1) };

            auto& last = schema->Emplace<Table>( //
                table_name,                      // table_name
                connect_->GetString(2),          // table_fields
                connect_->GetString(3),          // field_types
                connect_->GetString(4)           // field_nulls
            );
            tbl_map_.emplace(table_name, last);
        });
    }

    void FillSequenceList() const {
        ENSURE_QUERY(connect_, orm::PostgreSQL::sequence_sql());
        util::TraverseResultSet(connect_, [&]() -> void {
            auto const  curr_schema{ connect_->GetString(0) };
            auto const& schema{ scm_map_.at(curr_schema) };
            schema->Emplace<Sequence>(  //
                connect_->GetString(1), // seq_name
                connect_->GetInt64(2),  // seq_min
                connect_->GetInt64(3),  // seq_inc
                connect_->GetInt64(4),  // seq_max
                connect_->GetBool(5)    // seq_cycle
            );
        });
    }

    void FillProcedureList() const {
        ENSURE_QUERY(connect_, orm::PostgreSQL::procedure_sql());
        util::TraverseResultSet(connect_, [&]() -> void {
            auto const  curr_schema{ connect_->GetString(0) };
            auto const& schema{ scm_map_.at(curr_schema) };
            schema->Emplace<Procedure>( //
                connect_->GetString(1), // procedure_name
                connect_->GetString(2)  // procedure_md5
            );
        });
    }

    void FillTableIndex() const {
        ENSURE_QUERY(connect_, orm::PostgreSQL::index_sql());
        util::TraverseResultSet(connect_, [&]() -> void {
            auto const     table_name{ connect_->GetString(0) };
            table_t const& table{ tbl_map_.at(table_name) };
            table->Emplace<Index>(      //
                connect_->GetString(1), // index_name
                connect_->GetString(2), // indexed_fields
                connect_->GetString(3)  // index_md5
            );
        });
    }

    void FillTableTrigger() const {
        ENSURE_QUERY(connect_, orm::PostgreSQL::trigger_sql());
        util::TraverseResultSet(connect_, [&]() -> void {
            auto const     table_name{ connect_->GetString(0) };
            table_t const& table{ tbl_map_.at(table_name) };
            table->Emplace<Trigger>(    //
                connect_->GetString(1), // trigger_name
                connect_->GetString(2)  // trigger_md5
            );
        });
    }

private: // NOLINT
    DBParam   param_{};
    CConnect* connect_{};

    std::map<std::string, sn::tbl_t> mutable tbl_map_{};
    std::map<std::string, sn::scm_t> mutable scm_map_{};
};
} // namespace impl

using namespace db::impl;
} // namespace db

