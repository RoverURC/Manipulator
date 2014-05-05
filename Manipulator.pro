#-------------------------------------------------
#
# Project created by QtCreator 2014-03-03T19:31:08
#
#-------------------------------------------------

QT       += core network

QT       -= gui

TARGET = Manipulator
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    modbusserver.cpp \
    manipulator.cpp \
    servodriver.cpp \
    bcm2835.cpp \
    motor.cpp \
    dynamixelservo.cpp \
    ledindicator.cpp

HEADERS += \
    modbusserver.h \
    modbus.h \
    manipulator.h \
    servodriver.h \
    codes.h \
    bcm2835.h \
    motor.h \
    dynamixelservo.h \
    ledindicator.h

include(/home/pi/ExtSerialPort/qextserialport.pri)
CONFIG += extserialport
