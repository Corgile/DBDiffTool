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
    void SetName(std::string_view name) { schema_name_ = name; }

    template <typename... Args>
    void EmplaceTable(Args&&... args) {
        tables_.emplace_back(
            std::make_shared<Table>(std::forward<Args>(args)...));
    }

    template <typename... Args>
    void EmplaceSequence(Args&&... args) {
        sequences_.emplace_back(std::forward<Args>(args)...);
    }

    template <typename... Args>
    void EmplaceProcedure(Args&&... args) {
        procedure_.emplace_back(std::forward<Args>(args)...);
    }

    table_t Last() { return tables_.back(); }

private:
    std::string            schema_name_{};
    std::vector<table_t>   tables_{};
    std::vector<Sequence>  sequences_{};
    std::vector<Procedure> procedure_{};
};

using schema_t = std::shared_ptr<Schema>;

#endif // DBDIFFTOOL_SCHEMA_HH
