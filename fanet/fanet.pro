TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt
TARGET = fanet

INCLUDEPATH += $$(NS3DIR)/build
INCLUDEPATH += $$PWD/../

SOURCES += \
    ffnet.cc

#Other
HEADERS += \
    $$PWD/../utils/script.h

SOURCES += \
    $$PWD/../utils/script.cc

message ($$INCLUDEPATH)
