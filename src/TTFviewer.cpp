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
#include <QApplication>
#include <QMessageBox>
#include <QFileDialog>
#include <QToolTip>
#include <QPoint>
#include <QScreen>
#include <QDebug>
#include <QString>
#include <QSvgRenderer>
#include <QXmlStreamReader>
#include <QTranslator>
#include <QLibraryInfo>
#include <iostream>
#include "TTFviewer.h"
#include "ui_UI_TTFviewer.h"

const QString TTFviewer::VERSION = APP_VERSION;
const QString TTFviewer::GIT_TAG =
#include <git_tag.inc>
;

const QList<QPair<QString, QStringList>> TTFviewer::frameSizeTypeDict = {
    {"QQCIF",   {"88","72"}},
    {"QQVGA",   {"160","120"}},
    {"QCIF",    {"176","144"}},
    {"HQVGA",   {"240","160"}},
    {"QVGA",    {"320","240"}},
    {"CIF",     {"352","288"}},
    {"WQVGA",   {"400","240"}},
    {"HVGA",    {"480","320"}},
    {"nHD",     {"640","340"}},
    {"VGA",     {"640","480"}},
    {"WVGA",    {"800","480"}},
    {"SVGA",    {"800","600"}},
    {"qHD",     {"960","540"}},
    {"DVGA",    {"960","640"}},
    {"XGA",     {"1024","768"}},
    {"HD",      {"1280","720"}},
    {"UVGA",    {"1280","960"}},
    {"SXGA",    {"1280","1024"}},
    {"HD+",     {"1600","900"}},
    {"UXGA",    {"1600","1200"}},
    {"FHD",     {"1920","1080"}},
    {"WUXGA",   {"1920","1200"}},
    {"FHD+",    {"2160","1440"}},
    {"QXGA",    {"2048","1536"}},
    {"QHD",     {"2560","1440"}},
    {"WQXGA",   {"2560","1600"}},
    {"QSXGA",   {"2560","2048"}},
    {"QHD+",    {"3200","1800"}},
    {"QUXGA",   {"3200","2400"}},
    {"4K UHD",  {"3840","2160"}},
    {"8K UHD",  {"7680","4320"}},
};

