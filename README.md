# DBDiffTool

## ʵ��˼·

�������룩

## ע��

- ��Ҫ����̨����Ǵ���ɫ�Ļ�������ʱ���Ϻ� `ANSI_COLOR_ON`
- ��Ҫ������ļ��Ļ����� `>` �ض��򼴿�
- exe�ļ����ڵ�ǰĿ¼���� `profile.ini` ��������Դ����
- ���仹ûд���޸� `profile.ini` ����Ҫ�޸� `main.cpp`
  ```c++
  // todo: ģ�����Ӧ�ø���DBParam��db_type����, ��ʱû��.
  DataSource<db::PostgreSQL> dsA{ std::move(paramA) };
  DataSource<db::SQLite>     dsB{ std::move(paramB) };
  ```
- ��ʱ��֧�� `PG/SQLite`,`PG/PG`,`SQLite/SQLite`���ֱȽ�.

## ����

```bat
cmake.exe \
  -G "Visual Studio 17 2022" 
  -DCMAKE_BUILD_TYPE=RelWithDebInfo 
  -DDDT_ANSI:STRING=ANSI_COLOR_ON 
  -S .\DBDiffTool 
  -B .\build\release
```
