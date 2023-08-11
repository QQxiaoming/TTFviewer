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
#include <QDir>
#include <QFile>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include "configFile.h"

ConfigFile::ConfigFile(const QString &path) :
    configFilePath(path) {
    config_dict.lastPath = "";
    config_dict.frameSizeType = "Other";
    config_dict.TTFFormat = "ttf";
    config_dict.frameSize_Width = "2560";
    config_dict.frameSize_Height = "1920";
    config_dict.frameCodePiont = "*";

    QFileInfo fileInfo(configFilePath);
    if(!fileInfo.isFile()) {
        QFile file(configFilePath);
        file.open(QFile::WriteOnly | QFile::Text);
        QXmlStreamWriter writer(&file);
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
    } else {
        QFile file(configFilePath);
        file.open(QFile::ReadOnly | QFile::Text);
        QXmlStreamReader reader(&file);

        while(!reader.atEnd()) {
            if(reader.isStartElement()) {
                if(reader.name().toString() == "lastPath") {
                    config_dict.lastPath = reader.readElementText();
                } else if(reader.name().toString() == "frameSizeType") {
                    config_dict.frameSizeType = reader.readElementText();
                } else if(reader.name().toString() == "TTFFormat") {
                    config_dict.TTFFormat = reader.readElementText();
                } else if(reader.name().toString() == "frameSize_Width") {
                    config_dict.frameSize_Width = reader.readElementText();
                } else if(reader.name().toString() == "frameSize_Height") {
                    config_dict.frameSize_Height = reader.readElementText();
                } else if(reader.name().toString() == "frameCodePiont") {
                    config_dict.frameCodePiont = reader.readElementText();
                }
            }
            reader.readNext();
        }
        file.close();
    }
}

ConfigFile::~ConfigFile() {
    QFile file(configFilePath);
    file.open(QFile::ReadWrite | QFile::Text);
    file.resize(0);

    QXmlStreamWriter writer(&file);
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
