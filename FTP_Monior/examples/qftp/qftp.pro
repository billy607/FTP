HEADERS       = ftpwindow.h \
    ConfigureManager.h \
    ConfigureReaderAndWriter.h \
    LoginPage.h \
    LogUtil.h \
    context/Context.h \
    context/UserAuthorize.h \

SOURCES       = ftpwindow.cpp \
                main.cpp \
    ConfigureManager.cpp \
    ConfigureReaderAndWriter.cpp \
    LoginPage.cpp \
    LogUtil.cpp \
    context/Context.cpp \
    context/UserAuthorize.cpp \

RESOURCES    += ftp.qrc
QT           += network ftp widgets

# install
target.path = $$[QT_INSTALL_EXAMPLES]/qtbase/network/qftp
sources.files = $$SOURCES $$HEADERS $$RESOURCES $$FORMS *.pro images
sources.path = $$[QT_INSTALL_EXAMPLES]/qtbase/network/qftp
INSTALLS += target sources

FORMS += \
    LoginPage.ui


win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../LabelingBase/release/ -lLabelingBase
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../LabelingBase/debug/ -lLabelingBase
else:unix: LIBS += -L$$PWD/../../../LabelingBase/ -lLabelingBase

INCLUDEPATH += $$PWD/../../../LabelingBase/
DEPENDPATH += $$PWD/../../../LabelingBase/debug

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../Dependency/libcurl/lib/ -llibcurl
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../Dependency/libcurl/lib/ -llibcurl
else:unix: LIBS += -L$$PWD/../../../Dependency/libcurl/lib/ -llibcurl

INCLUDEPATH += $$PWD/../../../Dependency/libcurl/include
DEPENDPATH += $$PWD/../../../Dependency/libcurl/lib

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../Dependency/openssl/lib/ -llibeay32
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../Dependency/openssl/lib/ -llibeay32
else:unix: LIBS += -L$$PWD/../../../Dependency/openssl/lib/ -llibeay32

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../Dependency/openssl/lib/ -lssleay32
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../Dependency/openssl/lib/ -lssleay32
else:unix: LIBS += -L$$PWD/../../../Dependency/openssl/lib/ -lssleay32

INCLUDEPATH += $$PWD/../../../Dependency/
DEPENDPATH += $$PWD/../../../Dependency/openssl/lib



win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../Dependency/quazip/ -lquazip
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../Dependency/quazip/ -lquazipd
else:unix: LIBS += -L$$PWD/../../../Dependency/quazip/ -lquazip

INCLUDEPATH += $$PWD/../../../Dependency/quazip/include
DEPENDPATH += $$PWD/../../../Dependency/quazip
