TEMPLATE = app

QT = network xml sql

TARGET	= blrsrv

MOC_DIR		= ./moc
RCC_DIR			= ./moc
OBJECTS_DIR	= ./objects
DESTDIR			= ./release

HEADERS += ../MetaTypes/MetaTypes.h \
	ServerDaemon.h \
	TCPServer.h \
	Channel.h \
	ChannelWorker.h \
	Resources.h \
	DbConnection.h \
    DbScheduler.h \
    DbScheduleWorker.h

SOURCES += ../MetaTypes/MetaTypes.cpp main.cpp \
	ServerDaemon.cpp \
	TCPServer.cpp \
	Channel.cpp \
	ChannelWorker.cpp \
	Resources.cpp \
	DbConnection.cpp \
    DbScheduler.cpp \
    DbScheduleWorker.cpp

win32 {
	include(C:/Qt/solutions/qtservice/src/qtservice.pri)
}
else {
	include(/home/slava/Qt/solutions/qtservice/src/qtservice.pri)
}
