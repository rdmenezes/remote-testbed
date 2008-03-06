# The boost library autoconf macros were taken from
#
#  http://autoconf-archive.cryp.to/ax_boost_program_options.html
#
# and the mysql client library autoconf macro was taken from:
#
#  http://autoconf-archive.cryp.to/ax_lib_mysql.html
#
# Both have the following license:
#
#  Copyright (c) 2007 Thomas Porschberg <thomas@randspringer.de>
#  Copyright (c) 2006 Mateusz Loskot <mateusz@loskot.net>
#
#  Copying and distribution of this file, with or without modification,
#  are permitted in any medium without royalty provided the copyright
#  notice and this notice are preserved.

# {{{ Boost program options

AC_DEFUN([AX_BOOST_PROGRAM_OPTIONS],
[
AC_ARG_WITH([boost-program-options],
	AS_HELP_STRING([--with-boost-program-options@<:@=special-lib@:>@],
		[use the program options library from boost - it is possible to specify a certain library for the linker
		e.g. --with-boost-program-options=boost_program_options-gcc-mt-1_33_1 ]),
	[
		if test "$withval" = "no"; then
			want_boost="no"
		elif test "$withval" = "yes"; then
			want_boost="yes"
			ax_boost_user_program_options_lib=""
		else
			want_boost="yes"
			ax_boost_user_program_options_lib="$withval"
		fi
	],
	[want_boost="yes"]
)

CONFIG_BOOST_PROGRAM_OPTIONS=no

if test "x$want_boost" = "xyes"; then
	AC_REQUIRE([AC_PROG_CC])
	export want_boost
	CPPFLAGS_SAVED="$CPPFLAGS"
	CPPFLAGS="$CPPFLAGS $BOOST_CPPFLAGS"
	export CPPFLAGS
	LDFLAGS_SAVED="$LDFLAGS"
	LDFLAGS="$LDFLAGS $BOOST_LDFLAGS"
	export LDFLAGS

	AC_CACHE_CHECK([whether the Boost::Program_Options library is available],
		ax_cv_boost_program_options,
		[AC_LANG_PUSH(C++)
		AC_COMPILE_IFELSE(AC_LANG_PROGRAM([[@%:@include <boost/program_options.hpp>
                                                  ]],
						  [[boost::program_options::options_description generic("Generic options")]]),
				  ax_cv_boost_program_options=yes,
				  ax_cv_boost_program_options=no)
		AC_LANG_POP([C++])
        ])
	if test "$ax_cv_boost_program_options" = yes; then
		AC_DEFINE(HAVE_BOOST_PROGRAM_OPTIONS,[1],[Define if the Boost::PROGRAM_OPTIONS library is available])
		BN=boost_program_options
		AC_LANG_PUSH(C++)
		if test "x$ax_boost_user_program_options_lib" = "x"; then
			for ax_lib in $BN $BN-$CC $BN-$CC-mt $BN-$CC-mt-s $BN-$CC-s \
				lib$BN lib$BN-$CC lib$BN-$CC-mt lib$BN-$CC-mt-s lib$BN-$CC-s \
					$BN-mgw $BN-mgw $BN-mgw-mt $BN-mgw-mt-s $BN-mgw-s ; do
				AC_CHECK_LIB($ax_lib, main,
					[BOOST_PROGRAM_OPTIONS_LIB="-l$ax_lib"; AC_SUBST(BOOST_PROGRAM_OPTIONS_LIB) CONFIG_BOOST_PROGRAM_OPTIONS="yes"; break],
					[CONFIG_BOOST_PROGRAM_OPTIONS="no"])
			done
		else
			for ax_lib in $ax_boost_user_program_options_lib $BN-$ax_boost_user_program_options_lib; do
				AC_CHECK_LIB($ax_lib, main,
					[BOOST_PROGRAM_OPTIONS_LIB="-l$ax_lib"; AC_SUBST(BOOST_PROGRAM_OPTIONS_LIB) CONFIG_BOOST_PROGRAM_OPTIONS="yes"; break],
					[CONFIG_BOOST_PROGRAM_OPTIONS="no"])
			done
		fi
		AC_LANG_POP([C++])
		if test "x$CONFIG_BOOST_PROGRAM_OPTIONS" = "xno"; then
			AC_MSG_ERROR([Could not link against [$ax_lib] !])
		fi
	fi
	CPPFLAGS="$CPPFLAGS_SAVED"
	LDFLAGS="$LDFLAGS_SAVED"
fi
])

# }}}
# {{{ MySQL

