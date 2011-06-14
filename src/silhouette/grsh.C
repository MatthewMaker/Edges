//--------------------------------------------------------------------------
// Edges
// Copyright (C) 1999-2000 Vincent C. H. Ma
// http://edges.sourceforge.net
// vchma@users.sourceforge.net
// 
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
//
// $Id: grsh.C,v 1.2 2000/09/17 22:48:30 vchma Exp $
//--------------------------------------------------------------------------

#include <cstdlib>
#include <cstring>
#include <iostream>
#include <GL/gl.h>

// #include "tcl.h"
// #include "tk.h"
// #include "ogl_raster.h"

#include "gr.h"

/***************************************************************************
 * Here is the glue for all of the gr_ commands.  This "glue" converts
 * from arguments of the form argc, argv and checks for the right 
 * number of arguments.
 ***************************************************************************/



/***************************************************************************
 * First there are some helper routines that get points and colours
 * from tcl
 ***************************************************************************/

/* 
 * Get a 3D vector specified as a tcl-list 
 */
static int gr_getVector3D(Tcl_Interp *ip, char *inList, GrVector3D *p)
{
    char **indices;
    int num_doubles;
    int rtn;

    rtn = Tcl_SplitList(ip, inList, &num_doubles, &indices);
    if((rtn != TCL_OK) || (num_doubles != 3))
    {
	Tcl_AppendResult(ip, 
		"A vertex must contain 3 doubles.",
		(char *) 0);
	free(indices);
	return (TCL_ERROR);
    }

    if(Tcl_GetDouble(ip, indices[0], &p->x) == TCL_OK)
	if(Tcl_GetDouble(ip, indices[1], &p->y) == TCL_OK)
	    if(Tcl_GetDouble(ip, indices[2], &p->z) == TCL_OK)
	    {
		free(indices);
		return TCL_OK;
	    }  
    free(indices);

    return TCL_ERROR;
}

/* 
 * Get a 2D point specified as a tcl-list 
 */
static int gr_getPoint2D(Tcl_Interp *ip, char *inList, GrPoint2D *p)
{
    char **indices;
    int num_doubles;
    int rtn;

    rtn = Tcl_SplitList(ip, inList, &num_doubles, &indices);
    if((rtn != TCL_OK) || (num_doubles != 2))
    {
	Tcl_AppendResult(ip, 
		"A vertex must contain 2 ",
		"doubles for the 2D point",
		(char *) 0);
	free(indices);
	return (TCL_ERROR);
    }

    if(Tcl_GetDouble(ip, indices[0], &p->x) == TCL_OK)
	if(Tcl_GetDouble(ip, indices[1], &p->y) == TCL_OK)
	{
	    free(indices);
	    return TCL_OK;
	}  

    free(indices);
    return TCL_ERROR;
}

/***************************************************************************
 * The callbacks requested by the assignment specification.
 * 
 * The while error checking is performed by these function, the
 * error messages are not as precise as they might be. This is done
 * to keep the code simple. 
 *
 * Feel free to add better error messages.
 ***************************************************************************/
    static int 
Gr_identityCMD(ClientData cd, Tcl_Interp *ip, int argc, char *argv[]) 
{
    if (argc != 2) {
	Tcl_AppendResult(ip, 
		argv[0], 
		" needs 1 argument\n Usage: gr_identity <node>",
		(char *) 0 );
	return TCL_ERROR;
    }

    if(gr_identity() != TCL_OK)
    {
	Tcl_AppendResult(ip, 
		argv[0], 
		"Error in gr_idenity. Node might not exist.",  
		(char *) 0 );
	return TCL_ERROR;
    }      

    return TCL_OK;
}

    static int 
Gr_rotateCMD(ClientData cd, Tcl_Interp *ip, int argc, char *argv[]) 
{
    char axis;
    GLdouble angle ;

    if (argc != 4) {
	Tcl_AppendResult(ip, 
		argv[0], 
		" needs 3 arguments\nUsage: gr_rotate <node>",
		" <axis> <angle>",
		(char *) 0 );
	return TCL_ERROR;
    }

    /* the axis */
    axis = argv[2][0];

    if (Tcl_GetDouble(ip, argv[3], &angle) != TCL_OK)
	return TCL_ERROR;

    if(gr_rotate(axis, angle) != TCL_OK)
    {
	Tcl_AppendResult(ip, 
		argv[0], 
		"Error in gr_rotate. Node might not exist.",  
		(char *) 0 );
	return TCL_ERROR;
    }      

    return TCL_OK;
}

    static int 
