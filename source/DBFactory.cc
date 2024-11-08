/// @file DBfactory.cc
/// @author xianghongli\@hikvision.com.cn
/// @date 2024-11-09.
/// HangZhou HikVision Digital Technology Co., Ltd. All Right Reserved.

#include <DBLayer/DBLayer.h>

#include <DBDiffTool/factory/DBFactory.hh>

auto CreateDataSource(DBParam& param) -> DataSourceVariant {
    if (param.db_type == glb::DBTYPE::PostgreSQL) {
        return DataSource<db::PostgreSQL>{ std::move(param) };
    }
    if (param.db_type == glb::DBTYPE::SQLite) {
        return DataSource<db::SQLite>{ std::move(param) };
    }
    // unreachable
    throw std::runtime_error("Unsupported database type");
}

