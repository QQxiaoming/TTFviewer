/*
 * This file is part of the https://github.com/QQxiaoming/YUVviewer.git
 * project.
 *
 * Copyright (C) 2020 Quard <2014500726@smail.xtu.edu.cn>
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
#ifndef IMGVIEWER_H
#define IMGVIEWER_H

#include <QWidget>
#include <QThread>
#include <QString>
#include <QImage>
#include <QPainter>
#include <QCloseEvent> 
#include <QPaintEvent> 
#include <QMouseEvent> 
#include <QWheelEvent> 
#include <QResizeEvent>
#include "TTFdecoder.h"

namespace Ui {
class ImgViewerWindow;
}

class TTFDecodeThread : public QThread {
    Q_OBJECT

public:
    explicit TTFDecodeThread(QWidget *parent = nullptr,
                             const QString &ttffilename = QString(),
                             QString TTFFormat = nullptr,
                             int W = 0, int H = 0, int codepoint = 0);

protected:
    void run();

signals:
    void finsh_signal(QList<ImageDecoder::SvgInfo> frame_RGB_list,QString str);

private:
    QWidget *window;
    QString ttffilename;
    int W;
    int H;
    int codepoint;
    ImageDecoder::ttfdecoder_t decoder;
};

class ImgViewer : public QWidget {
    Q_OBJECT

public:
    explicit ImgViewer(QWidget *parent = nullptr,QWidget *parentWindow = nullptr);
    ~ImgViewer();
    bool setFileList(QStringList filelist,QString TTFFormat, int W, int H, int codepoint);
    bool setFileList_multithreading(QStringList filenamelist, QString TTFFormat, int W, int H, int codepoint);

private slots:
    void reciveimgdata(QList<ImageDecoder::SvgInfo> img_RGB_list, QString filename);
    void previousImg();
    void nextImg();

protected:
    void closeEvent(QCloseEvent *event);
    void paintEvent(QPaintEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);
    void resizeEvent(QResizeEvent *event);

private:
    void currentImg2scaledImg(const ImageDecoder::SvgInfo &currentImg,QPixmap &QPixmap,const QSize &size);
    void draw_img(QPainter *painter);
    Ui::ImgViewerWindow *ui;
    QWidget *parentWindow;
    bool left_click;

    QList<QList<ImageDecoder::SvgInfo>> img_list;
    QStringList filelist;

    QList<TTFDecodeThread*> decode_thread;
    QList<TTFDecodeThread*> decode_thread_finsh;

    QList<ImageDecoder::SvgInfo> currentImg_RGB_list;
    ImageDecoder::SvgInfo currentImg;
    QPixmap scaled_img;
    QPoint point;
    QPoint startPos;
    QPoint endPos;
    bool flipRGB = false;
};

#endif // IMGVIEWER_H
