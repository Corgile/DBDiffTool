//
// DBDiffTool / Util.cc
// Created by bryant on 2024 Nov 06.
//
#include <DBDiffTool/common/Util.hh>
#include <filesystem>
#include <fstream>

void util::detail::Load(ini_section const &sec, DBParam &param) {
    std::string db_type;
    inipp::get_value(sec, "db.db_type", db_type);
    param.db_type = glb::str2enum.at(db_type);
    inipp::get_value(sec, "db.db_name", param.db_name);
    inipp::get_value(sec, "db.ip", param.ip);
    inipp::get_value(sec, "db.port", param.port);
    inipp::get_value(sec, "db.user", param.user);
    inipp::get_value(sec, "db.passwd", param.passwd);
    inipp::get_value(sec, "db.min_conn", param.min_conn);
    inipp::get_value(sec, "db.max_conn", param.max_conn);
}

void util::LoadConfig(DBParam &dsa, DBParam &dsb) {
    namespace fs = std::filesystem;
    inipp::Ini<char> ini;
    fs::path         path{fs::current_path() / "profile.ini"};
    std::ifstream    is{path, std::ios::in};
    ini.parse(is);
    ini.strip_trailing_comments();
    detail::Load(ini.sections["DataSourceA"], dsa);
    detail::Load(ini.sections["DataSourceB"], dsb);
    glb::heterogeneous = dsa.db_type not_eq dsb.db_type;
}
void util::TraverseResultSet(CConnect                    *conn,
                             const std::function<void()> &func) {
    for (conn->First(); not conn->IsEoF(); conn->Next()) {
        func();
    }
    conn->First();
}

void util::SplitString(std::string_view str, std::vector<std::string> &tokens) {
    std::string        token;
    std::istringstream iss{str.data()};
    while (std::getline(iss, token, ',')) {
        tokens.push_back(token);
    }
}
