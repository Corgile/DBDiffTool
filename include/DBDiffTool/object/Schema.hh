//
// DBDiffTool / Schema.hh
// Created by bryant on 2024 Nov 06.
//

#ifndef DBDIFFTOOL_SCHEMA_HH
#define DBDIFFTOOL_SCHEMA_HH

#include <DBDiffTool/compare/Comparator.hh>
#include <memory>

#include <DBDiffTool/object/Node.hh>
#include <DBDiffTool/object/Sequence.hh>
#include <DBDiffTool/object/Table.hh>

class Schema final : public Node {
public:
    Schema() = default;

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

    ND std::string_view Name() const { return schema_name_; }

private:
    std::string            schema_name_{};
    std::vector<table_t>   tables_{};
    std::vector<Sequence>  sequences_{};
    std::vector<Procedure> procedure_{};
};

using schema_t    = std::shared_ptr<Schema>;
using schema_cref = schema_t const&;

#endif // DBDIFFTOOL_SCHEMA_HH
