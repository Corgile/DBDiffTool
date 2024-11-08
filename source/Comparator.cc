/// @file Comparator.cc
/// @author xianghongli\@hikvision.com.cn
/// @date 2024-11-08.
/// HangZhou HikVision Digital Technology Co., Ltd. All Right Reserved.

#include <DBDiffTool/compare/Comparator.hh>

void compare::Compare(std::vector<schema_t> const& listA,
                      std::vector<schema_t> const& listB,
                      std::stringstream&           diff) {
    std::stringstream ss{ "\x1B[1mComparing Schemas: \x1B[0m\n" };
    auto const        size_before{ ss.str().size() };
    auto              scmA{ listA.begin() }, scmB{ listB.begin() };
    while (scmA not_eq listA.end() and scmB not_eq listB.end()) {
        auto const scmA_name{ (*scmA)->Name() };
        auto const scmB_name{ (*scmB)->Name() };
        if (scmA_name == scmB_name) {
            Compare((*scmA)->Tables(), (*scmB)->Tables(), ss);
            Compare((*scmA)->Sequences(), (*scmB)->Sequences(), ss);
            Compare((*scmA)->Procedures(), (*scmB)->Procedures(), ss);
            ++scmA;
            ++scmB;
        } else if (string_less{}(scmA_name, scmB_name)) {
            ss << std::setw(35) << Present<Schema, Lang::EN>(scmA_name);
            ss << " | " << Absent<Schema, Lang::EN>() << '\n';
            ++scmA;
        } else if (string_less{}(scmB_name, scmA_name)) {
            ss << std::setw(35) << Absent<Schema, Lang::EN>() << " | "
               << Present<Schema, Lang::EN>(scmB_name) << '\n';
            ++scmB;
        }
    }
    while (scmA not_eq listA.end()) {
        ss << std::setw(35) << Present<Schema, Lang::EN>((*scmA)->Name());
        ss << " | " << Absent<Schema, Lang::EN>() << '\n';
        ++scmA;
    }
    while (scmB not_eq listB.end()) {
        ss << std::setw(35) << Absent<Schema, Lang::CN>() << " | "
           << Present<Schema, Lang::EN>((*scmB)->Name()) << '\n';
        ++scmB;
    }
    if (ss.str().size() == size_before) {
        ss << "\t一致\n";
    }
    diff << ss.str();
}
void compare::Compare(std::vector<table_t> const& listA,
                      std::vector<table_t> const& listB,
                      std::stringstream&          diff) {
    std::stringstream ss{ "\x1B[1mComparing Tables: \x1B[0m\n" };
    auto const        size_before{ ss.str().size() };
    auto              tblA{ listA.begin() }, tblB{ listB.begin() };
    while (tblA not_eq listA.end() and tblB not_eq listB.end()) {
        auto const tblA_name{ (*tblA)->Name() };
        auto const tblB_name{ (*tblB)->Name() };
        if (tblA_name == tblB_name) {
            Compare((*tblA)->Columns(), (*tblB)->Columns(), ss);
            Compare((*tblA)->Indexes(), (*tblB)->Indexes(), ss);
            Compare((*tblA)->Triggers(), (*tblB)->Triggers(), ss);
            ++tblA;
            ++tblB;
        } else if (string_less{}(tblA_name, tblB_name)) {
            ss << std::setw(35) << Present<Table, Lang::EN>(tblA_name);
            ss << " | " << Absent<Table, Lang::EN>() << '\n';
            ++tblA;
        } else if (string_less{}(tblB_name, tblA_name)) {
            ss << std::setw(35) << Absent<Table, Lang::EN>() << " | "
               << Present<Table, Lang::EN>(tblB_name) << '\n';
            ++tblB;
        }
    }
    while (tblA not_eq listA.end()) {
        ss << std::setw(35) << Present<Table, Lang::EN>((*tblA)->Name());
        ss << " | " << Absent<Table, Lang::EN>() << '\n';
        ++tblA;
    }
    while (tblB not_eq listB.end()) {
        ss << std::setw(35) << Absent<Table, Lang::CN>() << " | "
           << Present<Table, Lang::EN>((*tblB)->Name()) << '\n';
        ++tblB;
    }
    if (ss.str().size() == size_before) {
        ss << "\t一致\n";
    }
    diff << ss.str();
}
void compare::Compare(std::vector<Sequence> const& listA,
                      std::vector<Sequence> const& listB,
                      std::stringstream&           diff) {
    std::stringstream ss{ "\x1B[1mComparing Tables: \x1B[0m\n" };
    auto const        size_before{ ss.str().size() };
    auto              seq_itA{ listA.begin() }, seq_itB{ listB.begin() };
    while (seq_itA not_eq listA.end() and seq_itB not_eq listB.end()) {
        auto const seqA_name{ seq_itA->Name() }, seqB_name{ seq_itB->Name() };
        if (seqA_name == seqB_name) {
            ++seq_itA;
            ++seq_itB;
        } else if (string_less{}(seqA_name, seqB_name)) {
            ss << std::setw(35) << Present<Sequence, Lang::EN>(seqA_name);
            ss << " | " << Absent<Sequence, Lang::EN>() << '\n';
            ++seq_itA;
        } else if (string_less{}(seqB_name, seqA_name)) {
            ss << std::setw(35) << Absent<Sequence, Lang::EN>() << " | "
               << Present<Sequence, Lang::EN>(seqB_name) << '\n';
            ++seq_itB;
        }
    }
    while (seq_itA not_eq listA.end()) {
        ss << std::setw(35) << Present<Sequence, Lang::EN>(seq_itA->Name());
        ss << " | " << Absent<Sequence, Lang::EN>() << '\n';
        ++seq_itA;
    }
    while (seq_itB not_eq listB.end()) {
        ss << std::setw(35) << Absent<Sequence, Lang::CN>() << " | "
           << Present<Sequence, Lang::EN>(seq_itB->Name()) << '\n';
        ++seq_itB;
    }
    if (ss.str().size() == size_before) {
        ss << "\t一致\n";
    }
    diff << ss.str();
}

