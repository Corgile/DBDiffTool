//
// DBDiffTool / Table.hh
// Created by bryant on 2024 Nov 06.
//

#ifndef DBDIFFTOOL_TABLE_HH
#define DBDIFFTOOL_TABLE_HH

#include <memory>

#include <DBDiffTool/common/Macros.hh>
#include <DBDiffTool/common/Util.hh>
#include <DBDiffTool/object/Column.hh>
#include <DBDiffTool/object/Index.hh>
#include <DBDiffTool/object/Trigger.hh>

struct Table {
    Table(std::string_view table_name, std::string_view table_fields,
          std::string_view field_types, std::string_view field_nulls);

    template <typename... Args>
    void EmplaceIndex(Args&&... args) {
        indexes_.emplace_back(std::forward<Args>(args)...);
    }

    template <typename... Args>
    void EmplaceTrigger(Args&&... args) {
        triggers_.emplace_back(std::forward<Args>(args)...);
    }

    ND std::string_view Name() const;
    ND std::vector<Column> Columns() const;
    ND std::vector<Index> Indexes() const;
    ND std::vector<Trigger> Triggers() const;

private:
    std::string          table_name_{};
    std::vector<Column>  columns_{};
    std::vector<Index>   indexes_{};
    std::vector<Trigger> triggers_{};
};

using View    = Table;
using table_t = std::shared_ptr<Table>;
using view_t  = std::shared_ptr<View>;

#endif // DBDIFFTOOL_TABLE_HH
