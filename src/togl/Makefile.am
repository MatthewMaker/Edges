## Process this file with automake to produce Makefile.in
##
## $Id: Makefile.am,v 1.4 2001/03/21 04:22:59 vchma Exp $

SUBDIRS = .
CLEANFILES = *~
INCLUDES = @TK_CFLAGS@ @TCL_CFLAGS@ @GL_CFLAGS@ @X_CFLAGS@

lib_LIBRARIES = libTogl.a

libTogl_a_SOURCES = \
	togl.C

noinst_HEADERS = \
	tkFont.h \
	tkInt8.0.h \
	tkInt8.0p2.h \
	tkInt8.0p5.h \
	tkInt8.2.h \
	tkInt8.3.h \
	tkIntDecls.h \
	togl.h \
	LICENSE.togl

