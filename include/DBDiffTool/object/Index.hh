//
// DBDiffTool / Index.hh
// Created by bryant on 2024 Nov 06.
//

#ifndef DBDIFFTOOL_INDEX_HH
#define DBDIFFTOOL_INDEX_HH

#include <memory>
#include <string>

#include <DBDiffTool/common/Macros.hh>

struct Index {
    std::string index_name_{};
    std::string indexed_fields_{};
    std::string index_md5_{};
    ND std::string_view Name() const;
};

using index_t = std::shared_ptr<Index>;

#endif // DBDIFFTOOL_INDEX_HH
