QT += core gui network widgets



greaterThan(QT_MAJOR_VERSION, 4): QT += widgets




CONFIG += c++17



#win32: LIBS += -L"C:/Qt/Tools/OpenSSLv3/Win_x64/lib" -lssl -lcrypto -lssleay32 -llibeay32

INCLUDEPATH += "C:/Qt/Tools/OpenSSLv3/Win_x64/include"
DEFINES += QT_USE_QSTRINGBUILDER
# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    foodfunctionality.cpp \
    main.cpp \
    mainwindow.cpp \
    product.cpp

HEADERS += \
    foodfunctionality.hh \
    mainwindow.hh \
    product.hh

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    gatherDataPython.py \
    productDataBase.csv \
    recipeDataBase.csv \
    searchResults.csv

