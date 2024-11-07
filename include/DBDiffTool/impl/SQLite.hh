//
// DBDiffTool / SQLite.hh
// Created by bryant on 2024 Nov 06.
//

#ifndef SQLITE3_HH
#define SQLITE3_HH

#include <DBDiffTool/common/Macros.hh>
#include <DBDiffTool/orm/Type.hh>

namespace db {
namespace impl {
class SQLite {
public:
    explicit SQLite(DBParam&& param) : param_{ std::move(param) } {}

    static std::string_view Name() { return orm::SQLite::Name(); }

    ND std::vector<schema_t> SchemaList(orm::type const t) const {
        std::vector<schema_t> schemas;
        // ��һ��Ԫ����
        FillTableList(schemas, t);  // ��schema����
        FillSequenceList(schemas);  // ��schema����
        FillProcedureList(schemas); // ��schema����
        // �ڶ���Ԫ����
        FillTableIndex();   // ��table����
        FillTableTrigger(); // ��table����
        return schemas;
    }

    ~SQLite() = default;

private:
    void FillTableList(std::vector<schema_t>& schemas, orm::type t) const {}

    void FillSequenceList(std::vector<schema_t> const& schemas) const {}

    void FillProcedureList(std::vector<schema_t> const&) const {
        static_cast<void>(this);
    }

    void FillTableIndex() const {}

    void FillTableTrigger() const {}

private: // NOLINT
    DBParam param_{};

    std::map<std::string, table_t> mutable table_map_{};
    std::map<std::string, schema_t> mutable schema_map_{};
};
} // namespace impl

using namespace db::impl;
} // namespace db

#endif // SQLITE3_HH
