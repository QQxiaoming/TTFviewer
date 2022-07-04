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

class SvgInfo {
public:
    bool operator==(const SvgInfo &l) 
    {
        if((l.src == src)&&(l.W == W)&&(l.H == H)&&(l.codepoint == codepoint)) return true;
        else return false;
    }
    QString *src;
    int W;
    int H;
    int codepoint;
};

typedef QList<SvgInfo> (* ttfdecoder_t)(QString ttffilename,int W, int H, int codepoint);


class TTF2RGB
{
public:
    static QList<SvgInfo> ttf(QString ttffilename,int W, int H, int codepoint);
    static QMap<QString, ttfdecoder_t> ttfdecoder_map;
};


#endif // TTFDECODER_H
