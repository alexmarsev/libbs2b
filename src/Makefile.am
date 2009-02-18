lib_LTLIBRARIES = \
	libbs2b.la

bin_PROGRAMS = \
	bs2bconvert \
	bs2bstream


pkginclude_HEADERS = \
	bs2b.h \
	bs2bclass.h

libbs2b_la_SOURCES = \
	bs2b.c \
	bs2bclass.cpp

bs2bconvert_LDADD = \
	libbs2b.la

bs2bconvert_LDFLAGS = \
	-lsndfile

bs2bconvert_SOURCES = \
	bs2bconvert.c

bs2bstream_LDADD = \
	libbs2b.la

bs2bstream_SOURCES = \
	bs2bstream.c
