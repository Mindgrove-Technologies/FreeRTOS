#ifndef MALLOC_H
#define MALLOC_H 1

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <stdio.h>

#ifdef _LIBC
# define __MALLOC_HOOK_VOLATILE
# define __MALLOC_DEPRECATED
#else
# define __MALLOC_HOOK_VOLATILE volatile
# define __MALLOC_DEPRECATED __attribute_deprecated__
#endif

/* Allocate SIZE bytes of memory.  */
extern void *malloc (size_t size);

/* Free a block allocated by `malloc', `realloc' or `calloc'.  */
extern void free (void *ptr);

#ifdef __cplusplus
}
#endif

#endif
