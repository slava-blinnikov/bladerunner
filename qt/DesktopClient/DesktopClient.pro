QT		+=  widgets xml network webkitwidgets
TEMPLATE		=   app
TARGET			=   blrview
MOC_DIR		=   ./moc
RCC_DIR			=   ./moc
OBJECTS_DIR	=   ./objects
DESTDIR     	=   ./release
CONFIG          +=  silent

HEADERS += ../MetaTypes/MetaTypes.h \
	MainWindow.h \
	ProgramConfig.h \
	Resources.h \
	TCPClient.h \
	Types.h \
	MemberWidget.h \
    MapView.h \
    ConfigDialog.h

SOURCES +=  ../MetaTypes/MetaTypes.cpp main.cpp \
	MainWindow.cpp \
	ProgramConfig.cpp \
	Resources.cpp \
	TCPClient.cpp \
	MemberWidget.cpp \
    MapView.cpp \
    ConfigDialog.cpp

RESOURCES += resources.qrc
