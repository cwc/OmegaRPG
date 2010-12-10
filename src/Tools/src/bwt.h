
/*
 *  bwt.h
 *
 * Code modified from Mark Nelson's article:
 * "Data Compression With The Burrows-Wheeler Transform"
 * Dr. Dobbs Journal, September 1996.
 * See: http://www.dogma.net/markn/articles/bwt/bwt.htm to make sense 
 * of this source code.
 */

/* The following function does the actual transform.  It does this 
 * transform "in place" in the buffer, and returns an unsigned int 
 * in the range 0 <= X < length as out of band data for the transform.
 * This value needs to be passed to the "detransform".
 *
 * Large "length" values passed in may take awhile do to the (O)N lg N
 * time of sorting (On most platforms).
 */

unsigned int BWT_Transform(unsigned char *buffer, unsigned int length);

void BWT_Detransform( unsigned char *buffer, unsigned int length, unsigned int first );

