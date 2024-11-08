/// @file AnsiColor.hh
/// @author xianghongli\@hikvision.com.cn
/// @date 2024-11-08.
/// HangZhou HikVision Digital Technology Co., Ltd. All Right Reserved.
#pragma once

#ifndef ANSI_COLOR_HH
#define ANSI_COLOR_HH
#if defined(ANSI_COLOR_ON)

#define ESC "\x1B"

#define RST  ESC "[0m"
#define BLK  RST ESC "[30m"   // ��ɫ
#define RED  RST ESC "[31m"   // ��ɫ
#define GRN  RST ESC "[32m"   // ��ɫ
#define YLW  RST ESC "[33m"   // ��ɫ
#define BLU  RST ESC "[34m"   // ��ɫ
#define PRP  RST ESC "[35m"   // ��ɫ
#define CYN  RST ESC "[36m"   // ��ɫ
#define WHT  RST ESC "[37m"   // ��ɫ
#define BLKB RST ESC "[1;30m" // ��ɫ��������
#define REDB RST ESC "[1;31m" // ��ɫ��������
#define GRNB RST ESC "[1;32m" // ��ɫ��������
#define YLWB RST ESC "[1;33m" // ��ɫ��������
#define BLUB RST ESC "[1;34m" // ��ɫ��������
#define PRPB RST ESC "[1;35m" // ��ɫ��������
#define CYNB RST ESC "[1;36m" // ��ɫ��������
#define WHTB RST ESC "[1;37m" // ��ɫ��������

#define BOLD RST ESC "[1m" // ���ø����ȣ��Ӵ֣�
#define GLOW RST ESC "[2m" // ���õ�����
#define UNDL RST ESC "[4m" // �»���
#define BLNK RST ESC "[5m" // ��˸
#define RVSE RST ESC "[7m" // ����
#define HIDE RST ESC "[8m" // ����
#define DELT RST ESC "[9m" // ����

#else

#define ESC     ""
#define RST     ""
#define BLK     ""
#define RED     ""
#define GRN     ""
#define YLW     ""
#define BLU     ""
#define PRP     ""
#define CYN     ""
#define WHT     ""
#define BLKB    ""
#define REDB    ""
#define GRNB    ""
#define YLWB    ""
#define BLUB    ""
#define PRPB    ""
#define CYNB    ""
#define WHTB    ""
#define BOLD    ""
#define GLOW    ""
#define UNDL    ""
#define BLNK    ""
#define RVSE    ""
#define HIDE    ""
#define DELT    ""
#endif
#endif