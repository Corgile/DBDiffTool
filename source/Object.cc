/// @file Object.cc
/// @author xianghongli\@hikvision.com.cn
/// @date 2024-11-08.
/// HangZhou HikVision Digital Technology Co., Ltd. All Right Reserved.

#include <DBDiffTool/common/Global.hh>
#include <DBDiffTool/object/Object.hh>

Schema::Schema() = default;

void Schema::SetName(std::string_view name) { schema_name_ = name; }

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
        columns_.emplace_back(
            std::make_shared<Field>(name_vec[i], type_vec[i], null_vec[i]));
    }
}

std::vector<sn::tbl_t> Schema::Tbl() const { return tables_; }
std::vector<sn::seq_t> Schema::Seq() const { return sequences_; }
std::vector<sn::pro_t> Schema::Pro() const { return procedure_; }

std::vector<sn::col_t> Table::Col() const { return columns_; }
std::vector<sn::idx_t> Table::Idx() const { return indexes_; }
std::vector<sn::tgr_t> Table::Tgr() const { return triggers_; }

std::string_view Schema::Key() const { return schema_name_; }

std::string_view Table::Key() const { return table_name_; }

std::string_view Procedure::Key() const {
    if (glb::heterogeneous) {
        return procedure_name_;
    }
    std::stringstream ss;
    ss << *this;
    static std::string key{ ss.str() };
    return key;
}

std::string_view Sequence::Key() const {
    if (glb::heterogeneous) {
        return seq_name_;
    }
    std::stringstream ss;
    ss << *this;
    static std::string key{ ss.str() };
    return key;
}

std::string_view Trigger::Key() const {
    if (glb::heterogeneous) {
        return trigger_name_;
    }
    std::stringstream ss;
    ss << *this;
    static std::string key{ ss.str() };
    return key;
}

std::string_view Column::Key() const {
    if (glb::heterogeneous) {
        return field_name_;
    }
    std::stringstream ss;
    ss << *this;
    static std::string key{ ss.str() };
    return key;
}

std::string_view Index::Key() const {
    if (glb::heterogeneous) {
        return index_name_;
    }
    std::stringstream ss;
    ss << *this;
    static std::string key{ ss.str() };
    return key;
}

std::string_view Table::Name() const { return table_name_; }
std::string_view Index::Name() const { return index_name_; }
std::string_view Schema::Name() const { return schema_name_; }
std::string_view Column::Name() const { return field_name_; }
std::string_view Trigger::Name() const { return trigger_name_; }
std::string_view Sequence::Name() const { return seq_name_; }
std::string_view Procedure::Name() const { return procedure_name_; }
