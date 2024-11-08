//
// DBDiffTool / Trigger.hh
// Created by bryant on 2024 Nov 06.
//

#ifndef DBDIFFTOOL_TRIGGER_HH
#define DBDIFFTOOL_TRIGGER_HH
#include <memory>
#include <string>

#include <DBDiffTool/common/Macros.hh>

struct Trigger {
    std::string trigger_name_{};
    std::string trigger_md5_{};
    ND std::string_view Name() const;
};
using trigger_t = std::shared_ptr<Trigger>;
#endif // DBDIFFTOOL_TRIGGER_HH