Gr_translateCMD(ClientData cd, Tcl_Interp *ip, int argc, char *argv[]) 
{
    GrVector3D vec;

    if (argc != 3) {
	Tcl_AppendResult(ip, 
		argv[0], 
		" needs 2 arguments\nUsage: gr_translate <node>",
		" <vect>",
		(char *) 0 );
	return TCL_ERROR;
    }

    /* get the translation vector */
    if( gr_getVector3D(ip, argv[2], &vec) != TCL_OK )
	return TCL_ERROR;

    if(gr_translate(vec) != TCL_OK)
    {
	Tcl_AppendResult(ip, 
		argv[0], 
		"Error in gr_translate. Node might not exist.",  
		(char *) 0 );
	return TCL_ERROR;
    }      
    return TCL_OK;
}

/* 
   The following callbacks are used to create objects in the scene graph.
 */
    static int
Gr_trackballCMD(ClientData cd, Tcl_Interp *ip, int argc, char *argv[])
{
    double  scale;
    GrPoint2D old_p, new_p;

    if (argc != 5) {
	Tcl_AppendResult(ip, 
		argv[0], 
		" needs 4 arguments\n Usage: gr_trackball <node>"
		" <old> <new> <scale>",
		(char *) 0 );
	return TCL_ERROR;
    }

    if( gr_getPoint2D(ip, argv[2], &old_p) == TCL_OK )
	if( gr_getPoint2D(ip, argv[3], &new_p) == TCL_OK )
	    if (Tcl_GetDouble(ip, argv[4], &scale) == TCL_OK) 
		return gr_trackball(old_p, new_p, scale);

    return TCL_ERROR;
}

// Command to invoke loading of a mesh ----------------------------------------
    static int 
Gr_loadCMD(ClientData cd, Tcl_Interp *ip, int argc, char *argv[]) 
{
    if (argc != 2) {
	Tcl_AppendResult(ip, 
		argv[0], 
		" [fully qualified filename] ",
		(char *) 0 );
	return TCL_ERROR;
    }
    load( argv[1] );

    return TCL_OK;
}

// Command to set drawing parameters ------------------------------------------
    static int 
Gr_drawparamCMD(ClientData cd, Tcl_Interp *ip, int argc, char *argv[]) 
{
    double s, h, o;

    if (argc != 5) {
	Tcl_AppendResult(ip, 
		argv[0], 
		"<hidden> <silhouette> <strip> <optimization>",
		(char *) 0 );
	return TCL_ERROR;
    }

    if (Tcl_GetDouble(ip, argv[1], &h) == TCL_OK) 
	gr_setHidden( h == 1 ? 1 : 0 );
    if (Tcl_GetDouble(ip, argv[2], &s) == TCL_OK) 
	gr_setSilhouette( s == 1 ? 1 : 0 );
    if (Tcl_GetDouble(ip, argv[3], &o) == TCL_OK) 
	gr_setStrip( (int) o );
    if (Tcl_GetDouble(ip, argv[4], &o) == TCL_OK) 
	gr_setOptimization( (int) o );

    return TCL_OK;
}

/*************************************************************************
 * Associate all the Tcl commands with the functions above...
 *************************************************************************/
static void 
Gr_Init(Tcl_Interp *ip) 
{
    Tcl_CreateCommand (ip, "gr_identity",    Gr_identityCMD,    0, 0);
    Tcl_CreateCommand (ip, "gr_rotate",      Gr_rotateCMD,      0, 0);
    Tcl_CreateCommand (ip, "gr_translate",   Gr_translateCMD,   0, 0);
    Tcl_CreateCommand (ip, "gr_trackball",   Gr_trackballCMD,   0, 0);
    Tcl_CreateCommand (ip, "gr_load",	     Gr_loadCMD,        0, 0);
    Tcl_CreateCommand (ip, "gr_drawparam",   Gr_drawparamCMD,  0, 0);

    Togl_CreateFunc(gr_initialize);
    Togl_DisplayFunc(gr_render);
    Togl_ReshapeFunc(gr_reshape);
}

