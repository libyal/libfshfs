include_HEADERS = \
	libfshfs.h

pkginclude_HEADERS = \
	libfshfs/codepage.h \
	libfshfs/definitions.h \
	libfshfs/error.h \
	libfshfs/extern.h \
	libfshfs/features.h \
	libfshfs/types.h

EXTRA_DIST = \
	libfshfs.h.in \
	libfshfs/definitions.h.in \
	libfshfs/features.h.in \
	libfshfs/types.h.in

MAINTAINERCLEANFILES = \
	Makefile.in

distclean: clean
	-rm -f libfshfs.h
	-rm -f libfshfs/definitions.h
	-rm -f libfshfs/features.h
	-rm -f libfshfs/types.h
	-rm -f Makefile

