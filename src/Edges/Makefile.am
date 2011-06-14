## Process this file with automake to produce Makefile.in
##
## $Id: Makefile.am,v 1.6 2001/03/21 04:45:53 vchma Exp $

SUBDIRS = .
CLEANFILES = *~
INCLUDES = -I$(srcdir)/../Utilities -I$(srcdir)/../Geom \
	@TK_CFLAGS@ @TCL_CFLAGS@ @GL_CFLAGS@ @X_CFLAGS@

lib_LIBRARIES = libHE.a libMeshDraw.a

libHE_a_SOURCES = \
	DualEdge.C \
	DualVertex.C \
	Face.C \
	HalfEdge.C \
	InputFilter.C \
	InputFilterRegister.C \
	IVOutputOp.C \
	Mesh.C \
	OBJFilter.C \
	UWashFilter.C \
	Vertex.C 

libMeshDraw_a_SOURCES = \
	MeshDraw.C \
	MeshDraw.h

noinst_HEADERS = \
	DualEdge.h \
	DualVertex.h \
	Face.h \
	HalfEdge.h \
	InputFilter.h \
	InputFilterRegister.h \
	IVOutputOp.h \
	Mesh.h \
	OBJFilter.h \
	UWashFilter.h \
	Vertex.h 
