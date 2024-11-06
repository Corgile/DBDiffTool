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
concept ConcreteDS = requires(T db) {
    { db.SchemaList() } -> std::same_as<std::vector<std::string>>;
    { db.Name() } -> std::same_as<std::string>;
} and requires(DBParam &&param) {
    { T(param) } -> std::convertible_to<T>;
};

template <ConcreteDS DataBase>
class DataSource final {
public:
    explicit DataSource(DBParam &&param) :
        dsImpl_{std::move(param)}, db_type_{param.db_type} {}

    ND std::string Name() const { return dsImpl_.Name(); }

    ND std::vector<Schema> SchemaList() const { return dsImpl_.SchemaList(); }

    ND DBType Type() const { return db_type_; }

    ~DataSource() = default;

private:
    ND bool IsRemote() const {
        bool const offline{Type() == DBType::SQLITE3 or
                           Type() == DBType::SQLCIPHER};
        return not offline;
    }

private: // NOLINT
    DataBase dsImpl_;
    DBType   db_type_;
};

#endif // DBDIFFTOOL_DATASOURCE_HH
