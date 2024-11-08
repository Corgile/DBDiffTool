/// @file Comparator.hh
/// @author xianghongli\@hikvision.com.cn
/// @date 2024-11-08.
/// HangZhou HikVision Digital Technology Co., Ltd. All Right Reserved.
#pragma once
struct string_comp {
    bool operator()(std::string_view a, std::string_view b) const {
        /// 必须与 SQL 语句 ORDER BY LENGTH(xxx), xxx 效果一致
        return a.size() < b.size() or (a.size() == b.size() and a < b);
    }
};
