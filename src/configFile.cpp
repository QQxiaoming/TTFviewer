/**
 * @file configFile.cpp
 * @brief 管理配置文件
 * @version 1.0
 * @date 2020-04-14
 */
#include <QDir>
#include <QFile>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include "configFile.h"

ConfigFile::ConfigFile(QString path)
{
    config_dict.lastPath = "";
    config_dict.frameSizeType = "Other";
    config_dict.TTFFormat = "ttf";
    config_dict.frameSize_Width = "2560";
    config_dict.frameSize_Height = "1920";
    config_dict.frameCodePiont = "*";

    configFilePath = path;

    QFileInfo fileInfo(configFilePath);
    if(!fileInfo.isFile())
    {
        QFile file(configFilePath);
        file.open(QFile::WriteOnly | QFile::Text);
        QXmlStreamWriter writer(&file);
        writer.setCodec("UTF-8");  // XML 编码
        writer.setAutoFormatting(true); // 自动格式化
        writer.writeStartDocument("1.0");  // 开始文档（XML 声明）
        writer.writeStartElement("config");
        writer.writeTextElement("lastPath", config_dict.lastPath);
        writer.writeTextElement("frameSizeType", config_dict.frameSizeType);
        writer.writeTextElement("TTFFormat", config_dict.TTFFormat);
        writer.writeTextElement("frameSize_Width", config_dict.frameSize_Width);
        writer.writeTextElement("frameSize_Height", config_dict.frameSize_Height);
        writer.writeTextElement("frameCodePiont", config_dict.frameCodePiont);
        writer.writeEndElement();
        writer.writeEndDocument();
        file.close();
    }
    else
    {
        QFile file(configFilePath);
        file.open(QFile::ReadOnly | QFile::Text);
        QXmlStreamReader reader(&file);

        while(!reader.atEnd())
        {
            if(reader.isStartElement())
            {
                if(reader.name() == "lastPath")
                {
                    config_dict.lastPath = reader.readElementText();
                }
                else if(reader.name() == "frameSizeType")
                {
                    config_dict.frameSizeType = reader.readElementText();
                }
                else if(reader.name() == "TTFFormat")
                {
                    config_dict.TTFFormat = reader.readElementText();
                }
                else if(reader.name() == "frameSize_Width")
                {
                    config_dict.frameSize_Width = reader.readElementText();
                }
                else if(reader.name() == "frameSize_Height")
                {
                    config_dict.frameSize_Height = reader.readElementText();
                }
                else if(reader.name() == "frameCodePiont")
                {
                    config_dict.frameCodePiont = reader.readElementText();
                }
            }
            reader.readNext();
        }
        file.close();
    }
}

ConfigFile::~ConfigFile()
{
    QFile file(configFilePath);
    file.open(QFile::ReadWrite | QFile::Text);

    QXmlStreamWriter writer(&file);
    writer.setCodec("UTF-8");  // XML 编码
    writer.setAutoFormatting(true); // 自动格式化
    writer.writeStartDocument("1.0");  // 开始文档（XML 声明）
    writer.writeStartElement("config");
    writer.writeTextElement("lastPath", config_dict.lastPath);
    writer.writeTextElement("frameSizeType", config_dict.frameSizeType);
    writer.writeTextElement("TTFFormat", config_dict.TTFFormat);
    writer.writeTextElement("frameSize_Width", config_dict.frameSize_Width);
    writer.writeTextElement("frameSize_Height", config_dict.frameSize_Height);
    writer.writeTextElement("frameCodePiont", config_dict.frameCodePiont);
    writer.writeEndElement();
    writer.writeEndDocument();
    file.close();
}
