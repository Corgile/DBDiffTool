#include <DBDiffTool/DBDiffTool.hh>
#include <iostream>

int main(int argc, char* const* argv) {
    DBParam paramA, paramB;
    util::LoadConfig(paramA, paramB);
    // 手动反射
    DataSource<db::PostgreSQL> dsA{ std::move(paramA) };
    DataSource<db::PostgreSQL> dsB{ std::move(paramB) };
    std::cout << "Hello, World!" << std::endl;
    return 0;
}
