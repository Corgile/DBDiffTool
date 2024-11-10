/// @file DBFactory.hh
/// @author xianghongli\@hikvision.com.cn
/// @date 2024-11-09.
/// HangZhou HikVision Digital Technology Co., Ltd. All Right Reserved.
#pragma once

#include <variant>

#include <DBDiffTool/DBDiffTool.hh>

using DataSourceVariant = std::variant< // Trailing
    DataSource<db::SQLite>,             // comments
    DataSource<db::SQLCipher3>,         // added
    DataSource<db::MySQL>,              // are
    DataSource<db::Oracle>,             // used
    DataSource<db::SQLServer>,          // for
    DataSource<db::PostgreSQL>,         // line
    DataSource<db::SQLServerODBC>>;     // breaking

auto CreateDataSource(DBParam& param) -> DataSourceVariant;
