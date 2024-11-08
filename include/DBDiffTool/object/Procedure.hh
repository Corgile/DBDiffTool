//
// DBDiffTool / Procedure.hh
// Created by bryant on 2024 Nov 06.
//

#ifndef DBDIFFTOOL_PROCEDURE_HH
#define DBDIFFTOOL_PROCEDURE_HH
#include <memory>
#include <ostream>
#include <string>

#include <DBDiffTool/common/Macros.hh>

struct Procedure {
    std::string procedure_name_{};
    std::string procedure_md5_{};

    ND std::string_view Key() const;
    ND std::string_view Name() const;

    friend std::ostream& operator<<(std::ostream& os, Procedure const& obj) {
        return os << obj.procedure_name_ << obj.procedure_md5_;
    }

    static std::string prefix() {
        static std::string indent{ "    " };
        return indent;
    }
};
using procedure_t = std::shared_ptr<Procedure>;
namespace sn {
using pro_t = procedure_t;
}
#endif // DBDIFFTOOL_PROCEDURE_HH
