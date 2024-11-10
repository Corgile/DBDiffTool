/// @file DBFactory.hh
/// @author xianghongli\@hikvision.com.cn
/// @date 2024-11-09.
/// HangZhou HikVision Digital Technology Co., Ltd. All Right Reserved.
#pragma once

#include <variant>

#include <DBDiffTool/DataSource.hh>
#include <DBDiffTool/impl/impl.hh>

using DataSourceVariant = std::variant< // Trailing
    DataSource<db::SQLite>,             // comments
    DataSource<db::SQLCipher3>,         // ensure
    DataSource<db::MySQL>,              // better
    DataSource<db::Oracle>,             // alignment
    DataSource<db::SQLServer>,          // during
    DataSource<db::PostgreSQL>,         // code
    DataSource<db::SQLServerODBC>>;     // reformatting

auto CreateDataSource(DBParam& param) -> DataSourceVariant;
