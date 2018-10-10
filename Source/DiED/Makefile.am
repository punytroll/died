AM_CPPFLAGS = \
	@GLIBMM_CFLAGS@ \
	-I..

noinst_LIBRARIES = \
	libdied.a

libdied_a_SOURCES = \
	BasicMessage.cxx \
	Client.cxx \
	ClientFactory.cxx \
	Common.cxx \
	EventAction.cxx \
	MessageFactory.cxx \
	Messages.cxx \
	Server.cxx \
	System.cxx \
	User.cxx
	
libdied_a_CXXFLAGS = \
	-std=c++17 \
	-Wall \
	-Werror \
	-O0 \
	-g

EXTRA_DIST = \
	ActionTarget.h \
	BasicMessage.h \
	Client.h \
	ClientFactory.h \
	Common.h \
	EventAction.h \
	ExternalEnvironment.h \
	InternalEnvironment.h \
	MessageFactory.h \
	Messages.h \
	MessageTarget.h \
	Server.h \
	System.h \
	User.h
