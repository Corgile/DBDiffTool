//
// DBDiffTool / Schema.hh
// Created by bryant on 2024 Nov 06.
//

#ifndef DBDIFFTOOL_SCHEMA_HH
#define DBDIFFTOOL_SCHEMA_HH

#include <DBDiffTool/object/Table.hh>
#include <DBDiffTool/orm/orm.hh>

class Schema {
public:
    void AddTable(Table &&table) { tables.emplace_back(std::move(table)); }

private:
    std::vector<Table> tables;
};

#endif // DBDIFFTOOL_SCHEMA_HH
