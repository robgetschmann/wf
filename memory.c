/**
 * @file memory.c
 *
 * @author Robert A. Getschmann <rob@getschmann.net>
 *
 * @section DESCRIPTION
 *
 * Implementation module for memory wrapper functions
 *
 * Provide a wrapper for calloc, malloc, and realloc.  The wrapper function
 * calls the C library version.  If memory allocation fails the wrapper will
 * invoke and assert() to terminate the process.
 *
 * @section LICENSE
 *
 * ==============================
 * Copyright © 2018, Robert A. Getschmann <rob@getschmann.net>
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *    This product includes software developed by the <organization>.
 * 4. Neither the name of the <organization> nor the
 *    names of its contributors may be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY <COPYRIGHT HOLDER> ''AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * ==============================
 */

#include <wf.h>

/*
 * external function prototypes
 *
 * C Standard Library memory functions
 */
extern void* __real_calloc(size_t nmemb, size_t size);
extern void* __real_malloc(size_t size);
extern void* __real_realloc(void* memory, size_t size);
extern char* __real_strdup(const char* string);

/**
 * @brief   wrapper for C library calloc()
 * @param   same parameters as C library calloc()
 * @return  a pointer to the memory allocated or NULL
 */
void*
__attribute__ ((malloc))
__wrap_calloc(size_t nmemb,
              size_t size)
{

    void* m;

    m = __real_calloc(nmemb, size);
    assert(m);

    return (m);

}

/**
 * @brief   wrapper for C library malloc()
 * @param   same parameters as C library malloc()
 * @return  a pointer to the memory allocated or NULL
 */
void*
__attribute__ ((malloc))
__wrap_malloc(size_t size)
{

    void* m;

    m = __real_malloc(size);
    assert(m);

    return (m);

}

/**
 * @brief   wrapper for C library realloc()
 * @param   same parameters as C library realloc()
 * @return  a pointer to the memory reallocated or NULL
 */
void*
__attribute__ ((malloc))
__wrap_realloc(void* memory,
               size_t size)
{

    void* m;

    m = __real_realloc(memory, size);
    assert(m);

    return (m);

}

/**
 * @brief   wrapper for C library strdup()
 * @param   same parameters as C library realloc()
 * @return  a pointer to the duplicated string
 */
void*
__wrap_strdup(const char* string)
{

    char* s;

    s = __real_strdup(string);
    assert(s);

    return (s);

}

/*
 * vim: cindent:cinoptions+={0,>1s,(0,t0,l1,^0:expandtab:smartindent:sw=4:ts=4:tw=0
 */
