AUTOMAKE_OPTIONS = foreign
ACLOCAL_AMFLAGS = -I m4
AM_CPPFLAGS = $(BOOST_CPPFLAGS)
AM_CXXFLAGS=-std=gnu++11
AM_LDFLAGS = $(BOOST_LDFLAGS)  $(BOOST_FILESYSTEM_LIB) $(BOOST_SYSTEM_LIB)
bin_PROGRAMS = bamToFreq
bamToFreq_SOURCES = bamReader.cpp CodonData.cpp FreqMatrix.cpp ReadData.cpp
bamToFreq_LDADD = -lhts -lboost_filesystem -lboost_system
