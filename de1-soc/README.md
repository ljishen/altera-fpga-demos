# QT Application Demos for DE1-SoC Board

The `hello` and `ControlPanel` applications are built with VNC capabilities.

### Usage
- Decompress `qt-4.8.6-altera-soc.tar.bz2` into `/usr/local/` (so that you will have directory `/usr/local/qt-4.8.6-altera-soc/`)
- Run application with `<APPLICATION> -qws -display VNC:x` (repace `x` with the number so that the VNC connection port runs at 590x).

### Reference
http://doc.qt.io/qt-4.8/qt-embedded-vnc.html
