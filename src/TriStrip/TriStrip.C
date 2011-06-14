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
// $Id: TriStrip.C,v 1.13 2001/03/02 06:01:56 vchma Exp $
//--------------------------------------------------------------------------

#include "TriStrip.h"
#include "Array.h"
#include "Face.h"
#include "HalfEdge.h"
#include "Vertex.h"
#include "Vector.h"
#include "Vector2.h"
#include "PriQueue.h"
#include "PQFace.h"

#include "Timer.h"
#include "ProgressBar.h"

#include <cstdlib>
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>

// #define SMART

#define NUM_VERT_PER_LINE 3

using std::cout;
using std::endl;

enum
{
    RESTART,
    LEFT,
    RIGHT,
};

TriStrip::TriStrip( Mesh &m )
: myMesh( m ),
  myNumSingleton( 0 ),
  myNumPoints( 0 ),
  myNumStrips( 0 )
{
    TriStrip::Strip *curStrip;

    int i, daFace, numRemain, id;
    int state = RESTART;

    const int nf = m.numFaces();

    HalfEdge *e[3];
    bool usable[3], bleft = false;
    std::vector<bool> done( nf );

    PQFace **faces = new PQFace *[nf];
    PQFace *thePQFace;

    // temporary Vertex pointers
    Vertex *p, *q;

    // timing stuff
    Timer theTimer;

    PriQueue head[4];

    cout << "TriStrip: have " << nf << " triangles to strip" << endl;

    theTimer.restart();

    // initialize: create PQFaces, stick them into the list according to
    // their connectivity, and init "done" flags.
    for ( i = nf ; i --> 0 ; )
    {
	faces[i] = new PQFace( m.getFace(i) );
	head[faces[i]->key()].append( faces[i] );
	done[i] = 0;
    }

    cout << "Initialization: ";
    cout << theTimer.stop() << "s" << endl;

    numRemain = nf;

    theTimer.restart();

    ProgressBar pb( numRemain );

    // while there are remaining faces to be stripified.
    while ( numRemain )
    {
	if ( state == RESTART )
	{
	    curStrip = new TriStrip::Strip;

	    bool nomore = true;
	    daFace = -1;

	    // find a remaining face of the lowest connectivity.
	    for ( i = 0 ; i < 4 ; i++ )
	    {
 		if ( head[i] )
		{
		    thePQFace = dynamic_cast< PQFace * >( head[i].pop() );
		    daFace = thePQFace->getFace().getMeshId();
		    nomore = false;
		    break;
		}
	    }
	    if ( nomore ) break;

// 	    if ( done[daFace] ) cout << "!";

	    // this face will now be a start of a strip.  it's
	    // used, so flag it and now one less face to do.
	    done[daFace] = true;
	    numRemain--;

	    // update the progress bar
	    ++pb;

	    // get daFace's edges, and figure out which way can we
	    // construct the strip
	    e[0] = &(m.getFace( daFace ).getEdge());
	    e[1] = &(e[0]->getPrev());
	    e[2] = &(e[0]->getNext());

	    for ( int k = 3 ; k --> 0 ; )
	    {
		id = e[k]->getSym().getFace().getMeshId();
		usable[k] = !( e[k]->isSingle() || e[k]->isSeam() || done[id] );
		if ( usable[k] )
		{
		    head[faces[id]->key()].remove( faces[id] );
		    faces[id]->changeKey( -1 );
		    head[faces[id]->key()].append( faces[id] );
		}
	    }

	    // if this is an isolated triangle, this will be 
	    // a one-tri strip.. sigh.
	    if ( !usable[0] && !usable[1] && !usable[2] )
	    {
		bleft = false;

		// add the first 3 vertices to the strip
		curStrip->push_back( &(e[1]->getVertex()) );
		curStrip->push_back( &(e[0]->getVertex()) );
		curStrip->push_back( &(e[2]->getVertex()) );

		myNumSingleton++;

		state = RESTART;
	    }

	    // otherwise..
	    else
	    {
		// pick a base edge.  default to 0.   If 1 or 2 is
		// "unusable" then that will be the base.
		// i.e. if all 3 are usable, 0 will be base.
		int base = 0; 
		if ( !usable[1] ) base = 1;
		if ( !usable[2] ) base = 2;

		int left = ( base + 1 ) % 3; // left is prev of base
		int right = ( base + 2 ) % 3; // right is next of base

		// if both sides are usable, have to choose
		if ( usable[left] && usable[right] )
		{
		    int L = faces[e[left]->getSym().getFace().getMeshId()]->key();
		    int R = faces[e[right]->getSym().getFace().getMeshId()]->key();
		    bleft = ( L < R );
		}
		// o/w, only one side is
		else
		    bleft = usable[left];

// 		if ( usable[left] )
		if ( bleft )
		{
		    // add the first 3 vertices to the strip
		    curStrip->push_back( &(e[base]->getVertex()) );
		    curStrip->push_back( &(e[right]->getVertex()) );
		    curStrip->push_back( &(e[left]->getVertex()) );

		    e[0] = &(e[left]->getSym());
		}
// 		else if ( usable[right] )
		else
		{
		    // add the first 3 vertices to the strip
		    curStrip->push_back( &(e[left]->getVertex()) );
		    curStrip->push_back( &(e[base]->getVertex()) );
		    curStrip->push_back( &(e[right]->getVertex()) );

		    e[0] = &(e[right]->getSym());
		}
		usable[0] = false;

		// All strips enter the RIGHT state for their second
		// triangle (so that the preferred direction from the
		// second to third triangle is LEFT).
		state = RIGHT;  
	    }
	}

	else if ( state == RIGHT || state == LEFT )
	{
	    id = e[0]->getFace().getMeshId();
	    done[id] = true;

	    // remove those faces that are done already
 	    head[faces[id]->key()].remove( faces[id] );

	    numRemain--;
	    ++pb;

	    // find out useabilty of the other 2 edges
	    e[LEFT] = &(e[0]->getPrev());
	    id = e[LEFT]->getSym().getFace().getMeshId();
	    usable[LEFT] = !( e[LEFT]->isSingle() || e[LEFT]->isSeam() || done[id] );
	    if ( usable[LEFT] )
	    {
		head[faces[id]->key()].remove( faces[id] );
		faces[id]->changeKey( -1 );
		head[faces[id]->key()].append( faces[id] );
	    }

	    e[RIGHT] = &(e[0]->getNext());
	    id = e[RIGHT]->getSym().getFace().getMeshId();
	    usable[RIGHT] = !( e[RIGHT]->isSingle() || e[RIGHT]->isSeam() || done[id] );
	    if ( usable[RIGHT] )
	    {
		head[faces[id]->key()].remove( faces[id] );
		faces[id]->changeKey( -1 );
		head[faces[id]->key()].append( faces[id] );
	    }

	    // remember the new vertex on the strip,
	    q = &(e[RIGHT]->getVertex());

 	    int dir = ( state == RIGHT ? LEFT : RIGHT );
	    bool swap;

	    // see how we are to continue the strip

	    // if we are at a dead end, restart
	    if ( !( usable[LEFT] || usable[RIGHT] ) )
	    {
		// stick the new vertex on the strip,
		curStrip->push_back( q );
  		state = RESTART;
	    }

	    else 
	    {
		// if only one side is usable
		if ( usable[LEFT] != usable[RIGHT] )
		{
		    swap = usable[state];
		}

		// if BOTH sides are usable, we have to choose
		else if ( usable[LEFT] && usable[RIGHT] )
		{
		    float L = faces[e[LEFT]->getSym().getFace().getMeshId()]->key() * 2;
		    float R = faces[e[RIGHT]->getSym().getFace().getMeshId()]->key() * 2;

		    // dir is the direction that doesn't need swapping.
		    // score of direction without swapping will get -=1
		    // score of direction with swapping will get +=1
		    if ( dir == LEFT )
		    {
			L -=1;
			R +=1.5;
		    }
		    else if ( dir == RIGHT )
		    {
			R -=1;
			L +=1.5;
		    }

		    // we want to go in the direction of the lower score.
		    // so, we need to swap if that direction is not dir.
		    swap = ( ( L<R && dir==RIGHT ) || ( R<L && dir==LEFT ) );
		}

		if ( !swap )
		{
		    // NOW, stick the new vertex on the strip,
		    curStrip->push_back( q );

		    // set the base edge, and alternate to the next state
		    e[0] = &(e[dir]->getSym());
		    usable[0] = false;
		    state = dir;

		}
		else 
		{
		    // do a swap
		    p = *( curStrip->end() - 2 );
		    curStrip->push_back( p );
		    curStrip->push_back( q );

		    // set the base edge to the one of the same dir
		    e[0] = &(e[state]->getSym());
		    usable[0] = false;

		    // the state should be kept the same.. no op.
		}
	    } // else if only one side usable
	}

	// if we just finished a strip, we output.
	if ( state == RESTART && curStrip )
	{
	    // record stats
	    myNumPoints += curStrip->size();
	    myNumStrips++;

	    myStrips.push_back( curStrip );
	}
    }

//    if ( head[0] || head[1] || head[2] || head[3] )
//	cout << " urgh! " << endl;
    for ( int iii = 0 ; iii < 4 ; ++iii )
    {
	if ( head[iii] )
	    cout << " urgh! " << iii << endl;
    }

    cout << "Striping: ";
    cout << theTimer.stop() << "s" << endl;

    cout << "TriStrip: Generated " << myNumStrips << " strips" << endl;
    cout << "          " << myNumPoints << "/" << nf*3;
    cout << " points used" << endl;
    cout << "          Average strip length: ";
    cout << (float)myNumPoints / (float)myNumStrips << endl;
    cout << "          " << myNumSingleton << " single-tri strips" << endl;

    // cleanup
    for ( int i = m.numFaces() ; i --> 0 ; ) delete faces[i];
    delete [] faces;
}

