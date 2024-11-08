/// @file Object.cc
/// @author xianghongli\@hikvision.com.cn
/// @date 2024-11-08.
/// HangZhou HikVision Digital Technology Co., Ltd. All Right Reserved.
#include <DBDiffTool/object/Object.hh>

void             Schema::SetName(std::string_view name) { schema_name_ = name; }
table_t          Schema::Last() { return tables_.back(); }
std::string_view Schema::Name() const { return schema_name_; }
std::vector<table_t>   Schema::Tables() const { return tables_; }
std::vector<Sequence>  Schema::Sequences() const { return sequences_; }
std::vector<Procedure> Schema::Procedures() const { return procedure_; }

Table::Table(std::string_view table_name, std::string_view table_fields,
             std::string_view field_types, std::string_view field_nulls) :
    table_name_{ table_name } {
    std::vector<std::string> name_vec;
    std::vector<std::string> type_vec;
    std::vector<std::string> null_vec;
    util::SplitString(table_fields, name_vec);
    util::SplitString(field_types, type_vec);
    util::SplitString(field_nulls, null_vec);
    if (name_vec.size() not_eq type_vec.size() and
        name_vec.size() not_eq null_vec.size()) [[unlikely]] {
        throw std::logic_error{ "解析字段错误,应该是SQL写错了" };
    }
    for (std::size_t i = 0; i < name_vec.size(); ++i) {
        columns_.emplace_back(name_vec[i], type_vec[i], null_vec[i]);
    }
}

std::string_view     Table::Name() const { return table_name_; }
std::vector<Column>  Table::Columns() const { return columns_; }
std::vector<Index>   Table::Indexes() const { return indexes_; }
std::vector<Trigger> Table::Triggers() const { return triggers_; }

std::string_view Sequence::Name() const { return seq_name_; }
std::string_view Procedure::Name() const { return procedure_name_; }
std::string_view Trigger::Name() const {return trigger_name_;}
std::string_view Column::Name() const { return name_; }
std::string_view Index::Name() const { return index_name_; }
