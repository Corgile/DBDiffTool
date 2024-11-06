//
// DBDiffTool / DataSource.hh
// Created by bryant on 2024 Nov 06.
//

#ifndef DBDIFFTOOL_DATASOURCE_HH
#define DBDIFFTOOL_DATASOURCE_HH

#include <DBLayer/DBLayer.h>
#include <DBDiffTool/object/Object.hh>
#include <vector>

#define ND [[nodiscard]]

using namespace db_layer;

template <typename T>
concept ConcreteDS = requires(T db)
{
    { db.SchemaList() } -> std::same_as<std::vector<std::string>>;
    { db.Name() } -> std::same_as<std::string>;
    { db.sql() } -> std::same_as<std::string>;
    { db.conn() } -> std::same_as<CConnect*>;
};

template <ConcreteDS DataBase>
class DataSource final {
public:
    explicit DataSource(DBParam&& param) : instanceId_{}, initialized_{false}, param_{std::move(param)} {
        if (IsRemote()) {
            initialized_ = DBLayer_Init(param_, instanceId_);
        }
        if (initialized_) {
            connect_ = DBLayer_ApplyConn(instanceId_);
        }
    }

    ND std::string Name() const {
        return dsImpl_.Name();
    }

    ND std::vector<Schema> SchemaList() const {
        return dsImpl_.SchemaList();
    }

    ND DBType Type() const { return param_.db_type; }

    virtual ~DataSource() = default;

private:
    ND bool IsRemote() const {
        bool const offline{Type() == DBType::SQLITE3 or Type() == DBType::SQLCIPHER};
        return not offline;
    }

private:
    int instanceId_;
    bool initialized_;
    DBParam param_;
    CConnect* connect_;
    DataBase dsImpl_;
};

#endif //DBDIFFTOOL_DATASOURCE_HH
