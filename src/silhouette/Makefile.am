## Process this file with automake to produce Makefile.in
##
## $Id: Makefile.am,v 1.6 2001/03/21 04:45:53 vchma Exp $

SUBDIRS = .
CLEANFILES = *~
INCLUDES = -I$(srcdir)/../Utilities -I$(srcdir)/../Edges \
	-I$(srcdir)/../Geom -I$(srcdir)/../TriStrip -I$(srcdir)/../togl \
	@TCL_CFLAGS@ @TK_CFLAGS@ @GL_CFLAGS@ @X_CFLAGS@

bin_PROGRAMS = silhouette

silhouette_SOURCES = \
	Node.C \
	PMRTreeNode.C \
	PMRTree.C \
	OctTreeNode.C \
	OctTree.C \
	util.C \
	silhouette.C \
	gr.C \
	grsh.C

noinst_HEADERS = \
	gr.h \
	Node.h \
	OctTree.h \
	OctTreeNode.h \
	PMRTree.h \
	PMRTreeNode.h \
	silhouette.h \
	silhouette.tcl \
	util.h

# silhouette_LDADD = @TK_LDFLAGS@ @TCL_LDFLAGS@ @GL_LDFLAGS@ @X_LDFLAGS@ \
# 	-L../Utilities -L../Edges -L../TriStrip -L../togl \
# 	-lTogl -lTriStrip -lHE -lUtilities \
# 	@TK_LIBS@ @TCL_LIBS@ -lGL -lGLU -lX11 -lXmu

silhouette_LDADD = @TK_LDFLAGS@ @TCL_LDFLAGS@ @GL_LDFLAGS@ @X_LDFLAGS@ \
	$(srcdir)/../TriStrip/libTriStrip.a \
	$(srcdir)/../TriStrip/libTriStripDraw.a \
	$(srcdir)/../Edges/libHE.a \
	$(srcdir)/../Edges/libMeshDraw.a \
	$(srcdir)/../Geom/libGeom.a \
	$(srcdir)/../Utilities/libUtilities.a \
	$(srcdir)/../togl/libTogl.a \
	@TK_LIBS@ @TCL_LIBS@ -lGL -lGLU -lX11 -lXmu