TTFviewer::TTFviewer(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::TTFviewer) {
    ui->setupUi(this);

    this->setWindowTitle("TTFviewer " + VERSION);
    QRect screen = QGuiApplication::screenAt(this->mapToGlobal(QPoint(this->width()/2,0)))->geometry();
    QRect size = this->geometry();
    this->move((screen.width() - size.width()) / 2, (screen.height() - size.height()) / 2);

    ui->frameSizeType_ComboBox->setStyleSheet("combobox-popup: 0;");
    ui->TTFFormat_ComboBox->setStyleSheet("combobox-popup: 0;");

    QList<QPair<QString, QStringList>>::const_iterator config_it = frameSizeTypeDict.begin();
    while (config_it != frameSizeTypeDict.end()) {
        ui->frameSizeType_ComboBox->insertItem(ui->frameSizeType_ComboBox->count(),config_it->first);
        config_it++;
    }

    QFileInfo dir(QDir::homePath()+"/.TTFviewer");
    if(!dir.isDir()) {
        if(!dir.isFile()) {
            QDir mkdir(QDir::homePath());
            mkdir.mkdir(".TTFviewer");
        }
    }

    TTFviewerConfigFile = new ConfigFile(QDir::homePath()+"/.TTFviewer/TTFviewer.xml");
    if(TTFviewerConfigFile->config_dict.frameSizeType == "Other") {
        ui->frameSizeType_Other_RadioButton->setChecked(true);
        ui->frameSizeType_ComboBox->setEnabled(false);
        ui->frameSize_Width_LineEdit->setText(TTFviewerConfigFile->config_dict.frameSize_Width);
        ui->frameSize_Height_LineEdit->setText(TTFviewerConfigFile->config_dict.frameSize_Height);
        ui->frameCodePiont_LineEdit->setText(TTFviewerConfigFile->config_dict.frameCodePiont);
    } else {
        ui->frameSizeType_Combo_RadioButton->setChecked(true);
        ui->frameSizeType_ComboBox->setEnabled(true);
        ui->frameCodePiont_LineEdit->setText(TTFviewerConfigFile->config_dict.frameCodePiont);
        QList<QPair<QString, QStringList>>::const_iterator config_it = frameSizeTypeDict.begin();
        while (config_it != frameSizeTypeDict.end()) {
            if(config_it->first == TTFviewerConfigFile->config_dict.frameSizeType) {
                QStringList value = config_it->second;
                ui->frameSizeType_ComboBox->setCurrentText(TTFviewerConfigFile->config_dict.frameSizeType);
                ui->frameSize_Width_LineEdit->setText(value[0]);
                ui->frameSize_Width_LineEdit->setFocusPolicy(Qt::NoFocus);
                TTFviewerConfigFile->config_dict.frameSize_Width = value[0];
                ui->frameSize_Height_LineEdit->setText(value[1]);
                ui->frameSize_Height_LineEdit->setFocusPolicy(Qt::NoFocus);
                TTFviewerConfigFile->config_dict.frameSize_Height = value[1];
                break;
            }
            config_it++;
        }
    }

    QObject::connect(ui->frameSizeType_Combo_RadioButton, SIGNAL(clicked()), this, SLOT(configComboBox()));
    QObject::connect(ui->frameSizeType_Other_RadioButton, SIGNAL(clicked()), this, SLOT(configOther()));
    QObject::connect(ui->frameSizeType_ComboBox, SIGNAL(currentTextChanged(const QString &)), 
                                                    this,SLOT(changeFrameSizeType(const QString &)));

    QObject::connect(ui->frameSize_Height_LineEdit, SIGNAL(textEdited(const QString &)), 
                                                    this, SLOT(frameSizeHeightValidator(const QString &)));
    QObject::connect(ui->frameSize_Width_LineEdit, SIGNAL(textEdited(const QString &)), 
                                                    this, SLOT(frameSizeWidthValidator(const QString &)));
    QObject::connect(ui->frameCodePiont_LineEdit, SIGNAL(textEdited(const QString &)), 
                                                    this, SLOT(frameCodePiontValidator(const QString &)));

    QObject::connect(ui->exchange_PushButton, SIGNAL(clicked()), this, SLOT(exchaneSize()));
    QObject::connect(ui->openFile_PushButton, SIGNAL(clicked()), this, SLOT(openFile()));
    QObject::connect(ui->openFolder_PushButton, SIGNAL(clicked()), this, SLOT(openFolder()));
    QObject::connect(ui->help_PushButton, SIGNAL(clicked()), this, SLOT(help()));
    QObject::connect(ui->about_PushButton, SIGNAL(clicked()), this, SLOT(about()));
    QObject::connect(ui->aboutQt_PushButton, SIGNAL(clicked()), this, SLOT(aboutQt()));

    imgViewer = nullptr;
}

TTFviewer::~TTFviewer() {
    if(imgViewer != nullptr) {
        delete imgViewer;
    }
    if(TTFviewerConfigFile != nullptr) {
        delete TTFviewerConfigFile;
    }
    delete ui;
}

void TTFviewer::configComboBox() {
    ui->frameSizeType_ComboBox->setEnabled(true);
    QList<QPair<QString, QStringList>>::const_iterator config_it = frameSizeTypeDict.begin();
    while (config_it != frameSizeTypeDict.end()) {
        if(config_it->first == ui->frameSizeType_ComboBox->currentText()) {
            QStringList value = config_it->second;
            ui->frameSize_Width_LineEdit->setText(value[0]);
            ui->frameSize_Width_LineEdit->setFocusPolicy(Qt::NoFocus);
            ui->frameSize_Height_LineEdit->setText(value[1]);
            ui->frameSize_Height_LineEdit->setFocusPolicy(Qt::NoFocus);
            break;
        }
        config_it++;
    }
}

