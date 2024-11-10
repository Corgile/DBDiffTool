/// @file Procedure.hh
/// @author xianghongli\@hikvision.com.cn
/// @date 2024-11-06.
/// HangZhou HikVision Digital Technology Co., Ltd. All Right Reserved.
#pragma once

#include <memory>
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
namespace sn { // short name
using pro_t = procedure_t;
}
