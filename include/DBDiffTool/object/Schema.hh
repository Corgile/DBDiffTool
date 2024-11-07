//
// DBDiffTool / Schema.hh
// Created by bryant on 2024 Nov 06.
//

#ifndef DBDIFFTOOL_SCHEMA_HH
#define DBDIFFTOOL_SCHEMA_HH

#include "Sequence.hh"

#include <DBDiffTool/object/Node.hh>
#include <DBDiffTool/object/Table.hh>
#include <DBDiffTool/orm/orm.hh>
#include <memory>

class Schema final : public Node {
public:
    template <typename... Args>
    void EmplaceTable(Args&&... args) {
        tables_.emplace_back(std::forward<Args>(args)...);
        Table& table{ tables_.back() };
        table_map_.emplace(table.t_name_, table);
    }

    template <typename... Args>
    void EmplaceSequence(Args&&... args) {
        sequences_.emplace_back(std::forward<Args>(args)...);
    }

    template <typename... Args>
    void EmplaceProcedure(Args&&... args) {
        procedure_.emplace_back(std::forward<Args>(args)...);
    }

    template <typename... Args>
    void AddIndexToTable(std::string_view t_name, Args&&... args) {
        Table& table{ table_map_.at(t_name) };
        table.EmplaceIndex(std::forward<Args>(args)...);
    }

    Table& UnCachedTable() { return tables_.back(); }

private:
    std::vector<Table>     tables_;
    std::vector<Sequence>  sequences_;
    std::vector<Procedure> procedure_;
    /// mapping / cache
    std::map<std::string_view, Table&> table_map_;
};

using schema_t = std::shared_ptr<Schema>;

#endif // DBDIFFTOOL_SCHEMA_HH
