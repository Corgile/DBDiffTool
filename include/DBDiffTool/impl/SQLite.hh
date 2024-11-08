//
// DBDiffTool / SQLite.hh
// Created by bryant on 2024 Nov 06.
//

#ifndef SQLITE3_HH
#define SQLITE3_HH

#include <filesystem>
#include <mutex>
#include <ranges>
#include <thread>

#include <DBLayer/DBLayer.h>

#include <DBDiffTool/common/Macros.hh>
#include <DBDiffTool/orm/orm.hh>

namespace db {
namespace impl {
namespace fs = std::filesystem;
class SQLite {
public:
    explicit SQLite(DBParam&& param) : param_{ std::move(param) } {
        InitDBFiles(param_.db_name);
    }

    static std::string_view Name() { return orm::SQLite::Name(); }

    ND std::vector<schema_t> SchemaList(orm::type const orm_type) const {
        std::vector<std::thread> threads;
        std::vector<schema_t>    schemas;
        std::mutex               mtx;
        for (auto const& [schema_name, db_path] : schema_map_) {
            threads.emplace_back([&]() -> void {
                auto schema{ DumpingTask(schema_name, db_path, orm_type) };
                std::scoped_lock guard{ mtx };
                schemas.emplace_back(std::move(schema));
            });
        }
        for (auto& thread : threads) thread.join();
        std::ranges::sort(schemas, [](schema_cref lhs, schema_cref rhs) {
            auto const &val_a{ lhs->Name() }, val_b{ rhs->Name() };
            return val_a.size() < val_b.size() or
                (val_a.size() == val_b.size() and val_a < val_b);
        });
        return schemas;
    }

    ~SQLite() = default;

private: // NOLINT
    schema_t DumpingTask(std::string_view schema_name, fs::path const& db_file,
                         orm::type const t) const {
        DBParam param{ param_ }; // make a copy for every thread.
        param.db_name = db_file.string();
        int instanceId{};
        DBLayer_Init(param, instanceId);
        auto const conn{ DBLayer_ApplyConn(instanceId) };
        auto const schema{ std::make_shared<Schema>() };
        schema->SetName(schema_name);
        // 第一级元数据
        FillTableList(conn, schema, t);
        FillSequenceList(conn, schema);  // 与schema关联
        FillProcedureList(conn, schema); // 与schema关联
        // 第二级元数据
        FillTableIndex(conn);   // 与table关联
        FillTableTrigger(conn); // 与table关联
        DBLayer_FreeConn(conn);
        return schema;
    }

private: // NOLINT
    void FillTableList(CConnect* conn, schema_cref schema, orm::type t) const {
        ENSURE_QUERY(conn, orm::SQLite::table_sql(t));
        util::TraverseResultSet(conn, [&]() -> void {
            std::string table_name{ conn->GetString(0) };
            auto&       last = schema->Emplace<Table>( //
                table_name,                      // table_name
                conn->GetString(1),              // table_fields
                conn->GetString(2),              // field_types
                conn->GetString(3)               // field_nulls
            );
            table_map_.emplace(table_name, last);
        });
    }

    void FillSequenceList(CConnect* conn, schema_cref schema) const { // NOLINT
        ENSURE_QUERY(conn, orm::SQLite::sequence_sql());
        util::TraverseResultSet(conn, [&]() -> void {
            schema->Emplace<Sequence>( //
                conn->GetString(0),    // seq_name
                conn->GetInt64(1),     // seq_min
                conn->GetInt64(2),     // seq_inc
                conn->GetInt64(3),     // seq_max
                conn->GetBool(4)       // seq_cycle
            );
        });
    }

    void FillProcedureList(CConnect*, schema_cref) const {
        // SQLite 没有存储过程
        static_cast<void>(this);
    }

    void FillTableIndex(CConnect* conn) const {
        ENSURE_QUERY(conn, orm::PostgreSQL::index_sql());
        util::TraverseResultSet(conn, [&]() -> void {
            auto const     table_name{ conn->GetString(0) };
            table_t const& table{ table_map_.at(table_name) };
            table->Emplace<Index>(  //
                conn->GetString(1), // index_name
                conn->GetString(2), // indexed_fields
                conn->GetString(3)  // index_detail
            );
        });
    }

    void FillTableTrigger(CConnect* conn) const {
        ENSURE_QUERY(conn, orm::PostgreSQL::trigger_sql());
        util::TraverseResultSet(conn, [&]() -> void {
            auto const     table_name{ conn->GetString(0) };
            table_t const& table{ table_map_.at(table_name) };
            table->Emplace<Trigger>( //
                conn->GetString(1),  // trigger_name
                conn->GetString(2)   // trigger_detail
            );
        });
    }

    void InitDBFiles(fs::path const& directory) const {
        for (auto const& entry : fs::directory_iterator{ directory }) {
            if (not entry.is_regular_file()) continue;
            if (entry.path().extension() not_eq ".db") continue;
            fs::path abs_path{ absolute(entry) };
            schema_map_.emplace(entry.path().stem().string(), abs_path);
        }
    }

private: // NOLINT
    DBParam param_{};
    /// build caches to avoid multiple queries.
    std::map<std::string, table_t> mutable table_map_{};
    /// For SQLite-like databases, a mapping from \p schema_name to \p abs_path
    /// is recommended for parallel dumping of .db files.
    std::map<std::string, fs::path> mutable schema_map_{};
};
} // namespace impl

using namespace db::impl;
} // namespace db

#endif // SQLITE3_HH
