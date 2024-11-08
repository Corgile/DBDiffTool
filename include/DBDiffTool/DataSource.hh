//
// DBDiffTool / DataSource.hh
// Created by bryant on 2024 Nov 06.
//

#ifndef DBDIFFTOOL_DATASOURCE_HH
#define DBDIFFTOOL_DATASOURCE_HH

#include <vector>

#include <DBLayer/DBLayer.h>

#include <DBDiffTool/common/Macros.hh>
#include <DBDiffTool/common/Traits.hh>
#include <DBDiffTool/object/Object.hh>
#include <DBDiffTool/orm/Type.hh>

using namespace db_layer;

template <typename T>
concept ConcreteDB = requires(T db, orm::type t) {
    { T::Name() } -> is_string_like;
    { db.Name() } -> is_string_like;
    { db.SchemaList(t) } -> std::same_as<std::vector<schema_t>>;
} and requires(DBParam&& param) {
    { T(std::move(param)) } -> std::convertible_to<T>;
    { T(std::forward<DBParam>(param)) } -> std::convertible_to<T>;
};

template <ConcreteDB DataBase>
class DataSource final {
public:
    explicit DataSource(DBParam&& param) :
        dsImpl_{ std::move(param) }, db_type_{ param.db_type } {}

    ND std::string_view Name() const { return dsImpl_.Name(); }

    ND std::vector<schema_t> SchemaList(orm::type const t) const {
        return dsImpl_.SchemaList(t);
    }

    ND glb::DBTYPE Type() const { return db_type_; }

    ~DataSource() = default;

private: // NOLINT
    DataBase    dsImpl_;
    glb::DBTYPE db_type_;
};

#endif // DBDIFFTOOL_DATASOURCE_HH
