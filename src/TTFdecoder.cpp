/**
 * @file TTFdecoder.cpp
 * @brief 解码TTF数据
 * @version 1.0
 * @date 2020-04-14
 */
#include <QList>
#include <QFile>
#include <QFileInfo>
#include <QDataStream>
#include <QtEndian>
#include <QSvgRenderer>
#include <QXmlStreamReader>
#include <QPixmap>
#include <QPainter>
#include <QDebug>
#include "TTFdecoder.h"
#include "font_to_svg.hpp"

QMap<QString, ttfdecoder_t> TTF2RGB::ttfdecoder_map =
{
    {"ttf",            TTF2RGB::ttf},
    {"ttf_verbose",    TTF2RGB::ttf_verbose},
};

QList<SvgInfo> TTF2RGB::ttf(QString ttffilename,int W, int H, int codepoint)
{
    return ttf_decode(ttffilename,W,H,codepoint,false);
}

QList<SvgInfo> TTF2RGB::ttf_verbose(QString ttffilename,int W, int H, int codepoint)
{
    return ttf_decode(ttffilename,W,H,codepoint,true);
}

std::string TTF2RGB::ttf_glyph_out(void *glyph, bool verbose)
{
    font2svg::glyph *g = static_cast<font2svg::glyph *>(glyph);
    if(verbose) 
        return g->svgheader()      +
               g->svgborder()      +
               g->svgtransform()   +
               g->axes()           +
               g->typography_box() +
               g->points()         +
               g->pointlines()     +
               g->outline()        +
               g->labelpts()       +
               g->svgfooter();
    else 
        return g->svgheader()      +
               g->svgtransform()   +
               g->outline()        +
               g->svgfooter();
}

QList<SvgInfo> TTF2RGB::ttf_decode(QString ttffilename,int W, int H, int codepoint,bool verbose)
{
    QList<SvgInfo> rgbImglist;

    if(codepoint == -1) {
        font2svg::ttf_file tfile(ttffilename.toStdString());
        for(int index = 0;index<=0x10FFFF;index++) {
            font2svg::glyph g(tfile, index);
            if(g.glyph_index != 0) {
                SvgInfo info;
                info.src = new QString(QString::fromStdString(ttf_glyph_out(&g,verbose)));
                info.H = H;
                info.W = W;
                info.codepoint = index;
                rgbImglist.append(info);
            }
        }
        tfile.free();
    }
    else
    {
        font2svg::glyph g(ttffilename.toStdString(), codepoint );
        SvgInfo info;
        info.src = new QString(QString::fromStdString(ttf_glyph_out(&g,verbose)));
        info.H = H;
        info.W = W;
        info.codepoint = codepoint;
        rgbImglist.append(info);
        g.free();
    }

    return rgbImglist;
}
