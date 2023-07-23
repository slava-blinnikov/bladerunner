QT		+=  widgets xml sql network
TEMPLATE	=   app
TARGET		=   blrtest
MOC_DIR	=   ./moc
RCC_DIR		=   ./moc
OBJECTS_DIR	=   ./objects
DESTDIR     	=   ./release
CONFIG          +=  silent

HEADERS +=  ../MetaTypes/MetaTypes.h \
	MainWindow.h \
	ProgramConfig.h \
	Resources.h \
	BlrClient.h \
	Types.h \
	TcpClient.h \
	NewConnectionDialog.h \
	ClientWidget.h \
	CrewWidget.h \
	MemberWidget.h \
	MembersTableModel.h \
	MemberItemDelegate.h \
	MembersTableView.h \
	CrewTcpClient.h \
	CrewClient.h \
	MemberTcpClient.h \
	MemberClient.h

SOURCES +=  ../MetaTypes/MetaTypes.cpp main.cpp \
	MainWindow.cpp \
	ProgramConfig.cpp \
	Resources.cpp \
	BlrClient.cpp \
	TcpClient.cpp \
	NewConnectionDialog.cpp \
	ClientWidget.cpp \
	CrewWidget.cpp \
	MemberWidget.cpp \
	MembersTableModel.cpp \
	MemberItemDelegate.cpp \
	MembersTableView.cpp \
	CrewTcpClient.cpp \
	CrewClient.cpp \
	MemberTcpClient.cpp \
	MemberClient.cpp

RESOURCES += resources.qrc
