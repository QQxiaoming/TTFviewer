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

class TTFviewer : public QMainWindow {
    Q_OBJECT

public:
    explicit TTFviewer(QWidget *parent = nullptr);
    ~TTFviewer();
    static const QString VERSION;
    static const QString GIT_TAG;

protected:
    void closeEvent(QCloseEvent *event);

private slots:
    void configComboBox();
    void configOther();
    void changeFrameSizeType(const QString &text);
    void frameSizeHeightValidator(const QString &currentText);
    void frameSizeWidthValidator(const QString &currentText);
    void frameCodePiontValidator(const QString &currentText);
    void exchaneSize();
    void openFile();
    void openFolder();
    void help();
    void about();
    void aboutQt();

private:
    QString svgBoxSrc(int x, int y, int w, uint64_t c);
    QString svgBoxArraySrc(int x, int y, int w, int od, int xn, int yn,QList<uint64_t> ca);
    void updateUiSvg(QList<uint64_t> color_list);
    void showParaErrMessageBox(void);
    bool updateConfig(void);
    bool imgView(QStringList openfile_list);
    bool isCodePiontValidate(const QString &str,int *codepoint=nullptr);
    Ui::TTFviewer *ui;
    ConfigFile *TTFviewerConfigFile;
    ImgViewer *imgViewer;
    static const QList<QPair<QString, QStringList>> frameSizeTypeDict;
};

#endif // TTFviewer_H
