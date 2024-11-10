/// @file DBFactory.cc
/// @author xianghongli\@hikvision.com.cn
/// @date 2024-11-09.
/// HangZhou HikVision Digital Technology Co., Ltd. All Right Reserved.

#include <DBLayer/DBLayer.h>

#include <DBDiffTool/factory/DBFactory.hh>

auto CreateDataSource(DBParam& param) -> DataSourceVariant {
    switch (param.db_type) {
    case DBType::SQLite:     // 离线文件数据库
    case DBType::SQLCipher3: // 离线文件数据库
        return DataSource<db::SQLite>{ std::move(param) };
    case DBType::MySQL: //
        return DataSource<db::MySQL>{ std::move(param) };
    case DBType::Oracle: //
        return DataSource<db::Oracle>{ std::move(param) };
    case DBType::SQLServer: //
        return DataSource<db::SQLServer>{ std::move(param) };
    case DBType::PostgreSQL:
        return DataSource<db::PostgreSQL>{ std::move(param) };
    case DBType::SQLServerODBC:
        return DataSource<db::SQLServerODBC>{ std::move(param) };
    default:           // unreachable
    case DBType::Null: // unreachable
        throw std::runtime_error{ "Unsupported database type" };
    }
}
