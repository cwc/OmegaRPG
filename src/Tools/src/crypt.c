/* WDT: todo later: move this into the Omega source, probably changing both
 * encode and decode into command line options to omega itself.  Oh, and 
 * change the name from 'crypt' to 'pack'. */

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "bwt.h"

#define VERSION 2

/* These _must_ match the equivalent values in Omega, and if they
 * change, I need to define a new version. */
#define MAXWIDTH  128
#define MAXLENGTH  64

static char *fileName;
static int lineNumber;

void error(char *explanation, ...)
{
  va_list ap;
  
  va_start (ap, explanation);
  fprintf  (stderr, "%s:%d: ", fileName, lineNumber);
  vfprintf (stderr, explanation, ap);
  va_end (ap);
  abort();
}

int unique(int mapnumber)
{
  static int cities[256];
  static int max=0;
  int i=0;
  while (i<max)
    if ( cities[i++] == mapnumber )
      return 0;
  
  if ( max >= 255 ) return 0;
  cities[max++] = mapnumber;
  return 1;
}

void fputint(int x, FILE *f)
{
  assert(sizeof(int) == 4); /* This assumption is explicit here, but it's
			     * also elsewhere. */
  fputc(x      &0xFF,f);
  fputc((x>>8) &0xFF,f);
  fputc((x>>16)&0xFF,f);
  fputc((x>>24)&0xFF,f);
}

void fputshort(short x, FILE *f)
{
  assert(sizeof(int) == 4); /* This assumption is explicit here, but it's
			     * also elsewhere. */
  fputc(x      &0xFF,f);
  fputc((x>>8) &0xFF,f);
}

/*
 * RLE from in to out, returning the amount of output data.
 * Max expansion is 2 times original size (if every char has
 * the high bit set and no compression can be done).
 */
int rle_encode(char *in, char *out, int lengthIn)
{
  int pos = 0;
  int run = 0;
  int c = in[pos];
  int offset = 0;
  
  while (pos < lengthIn)
    {
      if (in[pos] == c && run < 127)
	run++;
      else
	{
	  if ( run > 1 )
	    {
	      out[ offset++ ] = run | 0x80;
	      run = 1;
	    }
	  else if ( c & 0x80 || c == 0x55 )
	    {
	      out[ offset++ ] = 0x55;
	    }
	  out[ offset++ ] = c;
	}
      c = in[pos++];
    }
  
  /* Be sure to dump the last token. */
  if ( run > 1 )
    {
      out[ offset++ ] = run | 0x80;
      run = 1;
    }
  else if ( c & 0x80 || c == 0x55 )
    {
      out[ offset++ ] = 0x55;
    }
  out[ offset++ ] = c;
  
  return offset;
}

void encode(char **data, int *length)
{
  char *result = malloc(*length * 2);
  *length = rle_encode(*data,result, *length);
  free(*data);
  *data = result;
}

int main(int num_args, char *args[])
{
  char *code;
  int c, i, x,y,scanned, depth;
  int size;
  FILE *out, *in = 0, *data;
  int data_offset = 0;
  
  fileName = ""; lineNumber = 0;
  if ( num_args < 3 || num_args > 258 )
    error("Usage: %s <output-filename> <input-filename>"
	  " [more input filenames, max 256...]\n",
	  args[0]);
  
  /* A file to hold the data portion of the conglomeration; it will be
   * appended to the output file when we're finished.  This allows us to
   * build the header portion of the file easily. */
  data = tmpfile();
  if ( data == NULL )
    error("Unable to open temporary file.\n");
  /* The final output. */
  out = fopen(args[1],"wb");
  if ( out == NULL )
    error("Unable to open output file '%s'.\n", args[1]);
  
  fputc(VERSION,out); /* The file version */
  fputshort(num_args-2,out); /* The number of files it's supposed to contain */
  for ( i=2; i<num_args; i++ )
    {
      int levels;
      fileName = args[i]; lineNumber = 0;
      in = fopen(args[i],"r");
      if (!in)
	error("Unable to open map file '%s'.\n", args[i]);
      
      lineNumber++;
      scanned = fscanf(in,"v%d\n",&c);
      if (scanned != 1)
	error("File %s missing version header.\n", args[i]);
      if (c != VERSION)
	error("Unable to convert file %s: bad version, %d (expected %d).\n",
	      args[i], c, VERSION);
      
      lineNumber++;
      scanned = fscanf(in,"map %d\n",&c);
      if (scanned != 1)
	error("File %s missing map number header.\n", args[i]);
      if (!unique(c))
	error("Map header number %d is not unique in map file %s.\n",
	      c, args[i]);
      fputshort(c, out);
      
      lineNumber++;
      scanned = fscanf(in,"%d %d,%d\n", &depth, &x, &y);
      if (scanned != 3)
	error("%s:3: missing dimensions header.\n", args[i]);
      if ( (unsigned)x > 256 || (unsigned)y > 256 || !x || !y )
	{
	  error("Unable to convert file: bad size, %dx%d.\n",
		  x, y);
	}
      if ( (unsigned)depth > 255 || !depth )
	{
	  error("Unable to convert file: impossible number of maps, %d.\n",
		  depth);
	}
      
      fputc(x,out); fputc(y,out); fputc(depth,out);
      fputint(data_offset,out);
      
      levels = depth;
      code = malloc(x*y + 2);
      while (depth--)
	{
	  int lines = y;
	  size = 0;
	  while (lines--) {
	    fgets(code+size, x+2, in);
	    if ( strlen(code+size) != x+1 && code[size+x] != '\n' )
	      error("Map line %d wrong size: was %d, expected %d\n",
		   y, strlen(code+size)-1, x );
	    lineNumber++;
	    size += x;
	  }
	  
	  /* Encode and store the whole map level. */
/*	  encode(&code,&size);*/
	  /* The size of the encoded data precedes each encoded block. */
	  fputshort(size,data);
	  fwrite( code, size, 1, data );
	  data_offset += size + 2; /* account for the putshort above. */
	  /* Permit any number of lines beginning with "=" to be at the
	   * end of a level (they can be used as a seperator between levels).
	   */
	  c = fgetc(in);
	  if ( c != '=' )
	    error("level %d missing seperator line\n", levels-depth);
	  else
	    {
	      lineNumber++;
	      while ( c != '\n' && !feof(in) )
		c = fgetc(in);
	    }
	}
      free(code);
    }
  
  /* Finally, dump the data in the data file into the output file. */
  rewind(data);
  do {
    c = fgetc(data);
    if ( c==EOF ) break;
    fputc(c,out);
  } while ( 1 );
  
  fclose(in);
  fclose(out);
  fclose(data);
  exit(EXIT_SUCCESS);
}

