/// @file Sequence.hh
/// @author xianghongli\@hikvision.com.cn
/// @date 2024-11-06.
/// HangZhou HikVision Digital Technology Co., Ltd. All Right Reserved.
#pragma once

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
namespace sn { // short name
using seq_t = sequence_t;
}
