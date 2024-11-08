//
// DBDiffTool / Procedure.hh
// Created by bryant on 2024 Nov 06.
//

#ifndef DBDIFFTOOL_PROCEDURE_HH
#define DBDIFFTOOL_PROCEDURE_HH

#include <string>

#include <DBDiffTool/common/Macros.hh>

struct Procedure {
    std::string procedure_name_{};
    std::string procedure_md5_{};
    ND std::string_view Name() const;
};

#endif // DBDIFFTOOL_PROCEDURE_HH
