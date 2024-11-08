//
// DBDiffTool / Procedure.hh
// Created by bryant on 2024 Nov 06.
//

#ifndef DBDIFFTOOL_PROCEDURE_HH
#define DBDIFFTOOL_PROCEDURE_HH
#include <memory>
#include <string>

#include <DBDiffTool/common/Macros.hh>

struct Procedure {
    std::string procedure_name_{};
    std::string procedure_md5_{};
    ND std::string_view Name() const;
    static std::string  prefix() {
        static std::string indent{ "\t" };
        return indent;
    }
    static int w() { return 8; }
};
using procedure_t = std::shared_ptr<Procedure>;
namespace sn {
using pro_t = procedure_t;
}
#endif // DBDIFFTOOL_PROCEDURE_HH