void TTFviewer::changeFrameSizeType(const QString &text) {
    QList<QPair<QString, QStringList>>::const_iterator config_it = frameSizeTypeDict.begin();
    while (config_it != frameSizeTypeDict.end()) {
        if(config_it->first == text) {
            QStringList value = config_it->second;
            ui->frameSize_Width_LineEdit->setText(value[0]);
            ui->frameSize_Width_LineEdit->setFocusPolicy(Qt::NoFocus);
            ui->frameSize_Height_LineEdit->setText(value[1]);
            ui->frameSize_Height_LineEdit->setFocusPolicy(Qt::NoFocus);
            break;
        }
        config_it++;
    }
}

void TTFviewer::configOther() {
    ui->frameSizeType_ComboBox->setEnabled(false);
    ui->frameSize_Width_LineEdit->setText(TTFviewerConfigFile->config_dict.frameSize_Width);
    ui->frameSize_Height_LineEdit->setText(TTFviewerConfigFile->config_dict.frameSize_Height);
    ui->frameSize_Width_LineEdit->setFocusPolicy(Qt::StrongFocus);
    ui->frameSize_Height_LineEdit->setFocusPolicy(Qt::StrongFocus);
}

void TTFviewer::frameSizeHeightValidator(const QString &currentText) {
    bool isInt;
    int currentVale = currentText.toInt(&isInt);
    if(isInt == true) {
        if(((currentVale%2) == 0) && (currentVale > 0)) {
            ui->frameSize_Height_LineEdit->setStyleSheet("QLineEdit{border:1px solid gray border-radius:1px}");
        } else {
            QToolTip::showText(ui->frameSize_Height_LineEdit->mapToGlobal(
                QPoint(0, 10)), "Height must be positive even");
            ui->frameSize_Height_LineEdit->setStyleSheet("QLineEdit{border: 1px solid red;border-radius: 3px;}");
        }
    } else {
        QToolTip::showText(ui->frameSize_Height_LineEdit->mapToGlobal(
            QPoint(0, 10)), "Height must be num");
        ui->frameSize_Height_LineEdit->setStyleSheet("QLineEdit{border: 1px solid red;border-radius: 3px;}");
    }
}

void TTFviewer::frameSizeWidthValidator(const QString &currentText)
{
    bool isInt;
    int currentVale = currentText.toInt(&isInt);
    if(isInt == true) {
        if(((currentVale%2) == 0) && (currentVale > 0)) {
            ui->frameSize_Width_LineEdit->setStyleSheet("QLineEdit{border:1px solid gray border-radius:1px}");
        } else {
            QToolTip::showText(ui->frameSize_Width_LineEdit->mapToGlobal(
                QPoint(0, 10)), "Width must be positive even");
            ui->frameSize_Width_LineEdit->setStyleSheet("QLineEdit{border: 1px solid red;border-radius: 3px;}");
        }
    } else {
        QToolTip::showText(ui->frameSize_Width_LineEdit->mapToGlobal(
            QPoint(0, 10)), "Width must be num");
        ui->frameSize_Width_LineEdit->setStyleSheet("QLineEdit{border: 1px solid red;border-radius: 3px;}");
    }
}

void TTFviewer::frameCodePiontValidator(const QString &currentText) {
    if(isCodePiontValidate(currentText)) {
        ui->frameCodePiont_LineEdit->setStyleSheet("QLineEdit{border:1px solid gray border-radius:1px}");
    } else {
        QToolTip::showText(ui->frameCodePiont_LineEdit->mapToGlobal(
            QPoint(0, 10)), "Codepoint must be 0x0-0x10FFFF or single char or * or **");
        ui->frameCodePiont_LineEdit->setStyleSheet("QLineEdit{border: 1px solid red;border-radius: 3px;}");
    }
}

