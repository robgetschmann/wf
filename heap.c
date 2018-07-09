/**
 * @file heap.c
 *
 * @author Robert A. Getschmann <rob@getschmann.net>
 *
 * @section DESCRIPTION
 *
 * Implementation module for ...
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

#include <wf.h>

#include <heap.h>

/*
 * Forward Declarations
 */
static void heapBuild(Heap* heap);
static void heapPercolate(Heap* heap, uint32_t index);

/**
 * @brief
 * @details
 * @param
 * @return
 */
static void
heapBuild(Heap* heap)
{

    int i, j;

    j = heap->count-1;
    i = (j-1) / 2;

    while (i >= 0) {
        heapPercolate(heap, i--);
    }

    return;

}

/**
 * @brief
 * @details
 * @param
 * @return
 */
void
heapInsert(Heap* heap,
           TrieNode** node,
           const char* word)
{

    // Case 1: the word is already present in heap
    if ((*node)->index != -1) {

        heap->vector[(*node)->index].frequency++;

        heapPercolate(heap, (*node)->index);

    }

    // Case 2: Word is not present and heap is not full
    else if (heap->count < heap->size) {

        uint32_t count = heap->count;

        heap->vector[count].frequency = (*node)->frequency;
#if 1
        heap->vector[count].word = calloc(strlen(word)+1, sizeof(char));

        strcpy(heap->vector[count].word, word);
#else
        heap->vector[0].word = strdup(word);
#endif

        heap->vector[count].node = *node;
        (*node)->index = heap->count;

        heap->count++;

        heapBuild(heap);

    }

    // Case 3: Word is not present and heap is full.And frequency of word
    // is more than root.The root is the least frequent word in heap,
    // replace root with new word
    else if ((*node)->frequency > heap->vector[0].frequency) {

        heap->vector[0].node->index = -1;
        heap->vector[0].node = *node;
        heap->vector[0].node->index = 0;
        heap->vector[0].frequency = (*node)->frequency;

        // delete previously allocated memoory and
        free(heap->vector[0].word);
#if 1
        heap->vector[0].word = calloc(strlen(word)+1, sizeof(char));

        strcpy(heap->vector[0].word, word);
#else
        heap->vector[0].word = strdup(word);
#endif

        heapPercolate(heap, 0);

    }

    return;

}

/**
 * @brief
 * @details
 * @param
 * @return
 */
Heap*
heapNew(uint32_t size)
{

    Heap* heap;

    heap = calloc(1, sizeof(Heap));

    heap->size = size;
    heap->count = 0;

    heap->vector = calloc(heap->size, sizeof(HeapNode));

    return (heap);

}

/**
 * @brief
 * @details
 * @param
 * @return
 */
static void
heapPercolate(Heap* heap,
              uint32_t index)
{

    uint32_t left = index*2 + 1;
    uint32_t right = left + 1;
    uint32_t smallest = index;

    if (left < heap->count
        && heap->vector[left].frequency < heap->vector[smallest].frequency)
    {
        smallest = left;
    }

    if (right < heap->count
        && heap->vector[right].frequency < heap->vector[smallest].frequency)
    {
        smallest = right;
    }

    if (smallest != index) {

        HeapNode node;

        // Update the corresponding index in Trie node.
        heap->vector[smallest].node->index = index;
        heap->vector[index].node->index = smallest;

        // Swap the nodes.
        node = heap->vector[smallest];
        heap->vector[smallest] = heap->vector[index];
        heap->vector[index] = node;

        heapPercolate(heap, smallest);

    }

    return;

}

/*
 * vim: cindent:cinoptions+={0,>1s,(0,t0,l1,^0:expandtab:smartindent:sw=4:ts=4:tw=0
 */
