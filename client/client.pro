QT += network widgets

HEADERS += \
    client.h \
    thread.h

SOURCES += \
    client.cpp \
    main.cpp \
    thread.cpp

# install
target.path = $$[QT_INSTALL_EXAMPLES]/network/fortuneclient
INSTALLS += target
