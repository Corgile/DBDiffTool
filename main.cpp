#include <iostream>
#include <DBDiffTool/DBDiffTool.hh>

int main(int argc, char* const* argv) {
  DBParam paramA, paramB;
  util::LoadConfig(paramA, paramB);
  // 手动反射
  DataSource<> dsA{std::move(paramA)};
  DataSource<> dsB{std::move(paramB)};
  std::cout << "Hello, World!" << std::endl;
  return 0;
}
