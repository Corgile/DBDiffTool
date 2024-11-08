/// @file DBFactory.hh
/// @author xianghongli\@hikvision.com.cn
/// @date 2024-11-09.
/// HangZhou HikVision Digital Technology Co., Ltd. All Right Reserved.
#pragma once

#include <variant>

#include <DBDiffTool/DBDiffTool.hh>

using DataSourceVariant =
    std::variant<DataSource<db::PostgreSQL>, DataSource<db::SQLite>>;

auto CreateDataSource(DBParam& param) -> DataSourceVariant;
