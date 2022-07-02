/**
 * @file ImgViewer.h
 * @brief 图像显示界面
 * @version 1.0
 * @date 2020-04-14
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

class TTFDecodeThread : public QThread
{
    Q_OBJECT

public:
    explicit TTFDecodeThread(QWidget *parent = nullptr,
                             QString ttffilename = nullptr,
                             QString TTFFormat = nullptr,
                             int W = 0, int H = 0, int codepoint = 0);

protected:
    void run();

signals:
    void finsh_signal(QList<QPixmap> frame_RGB_list,QString str);

private:
    QWidget *window;
    QString ttffilename;
    int W;
    int H;
    int codepoint;
    ttfdecoder_t decoder;
};

class ImgViewer : public QWidget
{
    Q_OBJECT

public:
    explicit ImgViewer(QWidget *parent = nullptr,QWidget *parentWindow = nullptr);
    ~ImgViewer();
    bool setFileList(QStringList filelist,QString TTFFormat, int W, int H, int codepoint);
    bool setFileList_multithreading(QStringList filenamelist, QString TTFFormat, int W, int H, int codepoint);

private slots:
    void reciveimgdata(QList<QPixmap> img_RGB_list, QString filename);
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
    void draw_img(QPainter *painter);
    Ui::ImgViewerWindow *ui;
    QWidget *parentWindow;
    bool left_click;

    QList<QList<QPixmap>> img_list;
    QStringList filelist;

    QList<TTFDecodeThread*> decode_thread;
    QList<TTFDecodeThread*> decode_thread_finsh;

    QList<QPixmap> currentImg_RGB_list;
    QPixmap currentImg;
    QPixmap scaled_img;
    QPoint point;
    QPoint startPos;
    QPoint endPos;
    bool flipRGB = false;
};

#endif // IMGVIEWER_H
