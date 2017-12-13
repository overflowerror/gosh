AC_INIT(gosh, 0.1)
AC_LANG(C)

AC_PROG_CC
AC_PROG_CC_STDC

AC_CHECK_HEADERS(editline/readline.h, [], [AC_ERROR([A working libedit is required])])
AC_SEARCH_LIBS(readline, edit, [], [AC_ERROR([A working libedit is required])])

AC_OUTPUT(Makefile)
