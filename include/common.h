#ifndef __COMMON_H__
#define __COMMON_H__
#pragma once
#include <QApplication>
#include "kvconf.h"
#include <string>
#include <vector>
#include <stdio.h>
#include <iostream>
#include <QMutex>
#include <QFile>
#include <QIODevice>
#include <QDateTime>
#include <QTextStream>

#define  FILE_TYPE_UNDEFIEND 0
#define  FILE_TYPE_IMAGE 1
#define  FILE_TYPE_VIDEO 2

struct DISPLAY_LIST{
    int cmd_cnt;
    QList<QString> cmd_list;
};
int getFile_type(const QString file);
DISPLAY_LIST get_normal_list();
DISPLAY_LIST get_face_list();
DISPLAY_LIST get_urgent_list();
void outputMessage(QtMsgType type, const QMessageLogContext &context, const QString &msg);
#endif