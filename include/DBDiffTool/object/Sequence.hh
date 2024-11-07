//
// DBDiffTool / Sequence.hh
// Created by bryant on 2024 Nov 06.
//

#ifndef DBDIFFTOOL_SEQUENCE_HH
#define DBDIFFTOOL_SEQUENCE_HH

struct Sequence {
    std::string  seq_name_;
    std::int64_t seq_min_;
    std::int64_t seq_inc_;
    std::int64_t seq_max_;
    bool         seq_cycle_;
};

#endif // DBDIFFTOOL_SEQUENCE_HH
