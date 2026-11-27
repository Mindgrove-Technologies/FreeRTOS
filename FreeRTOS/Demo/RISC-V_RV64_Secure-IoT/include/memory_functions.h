/**
 * SPDX-License-Identifier: Apache-2.0
 * @copyright Copyright (c) 2021-2026 Mindgrove Technologies. All rights reserved.
 *
 * @license Licensed under the Apache License, Version 2.0 (see LICENSE).
 * @licenseblock
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 * @endlicenseblock
 *
 * Project                   : MGS2401 SoC
 * @file memory_functions.h
 * @author Sai Kiran Narayanaswami (saikiran@mindgrovetech.in)
 * @author Kapil Shyam. M (kapil@mindgrovetech.in)
 * @brief This is a Baremetal memory functions library for Mindgrove Silicon
 * based on the TLSF allocator library.
 * @version 1.0
 * @date 2024-10-17
 *
 * @section History
 * -----------------------------------------------------------------------------
 * Date       | Version | Modified by             | Description
 * -----------|---------|-------------------------|-----------------------------
 * 17-10-2024 | 1.0     | Sai Kiran Narayanaswami | Initial release.
 * -----------------------------------------------------------------------------
 */

#ifndef MEMORY_FUNCTIONS_H
#define MEMORY_FUNCTIONS_H

#include <stdint.h>
#include <stddef.h>
#include <unistd.h>
#include <stdarg.h>
#include <sys/types.h>

#ifdef __cplusplus
extern "C"
{
#endif

extern size_t mem_usage;
extern size_t mem_usage_peak;

/**
 * @brief Initializes the heap memory allocator
 * @details Sets up the underlying memory structures, free lists, or boundary
 * tags required for the heap management system to dynamically allocate and free memory blocks.
 */
void Heap_Init();

/**
 * @brief Copies a block of memory from a source to a destination
 * @details Copies n bytes from the memory location pointed to by src directly
 * to the memory location pointed to by dest. The memory areas must not overlap.
 * @param void *dest - pointer to the destination array where the content is to be copied
 * @param const void *src - pointer to the source of data to be copied
 * @param size_t n - number of bytes to be copied
*/
void *memcpy(void *dest, const void *src, size_t n);

/**
 * @brief Fills a block of memory with a specified value
 * @details Sets the first n bytes of the block of memory pointed to by ptr to
 * the specified value.
 * @param void *ptr - pointer to the block of memory to fill
 * @param int value - value to be set, passed as an int but converted to an unsigned char
 * @param size_t n - number of bytes to be set to the value
 */
void *memset(void *ptr, int value, size_t n);

/**
 * @brief Compares two blocks of memory
 * @details Compares the first n bytes of the memory areas pointed to by ptr1 
 * and ptr2. It returns an integer less than, equal to, or greater than zero if 
 * the first block is found to be less than, matches, or is greater than the second block.
 * @param const void *ptr1 - pointer to the first block of memory
 * @param const void *ptr2 - pointer to the second block of memory
 * @param size_t n - number of bytes to be compared
 * 
 * @return int 
*/
int memcmp(const void *ptr1, const void *ptr2, size_t n);

/**
 * @brief  this function is used to dynamically allocate memory
 * @details  when this function is called memory is allocated in the heap during runtime
 * @param size_t size - size of the block to be allocated
 * @return  returns a pointer to the assigned block
 *          returns NULL if allocation fails
 */
extern void *malloc(size_t size);

/**
 * @brief  Allocates aligned memory block
 * @details Allocates a block of memory of the specified size, ensuring that the
 * boundary alignment matches the requested alignment value. The alignment
 * parameter must be a power of two.
 * @param size_t alignment - the power-of-two boundary to which the allocated memory must be aligned
 * @param size_t size - size of the memory block to be allocated in bytes
 */
void *memalign(size_t alignment, size_t size);

/**
 * @brief  this function is used to dynamically reallocate previously allocated memory
 * @details  when this function is called memory is reallocated from the heap during runtime
 * @param void* ptr - Previously allocated pointer to heap memory
 * @param size_t new_size - size of the block to be allocated
 */
extern void *realloc(void *ptr, size_t new_size);

/**
 * @brief frees the memory assigned by malloc
 * @details  frees the memory assigned by malloc and when freed this memory is
 * added back to the list to be reused
 * @param void *ptr - pointer to the memory assigned by malloc
 */
extern void free(void *ptr);

/**
 * @brief Allocates block of memory for an array and initializes its bytes to zero
 * @details Allocates memory for an array of size1 elements, each of size2 bytes, 
 * and initializes all allocated bits to zero. If the allocation succeeds,
 * a pointer to the lowest byte of the allocated memory block is returned.
 * @param size_t size1 - number of elements to be allocated
 * @param size_t size2 - size of each element in bytes
*/
extern void *calloc(size_t size1, size_t size2);

#ifdef __cplusplus
}
#endif
#endif
