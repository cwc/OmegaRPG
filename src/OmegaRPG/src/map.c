/* Copyright 2000 William D. Tanksley Jr., licensed under the terms of the
 * Omega license as part of Omega. */
#include "glob.h"
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <assert.h>

#define MAP_VERSION      2
#define MAP_HEADER_SIZE  3
#define MAP_ITEM_SIZE    9

/* I plan to eventually make this code handle ALL map loading, since map
 * loading is largely similar.  However, to keep from biting off more than
 * I can chew, I'm going to start by doing just a little bit at a time.
 * 
 * For starters, I'm just going to have this load the map, and that's all. */
struct symbolMapping
{
  /* Which L_* to assign this character to. */
  int locationFunction;
  /* What argument (if any) to give to the function which constructs that
   * location. */
  int argument;
};

struct map_type
{
  int length, width, depth, offset;
  char *sites;
#if 0
  struct symbolMapping symbols[128-32]; /* Only printable characters.
					 * This does assume ASCII. */
#endif
};

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

void decode(char **data, int *length)
{
  /* just testing...  Place a sophisticated BWT here. */
}

int map_getWidth(map *m)
{
  return m->width;
}

int map_getLength(map *m)
{
  return m->length;
}

int map_getDepth(map *m)
{
  return m->depth;
}

/* Read in the indicated sublevel of the current map.  Called from
 * map_setLevel. */
void map_read(map *m, FILE *in, int subLevel)
{
  int size;
  
  if ( subLevel > m->depth || subLevel < 0 )
    error("map has only %d levels; game requested level %d.\n",
	  m->depth, subLevel);
  
  /* Seek to the location of the data (as previously calculated). */
  fseek(in,m->offset,SEEK_SET);
  
  do {
    /* Read in the size of the next submap, and if it wasn't the one
     * we're looking for, skip past it. */
    size = fgetshort(in);
    if ( subLevel )
      fseek(in, size, SEEK_CUR);
  } while ( subLevel-- );
  
  /* We now know that we're looking at the right map, and we know its size. */
  /* Read the encoding of the level into a buffer. */
  m->sites = malloc(size);
  fread(m->sites,size,1,in);
  
  /* Decode the level. */
  decode(&m->sites,&size);
  
  if ( size != m->length*m->width )
    error("Map had invalid size: expected %d, got %d.\n",
	  m->length*m->width, size);
}

/* Read the map info out of the header of the mapfile, and load it into
 * the map. */
void map_readDimensions(map *m, FILE *in, enum map_identifier map)
{
  int version, maps, items, this_map;
  
  version = fgetc(in); /* The file version */
  if ( version != MAP_VERSION )
    error("Mapfile version mismatch: expected %d, got %d.\n",
	  MAP_VERSION, version);
  
  maps = fgetshort(in); /* The number of maps it's supposed to contain */
  
  items = 0;
  do {
    this_map = fgetshort(in);
    m->width = fgetc(in); m->length = fgetc(in);  m->depth = fgetc(in);
    m->offset = fgetint(in);
    items++;
  } while (this_map != map && items < maps);
  
  if ( (items == maps) && (this_map != map) )
    error("Map #%d was not found in mapfile.\n", map );
  
  /* The offset stored in the file was the offset from the end of the table of
   * contents.  Fix that to be an absolute offset. */
  m->offset += MAP_HEADER_SIZE + maps*MAP_ITEM_SIZE;
}

map *map_open(enum map_identifier mapNumber)
{
  map *m = malloc( sizeof(map) );
  FILE *fd;
  
  strcpy(Str3,Omegalib);
  strcat(Str3,"maps.dat");
  fd = checkfopen(Str3,"rb");
  
  map_readDimensions(m,fd,mapNumber);
  
  fclose(fd);
  return m;
}

void map_setLevel(map *m, int levelNumber)
{
  FILE *fd;
  
  strcpy(Str3,Omegalib);
  strcat(Str3,"maps.dat");
  fd = checkfopen(Str3,"rb");
  
  map_read(m,fd,levelNumber);
  
  fclose(fd);
}

void map_close(map *m)
{
  free(m->sites);
  free(m);
}

#if 0
/* Using the mappings given in this map structure, build an Omega
 * level, using appropriate terrain. */
void map_buildLevel(map *m, Level *lev)
{
  
}
#endif

char map_getSiteChar(map *m, int i, int j)
{
  assert(i < m->width);
  assert(j < m->length);
  assert(m->sites[j*m->width + i]);
  return m->sites[j*m->width + i];
}

