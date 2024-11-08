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
    static std::string  prefix() {
        static std::string indent{ "\t\t" };
        return indent;
    }
    static int w() { return 30; }
};

using index_t = std::shared_ptr<Index>;
namespace sn {
using idx_t = index_t;
}

#endif // DBDIFFTOOL_INDEX_HH
