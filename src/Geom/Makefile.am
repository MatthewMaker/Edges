## Process this file with automake to produce Makefile.in
##
## $Id: Makefile.am,v 1.1 2001/03/21 04:45:53 vchma Exp $

SUBDIRS = .
CLEANFILES = *~
INCLUDES = -I$(srcdir)/../Utilities \
	@TK_CFLAGS@ @TCL_CFLAGS@ @GL_CFLAGS@ @X_CFLAGS@

lib_LIBRARIES = libGeom.a 

libGeom_a_SOURCES = \
	Matrix.C \
	Point.C \
	Vector.C \
	Vector2.C 

noinst_HEADERS = \
	Matrix.h \
	Point.h \
	Vector.h \
	Vector2.h 