void TTFviewer::exchaneSize() { 
    ui->frameSizeType_Other_RadioButton->setChecked(true);
    ui->frameSizeType_ComboBox->setEnabled(false);
    QString width = ui->frameSize_Width_LineEdit->text();
    ui->frameSize_Width_LineEdit->setText(ui->frameSize_Height_LineEdit->text());
    ui->frameSize_Height_LineEdit->setText(width);
    frameSizeWidthValidator(ui->frameSize_Width_LineEdit->text());
    frameSizeHeightValidator(ui->frameSize_Height_LineEdit->text());
}

void TTFviewer::showParaErrMessageBox(void) {
    QMessageBox::critical(this, "Error", "parameter invalid!!", QMessageBox::Ok);
}

bool TTFviewer::isCodePiontValidate(const QString &str,int *codepoint) {
    bool isInt;
    if((str.size() == 1)&&(str.at(0) == '*')) {
        if(codepoint) *codepoint = -1;
        return true;
    }
    int vale = str.toInt(&isInt, 16);
    if((isInt == true)&&(str.size() > 2)&&(str.at(0) == '0')&&((str.at(1) == 'x')||(str.at(1) == 'X'))) {
        if((vale >= 0) && (vale <= 0x10FFFF)) {
            if(codepoint) *codepoint = vale;
            return true;
        } else {
            return false;
        }
    } else {
        if(str.size() == 1) {
            vale = *(int *)str.unicode();
            if((vale >= 0) && (vale <= 0x10FFFF)) {
                if(codepoint) *codepoint = vale;
                return true;
            } else {
                return false;
            }
        } else if((str.size() == 2)&&(str.at(0) == '*')&&(str.at(1) == '*')) {
            if(codepoint) *codepoint = 0x2a;
            return true;
        } else {
            return false;
        }
    }
}

bool TTFviewer::updateConfig(void)
{
    bool isInt;
    int temp_Width = ui->frameSize_Width_LineEdit->text().toInt(&isInt);
    if(!isInt) {
        showParaErrMessageBox();
        return false;
    }
    int temp_Height = ui->frameSize_Height_LineEdit->text().toInt(&isInt);
    if(!isInt) {
        showParaErrMessageBox();
        return false;
    }
    QString temp_CodePiontQStr = ui->frameCodePiont_LineEdit->text();
    if(!isCodePiontValidate(temp_CodePiontQStr)) {
        showParaErrMessageBox();
        return false;
    }

    if(((temp_Width % 2) == 0) && ((temp_Height % 2) == 0) && (temp_Width > 0) && (temp_Height > 0)) {
        if(ui->frameSizeType_Combo_RadioButton->isChecked()) {
            TTFviewerConfigFile->config_dict.frameSizeType = ui->frameSizeType_ComboBox->currentText();
        } else if (ui->frameSizeType_Other_RadioButton->isChecked()) {
            TTFviewerConfigFile->config_dict.frameSizeType = ui->frameSizeType_Other_RadioButton->text();
        }
        TTFviewerConfigFile->config_dict.TTFFormat = ui->TTFFormat_ComboBox->currentText();
        TTFviewerConfigFile->config_dict.frameSize_Width = ui->frameSize_Width_LineEdit->text();
        TTFviewerConfigFile->config_dict.frameSize_Height = ui->frameSize_Height_LineEdit->text();
        TTFviewerConfigFile->config_dict.frameCodePiont = ui->frameCodePiont_LineEdit->text();

        return true;
    } else {
        QMessageBox::critical(this, "Error", "frameSize invalid!!", QMessageBox::Ok);
        return false;
    }
}

