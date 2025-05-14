QT += widgets
TARGET = AirlineSystem
TEMPLATE = app

SOURCES += main.cpp \
           loginwindow.cpp \
           admindashboard.cpp \
           agentdashboard.cpp \
           passengerdashboard.cpp \
           mainwindow.cpp \
           systemdatabase.cpp \
           flight.cpp \
           aircraft.cpp\
           reservation.cpp

HEADERS += loginwindow.h \
           admindashboard.h \
           agentdashboard.h \
           passengerdashboard.h \
           mainwindow.h \
           systemdatabase.h \
           flight.h \
           aircraft.h\
           reservation.h

CONFIG += c++17