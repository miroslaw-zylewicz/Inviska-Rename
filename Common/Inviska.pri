DEFINES += \
    QT_DEPRECATED_WARNINGS \
    APP_VERSION=\"\\\"$${VERSION}\\\"\" \
    APP_NAME=\"\\\"$${NAME}\\\"\" \
    APP_NAME_NO_SPACES=\"\\\"$${NAME_NO_SPACES}\\\"\"


CONFIG(release, debug|release):DEFINES += QT_NO_DEBUG_OUTPUT


win32 {
    TARGET = $$NAME_NO_SPACES
    QMAKE_TARGET_PRODUCT = $$NAME
    QMAKE_TARGET_DESCRIPTION = $$NAME
    QMAKE_TARGET_COMPANY = "Inviska Software"
    QMAKE_TARGET_COPYRIGHT = "Released under the GPLv2 licence"
    RC_LANG = 0x0809
    RC_CODEPAGE = 1252
    RC_ICONS = ./Resources/Icon.ico
    QMAKE_LFLAGS *= -static-libgcc -static-libstdc++ -static -lpthread
    QT += winextras
}


macx {
    TARGET = $$NAME
    ICON = ./Resources/Icon.icns
    QMAKE_INFO_PLIST = ./Resources/Info.plist
    QMAKE_CFLAGS_RELEASE *= -fvisibility=hidden
    QMAKE_CXXFLAGS_RELEASE *= -fvisibility=hidden -fvisibility-inlines-hidden
}


unix:!macx {
    TARGET = $$LINUX_TARGET
    QMAKE_CPPFLAGS *= -Wdate-time -D_FORTIFY_SOURCE=2
    QMAKE_CFLAGS   *= -fPIE -g -O2 -fdebug-prefix-map=/home/user=. -fstack-protector-strong -Wformat -Werror=format-security
    QMAKE_CXXFLAGS *= -fPIE -g -O2 -fdebug-prefix-map=/home/user=. -fstack-protector-strong -Wformat -Werror=format-security
    QMAKE_LFLAGS   *= -pie -Wl,-Bsymbolic-functions -Wl,-z,relro,-z,now
}


# For building release from command line - qmake CONFIG+=BuildRelease
contains(CONFIG, BuildRelease) {
    message("Building the RELEASE Version")
    CONFIG -= debug_and_release
    CONFIG -= debug
    CONFIG += release

    OUTPUTDIR = release
    DESTDIR = $$OUTPUTDIR
    OBJECTS_DIR = $$OUTPUTDIR
    MOC_DIR = $$OUTPUTDIR
    RCC_DIR = $$OUTPUTDIR
    UI_DIR = $$OUTPUTDIR
}


# For building debug from command line - qmake CONFIG+=BuildDebug
contains(CONFIG, BuildDebug) {
    message("Building the DEBUG Version")
    CONFIG -= debug_and_release
    CONFIG -= release
    CONFIG += debug

    OUTPUTDIR = debug
    DESTDIR = $$OUTPUTDIR
    OBJECTS_DIR = $$OUTPUTDIR
    MOC_DIR = $$OUTPUTDIR
    RCC_DIR = $$OUTPUTDIR
    UI_DIR = $$OUTPUTDIR
}
