/**
 * @file heap.h
 *
 * @author Robert A. Getschmann <rob@getschmann.net>
 *
 * @section DESCRIPTION
 *
 * Interface module for Heap object
 *
 * @section LICENSE
 *
 * ===========================================================================
 *
 * Copyright (c) 2018, Robert A. Getschmann <rob@getschmann.net>
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
 *
 * ===========================================================================
 */

#include <trie.h>

#pragma once

/*
 * Forward declarations
 */
typedef struct Heap Heap;
typedef struct HeapNode HeapNode;

/**
 * @brief   Object representing a Sorted Heap
 */
struct Heap {
    uint32_t count;     /* number of nodes within the heap */
    uint32_t size;      /* maximum size of the heap */
    HeapNode* vector;   /* dynamically allocated vector of heap nodes */
};

/**
 * @brief   Object representing a Sorted Heap Node
 */
struct HeapNode {
    TrieNode* node;     /* pointer to associate trie and heap node */
    uint32_t frequency; /* word frequency */
    char* word;         /* dynamically allocatd null terminated word string */
};

/**
 * @brief   Function prototype
 */
Heap*
heapNew(uint32_t size);

/**
 * @brief   Function prototype
 */
int
heapInsert(Heap* heap,
           TrieNode** node,
           const char* word);

/**
 * @brief   Function prototype
 */
int
heapSort(Heap* heap);

/*
 * vim: cindent:cinoptions+={0,>1s,(0,t0,l1,^0:expandtab:smartindent:sw=4:ts=4:tw=0
 */