AC_DEFUN([AX_LIB_MYSQL],
[
AC_ARG_WITH([mysql],
	AC_HELP_STRING([--with-mysql=@<:@ARG@:>@],
		[use MySQL client library @<:@default=yes@:>@, optionally specify path to mysql_config]
	),
	[
	if test "$withval" = "no"; then
		want_mysql="no"
	elif test "$withval" = "yes"; then
		want_mysql="yes"
	else
		want_mysql="yes"
		MYSQL_CONFIG="$withval"
	fi
        ],
	[want_mysql="yes"]
)

MYSQL_CFLAGS=""
MYSQL_LDFLAGS=""
MYSQL_VERSION=""
CONFIG_MYSQL=no

dnl
dnl Check MySQL libraries (libpq)
dnl

if test "$want_mysql" = "yes"; then

	if test -z "$MYSQL_CONFIG" -o test; then
		AC_PATH_PROG([MYSQL_CONFIG], [mysql_config], [no])
	fi

	if test "$MYSQL_CONFIG" != "no"; then
		AC_MSG_CHECKING([for MySQL libraries])

		MYSQL_CFLAGS="`$MYSQL_CONFIG --cflags`"
		MYSQL_LDFLAGS="`$MYSQL_CONFIG --libs`"
		MYSQL_VERSION=`$MYSQL_CONFIG --version`

		AC_DEFINE([HAVE_MYSQL], [1],
			  [Define to 1 if MySQL libraries are available])

		CONFIG_MYSQL="yes"
		AC_MSG_RESULT([yes])
        fi
fi

dnl
dnl Check if required version of MySQL is available
dnl


mysql_version_req=ifelse([$1], [], [], [$1])

if test "$CONFIG_MYSQL" = "yes" -a -n "$mysql_version_req"; then

	AC_MSG_CHECKING([if MySQL version is >= $mysql_version_req])

	dnl Decompose required version string of MySQL
	dnl and calculate its number representation
	mysql_version_req_major=`expr $mysql_version_req : '\([[0-9]]*\)'`
	mysql_version_req_minor=`expr $mysql_version_req : '[[0-9]]*\.\([[0-9]]*\)'`
	mysql_version_req_micro=`expr $mysql_version_req : '[[0-9]]*\.[[0-9]]*\.\([[0-9]]*\)'`
	if test "x$mysql_version_req_micro" = "x"; then
		mysql_version_req_micro="0"
	fi

	mysql_version_req_number=`expr $mysql_version_req_major \* 1000000 \
				   \+ $mysql_version_req_minor \* 1000 \
				   \+ $mysql_version_req_micro`

	dnl Decompose version string of installed MySQL
	dnl and calculate its number representation
	mysql_version_major=`expr $MYSQL_VERSION : '\([[0-9]]*\)'`
	mysql_version_minor=`expr $MYSQL_VERSION : '[[0-9]]*\.\([[0-9]]*\)'`
	mysql_version_micro=`expr $MYSQL_VERSION : '[[0-9]]*\.[[0-9]]*\.\([[0-9]]*\)'`
	if test "x$mysql_version_micro" = "x"; then
		mysql_version_micro="0"
	fi

	mysql_version_number=`expr $mysql_version_major \* 1000000 \
				   \+ $mysql_version_minor \* 1000 \
				   \+ $mysql_version_micro`

	mysql_version_check=`expr $mysql_version_number \>\= $mysql_version_req_number`
	if test "$mysql_version_check" = "1"; then
		AC_MSG_RESULT([yes])
	else
		AC_MSG_RESULT([no])
		CONFIG_MYSQL=no
	fi
fi

AC_SUBST([MYSQL_VERSION])
AC_SUBST([MYSQL_CFLAGS])
AC_SUBST([MYSQL_LDFLAGS])
])

# }}}
# {{{ MySQL++

AC_DEFUN([AX_LIB_MYSQLPP],
[
AC_ARG_WITH([mysqlpp],
        AC_HELP_STRING([--with-mysqlpp=@<:@ARG@:>@],
		[use MySQL client C++ library @<:@default=yes@:>@, optionally specify path to the library]),
	[
        	if test "$withval" = "no"; then
			want_mysqlpp="no"
		elif test "$withval" = "yes"; then
			want_mysqlpp="yes"
			ax_mysqlpp_path=""
		else
			want_mysqlpp="yes"
			ax_mysqlpp_path="$withval"
		fi
	],
	[want_mysqlpp="yes"]
)

CONFIG_MYSQLPP=no
MYSQLPP_CXXFLAGS=""
test -z "$ax_mysqlpp_path" || MYSQLPP_CXXFLAGS="-I$ax_myslpp_path"

if test "x$want_mysqlpp" = "xyes"; then
	AC_REQUIRE([AC_PROG_CC])
	CPPFLAGS_SAVED="$CPPFLAGS"
	CPPFLAGS="$CPPFLAGS $MYSQLPP_CPPFLAGS"
	export CPPFLAGS

	CXXFLAGS_SAVED="$CXXFLAGS"
	CXXFLAGS="$CPPFLAGS $MYSQL_CFLAGS $MYSQL_CXXFLAGS"
	export CXXFLAGS

	LDFLAGS_SAVED="$LDFLAGS"
	LDFLAGS="$LDFLAGS $MYSQL_LDFLAGS $MYSQLPP_LDFLAGS"
	export LDFLAGS

	AC_CACHE_CHECK(whether the MySQL++ library is available,
		ax_cv_mysqlpp,
		[AC_LANG_PUSH([C++])
		AC_COMPILE_IFELSE(AC_LANG_PROGRAM([[@%:@include <mysql++/mysql++.h>
						  ]],
						  [[mysqlpp::Connection con(false)]]),
				  ax_cv_mysqlpp=yes,
				  ax_cv_mysqlpp=no)
		AC_LANG_POP([C++])
		])

	if test "x$ax_cv_mysqlpp" = "xyes"; then
		AC_DEFINE(HAVE_MYSQLPP,[1],[Define if the MySQL++ library is available])
		AC_LANG_PUSH([C++])
		AC_CHECK_LIB(mysqlpp, main,CONFIG_MYSQLPP=yes,CONFIG_MYSQLPP=no)
		AC_LANG_POP([C++])

		if test "x$CONFIG_MYSQLPP" = "xno"; then
			AC_MSG_ERROR(Could not link against mysqlpp !)
		fi
	fi

	CPPFLAGS="$CPPFLAGS_SAVED"
	CXXFLAGS="$CXXFLAGS_SAVED"
	LDFLAGS="$LDFLAGS_SAVED"
fi

AC_SUBST([MYSQLPP_CPPFLAGS])
AC_SUBST([MYSQLPP_CXXFLAGS])
])

# }}}
