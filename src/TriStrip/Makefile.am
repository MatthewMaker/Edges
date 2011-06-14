## Process this file with automake to produce Makefile.in
##
## $Id: Makefile.am,v 1.6 2001/03/21 04:45:53 vchma Exp $

SUBDIRS = .
CLEANFILES = *~
INCLUDES = -I$(srcdir)/../Utilities -I$(srcdir)/../Geom -I$(srcdir)/../Edges \
	@TK_CFLAGS@ @TCL_CFLAGS@ @GL_CFLAGS@ @X_CFLAGS@

lib_LIBRARIES= libTriStrip.a libTriStripDraw.a

libTriStrip_a_SOURCES = \
	PriQueue.C \
	PQFace.C \
	TriStrip.C

libTriStripDraw_a_SOURCES = \
	TriStripDraw.C

noinst_HEADERS = \
	PQFace.h \
	PriQueueElem.h \
	PriQueue.h \
	TriStrip.h \
	TriStripDraw.h

bin_PROGRAMS = ts

ts_SOURCES = \
	ts.C

# ts_LDADD = @TCL_LDFLAGS@ @GL_LDFLAGS@ @X_LDFLAGS@ \
# 	-L../Utilities -L. -L../Edges \
# 	-lTriStrip -lHE -lUtilities @TCL_LIBS@ 

ts_LDADD = \
	libTriStrip.a \
	$(srcdir)/../Edges/libHE.a \
	$(srcdir)/../Geom/libGeom.a \
	$(srcdir)/../Utilities/libUtilities.a \
	@TCL_LIBS@

