#include <DBDiffTool/DBDiffTool.hh>
#include <DBDiffTool/common/Util.hh>
#include <Windows.h>
#include <iostream>

int main(int argc, char* const* argv) {
    SetConsoleCP(65001);
    system("chcp 65001 > nul");
    DBParam paramA, paramB;
    util::LoadConfig(paramA, paramB);
    // todo: 模板参数应该根据DBParam的db_type反射, 暂时没做.
    DataSource<db::PostgreSQL> dsA{ std::move(paramA) };
    DataSource<db::SQLite>     dsB{ std::move(paramB) };

    auto const listA{ dsA.SchemaList(orm::type::table) };
    auto const listB{ dsB.SchemaList(orm::type::table) };

    std::stringstream diff;
    compare::Compare(listA, listB, diff, dsA.Name(), dsB.Name());
    std::cout << diff.str() << std::endl;
    return 0;
}
