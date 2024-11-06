//
// DBDiffTool / Macros.hh
// Created by bryant on 2024 Nov 06.
//

#ifndef MACROS_HH
#define MACROS_HH

#define ND [[nodiscard]]

#define HANDLE_ERROR(conn)                                                     \
    do {                                                                       \
        int         ec;                                                        \
        std::string em;                                                        \
        conn->GetLastError(ec, em);                                            \
        std::fprintf(stderr, "Execute Error: %d - %s\n", ec, em.c_str());      \
    } while (false)

#define ENSURE_QUERY(conn, sql)                                                \
    if (not conn->ExecuteQuery(sql)) {                                         \
        HANDLE_ERROR(conn);                                                    \
    }

#define ENSURE_AFFECT(conn, sql, affected)                                     \
    if (not conn->ExecuteAffect(sql, affected)) {                              \
        HANDLE_ERROR(conn);                                                    \
    }
#endif // MACROS_HH
