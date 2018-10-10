AM_CPPFLAGS = \
	-I.. \
	@GLIBMM_CFLAGS@

noinst_LIBRARIES = \
	libnetwork.a

libnetwork_a_SOURCES = \
	BasicBuffer.cxx \
	BasicMessage.cxx \
	BasicValue.cxx \
	BufferReader.cxx \
	BufferWriter.cxx \
	CircularBuffer.cxx \
	CircularBufferStd.cxx \
	MessageStream.cxx \
	Socket.cxx \
	Stream.cxx \
	StringValue.cxx
	
libnetwork_a_CXXFLAGS = \
	-std=c++17 \
	-Wall \
	-Werror \
	-O0 \
	-g

EXTRA_DIST = \
	BasicBuffer.h \
	BasicMessage.h \
	BasicReader.h \
	BasicValue.h \
	BasicWriter.h \
	BufferReader.h \
	BufferWriter.h \
	CircularBuffer.h \
	CircularBufferStd.h \
	MessageFactory.h \
	MessageStream.h \
	NotifyValue.h \
	Server.h \
	Socket.h \
	Stream.h \
	StringValue.h \
	Value.h \
	VectorValue.h
