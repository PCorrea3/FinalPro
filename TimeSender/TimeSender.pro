QT += network widgets

# install
target.path = $$[QT_INSTALL_EXAMPLES]/network/fortuneclient
INSTALLS += target

HEADERS += \
    timer.h

SOURCES += \
    timer.cpp \
    main.cpp
