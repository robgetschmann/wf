/**
 * @file heap.c
 *
 * @author Robert A. Getschmann <rob@getschmann.net>
 *
 * @section DESCRIPTION
 *
 * Implementation module for heap object
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
 * function prototypes
 */
static int
heapBuild(Heap* heap);

static int
heapHeapify(Heap* heap,
            uint32_t index,
            uint32_t count);

/**
 * @brief
 * @param   the heap to be built
 * @return  0 - success, -1 - failure
 */
static int
heapBuild(Heap* heap)
{

    if (heap->count > 1) {

        uint32_t count = heap->count - 1;
        uint32_t index = (count - 1) / 2;

        do {

            heapHeapify(heap, index, heap->count);

        } while (index-- != 0);

    }

    return (0);

}

/**
 * @brief   display the top frequency counts and the corresponding word
 * @param   heap the heap containing the top frequency counts and word
 * @param   ofp the output file stream
 * @return  0 - success, -1 - failure
 */
int
heapDump(Heap* heap,
         FILE* ofp)
{

    for (uint32_t i = 0; i < heap->count; i++) {

        fprintf(ofp, "%d %s\n",
                heap->vector[i].frequency,
                heap->vector[i].word);

    }

    return (0);

}

/**
 * @brief   apply the stock "heapify" to a heap
 * @param   heap the heap to be "heapified"
 * @param   the element within the heap be operated on
 * @param   the number of elements in the heap
 * @return  0 - success, 1 - failure
 */
static int
heapHeapify(Heap* heap,
            uint32_t index,
            uint32_t count)
{

    uint32_t left = 2*index + 1;
    uint32_t right = left + 1;
    uint32_t minimum = index;

    if (left < count) {
        if (heap->vector[left].frequency < heap->vector[minimum].frequency) {
            minimum = left;
        }
        else if (heap->vector[left].frequency == heap->vector[minimum].frequency
                 && strcmp(heap->vector[left].word, heap->vector[minimum].word) > 0) {
            minimum = left;
        }
    }

    if (right < count) {
        if (heap->vector[right].frequency < heap->vector[minimum].frequency) {
            minimum = right;
        }
        else if (heap->vector[right].frequency == heap->vector[minimum].frequency
                 && strcmp(heap->vector[right].word, heap->vector[minimum].word) > 0) {
            minimum = right;
        }
    }

    if (minimum != index) {

        /* Update the trie index into the heap. */
        heap->vector[minimum].node->index = index;
        heap->vector[index].node->index = minimum;

        /* Swap the nodes. */
        HeapNode node = heap->vector[minimum];
        heap->vector[minimum] = heap->vector[index];
        heap->vector[index] = node;

        heapHeapify(heap, minimum, count);

    }

    return (0);

}

/**
 * @brief   insert a word into the trie and possibly the heap
 * @param   heap the heap to have an item inserted
 * @param   node a pointer to the corresponding trie node
 * @param   word the word to be inserted into the heap
 * @return  0 - success, 1 - failure
 */
int
heapInsert(Heap* heap,
           TrieNode* node,
           const char* word)
{

    /* The word is already present in the heap, increase its frequency. */
    if (node->index != -1) {

        heap->vector[node->index].frequency++;

        heapHeapify(heap, node->index, heap->count);

    }

    /* The word is not present in the heap and the heap is not full. */
    else if (heap->count < heap->size) {

        uint32_t count = heap->count;

        heap->vector[count].frequency = node->frequency;
        heap->vector[count].word = strdup(word);

        heap->vector[count].node = node;
        node->index = heap->count;

        heap->count++;

        heapBuild(heap);

    }

    /*
     * The word is not present in the heap and the heap is full.  Replace the
     * root with the new node.
     */
    else if (
             (node->frequency > heap->vector[0].frequency)
             ||
             ((node->frequency == heap->vector[0].frequency)
              && (strcmp(word, heap->vector[0].word) < 0))
            )
    {

        heap->vector[0].node->index = -1;
        heap->vector[0].node = node;
        heap->vector[0].node->index = 0;
        heap->vector[0].frequency = node->frequency;

        /* Delete word no longer in the top frequency count. */
        free(heap->vector[0].word);
        heap->vector[0].word = strdup(word);

        heapHeapify(heap, 0, heap->count);

    }

#if 0
    else if ((node->frequency == heap->vector[0].frequency)
             && (strcmp(word, heap->vector[0].word) < 0))
    {

        heap->vector[0].node->index = -1;
        heap->vector[0].node = node;
        heap->vector[0].node->index = 0;
        heap->vector[0].frequency = node->frequency;

        /* Delete word no longer in the top frequency count. */
        free(heap->vector[0].word);
        heap->vector[0].word = strdup(word);

        heapHeapify(heap, 0, heap->count);
    }
#endif

    return (0);

}

/**
 * @brief   allocate a new heap object of a given size
 * @param   size the size of the heap
 * @return  a pointer to the newly allocated heap object
 */
Heap*
heapNew(uint32_t size)
{

    Heap* heap;

    heap = calloc(1, sizeof(*heap));

    heap->size = size;
    heap->count = 0;

    heap->vector = calloc(heap->size, sizeof(*heap->vector));

    return (heap);

}

/**
 * @brief   sort the given heap
 * @param   heap the heap to be sorted
 * @return  0 - success, 1 - failure
 */
int
heapSort(Heap* heap)
{

    uint32_t count;

    if ((count = heap->count) != 0) {

        /* Build the heap. */
        heapBuild(heap);

        uint32_t index = count - 1;

        /* Extract each element and add it back into the heap. */
        do {

            /* Move the current node to the end of the sorted list. */
            HeapNode node = heap->vector[0];
            heap->vector[0] = heap->vector[index];
            heap->vector[index] = node;

            /* Heapify the reduced heap. */
            heapHeapify(heap, 0, index);

        } while (index-- != 0);

    }

    return (0);

}

/*
 * vim: cindent:cinoptions+={0,>1s,(0,t0,l1,^0:expandtab:smartindent:sw=4:ts=4:tw=0
 */
