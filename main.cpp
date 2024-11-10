#include <Windows.h>

#include <iostream>

#include <DBDiffTool/DBDiffTool.hh>


int main(int argc, char* const* argv) {
    SetConsoleCP(65001);
    system("chcp 65001 > nul");
    DBParam paramA, paramB;
    util::LoadConfig(paramA, paramB);

    glb::heterogeneous = paramA.db_type not_eq paramB.db_type;

    DataSourceVariant dsA{ CreateDataSource(paramA) };
    DataSourceVariant dsB{ CreateDataSource(paramB) };

    std::vector<schema_t> listA, listB;
    std::visit([&](auto& ds) { listA = ds.SchemaList(orm::type::table); }, dsA);
    std::visit([&](auto& ds) { listB = ds.SchemaList(orm::type::table); }, dsB);

    std::string_view name_a, name_b;
    std::visit([&](auto& ds) { name_a = ds.Name(); }, dsA);
    std::visit([&](auto& ds) { name_b = ds.Name(); }, dsB);
    std::stringstream diff;
    compare::Compare(listA, listB, diff, name_a, name_b);
    std::cout << diff.str() << std::endl;
    return 0;
}
