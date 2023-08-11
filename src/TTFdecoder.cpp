/*
 * This file is part of the https://github.com/QQxiaoming/TTFviewer.git
 * project.
 *
 * Copyright (C) 2022 Quard <2014500726@smail.xtu.edu.cn>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see <http://www.gnu.org/licenses/>.
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

QMap<QString, ImageDecoder::ttfdecoder_t> ImageDecoder::ttfdecoder_map = {
    {"ttf",            ImageDecoder::ttf},
    {"ttf_verbose",    ImageDecoder::ttf_verbose},
};

QList<ImageDecoder::SvgInfo> ImageDecoder::ttf(const QString &ttffilename,int W, int H, int codepoint) {
    return ttf_decode(ttffilename,W,H,codepoint,false);
}

QList<ImageDecoder::SvgInfo> ImageDecoder::ttf_verbose(const QString &ttffilename,int W, int H, int codepoint) {
    return ttf_decode(ttffilename,W,H,codepoint,true);
}

std::string ImageDecoder::ttf_glyph_out(void *glyph, bool verbose) {
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

QList<ImageDecoder::SvgInfo> ImageDecoder::ttf_decode(QString ttffilename,int W, int H, int codepoint,bool verbose) {
    QList<SvgInfo> rgbImglist;

    if(codepoint == -1) {
        font2svg::ttf_file tfile(ttffilename.toStdString());
        for(int index = 0;index<=0x10FFFF;index++) {
            font2svg::glyph g(tfile, index, false);
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
    } else {
        font2svg::glyph g(ttffilename.toStdString(), codepoint, true );
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
