//
// DBDiffTool / Sequence.hh
// Created by bryant on 2024 Nov 06.
//

#ifndef DBDIFFTOOL_SEQUENCE_HH
#define DBDIFFTOOL_SEQUENCE_HH

#include <memory>
#include <ostream>
#include <string>

#include <DBDiffTool/common/Macros.hh>

struct Sequence {
    std::string  seq_name_{};
    std::int64_t seq_min_{};
    std::int64_t seq_inc_{};
    std::int64_t seq_max_{};
    bool         seq_cycle_{};

    ND std::string_view Key() const;
    ND std::string_view Name() const;

    friend std::ostream& operator<<(std::ostream& os, Sequence const& obj) {
        return os << obj.seq_name_ << obj.seq_min_ << obj.seq_inc_
                  << obj.seq_max_ << obj.seq_cycle_;
    }

    static std::string prefix() {
        static std::string indent{ "    " };
        return indent;
    }
};

using sequence_t = std::shared_ptr<Sequence>;
namespace sn {
using seq_t = sequence_t;
}

#endif // DBDIFFTOOL_SEQUENCE_HH
