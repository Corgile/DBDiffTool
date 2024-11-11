/// @file Table.hh
/// @author xianghongli\@hikvision.com.cn
/// @date 2024-11-06.
/// HangZhou HikVision Digital Technology Co., Ltd. All Right Reserved.
#pragma once

#include <memory>

#include <DBDiffTool/common/Macros.hh>
#include <DBDiffTool/object/Column.hh>
#include <DBDiffTool/object/Index.hh>
#include <DBDiffTool/object/Trigger.hh>

struct Table {
    Table(std::string_view table_name, std::string_view table_fields,
          std::string_view field_types, std::string_view field_nulls);

    template <typename T, typename... Args>
    auto& Emplace(Args&&... args) {
        // static std::shared_ptr<T> error_instance{};
        if constexpr (std::is_same_v<T, Index>) {
            return indexes_.emplace_back(MAKE_SHARED);
        }
        if constexpr (std::is_same_v<T, Trigger>) {
            return triggers_.emplace_back(MAKE_SHARED);
        }
        // unreachable
        // return error_instance;
    }

    ND std::string_view Key() const;
    ND std::string_view Name() const;

    ND std::vector<sn::col_t> Col() const;
    ND std::vector<sn::idx_t> Idx() const;
    ND std::vector<sn::tgr_t> Tgr() const;

    static std::string prefix() {
        static std::string indent{ "      " };
        return indent;
    }

private:
    std::string            table_name_{};
    std::vector<sn::col_t> columns_{};
    std::vector<sn::idx_t> indexes_{};
    std::vector<sn::tgr_t> triggers_{};
};

using View    = Table;
using table_t = std::shared_ptr<Table>;
using view_t  = std::shared_ptr<View>;
namespace sn { // short name
using tbl_t = table_t;
}
