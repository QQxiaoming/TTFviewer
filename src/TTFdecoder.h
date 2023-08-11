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
#ifndef TTFDECODER_H
#define TTFDECODER_H

#include <QString>
#include <QList>
#include <QMap>


class ImageDecoder {
public:
    class SvgInfo {
    public:
        bool operator==(const SvgInfo &l) const {
            if((l.src == this->src)&&(l.W == this->W)&&(l.H == this->H)&&(l.codepoint == this->codepoint)) return true;
            else return false;
        }
        QString *src;
        int W;
        int H;
        int codepoint;
    };
    typedef QList<SvgInfo> (* ttfdecoder_t)(const QString &ttffilename,int W, int H, int codepoint);
    static QList<SvgInfo> ttf(const QString &ttffilename,int W, int H, int codepoint);
    static QList<SvgInfo> ttf_verbose(const QString &ttffilename,int W, int H, int codepoint);
    static QMap<QString, ttfdecoder_t> ttfdecoder_map;
private:
    static std::string ttf_glyph_out(void *glyph, bool verbose);
    static QList<SvgInfo> ttf_decode(QString ttffilename,int W, int H, int codepoint,bool verbose);
};


#endif // TTFDECODER_H
