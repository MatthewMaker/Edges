//--------------------------------------------------------------------------
// $Id: Array.h,v 1.1 2001/03/21 04:23:38 vchma Exp $
//--------------------------------------------------------------------------

#ifndef __ARRAY_H__
#define __ARRAY_H__

#define DEFAULT_SIZE 10

#include <cstring>

// WARNING: This class was never designed to be an array of classes.
// 	    One should only use this for arrays of values (like int,
// 	    float, etc.) or arrays of pointers to classes or what not.

typedef int (*ArrayCompare)(const void *, const void *);

template <class T> class Array
{
public:

    Array( bool unforgettable = true )
    : myArray(0), myNumItems(0), mySize(0), remember( unforgettable )
    {
//	init();
    }

    ~Array() { forget(); free ( myArray ); }

    // creates space for stuff
    int		 init( unsigned int size = DEFAULT_SIZE )
    {
	// if it's the same size, don't bother.
	if ( size == mySize ) return 1;
	
	forget();

	if ( size > 0 )
	{
	    mySize = size;
	    myNumItems = 0;

	    myArray = (T *) realloc( myArray, sizeof( T )*size );
	    if ( myArray ) return 1;
	}
	else
	{
	    if ( mySize || myNumItems ) free( myArray );
	    mySize = myNumItems = 0;
	    myArray = 0;
	}

	return 0;
    }

    // array access
    const T	&operator[]( const int index ) const
    { return myArray[index]; }
    T		&operator[]( const int index )
    { return myArray[index]; }

    // append to array
    int		 append( T n )
    {
	if ( myNumItems == mySize )
	    if ( !doubleSize() )
		return -1;

	myArray[myNumItems++] = n;
	return myNumItems-1;
    }

    // size of array
    unsigned int size() const { return myNumItems; }

    // return a copy of my array - caller responsible to free() it after use
    T		*dupeGut() const
    {
	T *gut = (T *) calloc( mySize, sizeof( T ) );

	if ( gut )
	    memcpy( gut, myArray, mySize*sizeof( T ) );

	return gut;
    }

    // sort the (sub)list with "func" as the compare function.
    bool	 sort( unsigned int s, unsigned int e, ArrayCompare func )
    {
	if (!( s >= 0 && s < myNumItems && e >= 0 && e < myNumItems && s < e ))
	    return false;

	int num = e - s + 1;

	qsort( (void *)&(myArray[s]), num, sizeof( T ), func );

	return true;
    }

private:
    
    T			*myArray;
    unsigned int	 myNumItems;
    unsigned int	 mySize;
    bool		 remember;

    // When the array is filled, we double it's size to make room.
    bool	 doubleSize()
    {
	if ( mySize == 0 ) mySize = 10;

	T* tmp = (T *) realloc( myArray, sizeof(T)*2*mySize );
	if ( tmp )
	{
	    mySize *=2;
	    myArray = tmp;
	    return 1;
	}
	return 0;
    }

    // forget the array contents
    void	 forget()
    {
	if ( myArray && !remember )
//	    for( int i = myNumItems ; i --> 0 ; )
//	    {
//		myArray[i] = 0;
//	    }
	    memset( myArray, 0, mySize * sizeof( T ) );
    }

};

#endif //__ARRAY_H__
