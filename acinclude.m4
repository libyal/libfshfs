dnl Function to detect if libfshfs dependencies are available
AC_DEFUN([AX_LIBFSHFS_CHECK_LOCAL],
 [dnl Check for internationalization functions in libfshfs/libfshfs_i18n.c 
 AC_CHECK_FUNCS([bindtextdomain])
 ])

