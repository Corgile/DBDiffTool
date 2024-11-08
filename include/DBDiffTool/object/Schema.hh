//
// DBDiffTool / Schema.hh
// Created by bryant on 2024 Nov 06.
//

#ifndef DBDIFFTOOL_SCHEMA_HH
#define DBDIFFTOOL_SCHEMA_HH

#include <memory>
#include <string>

#include <DBDiffTool/common/Macros.hh>
#include <DBDiffTool/object/Procedure.hh>
#include <DBDiffTool/object/Sequence.hh>
#include <DBDiffTool/object/Table.hh>

class Schema final {
public:
    Schema();

    void SetName(std::string_view name);

    template <typename T, typename... Args>
    auto& Emplace(Args&&... args) {
        // static std::shared_ptr<T> error_instance{};
        if constexpr (std::is_same_v<T, Table>) {
            return tables_.emplace_back(MAKE_SHARED);
        }
        if constexpr (std::is_same_v<T, Sequence>) {
            return sequences_.emplace_back(MAKE_SHARED);
        }
        if constexpr (std::is_same_v<T, Procedure>) {
            return procedure_.emplace_back(MAKE_SHARED);
        }
        // unreachable
        // return error_instance;
    }

    ND std::string_view Key() const;
    ND std::string_view Name() const;

    ND std::vector<sn::tbl_t> Tbl() const;
    ND std::vector<sn::seq_t> Seq() const;
    ND std::vector<sn::pro_t> Pro() const;

    static std::string prefix() {
        static std::string indent{};
        return indent;
    }

private:
    std::string            schema_name_{};
    std::vector<sn::tbl_t> tables_{};
    std::vector<sn::seq_t> sequences_{};
    std::vector<sn::pro_t> procedure_{};
};

using schema_t    = std::shared_ptr<Schema>;
using schema_cref = schema_t const&;
namespace sn {
using scm_t = schema_t;
}

#endif // DBDIFFTOOL_SCHEMA_HH
