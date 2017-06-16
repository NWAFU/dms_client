TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
SOURCES += src/main.cpp \
    src/client.cpp \
    src/log_reader.cpp \
    src/socket_sender.cpp \
    src/client_exception.cpp \
    src/backup_exception.cpp \
    src/read_exception.cpp \
    src/socket_exception.cpp \
    src/save_exception.cpp \
    src/send_exception.cpp \
    src/log_sender.cpp \
    src/data.cpp \
    src/console_sender.cpp
HEADERS += header/client_exception.h \
    header/client.h \
    header/data.h \
    header/log_reader.h \
    header/log_sender.h \
    header/socket_sender.h \
    header/console_sender.h \
    header/backup_exception.h \
    header/read_exception.h \
    header/socket_exception.h \
    header/save_exception.h \
    header/send_exception.h

OTHER_FILES += \
    script/backup.sh \
    script/cleanup_file.sh
