//
// DBDiffTool / Column.hh
// Created by bryant on 2024 Nov 06.
//

#ifndef DBDIFFTOOL_COLUMN_HH
#define DBDIFFTOOL_COLUMN_HH

#include <memory>
#include <string>

#include <DBDiffTool/common/Macros.hh>

struct Column {
    std::string name_{};
    std::string type_{};
    std::string null_{};
    ND std::string_view Name() const;
    static std::string  prefix() {
        static std::string indent{ "\t\t" };
        return indent;
    }
    static int w() { return 20; }
};

using Field   = Column;
using field_t = std::shared_ptr<Field>;
namespace sn {
using col_t = field_t;
}

#endif // DBDIFFTOOL_COLUMN_HH