void compare::Compare(std::vector<Procedure> const& listA,
                      std::vector<Procedure> const& listB,
                      std::stringstream&            diff) {
    std::stringstream ss{ "\x1B[1mComparing Tables: \x1B[0m\n" };
    auto const        size_before{ ss.str().size() };
    auto              pro_itA{ listA.begin() }, pro_itB{ listB.begin() };
    while (pro_itA not_eq listA.end() and pro_itB not_eq listB.end()) {
        auto const proA_name{ pro_itA->Name() }, proB_name{ pro_itB->Name() };
        if (proA_name == proB_name) {
            ++pro_itA;
            ++pro_itB;
        } else if (string_less{}(proA_name, proB_name)) {
            ss << std::setw(35) << Present<Procedure, Lang::EN>(proA_name);
            ss << " | " << Absent<Procedure, Lang::EN>() << '\n';
            ++pro_itA;
        } else if (string_less{}(proB_name, proA_name)) {
            ss << std::setw(35) << Absent<Procedure, Lang::EN>() << " | "
               << Present<Procedure, Lang::EN>(proB_name) << '\n';
            ++pro_itB;
        }
    }
    while (pro_itA not_eq listA.end()) {
        ss << std::setw(35) << Present<Procedure, Lang::EN>(pro_itA->Name());
        ss << " | " << Absent<Procedure, Lang::EN>() << '\n';
        ++pro_itA;
    }
    while (pro_itB not_eq listB.end()) {
        ss << std::setw(35) << Absent<Procedure, Lang::CN>() << " | "
           << Present<Procedure, Lang::EN>(pro_itB->Name()) << '\n';
        ++pro_itB;
    }
    if (ss.str().size() == size_before) {
        ss << "\t一致\n";
    }
    diff << ss.str();
}

