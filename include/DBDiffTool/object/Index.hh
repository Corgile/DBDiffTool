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

    friend std::ostream& operator<<(std::ostream& os, Index const& obj) {
        return os << obj.index_name_ << obj.indexed_fields_ << obj.index_md5_;
    }

    ND std::string_view Key() const;
    ND std::string_view Name() const;

    static std::string prefix() {
        static std::string indent{ "        " };
        return indent;
    }
};

using index_t = std::shared_ptr<Index>;
namespace sn { // short name
using idx_t = index_t;
}

#endif // DBDIFFTOOL_INDEX_HH
