/**
 * Project                           : Secure IoT SoC
 * Name of the file                  : memory_functions.h
 * Brief Description of file         : This is a Baremetal memory functions library for Mindgrove Silicon.
 *
 *
 *
 * @file memory_functions.h
 * @author Sai Kiran Narayanaswami (saikiran@mindgrovetech.in)
 * @author Kapil Shyam. M (kapil@mindgrovetech.in)
 * @brief This is a Baremetal memory functions library for Mindgrove Silicon based on the TLSF allocator library.
 * @version 1.0
 * @date 2024-10-17
 *
 * @copyright Copyright (c) Mindgrove Technologies Pvt. Ltd 2024-2025. All rights reserved.
 *
 */

#ifndef MEMORY_FUNCTIONS_H
#define MEMORY_FUNCTIONS_H
#include <stdint.h>
#include <stddef.h>
#include <unistd.h>
#include <sys/types.h>

#include "defines.h"
#include "log.h"

#ifdef __cplusplus
extern "C"
{
#endif

    extern size_t mem_usage;
    extern size_t mem_usage_peak;

    void Heap_Init();

    void *memcpy(void *dest, const void *src, size_t n);

    void *memset(void *ptr, int value, size_t n);

    int memcmp(const void *ptr1, const void *ptr2, size_t n);

    /* Allocate SIZE bytes of memory.  */
    /** @fn  void *malloc(size_t size)
     * @brief  this function is used to dynamically allocate memory
     * @details  when this function is called memory is allocated in the heap during runtime
     * @param size_t size - size of the block to be allocated
     * @return  returns a pointer to the assigned block
     *          returns NULL if allocation fails
     */
    extern void *malloc(size_t size);

    void *memalign(size_t alignment, size_t size);

    /* Allocate new_size bytes of memory.  */
    /** @fn  void *realloc(void* ptr, size_t new_size)
     * @brief  this function is used to dynamically reallocate previously allocated memory
     * @details  when this function is called memory is reallocated from the heap during runtime
     * @param void* ptr - Previously allocated pointer to heap memory
     * @param size_t new_size - size of the block to be allocated
     * @return  returns a pointer to the assigned block
     *          returns NULL if allocation fails/if size is 0
     */
    extern void *realloc(void *ptr, size_t new_size);

    /* Free a block allocated by `malloc', `realloc' or `calloc'.  */
    /** @fn  void free(void *ptr)
     * @brief frees the memory assigned by malloc
     * @details  frees the memory assigned by malloc and when freed this memory is added back to the list to be reused
     * @param void *ptr - pointer to the memory assigned by malloc
     */
    extern void free(void *ptr);
    extern void *calloc(size_t size1, size_t size2);
#ifdef __cplusplus
}
#endif
#endif
