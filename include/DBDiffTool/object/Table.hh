//
// DBDiffTool / Table.hh
// Created by bryant on 2024 Nov 06.
//

#ifndef DBDIFFTOOL_TABLE_HH
#define DBDIFFTOOL_TABLE_HH

#include <DBDiffTool/object/Column.hh>
#include <DBDiffTool/object/Index.hh>
#include <DBDiffTool/object/Trigger.hh>

struct Table {
    std::string         s_name_;
    std::string         t_name_;
    std::vector<Column> columns_;

    Table(std::string_view s_name, std::string_view t_name,
          std::string_view names, std::string_view types,
          std::string_view nulls) : s_name_{s_name}, t_name_{t_name} {
        std::vector<std::string> name_vec;
        std::vector<std::string> type_vec;
        std::vector<std::string> null_vec;
        util::SplitString(names, name_vec);
        util::SplitString(types, type_vec);
        util::SplitString(nulls, null_vec);
        if (name_vec.size() not_eq types.size() and
            name_vec.size() not_eq nulls.size()) [[unlikely]] {
            throw std::logic_error{"解析字段错误,因该是SQL写错了"};
        }
        for (std::size_t i = 0; i < name_vec.size(); ++i) {
            columns_.emplace_back(name_vec[i], type_vec[i], null_vec[i]);
        }
    }
};

#endif // DBDIFFTOOL_TABLE_HH
