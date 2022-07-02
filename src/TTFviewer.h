/**
 * @file TTFviewer.h
 * @brief 主界面
 * @version 1.0
 * @date 2020-04-14
 */
#ifndef TTFviewer_H
#define TTFviewer_H

#include <QMainWindow>
#include <QCloseEvent>
#include "ImgViewer.h"
#include "configFile.h"

namespace Ui {
class TTFviewer;
}

class TTFviewer : public QMainWindow
{
    Q_OBJECT

public:
    explicit TTFviewer(QWidget *parent = nullptr);
    ~TTFviewer();

protected:
    void closeEvent(QCloseEvent *event);

private slots:
    void changeFormat(const QString &text);
    void configComboBox();
    void configOther();
    void changeFrameSizeType(const QString &text);
    void frameSizeHeightValidator(const QString &currentText);
    void frameSizeWidthValidator(const QString &currentText);
    void exchaneSize();
    void openFile();
    void about();
    void help();

private:
    QString svgBoxSrc(int x, int y, int w, uint64_t c);
    QString svgBoxArraySrc(int x, int y, int w, int od, int xn, int yn,QList<uint64_t> ca);
    void updateUiSvg(QList<uint64_t> color_list);
    void showParaErrMessageBox(void);
    bool updateConfig(void);
    bool imgView(QStringList openfile_list);
    Ui::TTFviewer *ui;
    ConfigFile *TTFviewerConfigFile;
    ImgViewer *imgViewer;
    static const QList<QPair<QString, QStringList>> frameSizeTypeDict;
    static const QList<QPair<QString, QList<uint64_t>>> TTFFormat_list;
};

#endif // TTFviewer_H
