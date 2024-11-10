/// @file Trigger.hh
/// @author xianghongli\@hikvision.com.cn
/// @date 2024-11-06.
/// HangZhou HikVision Digital Technology Co., Ltd. All Right Reserved.
#pragma once

#include <memory>
#include <string>

#include <DBDiffTool/common/Macros.hh>

struct Trigger {
    std::string trigger_name_{};
    std::string trigger_md5_{};

    friend std::ostream& operator<<(std::ostream& os, Trigger const& obj) {
        return os << obj.trigger_name_ << obj.trigger_md5_;
    }

    ND std::string_view Key() const;
    ND std::string_view Name() const;

    static std::string prefix() {
        static std::string indent{ "        " };
        return indent;
    }
};
using trigger_t = std::shared_ptr<Trigger>;
namespace sn { // short name
using tgr_t = trigger_t;
}
