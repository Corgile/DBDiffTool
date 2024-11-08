#include <DBDiffTool/DBDiffTool.hh>
#include <iostream>

int main(int argc, char* const* argv) {
    DBParam paramA, paramB;
    util::LoadConfig(paramA, paramB);
    // todo: 手动反射
    DataSource<db::PostgreSQL> dsA{ std::move(paramA) };
    DataSource<db::SQLite> dsB{ std::move(paramB) };
    auto const listA{ dsA.SchemaList(orm::type::table) };
    auto const listB{ dsB.SchemaList(orm::type::table) };
    std::cout << "Hello, World!" << std::endl;
    return 0;
}
