AC_INIT([bamToFreq], [0.1], [mdoering@mpi-inf.mpg.de])
AM_INIT_AUTOMAKE
AC_PROG_CXX
AC_CONFIG_MACRO_DIRS([m4])
AC_CHECK_LIB([hts], [sam_read1], [], 
   AC_MSG_ERROR([Could  not find hts lib])
)
AX_BOOST_BASE([1.41],[], 
   AC_MSG_ERROR([Could  not find boost lib])
)
AX_BOOST_SYSTEM
AX_BOOST_FILESYSTEM

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