bool TTFviewer::imgView(QStringList openfile_list)
{
    if (openfile_list.empty()) {
        return false;
    }
    if(imgViewer != nullptr) {
        delete imgViewer;
        imgViewer = nullptr;
    }
    imgViewer = new ImgViewer(nullptr,this);
    int frameSize_Width = ui->frameSize_Width_LineEdit->text().toInt();
    int frameSize_Height = ui->frameSize_Height_LineEdit->text().toInt();
    int frameCodePiont = -1;
    isCodePiontValidate(ui->frameCodePiont_LineEdit->text(),&frameCodePiont);
    #if 1
    // 多线程
    bool isSuccess = imgViewer->setFileList_multithreading(openfile_list,
                           TTFviewerConfigFile->config_dict.TTFFormat,
                           frameSize_Width,
                           frameSize_Height,
                           frameCodePiont
                           );
    #else
    // 单线程
    bool isSuccess = imgViewer->setFileList(openfile_list,
                           TTFviewerConfigFile->config_dict.TTFFormat,
                           frameSize_Width,
                           frameSize_Height,
                           frameCodePiont
                           );
    #endif
    if(!isSuccess) {
        QMessageBox::critical(this, "Error", "unsupport TTFFormat!!", QMessageBox::Ok);
        this->show();
        return false;
    }
    float fframeSize_Width = (float)frameSize_Width;
    float fframeSize_Height = (float)frameSize_Height;
    if(frameSize_Width > frameSize_Height) {
        imgViewer->resize(800, fframeSize_Height/fframeSize_Width*800.0);
    } else {
        imgViewer->resize(fframeSize_Width/fframeSize_Height*400.0, 400);
    }
    QRect screen = QGuiApplication::screenAt(this->mapToGlobal(QPoint(this->width()/2,0)))->geometry();
    QRect size = imgViewer->geometry();
    imgViewer->move((screen.width() - size.width()) / 2, (screen.height() - size.height()) / 2);
    this->hide();
    imgViewer->show();

    return true;
}

void TTFviewer::openFile() {
    if(updateConfig()) {
        QString openDir = "";
        QFileInfo lastPath(TTFviewerConfigFile->config_dict.lastPath);
        if(lastPath.isDir()) {
            openDir = TTFviewerConfigFile->config_dict.lastPath;
        }
        QStringList openfile_list = QFileDialog::getOpenFileNames(
                            this, "选择文件", openDir, "TTF files(*.ttf *.data *.raw)");
        if(openfile_list.size() != 0) {
            QFileInfo file(openfile_list[0]);
            TTFviewerConfigFile->config_dict.lastPath = file.absolutePath();
            imgView(openfile_list);
        }
    }
}

void TTFviewer::openFolder() {
    if(updateConfig()) {
        QString openDir = "";
        QFileInfo lastPath(TTFviewerConfigFile->config_dict.lastPath);
        if(lastPath.isDir()) {
            openDir = TTFviewerConfigFile->config_dict.lastPath;
        }
        QString openfolder_name = QFileDialog::getExistingDirectory(this, "选择文件夹", openDir);
        if (!openfolder_name.isEmpty()) {
            TTFviewerConfigFile->config_dict.lastPath = openfolder_name;
            QDir dir(openfolder_name);
            QStringList nameFilters = {"*.ttf","*.data","*.raw"};
            QStringList openfilename_list = dir.entryList(nameFilters, QDir::Files|QDir::Readable, QDir::Name);
            QStringList openfile_list;
            foreach (QString file_name, openfilename_list) {
                openfile_list.append(QDir::toNativeSeparators(openfolder_name + '/' +file_name));
            }
            if(openfile_list.size() != 0) {
                imgView(openfile_list);
            }
        }
    }
}

