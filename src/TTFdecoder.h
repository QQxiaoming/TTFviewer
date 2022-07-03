/**
 * @file TTFdecoder.h
 * @brief 解码TTF数据
 * @version 1.0
 * @date 2020-04-14
 */
#ifndef TTFDECODER_H
#define TTFDECODER_H

#include <QString>
#include <QList>
#include <QMap>

typedef QList<QPixmap *> (* ttfdecoder_t)(QString ttffilename,int W, int H, int codepoint);

class TTF2RGB
{
public:
    static QList<QPixmap *> ttf(QString ttffilename,int W, int H, int codepoint);
    static QMap<QString, ttfdecoder_t> ttfdecoder_map;
};


#endif // TTFDECODER_H
