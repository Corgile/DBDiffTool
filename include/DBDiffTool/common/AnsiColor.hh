/// @file AnsiColor.hh
/// @author xianghongli\@hikvision.com.cn
/// @date 2024-11-08.
/// HangZhou HikVision Digital Technology Co., Ltd. All Right Reserved.
#pragma once

#ifndef ANSI_COLOR_HH
#define ANSI_COLOR_HH

#define ESC "\x1B"

#define RST  ESC "[0m"
#define BLK  RST ESC "[30m"   // 黑色
#define RED  RST ESC "[31m"   // 红色
#define GRN  RST ESC "[32m"   // 绿色
#define YLW  RST ESC "[33m"   // 黄色
#define BLU  RST ESC "[34m"   // 蓝色
#define PRP  RST ESC "[35m"   // 紫色
#define CYN  RST ESC "[36m"   // 青色
#define WHT  RST ESC "[37m"   // 白色
#define BLKB RST ESC "[1;30m" // 黑色（加亮）
#define REDB RST ESC "[1;31m" // 红色（加亮）
#define GRNB RST ESC "[1;32m" // 绿色（加亮）
#define YLWB RST ESC "[1;33m" // 黄色（加亮）
#define BLUB RST ESC "[1;34m" // 蓝色（加亮）
#define PRPB RST ESC "[1;35m" // 紫色（加亮）
#define CYNB RST ESC "[1;36m" // 青色（加亮）
#define WHTB RST ESC "[1;37m" // 白色（加亮）

#define BOLD RST ESC "[1m" // 设置高亮度（加粗）
// #define G ESC "[2m" // 设置低亮度
// #define U ESC "[4m" // 下划线
// #define S ESC "[5m" // 闪烁
// #define R ESC "[7m" // 反显
// #define H ESC "[8m" // 消隐
// #define D ESC "[9m" // 划掉

#endif