void compare::Compare(std::vector<Column> const& listA,
                      std::vector<Column> const& listB,
                      std::stringstream&         diff) {
    std::stringstream ss{ "\x1B[1mComparing Tables: \x1B[0m\n" };
    auto const        size_before{ ss.str().size() };
    auto              col_itA{ listA.begin() }, col_itB{ listB.begin() };
    while (col_itA not_eq listA.end() and col_itB not_eq listB.end()) {
        auto const seqA_name{ col_itA->Name() }, seqB_name{ col_itB->Name() };
        if (seqA_name == seqB_name) {
            ++col_itA;
            ++col_itB;
        } else if (string_less{}(seqA_name, seqB_name)) {
            ss << std::setw(35) << Present<Column, Lang::EN>(seqA_name);
            ss << " | " << Absent<Column, Lang::EN>() << '\n';
            ++col_itA;
        } else if (string_less{}(seqB_name, seqA_name)) {
            ss << std::setw(35) << Absent<Column, Lang::EN>() << " | "
               << Present<Column, Lang::EN>(seqB_name) << '\n';
            ++col_itB;
        }
    }
    while (col_itA not_eq listA.end()) {
        ss << std::setw(35) << Present<Column, Lang::EN>(col_itA->Name());
        ss << " | " << Absent<Column, Lang::EN>() << '\n';
        ++col_itA;
    }
    while (col_itB not_eq listB.end()) {
        ss << std::setw(35) << Absent<Column, Lang::CN>() << " | "
           << Present<Column, Lang::EN>(col_itB->Name()) << '\n';
        ++col_itB;
    }
    if (ss.str().size() == size_before) {
        ss << "\t一致\n";
    }
    diff << ss.str();
}
void compare::Compare(std::vector<Index> const& listA,
                      std::vector<Index> const& listB,
                      std::stringstream&        diff) {
    std::stringstream ss{ "\x1B[1mComparing Tables: \x1B[0m\n" };
    auto const        size_before{ ss.str().size() };
    auto              idx_itA{ listA.begin() }, idx_itB{ listB.begin() };
    while (idx_itA not_eq listA.end() and idx_itB not_eq listB.end()) {
        auto const idxA_name{ idx_itA->Name() }, idxB_name{ idx_itB->Name() };
        if (idxA_name == idxB_name) {
            ++idx_itA;
            ++idx_itB;
        } else if (string_less{}(idxA_name, idxB_name)) {
            ss << std::setw(35) << Present<Index, Lang::EN>(idxA_name);
            ss << " | " << Absent<Index, Lang::EN>() << '\n';
            ++idx_itA;
        } else if (string_less{}(idxB_name, idxA_name)) {
            ss << std::setw(35) << Absent<Index, Lang::EN>() << " | "
               << Present<Index, Lang::EN>(idxB_name) << '\n';
            ++idx_itB;
        }
    }
    while (idx_itA not_eq listA.end()) {
        ss << std::setw(35) << Present<Index, Lang::EN>(idx_itA->Name());
        ss << " | " << Absent<Index, Lang::EN>() << '\n';
        ++idx_itA;
    }
    while (idx_itB not_eq listB.end()) {
        ss << std::setw(35) << Absent<Index, Lang::CN>() << " | "
           << Present<Index, Lang::EN>(idx_itB->Name()) << '\n';
        ++idx_itB;
    }
    if (ss.str().size() == size_before) {
        ss << "\t一致\n";
    }
    diff << ss.str();
}
void compare::Compare(std::vector<Trigger> const& listA,
                      std::vector<Trigger> const& listB,
                      std::stringstream&          diff) {
    std::stringstream ss{ "\x1B[1mComparing Tables: \x1B[0m\n" };
    auto const        size_before{ ss.str().size() };
    auto              tgr_itA{ listA.begin() }, tgr_itB{ listB.begin() };
    while (tgr_itA not_eq listA.end() and tgr_itB not_eq listB.end()) {
        auto const tgrA_name{ tgr_itA->Name() }, tgrB_name{ tgr_itB->Name() };
        if (tgrA_name == tgrB_name) {
            ++tgr_itA;
            ++tgr_itB;
        } else if (string_less{}(tgrA_name, tgrB_name)) {
            ss << std::setw(35) << Present<Trigger, Lang::EN>(tgrA_name);
            ss << " | " << Absent<Trigger, Lang::EN>() << '\n';
            ++tgr_itA;
        } else if (string_less{}(tgrB_name, tgrA_name)) {
            ss << std::setw(35) << Absent<Trigger, Lang::EN>() << " | "
               << Present<Trigger, Lang::EN>(tgrB_name) << '\n';
            ++tgr_itB;
        }
    }
    while (tgr_itA not_eq listA.end()) {
        ss << std::setw(35) << Present<Trigger, Lang::EN>(tgr_itA->Name());
        ss << " | " << Absent<Trigger, Lang::EN>() << '\n';
        ++tgr_itA;
    }
    while (tgr_itB not_eq listB.end()) {
        ss << std::setw(35) << Absent<Trigger, Lang::CN>() << " | "
           << Present<Trigger, Lang::EN>(tgr_itB->Name()) << '\n';
        ++tgr_itB;
    }
    if (ss.str().size() == size_before) {
        ss << "\t一致\n";
    }
    diff << ss.str();
}
