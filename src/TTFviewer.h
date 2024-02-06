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
