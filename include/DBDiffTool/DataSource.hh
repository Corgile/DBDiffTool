//
// DBDiffTool / DataSource.hh
// Created by bryant on 2024 Nov 06.
//

#ifndef DBDIFFTOOL_DATASOURCE_HH
#define DBDIFFTOOL_DATASOURCE_HH

#include <DBDiffTool/common/Macros.hh>
#include <DBDiffTool/object/Object.hh>
#include <DBLayer/DBLayer.h>
#include <vector>

using namespace db_layer;

template <typename T>
concept is_string_like = requires(T a) {
    { std::string{ a } } -> std::convertible_to<std::string>;
};

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

    ND DBTYPE Type() const { return db_type_; }

    ~DataSource() = default;

private:
    ND bool IsRemote() const {
        bool const offline{ Type() == DBType::SQLite or
                            Type() == DBType::SQLCipher3 };
        return not offline;
    }

private: // NOLINT
    DataBase dsImpl_;
    DBTYPE   db_type_;
};

#endif // DBDIFFTOOL_DATASOURCE_HH
