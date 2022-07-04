/**
 * @file TTFviewer.cpp
 * @brief 主界面
 * @version 1.0
 * @date 2020-04-14
 */
#include <QApplication>
#include <QDesktopWidget>
#include <QMessageBox>
#include <QFileDialog>
#include <QToolTip>
#include <QPoint>
#include <QScreen>
#include <QDebug>
#include <QString>
#include <QSvgRenderer>
#include <QXmlStreamReader>
#include <iostream>
#include "TTFviewer.h"
#include "ui_UI_TTFviewer.h"

const QString VERSION = APP_VERSION;
const QString GIT_TAG =
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
    QMainWindow(parent),
    ui(new Ui::TTFviewer)
{
    ui->setupUi(this);

    this->setWindowTitle("TTFviewer " + VERSION);
    QRect screen = QGuiApplication::screenAt(this->mapToGlobal({this->width()/2,0}))->geometry();
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
    if(TTFviewerConfigFile->config_dict.frameSizeType == "Other")
    {
        ui->frameSizeType_Other_RadioButton->setChecked(true);
        ui->frameSizeType_ComboBox->setEnabled(false);
        ui->frameSize_Width_LineEdit->setText(TTFviewerConfigFile->config_dict.frameSize_Width);
        ui->frameSize_Height_LineEdit->setText(TTFviewerConfigFile->config_dict.frameSize_Height);
        ui->frameCodePiont_LineEdit->setText(TTFviewerConfigFile->config_dict.frameCodePiont);
    }
    else
    {
        ui->frameSizeType_Combo_RadioButton->setChecked(true);
        ui->frameSizeType_ComboBox->setEnabled(true);
        ui->frameCodePiont_LineEdit->setText(TTFviewerConfigFile->config_dict.frameCodePiont);
        QList<QPair<QString, QStringList>>::const_iterator config_it = frameSizeTypeDict.begin();
        while (config_it != frameSizeTypeDict.end()) {
            if(config_it->first == TTFviewerConfigFile->config_dict.frameSizeType)
            {
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


    QList<uint64_t> color_list;


    QObject::connect(ui->frameSizeType_Combo_RadioButton, SIGNAL(clicked()), this, SLOT(configComboBox()));
    QObject::connect(ui->frameSizeType_Other_RadioButton, SIGNAL(clicked()), this, SLOT(configOther()));
    QObject::connect(ui->frameSizeType_ComboBox, SIGNAL(currentTextChanged(const QString &)), this,SLOT(changeFrameSizeType(const QString &)));

    QObject::connect(ui->frameSize_Height_LineEdit, SIGNAL(textEdited(const QString &)), this, SLOT(frameSizeHeightValidator(const QString &)));
    QObject::connect(ui->frameSize_Width_LineEdit, SIGNAL(textEdited(const QString &)), this, SLOT(frameSizeWidthValidator(const QString &)));
    QObject::connect(ui->frameCodePiont_LineEdit, SIGNAL(textEdited(const QString &)), this, SLOT(frameCodePiontValidator(const QString &)));

    QObject::connect(ui->exchange_PushButton, SIGNAL(clicked()), this, SLOT(exchaneSize()));
    QObject::connect(ui->openFile_PushButton, SIGNAL(clicked()), this, SLOT(openFile()));
    QObject::connect(ui->openFolder_PushButton, SIGNAL(clicked()), this, SLOT(openFolder()));
    QObject::connect(ui->about_PushButton, SIGNAL(clicked()), this, SLOT(about()));
    QObject::connect(ui->help_PushButton, SIGNAL(clicked()), this, SLOT(help()));
    imgViewer = nullptr;
}

TTFviewer::~TTFviewer()
{
    if(imgViewer != nullptr)
    {
        delete imgViewer;
    }
    if(TTFviewerConfigFile != nullptr)
    {
        delete TTFviewerConfigFile;
    }
    delete ui;
}

void TTFviewer::configComboBox()
{
    ui->frameSizeType_ComboBox->setEnabled(true);
    QList<QPair<QString, QStringList>>::const_iterator config_it = frameSizeTypeDict.begin();
    while (config_it != frameSizeTypeDict.end()) {
        if(config_it->first == ui->frameSizeType_ComboBox->currentText())
        {
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

void TTFviewer::changeFrameSizeType(const QString &text)
{
    QList<QPair<QString, QStringList>>::const_iterator config_it = frameSizeTypeDict.begin();
    while (config_it != frameSizeTypeDict.end()) {
        if(config_it->first == text)
        {
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

void TTFviewer::configOther()
{
    ui->frameSizeType_ComboBox->setEnabled(false);
    ui->frameSize_Width_LineEdit->setText(TTFviewerConfigFile->config_dict.frameSize_Width);
    ui->frameSize_Height_LineEdit->setText(TTFviewerConfigFile->config_dict.frameSize_Height);
    ui->frameSize_Width_LineEdit->setFocusPolicy(Qt::StrongFocus);
    ui->frameSize_Height_LineEdit->setFocusPolicy(Qt::StrongFocus);
}

void TTFviewer::frameSizeHeightValidator(const QString &currentText)
{
    bool isInt;
    int currentVale = currentText.toInt(&isInt);
    if(isInt == true)
    {
        if(((currentVale%2) == 0) && (currentVale > 0))
        {
            ui->frameSize_Height_LineEdit->setStyleSheet("QLineEdit{border:1px solid gray border-radius:1px}");
        }
        else
        {
            QToolTip::showText(ui->frameSize_Height_LineEdit->mapToGlobal(QPoint(0, 10)), "Height must be positive even");
            ui->frameSize_Height_LineEdit->setStyleSheet("QLineEdit{border: 1px solid red;border-radius: 3px;}");
        }
    }
    else
    {
        QToolTip::showText(ui->frameSize_Height_LineEdit->mapToGlobal(QPoint(0, 10)), "Height must be num");
        ui->frameSize_Height_LineEdit->setStyleSheet("QLineEdit{border: 1px solid red;border-radius: 3px;}");
    }
}

void TTFviewer::frameSizeWidthValidator(const QString &currentText)
{
    bool isInt;
    int currentVale = currentText.toInt(&isInt);
    if(isInt == true)
    {
        if(((currentVale%2) == 0) && (currentVale > 0))
        {
            ui->frameSize_Width_LineEdit->setStyleSheet("QLineEdit{border:1px solid gray border-radius:1px}");
        }
        else
        {
            QToolTip::showText(ui->frameSize_Width_LineEdit->mapToGlobal(QPoint(0, 10)), "Width must be positive even");
            ui->frameSize_Width_LineEdit->setStyleSheet("QLineEdit{border: 1px solid red;border-radius: 3px;}");
        }
    }
    else
    {
        QToolTip::showText(ui->frameSize_Width_LineEdit->mapToGlobal(QPoint(0, 10)), "Width must be num");
        ui->frameSize_Width_LineEdit->setStyleSheet("QLineEdit{border: 1px solid red;border-radius: 3px;}");
    }
}

void TTFviewer::frameCodePiontValidator(const QString &currentText)
{
    bool isInt;
    if((currentText.size() == 1)&&(currentText.at(0) == '*')) {
        ui->frameCodePiont_LineEdit->setStyleSheet("QLineEdit{border:1px solid gray border-radius:1px}");
        return;
    }
    int currentVale = currentText.toInt(&isInt, 16);
    if(isInt == true)
    {
        if((currentVale >= 0) && (currentVale <= 0x10FFFF))
        {
            ui->frameCodePiont_LineEdit->setStyleSheet("QLineEdit{border:1px solid gray border-radius:1px}");
        }
        else
        {
            QToolTip::showText(ui->frameCodePiont_LineEdit->mapToGlobal(QPoint(0, 10)), "CodePiont must be 0-0x10FFFF or *");
            ui->frameCodePiont_LineEdit->setStyleSheet("QLineEdit{border: 1px solid red;border-radius: 3px;}");
        }
    }
    else
    {
        QToolTip::showText(ui->frameCodePiont_LineEdit->mapToGlobal(QPoint(0, 10)), "Width must be num or *");
        ui->frameCodePiont_LineEdit->setStyleSheet("QLineEdit{border: 1px solid red;border-radius: 3px;}");
    }
}

void TTFviewer::exchaneSize() 
{ 
    ui->frameSizeType_Other_RadioButton->setChecked(true);
    ui->frameSizeType_ComboBox->setEnabled(false);
    QString width = ui->frameSize_Width_LineEdit->text();
    ui->frameSize_Width_LineEdit->setText(ui->frameSize_Height_LineEdit->text());
    ui->frameSize_Height_LineEdit->setText(width);
    frameSizeWidthValidator(ui->frameSize_Width_LineEdit->text());
    frameSizeHeightValidator(ui->frameSize_Height_LineEdit->text());
}

void TTFviewer::showParaErrMessageBox(void)
{
    QMessageBox::critical(this, "Error", "parameter invalid!!", QMessageBox::Ok);
}

bool TTFviewer::updateConfig(void)
{
    bool isInt;
    int temp_Width = ui->frameSize_Width_LineEdit->text().toInt(&isInt);
    if(!isInt)
    {
        showParaErrMessageBox();
        return false;
    }
    int temp_Height = ui->frameSize_Height_LineEdit->text().toInt(&isInt);
    if(!isInt)
    {
        showParaErrMessageBox();
        return false;
    }
    QString temp_CodePiontQStr = ui->frameCodePiont_LineEdit->text();
    int temp_CodePiont = temp_CodePiontQStr.toInt(&isInt,16);
    if(!isInt)
    {
        if(!((temp_CodePiontQStr.size() == 1)&&(temp_CodePiontQStr.at(0) == '*'))) 
        {
            showParaErrMessageBox();
            return false;
        }
    } 
    else 
    {
        if(!((temp_CodePiont >= 0) && (temp_CodePiont <= 0x10FFFF)))
        {
            showParaErrMessageBox();
            return false;
        }
    }

    if(((temp_Width % 2) == 0) && ((temp_Height % 2) == 0) && (temp_Width > 0) && (temp_Height > 0))
    {
        if(ui->frameSizeType_Combo_RadioButton->isChecked())
        {
            TTFviewerConfigFile->config_dict.frameSizeType = ui->frameSizeType_ComboBox->currentText();
        }
        else if (ui->frameSizeType_Other_RadioButton->isChecked())
        {
            TTFviewerConfigFile->config_dict.frameSizeType = ui->frameSizeType_Other_RadioButton->text();
        }
        TTFviewerConfigFile->config_dict.TTFFormat = ui->TTFFormat_ComboBox->currentText();
        TTFviewerConfigFile->config_dict.frameSize_Width = ui->frameSize_Width_LineEdit->text();
        TTFviewerConfigFile->config_dict.frameSize_Height = ui->frameSize_Height_LineEdit->text();
        TTFviewerConfigFile->config_dict.frameCodePiont = ui->frameCodePiont_LineEdit->text();

        return true;
    }
    else
    {
        QMessageBox::critical(this, "Error", "frameSize invalid!!", QMessageBox::Ok);
        return false;
    }
}

bool TTFviewer::imgView(QStringList openfile_list)
{
    if (openfile_list.empty())
    {
        return false;
    }
    if(imgViewer != nullptr)
    {
        delete imgViewer;
        imgViewer = nullptr;
    }
    imgViewer = new ImgViewer(nullptr,this);
    int frameSize_Width = ui->frameSize_Width_LineEdit->text().toInt();
    int frameSize_Height = ui->frameSize_Height_LineEdit->text().toInt();
    bool isInt;
    int frameCodePiont = ui->frameCodePiont_LineEdit->text().toInt(&isInt,16);
    if(!isInt) frameCodePiont = -1;
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
    if(!isSuccess)
    {
        QMessageBox::critical(this, "Error", "unsupport TTFFormat!!", QMessageBox::Ok);
        this->show();
        return false;
    }
    float fframeSize_Width = (float)frameSize_Width;
    float fframeSize_Height = (float)frameSize_Height;
    if(frameSize_Width > frameSize_Height)
    {
        imgViewer->resize(800, fframeSize_Height/fframeSize_Width*800.0);
    }
    else
    {
        imgViewer->resize(fframeSize_Width/fframeSize_Height*400.0, 400);
    }
    QRect screen = QGuiApplication::screenAt(this->mapToGlobal({this->width()/2,0}))->geometry();
    QRect size = imgViewer->geometry();
    imgViewer->move((screen.width() - size.width()) / 2, (screen.height() - size.height()) / 2);
    this->hide();
    imgViewer->show();

    return true;
}

void TTFviewer::openFile()
{
    if(updateConfig())
    {
        QString openDir = "";
        QFileInfo lastPath(TTFviewerConfigFile->config_dict.lastPath);
        if(lastPath.isDir())
        {
            openDir = TTFviewerConfigFile->config_dict.lastPath;
        }
        QStringList openfile_list = QFileDialog::getOpenFileNames(this, "选择文件", openDir, "TTF files(*.ttf *.data *.raw)");
        if(openfile_list.size() != 0)
        {
            QFileInfo file(openfile_list[0]);
            TTFviewerConfigFile->config_dict.lastPath = file.absolutePath();
            imgView(openfile_list);
        }
    }
}

void TTFviewer::openFolder()
{
    if(updateConfig())
    {
        QString openDir = "";
        QFileInfo lastPath(TTFviewerConfigFile->config_dict.lastPath);
        if(lastPath.isDir())
        {
            openDir = TTFviewerConfigFile->config_dict.lastPath;
        }
        QString openfolder_name = QFileDialog::getExistingDirectory(this, "选择文件夹", openDir);
        if (!openfolder_name.isEmpty())
        {
            TTFviewerConfigFile->config_dict.lastPath = openfolder_name;
            QDir dir(openfolder_name);
            QStringList nameFilters = {"*.ttf","*.data","*.raw"};
            QStringList openfilename_list = dir.entryList(nameFilters, QDir::Files|QDir::Readable, QDir::Name);
            QStringList openfile_list;
            foreach (QString file_name, openfilename_list)
            {
                openfile_list.append(QDir::toNativeSeparators(openfolder_name + '/' +file_name));
            }
            if(openfile_list.size() != 0)
            {
                imgView(openfile_list);
            }
        }
    }
}

void TTFviewer::about()
{
    QMessageBox::about(this, "About", "Version \n " + VERSION + "\nCommit \n " + GIT_TAG + "\nAuthor\n qiaoqm@aliyun.com \nWebsite\n https://github.com/QQxiaoming/TTFviewer");
}

void TTFviewer::help()
{
    QMessageBox::question(this, "Help", "1.主界面选择数据参数。\n2.点击打开文件或文件夹将进行图像数据解析并显示图像。\n3.图像显示界面中使用滚轮放大缩小图像，使用左键可拖动图像，双击左键保存图像为png格式，单击右键复位图像大小和位置，双击右键交换图像R和B通道显示，单击中键显示图像原始大小。", QMessageBox::Ok);
}

void TTFviewer::closeEvent(QCloseEvent *event)
{
    if(TTFviewerConfigFile != nullptr)
    {
        delete TTFviewerConfigFile;
        TTFviewerConfigFile = nullptr;
    }
    event->accept();
}

int main(int argc, char *argv[])
{
    if(argc == 2) {
        if((!strncmp(argv[1],"--version",9)) | (!strncmp(argv[1],"-v",2)) ) {
            std::cout << "TTFviewer " << VERSION.toStdString() << "\n" << GIT_TAG.toStdString() << "\n";
            return 0;
        }
    }
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
    QApplication::setAttribute(Qt::AA_DontUseNativeDialogs);
    QApplication::setAttribute(Qt::AA_DontUseNativeMenuBar);
    QApplication::setAttribute(Qt::AA_DontCreateNativeWidgetSiblings);
    QApplication application(argc, argv);
    QFont font = application.font();
    font.setFamily(font.defaultFamily());
    font.setPixelSize(13);
    application.setFont(font);
    TTFviewer window;
    window.show();

    return application.exec();
}
