/// @file orm.hh
/// @author xianghongli\@hikvision.com.cn
/// @date 2024-11-06.
/// HangZhou HikVision Digital Technology Co., Ltd. All Right Reserved.
#pragma once

/*
 * 本文件提供 SQL查询结果 -> 实体 的对象映射关系.
 * 具体:
 *   1. SQL结果的字段和顺序必须与结构体的字段对应。 相当于Java的mapper
 *   2. 具体的实现层在 DBDiffTool/impl 下, 相当于Java的 DAO
 */

#include <DBDiffTool/orm/MySQL.hh>
#include <DBDiffTool/orm/Oracle.hh>
#include <DBDiffTool/orm/PostgreSQL.hh>
#include <DBDiffTool/orm/SQLCipher3.hh>
#include <DBDiffTool/orm/SQLServer.hh>
#include <DBDiffTool/orm/SQLServerODBC.hh>
#include <DBDiffTool/orm/SQLite.hh>
#include <DBDiffTool/orm/Type.hh>
