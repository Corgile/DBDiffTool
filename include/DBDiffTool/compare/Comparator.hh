/// @file Comparator.hh
/// @author xianghongli\@hikvision.com.cn
/// @date 2024-11-08.
/// HangZhou HikVision Digital Technology Co., Ltd. All Right Reserved.
#pragma once
struct string_comp {
    bool operator()(std::string_view a, std::string_view b) const {
        /// ������ SQL ��� ORDER BY LENGTH(xxx), xxx Ч��һ��
        return a.size() < b.size() or (a.size() == b.size() and a < b);
    }
};
