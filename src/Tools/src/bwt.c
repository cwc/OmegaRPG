
/*
 *  bwt.c
 *
 * Code modified from Mark Nelson's article:
 * "Data Compression With The Burrows-Wheeler Transform"
 * Dr. Dobbs Journal, September 1996.
 * See: http://www.dogma.net/markn/articles/bwt/bwt.htm to make sense 
 * of this source code.
 */

/*
 * It is because of limitations of the standard C qsort function that
 * we have global data.
 * we want to sort indices into the temporaryBuffer, but qsort doesn't provide
 * any mechanism of forwarding arbitrary out of band data to the called
 * function.
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

unsigned char *BWT_temporaryBuffer;
unsigned int BWT_length;


/* The following function compares two substrings of length "length" 
 * and returns a value less than 0 if the index1 string would be sorted 
 * first, 0 if they are the same, and a positive number if the index2
 * string should be sorted first.
 *
 * Standard memcmp has an issue that some implementations treat 
 * characters as signed, and others don't.  Since this is a string 
 * compression algorithm, that should not make a difference to 
 * compressibility, but it will make the output different on two
 * different platforms.  
 * We are only using this for ASCII data with OMEGA, so this is not
 * an issue at present.
 * 
 */

int BWT_Compare( const void *pointer1, const void *pointer2 )
{
  unsigned int index1=*( unsigned int * )pointer1;
  unsigned int index2=*( unsigned int * )pointer2;
  
  return memcmp(BWT_temporaryBuffer+index1, BWT_temporaryBuffer+index2, BWT_length); 
};

/* The following function does the actual transform.  It does this 
 * transform "in place" in the buffer, and returns an unsigned int 
 * in the range 0 <= X < length as out of band data for the transform.
 * This value needs to be passed to the "detransform".
 *
 * Large "length" values passed in may take awhile do to the (O)N lg N
 * time of sorting (On most platforms).
 */

unsigned int BWT_Transform(unsigned char *buffer, unsigned int length)
{
  unsigned int i;
  unsigned int first = 0;
  unsigned int index;
  
  unsigned int *indices;
  
  BWT_length=length;
  BWT_temporaryBuffer=malloc(2*length*sizeof(unsigned char));
  if(BWT_temporaryBuffer==NULL)
    {
      printf( "Failure allocating memory in BWT_transform.\n" );
      exit( EXIT_FAILURE );
    }
  indices=malloc(length*sizeof(unsigned int));
  if(indices==NULL)
    {
      free( BWT_temporaryBuffer );
      printf( "Failure allocating memory in BWT_transform.\n" );
      exit( EXIT_FAILURE );
    }
  

  memcpy(BWT_temporaryBuffer, buffer, length);
  memcpy(BWT_temporaryBuffer+length, buffer, length);

  for( i=0; i < length; i++)
    {
      indices[ i ] = i;
    }
  qsort( indices, length, sizeof( unsigned int ), BWT_Compare );

  for( i = 0; i < length; i++ )
    {
      if( indices[ i ] == 0 )
	{
	  index = length-1;
	}
      else
	{
	  index = indices[ i ]-1;
	  if( index == 0 )
	    {
	      first=i;
	    }
	}
      
      buffer[i] = BWT_temporaryBuffer[ index ];
    }
  free( BWT_temporaryBuffer );
  free( indices );
  return first;
}

void BWT_Detransform( unsigned char *buffer, unsigned int length, unsigned int first )
{
  unsigned int i,index;
  unsigned char *temporaryBuffer;
  unsigned int *characterCountArray, *runningCountArray;
  unsigned int *transpositionBuffer;
  unsigned int sum=0;
  unsigned int previous;
  unsigned char character;
  

  runningCountArray = malloc( 256*sizeof( unsigned int ) ); /* Too big to put on stack */
  if( runningCountArray == NULL )
    {
      printf( "Failure allocating memory in BWT_Detransform.\n" );
      exit( EXIT_FAILURE );
    }

  characterCountArray = malloc( 256*sizeof( unsigned int ) ); /* Too big to put on stack */
  if( characterCountArray == NULL )
    {
      free( runningCountArray );
      printf( "Failure allocating memory in BWT_Detransform.\n" ); /* Too big to put on stack */
      exit( EXIT_FAILURE );
    }
  
  temporaryBuffer=malloc( length*sizeof( unsigned char ) );
  if( temporaryBuffer == NULL )
    {
      free( runningCountArray );
      free( characterCountArray );
      printf( "Failure allocating memory in BWT_transform.\n" ); /* Too big to put on stack */
      exit( EXIT_FAILURE );
    }
  memcpy( temporaryBuffer, buffer, length );
  transpositionBuffer = malloc(length*sizeof(unsigned int));
  if( transpositionBuffer == NULL )
    {
      free( runningCountArray );
      free( characterCountArray );
      free( temporaryBuffer );
    }
  
  /* Calculate the running totals for characters fo the alphabet. */
  
  /* First initialize arrays */
  for( i = 0 ; i < 256 ; i++ )
    {
      runningCountArray[ i ] = 0;
      characterCountArray[ i ] = 0;
    }
  for( i = 0; i < length; i++ )
    {
      runningCountArray[ buffer[ i ] ]++;
    }
  
  
  for( i = 0; i < 256; i++ )
    {
      previous=runningCountArray[ i ];
      runningCountArray[ i ]=sum;
      sum = sum + previous; 
    }
  
  /* Okay, now, for each character we have a map of how many characters are in
   * the file that are smaller than it.
   */ 
  
  /* Given this information (which represents the "sorted" block) */
  /* And the information of how many times the character was      */
  /* found previously in the "buffer" added together we come up   */
  /* with a unique position on the transposition table            */
  
  for ( i = 0 ; i < length ; i++ ) 
    {
      character = buffer[ i ];
      transpositionBuffer[ characterCountArray[ character ] + runningCountArray[ character ] ] = i;
      characterCountArray[ character ]++;
    }

  /* Transpose by following indices. */
  index = first;
  for ( i = 0 ; i < length ; i++ ) 
    {
      buffer[ i ]=temporaryBuffer[ index ];
      index = transpositionBuffer[ index ];
    }
  free( runningCountArray );
  free( characterCountArray );
  free( temporaryBuffer );
  free( transpositionBuffer );
}

		
