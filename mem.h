#ifndef MEM_H
#define MEM_H

void *gmalloc(int size);
void *grealloc(void *p, int size);
void gfree(void *p);
void gMemReport( void);

#endif


