SUBDIRS = \
	DiED \
	GUI \
	Network

INCLUDES = \
	@GTKMM_CFLAGS@
	
bin_PROGRAMS = \
	died
	
died_SOURCES = \
	Common.cxx \
	DiED.cxx
	
died_CXXFLAGS = \
	-Wall \
	-O0 \
	-g
	
died_LDFLAGS = \
	@GTKMM_LIBS@
	
died_LDADD = \
	DiED/libdied.a \
	GUI/libgui.a \
	Network/libnetwork.a

EXTRA_DIST = \
	Common.h
