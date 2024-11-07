//
// DBDiffTool / Column.hh
// Created by bryant on 2024 Nov 06.
//

#ifndef DBDIFFTOOL_COLUMN_HH
#define DBDIFFTOOL_COLUMN_HH

struct Column {
    std::string name_{};
    std::string type_{};
    std::string null_{};
};
using Field = Column;

#endif // DBDIFFTOOL_COLUMN_HH
