# Pty-Qt

Qt/C++ 对WinPty、ConPty和标准Unix Pty的封装易用库。从 https://github.com/kafeg/ptyqt fork 而来，为了自己的一些开源项目（[quardCRT](https://github.com/QQxiaoming/quardCRT.git)）而修改。复用了大量原始代码但同时也大量修改了很多，因此只能作为一个单独的项目存在。

主要修改如下：

- 原项目在windows上使用conpty的情况下存在当终端输出大量数据时程序主进程卡死情况，现在参考qt-creator源码中部分代码，拣选修改解决了这个问题。
- 完善（增加）了启动PTY设置WorkDir、Environment功能，三种pty均可以使用。
- 增加了hasChildProcess、processInfoTree接口，三种pty均可以使用。
- conpty增加了setResizeQuirk接口，用于适配是否支持window conhost resizequirk行为。
- 增加qmake构建支持，通过 include(./lib/ptyqt.pri) 引入即可，极为方便通过源码引入其他项目。

一些注意：

- 原始项目使用cmake构建，但由于个人需要，我使用了qmake构建，因此在lib目录增加了ptyqt.pri文件，可以直接使用qmake构建，原本的cmake构建未做修补无法使用因此删除。
- 在Qt6.6.1上测试通过。
- 本项目完全遵守原始项目的LICENSE，修改新增的代码也遵守原始项目的LICENSE。

以下为原始的README：

[README.md](./README-ptyqt.md)
