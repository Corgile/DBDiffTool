/// @file impl.hh
/// @author xianghongli\@hikvision.com.cn
/// @date 2024-11-06.
/// HangZhou HikVision Digital Technology Co., Ltd. All Right Reserved.
#pragma once

/*
 * 本文件提供多种查询服务的具体实现, 目前仅包含PG和SQLite
 * 相当于Java的 DAO 数据库访问层
 */
#include <DBDiffTool/impl/MySQL.hh>
#include <DBDiffTool/impl/Oracle.hh>
#include <DBDiffTool/impl/PostgreSQL.hh>
#include <DBDiffTool/impl/SQLCipher3.hh>
#include <DBDiffTool/impl/SQLite.hh>
#include <DBDiffTool/impl/SQLServer.hh>
#include <DBDiffTool/impl/SQLServerODBC.hh>

