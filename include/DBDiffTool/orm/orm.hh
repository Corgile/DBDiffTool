//
// DBDiffTool / orm.hh
// Created by bryant on 2024 Nov 06.
//

/*
 * 本文件提供 SQL查询结果 -> 实体 的对象映射关系.
 * 具体:
 *   1. SQL结果的字段和顺序必须与结构体的字段对应。 相当于Java的mapper
 *   2. 具体的实现层在 DBDiffTool/impl 下, 相当于Java的 DAO
 */

#ifndef DBDIFFTOOL_ORM_HH
#define DBDIFFTOOL_ORM_HH

#include <DBDiffTool/orm/PostgreSql.hh>
#include <DBDiffTool/orm/SqLite.hh>
#include <DBDiffTool/orm/Type.hh>

#endif //DBDIFFTOOL_ORM_HH
