//
// DBDiffTool / Table.hh
// Created by bryant on 2024 Nov 06.
//

#ifndef DBDIFFTOOL_TABLE_HH
#define DBDIFFTOOL_TABLE_HH

#include <memory>

#include <DBDiffTool/object/Column.hh>
#include <DBDiffTool/object/Index.hh>
#include <DBDiffTool/object/Trigger.hh>

struct Table {
    std::string          table_name_{};
    std::vector<Column>  columns_{};
    std::vector<Index>   indexes_{};
    std::vector<Trigger> triggers_{};

    Table(std::string_view t_name, std::string_view names,
          std::string_view types, std::string_view nulls) :
        table_name_{ t_name } {
        std::vector<std::string> name_vec;
        std::vector<std::string> type_vec;
        std::vector<std::string> null_vec;
        util::SplitString(names, name_vec);
        util::SplitString(types, type_vec);
        util::SplitString(nulls, null_vec);
        if (name_vec.size() not_eq type_vec.size() and
            name_vec.size() not_eq null_vec.size()) [[unlikely]] {
            throw std::logic_error{ "解析字段错误,因该是SQL写错了" };
        }
        for (std::size_t i = 0; i < name_vec.size(); ++i) {
            columns_.emplace_back(name_vec[i], type_vec[i], null_vec[i]);
        }
    }

    template <typename... Args>
    void EmplaceIndex(Args&&... args) {
        indexes_.emplace_back(std::forward<Args>(args)...);
    }

    template <typename... Args>
    void EmplaceTrigger(Args&&... args) {
        triggers_.emplace_back(std::forward<Args>(args)...);
    }
};
using View    = Table;
using table_t = std::shared_ptr<Table>;
using view_t = std::shared_ptr<View>;

#endif // DBDIFFTOOL_TABLE_HH
