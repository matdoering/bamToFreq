AUTOMAKE_OPTIONS = foreign
ACLOCAL_AMFLAGS = -I m4
AM_CPPFLAGS = $(BOOST_CPPFLAGS)
AM_LDFLAGS = $(BOOST_LDFLAGS)  $(BOOST_FILESYSTEM_LIB) $(BOOST_SYSTEM_LIB)
bin_PROGRAMS = bamToFreq
LIBADD = hts boost_filesystem boost_system
bamToFreq_SOURCES = bamReader.cpp CodonData.cpp FreqMatrix.cpp ReadData.cpp
