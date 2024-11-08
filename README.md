# DBDiffTool

## 实现思路

（见代码）

## 注意

- 想要控制台输出是带颜色的话，编译时加上宏 `ANSI_COLOR_ON`
- 想要输出到文件的话，用 `>` 重定向即可
- exe文件会在当前目录下找 `profile.ini` 加载数据源配置
- 反射还没写，修改 `profile.ini` 还需要修改 `main.cpp`
  ```c++
  // todo: 模板参数应该根据DBParam的db_type反射, 暂时没做.
  DataSource<db::PostgreSQL> dsA{ std::move(paramA) };
  DataSource<db::SQLite>     dsB{ std::move(paramB) };
  ```
- 暂时仅支持 `PG/SQLite`,`PG/PG`,`SQLite/SQLite`三种比较.

## 编译

```bat
cmake.exe \
  -G "Visual Studio 17 2022" 
  -DCMAKE_BUILD_TYPE=RelWithDebInfo 
  -DDDT_ANSI:STRING=ANSI_COLOR_ON 
  -S .\DBDiffTool 
  -B .\build\release
```
