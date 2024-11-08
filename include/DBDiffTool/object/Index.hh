//
// DBDiffTool / Index.hh
// Created by bryant on 2024 Nov 06.
//

#ifndef DBDIFFTOOL_INDEX_HH
#define DBDIFFTOOL_INDEX_HH

#include <string>

#include <DBDiffTool/common/Macros.hh>

struct Index {
    std::string index_name_{};
    std::string indexed_fields_{};
    std::string index_md5_{};
    ND std::string_view Name() const;
};

#endif // DBDIFFTOOL_INDEX_HH
