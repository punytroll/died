SUBDIRS = \
	DiED \
	Network

INCLUDES = \
	@GLIBMM_CFLAGS@
	
bin_PROGRAMS = \
	died
	
died_SOURCES = \
	Common.cxx \
	DiED.cxx
	
died_CXXFLAGS = \
	-Wall -O2 -g
	
died_LDFLAGS = \
	@GLIBMM_LIBS@
	
died_LDADD = \
	DiED/libdied.a \
	Network/libnetwork.a

EXTRA_DIST = \
	Common.h
