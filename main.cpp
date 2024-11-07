#include <DBDiffTool/DBDiffTool.hh>
#include <iostream>

int main(int argc, char* const* argv) {
    DBParam paramA, paramB;
    util::LoadConfig(paramA, paramB);
    // 手动反射
    DataSource<db::PostgreSQL> dsA{ std::move(paramA) };
    DataSource<db::PostgreSQL> dsB{ std::move(paramB) };
    auto const list{ dsA.SchemaList(orm::type::view) };
    std::cout << "Hello, World!" << std::endl;
    return 0;
}
