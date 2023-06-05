find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Quick Widgets Gui)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Quick Widgets Gui)