TriStrip::~TriStrip()
{
    // blow away each strip that I have generated
    StripsArray::const_iterator ii;
    for ( ii = myStrips.begin() ; ii < myStrips.end() ; ++ii )
    {
	delete *ii;
    }
}

bool
TriStrip::output( const char *filename )
{
    if ( !filename )
    {
	std::cerr << "TriStrip::output - NULL filename" << std::endl;
	return false;
    }

    std::string fn = filename;
    fn += ".strip";

    std::ofstream ofs( fn.c_str() );

    if ( !ofs )
    {
	std::cerr << "TriStrip::output - unable to open "
	     << filename << " for writing" << std::endl;
	return false;
    }

    std::cout << "TriStrip output in " << fn << std::endl;

    int i, counter;
    int numV = myMesh.numPoints();
    int numF = myMesh.numFaces();
    int numN = myMesh.numNormals();
    int numT = myMesh.numUVs();

    // write out the header.
    ofs << "# Strips generated by Edges" << std::endl;
    ofs << "# EDGES Copyright (C) 1999-2000 Vincent C. H. Ma" << std::endl;
    ofs << "#" << std::endl;
    ofs << "# " << numV << " vertices" << std::endl;
    ofs << "# " << numF << " faces" << std::endl;
    ofs << "# " << numN << " normals" << std::endl;
    ofs << "# " << numT << " UVs" << std::endl;
    ofs << "#" << std::endl;
    ofs << "# Generated " << myNumStrips << " strips" << std::endl;
    ofs << "# " << myNumPoints << "/" << numF*3 << " points used" << std::endl;
    ofs << "# Average strip length: ";
    ofs << (float)myNumPoints / (float)myNumStrips << std::endl;
    ofs << "# " << myNumSingleton << " single-tri strips" << std::endl;

    // format the output of floats.
    ofs << std::setw( 13 )
	<< std::setprecision( 6 )
	<< setiosflags( std::ios::showpoint )
	<< setiosflags( std::ios::fixed ) << " " << std::endl;

//    ofs << " " << endl;

    // output a bunch of statistics so the reader can pre-allocate
    // arrays of the correct sizes to store these information.
    ofs << "nv " << numV << std::endl;
    ofs << "nn " << numN << std::endl;
    ofs << "nt " << numT << std::endl;
    ofs << "ns " << myNumStrips << std::endl;
    ofs << "nf " << numF << std::endl;

    // output bounding sphere information
    const Point &p = myMesh.getCentroid();
    ofs << "cn " << p.x << " " << p.y << " " << p.z << std::endl;
    ofs << "md " << myMesh.getBoundingSphereRadius() << std::endl;

    for ( i = 0 ; i < numV ; ++i )
    {
	const Point &p = myMesh.getPoint( i );
	ofs << "v " << p.x << " " << p.y << " " << p.z << std::endl;
    }

    for ( i = 0 ; i < numN ; ++i )
    {
	const Vector &p = myMesh.getNormal( i );
	ofs << "vn " << p.x << " " << p.y << " " << p.z << std::endl;
    }

    for ( i = 0 ; i < numT ; ++i )
    {
	const Vector2 &p = myMesh.getUV( i );
	ofs << "vt " << p.x << " " << p.y << std::endl;
    }

    TriStrip::Strip *curStrip;

    // declare the iterators
    StripsArray::const_iterator ii;
    TriStrip::Strip::const_iterator j;

    for ( ii = myStrips.begin() ; ii < myStrips.end() ; ++ii )
    {
	// start of a strip, pre-write the start-of-strip command
	// into the file, and reset the ocunter.
	curStrip = *ii;

	ofs << "s " << curStrip->size();
	counter = NUM_VERT_PER_LINE;

	for ( j = curStrip->begin() ; j < curStrip->end(); ++j )
	{
	    if ( counter <= 0 )
	    {
		ofs << endl << "c ";
		counter = NUM_VERT_PER_LINE;
	    }
	    else
	    {
		--counter;
		ofs << ' ';
	    }

	    Vertex &v = *(*j);
	    const Point &p = v.getPoint();
	    const Vector *n = v.getNormal();
	    const Vector2 *t = v.getUV();

	    ofs << p.getMeshId();

	    // if the vertex has a normal..
	    if ( n )
	    {
		// .. and a set of UV
		if ( t )
		{
		    ofs << '/' << t->getMeshId() << '/' << n->getMeshId();
		}
		// .. without UV
		else
		{
		    ofs << "//" << n->getMeshId();
		}
	    }

	    // else if the vertex only has a set of UV
	    else if ( t )
	    {
		ofs << '/' << t->getMeshId();
	    }
	}
	ofs << std::endl << 'e' << std::endl;
    }

    ofs.close();
    return true;
}