void TTFviewer::help()
{
    QMessageBox::question(this, "Help", tr(
        "1.Select parameters on the main interface.\n"
        "2.Click Open File or Folder to parse the image data and display the image.\n"
        "3.Used in the image display interface\n"
            "  - Scroll wheel to zoom in and out of the image,\n"
            "  - Use the left button to drag the image,\n"
            "  - Double click the left button to save the image as svg format,\n"
            "  - Right click to reset image size and position,\n"
            "  - Middle click to display the original size of the image."),
         QMessageBox::StandardButtons(QMessageBox::Ok));
}

void TTFviewer::about()
{
    QMessageBox::about(this, tr("About"),
        tr(
            "<p>Version</p>"
            "<p>&nbsp;%1</p>"
            "<p>Commit</p>"
            "<p>&nbsp;%2</p>"
            "<p>Author</p>"
            "<p>&nbsp;qiaoqm@aliyun.com</p>"
            "<p>Website</p>"
            "<p>&nbsp;<a href='https://github.com/QQxiaoming/TTFviewer'>https://github.com/QQxiaoming</p>"
            "<p>&nbsp;<a href='https://gitee.com/QQxiaoming/TTFviewer'>https://gitee.com/QQxiaoming</a></p>"
        ).arg(VERSION,GIT_TAG)
    );
}

void TTFviewer::aboutQt()
{
    QMessageBox::aboutQt(this);
}

void TTFviewer::closeEvent(QCloseEvent *event)
{
    if(TTFviewerConfigFile != nullptr) {
        delete TTFviewerConfigFile;
        TTFviewerConfigFile = nullptr;
    }
    event->accept();
}

static QTranslator qtTranslator;
static QTranslator qtbaseTranslator;
static QTranslator appTranslator;

int main(int argc, char *argv[]) {
    if(argc == 2) {
        if((!strncmp(argv[1],"--version",9)) | (!strncmp(argv[1],"-v",2)) ) {
            std::cout << "TTFviewer " << TTFviewer::VERSION.toStdString() << "\n" << TTFviewer::GIT_TAG.toStdString() << "\n";
            return 0;
        }
    }
    QApplication::setAttribute(Qt::AA_DontUseNativeDialogs);
    QApplication::setAttribute(Qt::AA_DontUseNativeMenuBar);
    QApplication::setAttribute(Qt::AA_DontCreateNativeWidgetSiblings);
    QApplication application(argc, argv);

    QLocale locale;
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    QString qlibpath = QLibraryInfo::path(QLibraryInfo::TranslationsPath);
#else
    QString qlibpath = QLibraryInfo::location(QLibraryInfo::TranslationsPath);
#endif
    QLocale::Language lang = locale.language();
    switch(lang) {
    case QLocale::Chinese:
        if(qtTranslator.load("qt_zh_CN.qm",qlibpath))
            application.installTranslator(&qtTranslator);
        if(qtbaseTranslator.load("qtbase_zh_CN.qm",qlibpath))
            application.installTranslator(&qtbaseTranslator);
        if(appTranslator.load(":/lang/lang/ttfviewer_zh_CN.qm"))
            application.installTranslator(&appTranslator);
        break;
    case QLocale::Japanese:
        if(qtTranslator.load("qt_ja.qm",qlibpath))
            application.installTranslator(&qtTranslator);
        if(qtbaseTranslator.load("qtbase_ja.qm",qlibpath))
            application.installTranslator(&qtbaseTranslator);
        if(appTranslator.load(":/lang/lang/ttfviewer_ja_JP.qm"))
            application.installTranslator(&appTranslator);
        break;
    default:
    case QLocale::English:
        if(qtTranslator.load("qt_en.qm",qlibpath))
            application.installTranslator(&qtTranslator);
        if(qtbaseTranslator.load("qtbase_en.qm",qlibpath))
            application.installTranslator(&qtbaseTranslator);
        if(appTranslator.load(":/lang/lang/ttfviewer_en_US.qm"))
            application.installTranslator(&appTranslator);
        break;
    }

    TTFviewer window;
    window.show();

    return application.exec();
}
