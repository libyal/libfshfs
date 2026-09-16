#!/bin/sh
# Script to run tests
#
# Version: 20260714

if [ -f "${PWD}/libfshfs/.libs/libfshfs.1.dylib" ] && [ -f ./pyfshfs/.libs/pyfshfs.so ]
then
    install_name_tool -change /usr/local/lib/libfshfs.1.dylib "${PWD}/libfshfs/.libs/libfshfs.1.dylib" ./pyfshfs/.libs/pyfshfs.so
fi

make check-build > /dev/null

# shellcheck disable=SC2068
make check $@
RESULT=$?

if [ ${RESULT} -ne 0 ]
then
    find . -name \*.log -path \*.dir/\*/\*.log -print -exec cat {} \;
fi
exit ${RESULT}

