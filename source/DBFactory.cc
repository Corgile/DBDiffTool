/// @file DBFactory.cc
/// @author xianghongli\@hikvision.com.cn
/// @date 2024-11-09.
/// HangZhou HikVision Digital Technology Co., Ltd. All Right Reserved.

#include <DBLayer/DBLayer.h>
#include <cassert>

#include <DBDiffTool/factory/DBFactory.hh>

auto CreateDataSource(DBParam& param) -> DataSourceVariant {
    switch (param.db_type) {
    case DBType::SQLite:     // 离线文件数据库
    case DBType::SQLCipher3: // 离线文件数据库
        return DataSource<db::SQLite>{ std::move(param) };
    case DBType::MySQL: //
        return DataSource<db::MySQL>{ std::move(param) };
    case DBType::Oracle: break;
    case DBType::SQLServer: break;
    case DBType::PostgreSQL:
        return DataSource<db::PostgreSQL>{ std::move(param) };
    case DBType::SQLServerODBC: break;
    default:           // unreachable
    case DBType::Null: // unreachable
        throw std::runtime_error{ "Unsupported database type" };
    }
    // unreachable
    return DataSource<db::SQLite>{ std::move(param) };
}
