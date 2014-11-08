//Waleed Gudah CS 2240 2/4/14
#include "nu_malloc.h"
#include <stdlib.h>
#include <sys/mman.h>

void *nu_malloc ( size_t size )
{
    int *plen;
    
    int len = size + sizeof( size ); // Add sizeof( size ) for holding length.

    plen = mmap( 0, len, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, 0, 0 );

    *plen = len;                     // First 4 bytes contain length.
    
    return (void*)(&plen[1]);        // Memory that is after length variable.
}


void *nu_calloc (size_t count, size_t eltsize)
     {
       size_t size = count * eltsize;
       void *value = nu_malloc (size);
       if (value != 0)
         memset (value, 0, size);
       return value;
     }



void *nu_realloc (void *ptr, size_t size) {
	

    int minsize;

    size_t min_size;
   
    void *newptr;

   if(ptr == NULL){
 
 	return nu_malloc(size);

   }
   
  newptr = nu_malloc (size);
    
    if (newptr == NULL) {

      return NULL;

    }
      min_size = sizeof(ptr);

        if (size < min_size)

           min_size = size;

        memcpy (newptr, ptr, min_size);

        nu_free (ptr);
    

    return newptr;
}





void nu_free ( void * ptr )
{
    int *plen = (int*)ptr;
    int len;

    plen--;                          // Reach top of memory
    len = *plen;                     // Read length

    munmap( (void*)plen, len );
}


