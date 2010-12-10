/* WDT: todo later: move this into the Omega source, probably changing both
 * encode and decode into command line options to omega itself.  Oh, and 
 * change the name from 'crypt' to 'pack'. */

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <assert.h>

#define VERSION      2
#define HEADER_SIZE  3
#define ITEM_SIZE    9

/* These _must_ match the equivalent values in Omega, and if they
 * change, I need to define a new version. */
#define MAXWIDTH  128
#define MAXLENGTH  64

void error(char *explanation, ...)
{
  va_list ap;
  
  va_start (ap, explanation);
  vfprintf (stderr, explanation, ap);
  va_end (ap);
  abort();
}

int fgetint(FILE *f)
{
  int x;
  assert(sizeof(int) == 4); /* This assumption is explicit here, but it's
			     * also elsewhere. */
  x = fgetc(f)        &0x000000FF;
  x|= (fgetc(f) << 8) &0x0000FF00;
  x|= (fgetc(f) <<16) &0x00FF0000;
  x|= (fgetc(f) <<24) &0xFF000000;
  
  if ( feof(f) )
    error("Unexpected end of file while parsing binary file.");
  
  return x;
}

int fgetshort(FILE *f)
{
  int x;
  assert(sizeof(int) == 4); /* This assumption is explicit here, but it's
			     * also elsewhere. */
  x = fgetc(f)        &0x000000FF;
  x|= (fgetc(f) << 8) &0x0000FF00;
  
  if ( feof(f) )
    error("Unexpected end of file while parsing binary file.");
  
  return x;
}

int rle_decode(char *in, char *out, int inLength, int outLength)
{
  int pos = 0;
  int run = 0;
  int offset = 0;
  
  while (pos < inLength && offset < outLength)
    {
      run = 1;
      if ( in[pos] == 0x55 )
	pos++;
      else if ( in[pos] & 0x80 )
	run = in[pos++] & ~0x80;
      
      if ( offset + run > outLength )
	{
	  fprintf(stderr,"Error: dest overrun during decompression, expected"
		  " %d, got %d.\n", outLength, offset+run);
	  return 0;
	}
      while ( run-- > 0 )
	out[offset++] = in[pos];
      pos++;
    }
  
  return offset;
}

void decode(char **data, int *length, int expectedLength)
{
  char *result = malloc(expectedLength);
  *length = rle_decode(*data,result,*length,expectedLength);
  if ( !*length )
    error("Unable to decrypt input map data.\n");
  free(*data);
  *data = result;
}

int main(int num_args, char *args[])
{
  char *code;
  FILE *out, *in;
  int map, version, maps, x, y, this_map, offset, size, items, depth;
  
  if ( num_args != 4 )
    error("Usage: %s <mapfile name> <map number> <output-filename>\n",
	  args[0]);
  
  out = fopen(args[3],"w");
  if ( out == NULL )
    error("Unable to open output file '%s'.\n", args[3]);
  
  map = atoi(args[2]);
  
  in = fopen(args[1],"rb");
  if (!in)
    error("Unable to open map file '%s'.\n", args[1]);
  
  version = fgetc(in); /* The file version */
  if ( version != VERSION )
    error("Mapfile version mismatch: expected %d, got %d.\n",
	  VERSION, version);
  
  maps = fgetshort(in); /* The number of files it's supposed to contain */
  if ( maps < 1 )
    error("Mapfile has bad format: claims to contain %d maps.", maps);
  
  fprintf(out,"v%d\n",VERSION);
  fprintf(out,"map %d\n",map);
  
  items = 0;
  do {
    this_map = fgetshort(in);
    x = fgetc(in); y = fgetc(in); depth = fgetc(in);
    offset = fgetint(in);
    items++;
    printf("%d: map %d, %dx%d, offset %d.\n", items, this_map, x, y, offset);
  } while (this_map != map && items < maps);
  
  if ( items == maps )
    error("Map #%d was not found in mapfile.\n", map );
  
  fprintf(out,"%d %d,%d\n", depth, x, y);
  
  /* Then seek to the location of the data -- to its offset, plus the constant
   * space taken by the header, plus the space taken by all of the map
   * table of content entries. */
  fseek(in,offset+HEADER_SIZE+maps*ITEM_SIZE,SEEK_SET);
  
  while ( depth-- )
    {
      /* Read the size of this level. */
      size = fgetshort(in);
      code = malloc(size);
      fread(code,size,1,in);
      
      /* Decode the whole level. */
/*      decode(&code,&size, x*y);*/
      
      if ( size != x*y )
	error("Invalid size: expected %d, got %d.\n", x*y, size);
      
      /* Write every line of the level to the output file. */
      y = 0;
      while(size)
	{
	  fwrite(code+y*x, x, 1, out);
	  fputc('\n', out);
	  y++; size -= x;
	}
      /* If there's another level in the map, print a seperator. */
      if ( depth )
	fprintf(out, "========*** Level Seperator ***=======\n");
      else fprintf(out, "===END OF MAP===\n");
      free(code);
    }
  
  fclose(in); fclose(out);
  
  exit(EXIT_SUCCESS);
}

