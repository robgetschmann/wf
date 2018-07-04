/**
 * @file heap.c
 *
 * @author Robert A. Getschmann <rob@getschmann.net>
 *
 * @section DESCRIPTION
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

#include <stdlib.h>
#include <string.h>

#include <heap.h>

MinHeap*
createMinHeap( int capacity )
{

	MinHeap* minHeap = calloc(1, sizeof(MinHeap));

	minHeap->capacity = capacity;
	minHeap->count = 0;

	minHeap->array = calloc(minHeap->capacity, sizeof(MinHeapNode));

	return minHeap;

}

// A utility function to swap two min heap nodes. This function
// is needed in minHeapify
void
swapMinHeapNodes(MinHeapNode* a,
                 MinHeapNode* b)
{
	MinHeapNode temp = *a;
	*a = *b;
	*b = temp;
}

// This is the standard minHeapify function. It does one thing extra.
// It updates the minHapIndex in Trie when two nodes are swapped in
// in min heap
void minHeapify( MinHeap* minHeap, int idx )
{
	int left, right, smallest;

	left = 2 * idx + 1;
	right = 2 * idx + 2;
	smallest = idx;
	if ( left < minHeap->count &&
		minHeap->array[ left ]. frequency <
		minHeap->array[ smallest ]. frequency
	)
		smallest = left;

	if ( right < minHeap->count &&
		minHeap->array[ right ]. frequency <
		minHeap->array[ smallest ]. frequency
	)
		smallest = right;

	if( smallest != idx )
	{
		// Update the corresponding index in Trie node.
		minHeap->array[ smallest ]. root->indexMinHeap = idx;
		minHeap->array[ idx ]. root->indexMinHeap = smallest;

		// Swap nodes in min heap
		swapMinHeapNodes (&minHeap->array[ smallest ], &minHeap->array[ idx ]);

		minHeapify( minHeap, smallest );
	}
}

// A standard function to build a heap
void buildMinHeap( MinHeap* minHeap )
{
	int n, i;
	n = minHeap->count - 1;

	for( i = ( n - 1 ) / 2; i >= 0; --i )
		minHeapify( minHeap, i );
}

// Inserts a word to heap, the function handles the 3 cases explained above
void insertInMinHeap( MinHeap* minHeap, TrieNode** root, const char* word )
{
	// Case 1: the word is already present in minHeap
	if( (*root)->indexMinHeap != -1 )
	{
		++( minHeap->array[ (*root)->indexMinHeap ]. frequency );

		// percolate down
		minHeapify( minHeap, (*root)->indexMinHeap );
	}

	// Case 2: Word is not present and heap is not full
	else if( minHeap->count < minHeap->capacity )
	{
		int count = minHeap->count;
		minHeap->array[ count ]. frequency = (*root)->frequency;
		minHeap->array[ count ]. word = calloc(strlen(word)+1, sizeof(char));
		strcpy( minHeap->array[ count ]. word, word );

		minHeap->array[ count ]. root = *root;
		(*root)->indexMinHeap = minHeap->count;

		++( minHeap->count );
		buildMinHeap( minHeap );
	}

	// Case 3: Word is not present and heap is full. And frequency of word
	// is more than root. The root is the least frequent word in heap,
	// replace root with new word
	else if ( (*root)->frequency > minHeap->array[0]. frequency )
	{

		minHeap->array[ 0 ]. root->indexMinHeap = -1;
		minHeap->array[ 0 ]. root = *root;
		minHeap->array[ 0 ]. root->indexMinHeap = 0;
		minHeap->array[ 0 ]. frequency = (*root)->frequency;

		// delete previously allocated memoory and
		free(minHeap->array[ 0 ].word);
		minHeap->array[ 0 ]. word = calloc(strlen(word)+1, sizeof(char));
		strcpy( minHeap->array[ 0 ]. word, word );

		minHeapify ( minHeap, 0 );
	}
}

/*
 * vim: cindent:cinoptions+={0,>1s,(0,t0,l1,^0:expandtab:smartindent:sw=4:ts=4:tw=0
 */
