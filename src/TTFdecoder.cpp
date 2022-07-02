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
#include "TTFdecoder.h"
#include "font_to_svg.hpp"

QMap<QString, ttfdecoder_t> TTF2RGB::ttfdecoder_map =
{
    {"ttf",            TTF2RGB::ttf},
};

QList<QPixmap> TTF2RGB::ttf(QString ttffilename,int W, int H, int codepoint)
{
    QList<QPixmap> rgbImglist;
    QString svgImg;
    QByteArray naBA = ttffilename.toLatin1();

    font2svg::glyph g(naBA.constData(), codepoint );
    svgImg = QString::fromStdString(
                 g.svgheader() +
                 g.svgborder() +
                 g.svgtransform() +
                 g.axes() +
                 g.typography_box() +
                 g.points() +
                 g.pointlines() +
                 g.outline() +
                 g.labelpts() +
                 g.svgfooter());
    g.free();
    QXmlStreamReader svgXmlStreamReader(svgImg);
    QSvgRenderer svgRender;
    svgRender.load(&svgXmlStreamReader);
    QPixmap svgPixmap(W,H);
    svgPixmap.fill(Qt::transparent);
    QPainter svgPainter(&svgPixmap);
    svgRender.render(&svgPainter);

    rgbImglist.append(svgPixmap);
    return rgbImglist;
}