/*************************************************************************
 * Tcl expects this function to be here.  The real main() is in the
 * tcl library.  Note that we also initialize Tk and the ogl_raster
 * widget.
 *************************************************************************/
int
Tcl_AppInit(Tcl_Interp *ip) {
    if (Tcl_Init(ip) == TCL_ERROR) return TCL_ERROR;
    if (Tk_Init(ip)  == TCL_ERROR) return TCL_ERROR;
    if (Togl_Init(ip) == TCL_ERROR) return TCL_ERROR;

    Gr_Init(ip);
    Tcl_SetVar(ip, "tcl_rcFileName", "./silhouette.tcl", TCL_GLOBAL_ONLY);

    return TCL_OK;
}

/*************************************************************************
 * The main function just calls Tk_Main to initialize the system
 *************************************************************************/

static const char *WARRANTY =
"Quoted from GPL v2, http://www.gnu.org/copyleft/gpl.html\n"
"NO WARRANTY\n-----------\n\n"
" 11. BECAUSE THE PROGRAM IS LICENSED FREE OF CHARGE, THERE IS\n"
"     NO WARRANTY FOR THE PROGRAM, TO THE EXTENT PERMITTED BY\n"
"     APPLICABLE LAW.  EXCEPT WHEN OTHERWISE STATED IN WRITING THE\n"
"     COPYRIGHT HOLDERS AND/OR OTHER PARTIES PROVIDE THE PROGRAM \"AS IS\"\n"
"     WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESSED OR IMPLIED,\n"
"     INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF\n"
"     MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. THE ENTIRE\n"
"     RISK AS TO THE QUALITY AND PERFORMANCE OF THE PROGRAM IS WITH\n"
"     YOU. SHOULD THE PROGRAM PROVE DEFECTIVE, YOU ASSUME THE COST OF\n"
"     ALL NECESSARY SERVICING, REPAIR OR CORRECTION. \n\n"
" 12. IN NO EVENT UNLESS REQUIRED BY APPLICABLE LAW OR AGREED TO IN\n"
"     WRITING WILL ANY COPYRIGHT HOLDER, OR ANY OTHER PARTY WHO MAY\n"
"     MODIFY AND/OR REDISTRIBUTE THE PROGRAM AS PERMITTED ABOVE, BE\n"
"     LIABLE TO YOU FOR DAMAGES, INCLUDING ANY GENERAL, SPECIAL,\n"
"     INCIDENTAL OR CONSEQUENTIAL DAMAGES ARISING OUT OF THE USE OR\n"
"     INABILITY TO USE THE PROGRAM (INCLUDING BUT NOT LIMITED TO LOSS\n"
"     OF DATA OR DATA BEING RENDERED INACCURATE OR LOSSES SUSTAINED BY\n"
"     YOU OR THIRD PARTIES OR A FAILURE OF THE PROGRAM TO OPERATE WITH\n"
"     ANY OTHER PROGRAMS), EVEN IF SUCH HOLDER OR OTHER PARTY HAS BEEN\n"
"     ADVISED OF THE POSSIBILITY OF SUCH DAMAGES. \n\n";

int
main(int argc, char **argv)
{
    std::cout << "EDGES, Copyright (C) 1999-2000 Vincent C. H. Ma\n";
    std::cout << "EDGES comes with ABSOLUTELY NO WARRANTY; for details type\n";
    std::cout << "\"" << argv[0] << " w\"\n";
    std::cout << "This is free software, and you are welcome to redistribute\n";
    std::cout << "it under certain conditions; see www.gnu.org/copyleft/gpl.html\n";
    std::cout << std::endl;

    if ( argc == 2 )
    {
	if ( argv[1][0] == 'w' && argv[1][1] == 0 )
	{
	    std::cout << WARRANTY;
	    return 0;
	}
    }

    Tk_Main(argc, argv, Tcl_AppInit);
    return 0;           /* Needed only to prevent compiler warning. */
}
