[![Windows ci](https://img.shields.io/github/actions/workflow/status/qqxiaoming/ttfviewer/windows.yml?branch=main&logo=data:image/svg+xml;base64,PHN2ZyByb2xlPSJpbWciIHZpZXdCb3g9IjAgMCAyNCAyNCIgeG1sbnM9Imh0dHA6Ly93d3cudzMub3JnLzIwMDAvc3ZnIj48dGl0bGU+V2luZG93czwvdGl0bGU+PHBhdGggZD0iTTAsMEgxMS4zNzdWMTEuMzcySDBaTTEyLjYyMywwSDI0VjExLjM3MkgxMi42MjNaTTAsMTIuNjIzSDExLjM3N1YyNEgwWm0xMi42MjMsMEgyNFYyNEgxMi42MjMiIGZpbGw9IiNmZmZmZmYiLz48L3N2Zz4=)](https://github.com/QQxiaoming/ttfviewer/actions/workflows/windows.yml)
[![Linux ci](https://img.shields.io/github/actions/workflow/status/qqxiaoming/ttfviewer/linux.yml?branch=main&logo=linux&logoColor=white)](https://github.com/QQxiaoming/ttfviewer/actions/workflows/linux.yml)
[![Macos ci](https://img.shields.io/github/actions/workflow/status/qqxiaoming/ttfviewer/macos.yml?branch=main&logo=apple)](https://github.com/QQxiaoming/ttfviewer/actions/workflows/macos.yml)
[![CodeFactor](https://img.shields.io/codefactor/grade/github/qqxiaoming/ttfviewer.svg?logo=codefactor)](https://www.codefactor.io/repository/github/qqxiaoming/ttfviewer)
[![License](https://img.shields.io/github/license/qqxiaoming/ttfviewer.svg?colorB=f48041&logo=gnu)](https://github.com/QQxiaoming/ttfviewer)
[![GitHub tag (latest SemVer)](https://img.shields.io/github/tag/QQxiaoming/TTFviewer.svg?logo=git)](https://github.com/QQxiaoming/TTFviewer/releases)
[![GitHub All Releases](https://img.shields.io/github/downloads/QQxiaoming/TTFviewer/total.svg?logo=pinboard)](https://github.com/QQxiaoming/TTFviewer/releases)
[![GitHub stars](https://img.shields.io/github/stars/QQxiaoming/TTFviewer.svg?logo=github)](https://github.com/QQxiaoming/TTFviewer)
[![GitHub forks](https://img.shields.io/github/forks/QQxiaoming/TTFviewer.svg?logo=github)](https://github.com/QQxiaoming/TTFviewer)
[![Gitee stars](https://gitee.com/QQxiaoming/TTFviewer/badge/star.svg?theme=dark)](https://gitee.com/QQxiaoming/TTFviewer)
[![Gitee forks](https://gitee.com/QQxiaoming/TTFviewer/badge/fork.svg?theme=dark)](https://gitee.com/QQxiaoming/TTFviewer)

# TTFviewer

🇺🇸 English | [🇨🇳 简体中文](./README_zh_CN.md)

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

### codepoint parameters

The following legal inputs exist for the codepoint parameter:

1. Enter a single character, and ttfviewer will display the image corresponding to the character's unicode in the target ttf file. Example:

    ![img11](./img/docimg11.png)![img10](./img/docimg10.png)

    ![img31](./img/docimg31.png)![img30](./img/docimg30.png)

    ![img51](./img/docimg51.png)![img50](./img/docimg50.png)


2. Enter a hexadecimal unicode code value starting with 0x or 0X, and ttfviewer will display the image corresponding to the unicode code value in the target ttf file. Example:

    ![img21](./img/docimg21.png)![img20](./img/docimg20.png)

    ![img41](./img/docimg41.png)![img40](./img/docimg40.png)

    ![img61](./img/docimg61.png)![img60](./img/docimg60.png)

3. Enter the wildcard *, at this time ttfviewer will search for all image displays in the target ttf file (may consume more system memory), click left and right to switch browsing. Example:

    ![img74](./img/docimg74.png)
    
    ![img70](./img/docimg70.png)![img71](./img/docimg71.png)![img72](./img/docimg72.png)![img73](./img/docimg73.png)

4. Enter double wildcards *, then ttfviewer will display the wildcard itself (ie: 0x2a) corresponding to the image in the target ttf file. Example:

    ![img81](./img/docimg81.png)![img80](./img/docimg80.png)

## Format

Currently supported formats include:

ttf\ttf_verbose

## Build

　> [Build documentation](./DEVELOPNOTE.md)

## Contributing

If you have suggestions or ideas for this project, please submit issues and pull requests on GitHub or Gitee.

At present, the project has been completely migrated to Qt6 development, it is recommended to use version Qt6.5.0 or higher.
