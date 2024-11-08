//
// DBDiffTool / Sequence.hh
// Created by bryant on 2024 Nov 06.
//

#ifndef DBDIFFTOOL_SEQUENCE_HH
#define DBDIFFTOOL_SEQUENCE_HH

#include <memory>
#include <string>

#include <DBDiffTool/common/Macros.hh>

struct Sequence {
    std::string  seq_name_{};
    std::int64_t seq_min_{};
    std::int64_t seq_inc_{};
    std::int64_t seq_max_{};
    bool         seq_cycle_{};
    ND std::string_view Name() const;
};

using sequence_t = std::shared_ptr<Sequence>;

#endif // DBDIFFTOOL_SEQUENCE_HH
