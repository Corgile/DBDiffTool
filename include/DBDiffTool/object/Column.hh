﻿/// @file Column.hh
/// @author xianghongli\@hikvision.com.cn
/// @date 2024-11-06.
/// HangZhou HikVision Digital Technology Co., Ltd. All Right Reserved.
#pragma once

#include <memory>
#include <string>

#include <DBDiffTool/common/Macros.hh>

struct Column {
    std::string field_name_{};
    std::string field_type_{};
    std::string field_null_{};

    friend std::ostream& operator<<(std::ostream& os, Column const& obj) {
        return os << obj.field_name_ << obj.field_type_ << obj.field_null_;
    }

    ND std::string_view Key() const;
    ND std::string_view Name() const;

    static std::string prefix() {
        static std::string indent{ "        " };
        return indent;
    }
};

using Field   = Column;
using field_t = std::shared_ptr<Field>;
namespace sn { // short name
using col_t = field_t;
}
