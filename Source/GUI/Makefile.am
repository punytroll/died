AM_CPPFLAGS = \
	@GTKMM_CFLAGS@ \
	-I..

noinst_LIBRARIES = \
	libgui.a

libgui_a_SOURCES = \
	Client.cxx \
	ClientFactory.cxx \
	MainWindow.cxx \
	MessageListStore.cxx
	
libgui_a_CXXFLAGS = \
	-std=c++17 \
	-Wall \
	-Werror \
	-O0 \
	-g

EXTRA_DIST = \
	Client.h \
	ClientFactory.h \
	MainWindow.h \
	MessageListStore.h
