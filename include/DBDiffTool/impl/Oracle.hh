﻿/// @file Oracle.hh
/// @author xianghongli\@hikvision.com.cn
/// @date 2024-11-10.
/// HangZhou HikVision Digital Technology Co., Ltd. All Right Reserved.
#pragma once

#include <DBLayer/DBLayer.h>

#include <DBDiffTool/common/Macros.hh>
#include <DBDiffTool/orm/orm.hh>

namespace db {
namespace impl {
class Oracle {
public:
    explicit Oracle(DBParam&& param) : param_{ std::move(param) } {
        DBLayer_Init(param_);
        Module_Register(param_, INSTANCE(Oracle));
        connect_ = DBLayer_ApplyConn(INSTANCE(Oracle));
    }

    static std::string_view Name() { return orm::Oracle::Name(); }

    ND std::vector<schema_t> SchemaList(orm::type const t) const {
        static_cast<void>(t);
        static_cast<void>(this);
        throw std::runtime_error{ "Oracle::SchemaList not implemented!" };
    }

    ~Oracle() { DBLayer_FreeConn(connect_); }

private:
    void FillTableList(std::vector<schema_t>& schemas,
                       orm::type const        orm_type) const {
        static_cast<void>(schemas[0] = nullptr);
        static_cast<void>(orm_type);
        static_cast<void>(this);
        throw std::runtime_error{ "Oracle::FillTableList not implemented!" };
    }

    void FillSequenceList() const {
        static_cast<void>(this);
        throw std::runtime_error{ "Oracle::FillSequenceList not implemented!" };
    }

    void FillProcedureList() const {
        static_cast<void>(this);
        throw std::runtime_error{ "Oracle::FillProcedureList not implemented!" };
    }

    void FillTableIndex() const {
        static_cast<void>(this);
        throw std::runtime_error{ "Oracle::FillTableIndex not implemented!" };
    }

    void FillTableTrigger() const {
        static_cast<void>(this);
        throw std::runtime_error{ "Oracle::FillTableTrigger not implemented!" };
    }

private: // NOLINT
    DBParam   param_{};
    CConnect* connect_{};

    std::map<std::string, sn::tbl_t> mutable tbl_map_{};
    std::map<std::string, sn::scm_t> mutable scm_map_{};
};
} // namespace impl

using namespace db::impl;
} // namespace db
