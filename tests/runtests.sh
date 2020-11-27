#!/bin/sh
# Script to run tests
#
# Version: 20201121

if test -f ${PWD}/libfshfs/.libs/libfshfs.1.dylib && test -f ./pyfshfs/.libs/pyfshfs.so;
then
	install_name_tool -change /usr/local/lib/libfshfs.1.dylib ${PWD}/libfshfs/.libs/libfshfs.1.dylib ./pyfshfs/.libs/pyfshfs.so;
fi

make check CHECK_WITH_STDERR=1;
RESULT=$?;

if test ${RESULT} -ne 0 && test -f tests/test-suite.log;
then
	cat tests/test-suite.log;
fi
exit ${RESULT};

