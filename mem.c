#include "general.h"
#include "mem.h"

#ifdef DEBUG
typedef struct _GMemHdr 
{
	int size;
	int index;
	struct _GMemHdr *next;
} GMemHdr;

#define gMemHdrSize (( sizeof(GMemHdr) + 7) & ~7)
#define gMemTrlSize 4
#define gMemDeadVal 0xFFFFFFFFUL
#define gMemDataSize(size) ((((size) + gMemTrlSize - 1) / gMemTrlSize) * gMemTrlSize)
#define gMemNLists    64
#define gMemListShift  4
#define gMemListMask  (gMemNLists - 1)

static GMemHdr *gMemList[gMemNLists] = 
{
  NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
  NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
  NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
  NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
  NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
  NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
  NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
  NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL
};

static int gMemIndex = 0;
static int gMemAlloc = 0;
#endif /* DEBUG */


void *gmalloc(int size) 
{
#ifdef DEBUG
	int size1;
	char *mem;
	GMemHdr *hdr;
	void *data;
	int lst;
	unsigned long *trl, *p;

	if (size <= 0)
		return NULL;

	size1 = gMemDataSize(size);

	mem = (char *)malloc(size1 + gMemHdrSize + gMemTrlSize);

	if( mem == NULL) 
	{
    	zdebug( "Out of memory");
		return NULL;
  	}

	hdr = ( GMemHdr *)mem;
	data = ( void *)(mem + gMemHdrSize);
	trl = ( unsigned long *)(mem + gMemHdrSize + size1);
	hdr->size = size;
	hdr->index = gMemIndex++;
	lst = (( int)hdr >> gMemListShift) & gMemListMask;
	hdr->next = gMemList[lst];
	gMemList[lst] = hdr;
	++gMemAlloc;

	for( p = ( unsigned long *)data; p <= trl; ++p)
		*p = gMemDeadVal;

	return data;
#else
	return malloc(size);
#endif
}

void *grealloc(void *p, int size) 
{
#ifdef DEBUG
	GMemHdr *hdr;
	void *q;
	int oldSize;

	if (size <= 0) 
	{
	    if (p)
			gfree(p);
    	return NULL;
  	}
  	if (p) 
  	{
	    hdr = (GMemHdr *)((char *)p - gMemHdrSize);
	    oldSize = hdr->size;
	    q = gmalloc(size);
	    memcpy(q, p, size < oldSize ? size : oldSize);
	    gfree(p);
  	} 
  	else 
  	{
    	q = gmalloc(size);
  	}
  	return q;
#else
  	return realloc( p, size);
#endif
}

void gfree(void *p) 
{
#ifdef DEBUG
	int size;
	GMemHdr *hdr;
	GMemHdr *prevHdr, *q;
	int lst;
	unsigned long *trl, *clr;

	if (p) 
	{
    	hdr = (GMemHdr *)((char *)p - gMemHdrSize);
    	lst = ((int)hdr >> gMemListShift) & gMemListMask;
 
    	for (prevHdr = NULL, q = gMemList[lst]; q; prevHdr = q, q = q->next) 
    	{
        	if ( q == hdr)
				break;
    	}

    	if( q) 
    	{
      		if (prevHdr)
				prevHdr->next = hdr->next;
      		else
				gMemList[lst] = hdr->next;
      
      		--gMemAlloc;
      		size = gMemDataSize(hdr->size);
      		trl = (unsigned long *)((char *)hdr + gMemHdrSize + size);
      
      		if (*trl != gMemDeadVal) 
      		{
				zdebug( "Overwrite past end of block %d at address %p", hdr->index, p);
      		}

			for (clr = (unsigned long *)hdr; clr <= trl; ++clr)
				*clr = gMemDeadVal;
				
      		free(hdr);
    	} 
    	else 
    	{
      		zdebug( "Attempted to free bad address %p", p);
    	}
	}
#else
	free(p);
#endif
}

void gMemReport( void) 
{
#ifdef DEBUG
	FILE 	*inffile;
	GMemHdr *p;
	int lst;

	if(( inffile = fopen( "C:\\memdebug.txt", "wb+")) == NULL)
		return;

	fprintf( inffile, "%d memory allocations in all\n", gMemIndex);

	if( gMemAlloc > 0) 
	{
	    fprintf( inffile, "%d memory blocks left allocated:\n", gMemAlloc);
	    fprintf( inffile, " index     size\n");
	    fprintf( inffile, "-------- --------\n");

	    for (lst = 0; lst < gMemNLists; ++lst) 
	    {
			for (p = gMemList[lst]; p; p = p->next)
				fprintf( inffile, "%8d %8d\n", p->index, p->size);
    	} 
  	} 
  	else 
  	{
		fprintf( inffile, "No memory blocks left allocated\n");
	}
	
	fprintf( inffile, "--= End =--");
	fclose( inffile);
#endif
}

