/// @file DataSource.hh
/// @author xianghongli\@hikvision.com.cn
/// @date 2024-11-06.
/// HangZhou HikVision Digital Technology Co., Ltd. All Right Reserved.
#pragma once

#include <vector>

#include <DBLayer/DBLayer.h>

#include <DBDiffTool/common/common.hh>
#include <DBDiffTool/object/object.hh>
#include <DBDiffTool/orm/Type.hh>

using namespace db_layer;

template <typename T>
concept ConcreteDB = requires(T db, orm::type t) {
    { T::Name() } -> is_string_like;
    { db.Name() } -> is_string_like;
    { db.SchemaList(t) } -> std::same_as<std::vector<schema_t>>;
} and std::movable<T> and requires(DBParam&& param) {
    { T(std::move(param)) } -> std::convertible_to<T>;
    { T(std::forward<DBParam>(param)) } -> std::convertible_to<T>;
};

template <ConcreteDB DataBase>
class DataSource final {
public:
    explicit DataSource(DBParam&& param) :
        db_type_{ param.db_type }, ds_impl_{ std::move(param) } {}

    ND std::string_view Name() const { return DataBase::Name(); }

    ND std::vector<schema_t> SchemaList(orm::type const t) const {
        return ds_impl_.SchemaList(t);
    }

    ND glb::DBTYPE Type() const { return db_type_; }

    ~DataSource() = default;

private:
    glb::DBTYPE db_type_;
    DataBase    ds_impl_;
};
