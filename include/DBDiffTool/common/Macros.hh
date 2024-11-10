/// @file Macros.hh
/// @author xianghongli\@hikvision.com.cn
/// @date 2024-11-06.
/// HangZhou HikVision Digital Technology Co., Ltd. All Right Reserved.
#pragma once

#define ND [[nodiscard]]

#define HANDLE_ERROR(conn)                                                     \
    do {                                                                       \
        int         ec{};                                                      \
        std::string em{};                                                      \
        conn->GetLastError(ec, em);                                            \
        /*std::fprintf(stderr, "Execute error: %d - %s\n", ec, em.c_str());*/  \
    } while (false)

#define ENSURE_QUERY(conn, sql)                                                \
    if (not conn->ExecuteQuery(sql)) {                                         \
        HANDLE_ERROR(conn);                                                    \
    }

#define ENSURE_AFFECT(conn, sql, affected)                                     \
    if (not conn->ExecuteAffect(sql, affected)) {                              \
        HANDLE_ERROR(conn);                                                    \
    }
#define MAKE_SHARED std::make_shared<T>(std::forward<Args>(args)...)

#define INSTANCE(x) "DBDiffTool_" #x
