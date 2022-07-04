[![CI](https://github.com/QQxiaoming/TTFviewer/actions/workflows/ci.yml/badge.svg?branch=main)](https://github.com/QQxiaoming/TTFviewer/actions/workflows/ci.yml)
[![CodeFactor](https://www.codefactor.io/repository/github/qqxiaoming/ttfviewer/badge)](https://www.codefactor.io/repository/github/qqxiaoming/ttfviewer)
[![GitHub tag (latest SemVer)](https://img.shields.io/github/tag/QQxiaoming/TTFviewer.svg)](https://github.com/QQxiaoming/TTFviewer/releases)
[![GitHub All Releases](https://img.shields.io/github/downloads/QQxiaoming/TTFviewer/total.svg)](https://github.com/QQxiaoming/TTFviewer/releases)
[![GitHub stars](https://img.shields.io/github/stars/QQxiaoming/TTFviewer.svg)](https://github.com/QQxiaoming/TTFviewer)

# TTFviewer

English | [简体中文](./README_zh_CN.md)

TTFviewer is a small tool for developers to view and preview various ttf font/icon image formats. It is based on Qt, and some of the code comes from the project: https://github.com/donbright/font_to_svg and https://github.com/QQxiaoming/YUVviewer , supports windows/linux/macos, the interface is simple and clear, and the operation is portable. The main interface is as follows:

![img0](./img/docimg0.png)

## Feature

1. Select parameters on the main interface.
2. Click Open File or Folder to parse the image data and display the image.
3. Used in the image display interface
    - Scroll wheel to zoom in and out of the image,
    - Use the left button to drag the image,
    - Double click the left button to save the image as svg format,
    - Right click to reset image size and position,
    - Middle click to display the original size of the image.

## Format

Currently supported formats include:

ttf\ttf_verbose

## Build

　> [Build documentation](./DEVELOPNOTE.md)
