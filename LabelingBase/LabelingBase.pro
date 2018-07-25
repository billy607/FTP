#-------------------------------------------------
#
# Project created by QtCreator 2018-05-03T14:59:28
#
#-------------------------------------------------

QT       -= gui

QT += widgets


CONFIG   += c++11

TARGET = LabelingBase
TEMPLATE = lib

DEFINES += LABELINGBASE_LIBRARY

DEFINES += ZIP_STD
SOURCES += \
    LogUtil.cpp \
    LogEvent.cpp \
    context/ManagerContext.cpp \
    context/Context.cpp \
    context/ToolContext.cpp \
    crypto/Crypto.cpp \
    crypto/CryptoImpl.cpp \
    http/CURLUtil.cpp \
    http/HttpUtil.cpp \
    task/LabelingDispatch.cpp \
    task/OnlineTask.cpp \
    task/TaskManager.cpp \
    task/ToolInfomation.cpp \
    zip/ZipUtil.cpp \
    Scanner.cpp \
    task/LabelingData.cpp \
    task/LabelingSet.cpp \
    LabelingSpeciication.cpp \
    task/JobConfiguration.cpp \
    task/TaskStatistics.cpp \
    Log4Labeling.cpp \
    context/UserAuthorize.cpp


HEADERS +=\
        labelingbase_global.h \
    LogUtil.h \
    LogEvent.h \
    context/Context.h \
    context/ToolContext.h \
    context/ManagerContext.h \
    crypto/Crypto.h \
    crypto/CryptoImpl.h \
    http/CURLUtil.h \
    http/HttpUtil.h \
    task/Dispatch.h \
    task/LabelingSet.h \
    task/OnlineTask.h \
    task/TaskManager.h \
    task/ToolInfomation.h \
    Scanner.h \
    task/LabelingData.h \
    LabelingSpeciication.h \
    task/JobConfiguration.h \
    zip/ZipUtil.h \
    task/TaskStatistics.h \
    LabelingFieldNames.h \
    Log4Labeling.h \
    context/UserAuthorize.h


unix {
    target.path = /usr/lib
    INSTALLS += target
}

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../Dependency/libcurl/lib/ -llibcurl
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../Dependency/libcurl/lib/ -llibcurl
else:unix: LIBS += -L$$PWD/../Dependency/libcurl/lib/ -llibcurl

INCLUDEPATH += $$PWD/../Dependency/libcurl/include
DEPENDPATH += $$PWD/../Dependency/libcurl/lib

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../Dependency/openssl/lib/ -llibeay32
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../Dependency/openssl/lib/ -llibeay32
else:unix: LIBS += -L$$PWD/../Dependency/openssl/lib/ -llibeay32

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../Dependency/openssl/lib/ -lssleay32
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../Dependency/openssl/lib/ -lssleay32
else:unix: LIBS += -L$$PWD/../Dependency/openssl/lib/ -lssleay32

INCLUDEPATH += $$PWD/../Dependency/
DEPENDPATH += $$PWD/../Dependency/openssl/lib



win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../Dependency/quazip/ -lquazip
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../Dependency/quazip/ -lquazipd
else:unix: LIBS += -L$$PWD/../Dependency/quazip/ -lquazip

INCLUDEPATH += $$PWD/../Dependency/quazip/include
DEPENDPATH += $$PWD/../Dependency/quazip

#DEPENDPATH += . log4qt log4qt/helpers log4qt/spi log4qt/varia
#INCLUDEPATH += . log4qt log4qt/helpers log4qt/spi log4qt/varia

FORMS +=

#win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../Dependency/zlib-1.2.3/lib/ -lz
#else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../Dependency/zlib-1.2.3/lib/ -lz
#else:unix: LIBS += -L$$PWD/../Dependency/zlib-1.2.3/lib/ -lz

#INCLUDEPATH += $$PWD/../Dependency/zlib-1.2.3/include
#DEPENDPATH += $$PWD/../Dependency/zlib-1.2.3/lib

DISTFILES += \
    log4qt/log4qt.pri \
    log4qt/log4j.properties
